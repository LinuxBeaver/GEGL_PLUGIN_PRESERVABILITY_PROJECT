/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <https://www.gnu.org/licenses/>.
 *
 * Credit to Øyvind Kolås (pippin) for major GEGL contributions
 * 2023 Beaver, Crayon Text

Test this plugin without installing by pasting this syntax into Gimp's GEGL Graph filter. This plugin requires my "bevel" plugin to work and it does ship with it.

color-overlay value=#36ff00
id=1  subtract aux=[ ref=1 lb:bevel  radius1=10  bevel1=90 bevel2=40  ]
median-blur radius=0 color-to-alpha color=#36ff00 transparency-threshold=0.211755
noise-spread amount-x=4 amount-y=4
gaussian-blur std-dev-x=0.5 std-dev-y=0.5 median-blur radius=0
color-overlay value=#ff0000

 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

property_double (spread, _("Crayon Effect"), 5.5)
   description (_("A noise spread distortion operation to create a crayon effect on the drawing"))
   value_range (0.0, 10.0)
   ui_range    (0.0, 10.0)
   ui_gamma    (2.0)
  ui_steps      (0.01, 0.50)

property_double (intensity, _("Intensity control"), 20.0)
   description (_("Control the intensity of the crayon text. Lower values increase the intensity. Higher values will make the crayon text even more thin. Use high values on very bold fonts and low values on normal size fonts."))
   value_range (15.0, 100.0)
   ui_range    (15.0, 100.0)
   ui_gamma    (2.0)
  ui_steps      (0.01, 0.50)

property_color  (color, _("Color"), "#ff0000")
    description (_("Color of the drawn text"))
  ui_meta     ("sensitive", "! originalcolor")


property_boolean (originalcolor, _("Use original color instead of color overlay"), FALSE)
  description    (_("Check to use original color on crayon text instead of color overlay"))

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     crayontext
#define GEGL_OP_C_SOURCE crayontext.c

#include "gegl-op.h"


typedef struct
{
GeglNode *input;
GeglNode *output;
GeglNode *thecolor;
GeglNode *thecolorremover; 
GeglNode *blurfix; 
GeglNode *subtract;
GeglNode *spread; 
GeglNode *bevel;
GeglNode *color;
GeglNode *idref;
GeglNode *idref2;
GeglNode *white;
GeglNode *multiply;
} State; 



static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);

  State *state = o->user_data = g_malloc0 (sizeof (State));

  state->input    = gegl_node_get_input_proxy (gegl, "input");
  state->output   = gegl_node_get_output_proxy (gegl, "output");


#define muhcolor \
" color-overlay value=#36ff00 "\

  state->thecolor = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", muhcolor,
                                  NULL);

#define muhcolorremoval \
" median-blur radius=0 color-to-alpha color=#36ff00 transparency-threshold=0.211755 "\

  state->thecolorremover = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", muhcolorremoval,
                                  NULL);
#define muhblur \
" gaussian-blur std-dev-x=0.5 std-dev-y=0.5 median-blur radius=0 "\

  state->blurfix = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", muhblur,
                                  NULL);

  state->subtract = gegl_node_new_child (gegl,
                                  "operation", "gegl:subtract", 
                                  NULL);

  state->spread = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-spread", 
                                  NULL);

  state->bevel = gegl_node_new_child (gegl,
                                  "operation", "lb:bevel", "radius1", 10.0, "bevel1", 90.0,  
                                  NULL);

  state->color = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay", 
                                  NULL);

  state->idref = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop", 
                                  NULL);

  state->idref2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop", 
                                  NULL);

  state->multiply = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply", 
                                  NULL);

#define colorwhite \
" color-overlay value=#ffffff "\

 state->white = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", colorwhite,
                                  NULL);

} /* attach */

static void update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;

  if (o->originalcolor)
  {
  gegl_node_link_many (state->input, state->idref2, state->thecolor, state->idref, state->subtract, state->thecolorremover, state->spread, state->blurfix, state->white, state->multiply, state->output, NULL);
  gegl_node_link_many (state->idref, state->bevel,  NULL);
  gegl_node_connect (state->subtract, "aux", state->bevel, "output");
  gegl_node_connect (state->multiply, "aux", state->idref2, "output");
  }
  else
  {
  gegl_node_link_many (state->input, state->thecolor, state->idref, state->subtract, state->thecolorremover, state->spread, state->blurfix, state->color, state->output, NULL);
  gegl_node_link_many (state->idref, state->bevel,  NULL);
  gegl_node_connect (state->subtract, "aux", state->bevel, "output");
  }

  gegl_operation_meta_redirect (operation, "spread", state->spread, "amount-x"); 
  gegl_operation_meta_redirect (operation, "spread", state->spread, "amount-y"); 
  gegl_operation_meta_redirect (operation, "intensity", state->bevel, "bevel2"); 
  gegl_operation_meta_redirect (operation, "color", state->color, "value"); 
}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;
GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);
  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;
  operation_meta_class->update = update_graph;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:crayon", 
    "title",       _("Crayon/Hand Drawn Text"),
    "reference-hash", "fseak3crayosn234421gyzjhee",
    "description", _("Give your text the appearance of a crayon drawing or hand drawn text. Try using a semi bold font that resemble human hand drawings with this. "),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Crayon Text..."),
    NULL);
}

#endif
