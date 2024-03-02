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
 * Copyright 2006 Øyvind Kolås <pippin@gimp.org>
 * 2023 Beaver, Extract thin structures (hair extractor)

Test this filter without installing by running this syntax inside Gimp's GEGL graph filter.This filter uses my gegl SSG plugin's syntax, but it does not ship with SSG, just a static graph of it at 1% radius.

id=1
color-to-alpha color=#7f907e  opacity-threshold=0.19 transparency-threshold=0.09
src-in  aux=[ ref=1 opacity value=1 ]
opacity value=1
 median-blur radius=0
id=2 gimp:layer-mode layer-mode=split composite-mode=auto aux=[ ref=2  id=x opacity value=1 median-blur alpha-percentile=0 radius=1  gaussian-blur std-dev-x=0 std-dev-y=0 dropshadow x=0 y=0 radius=0.50  grow-radius=1 opacity=0.5 xor aux=[ ref=x ] color-overlay value=#ffffff ]
gaussian-blur std-dev-x=0.5 std-dev-y=0.5




This filter is special because it is expected to be used with Gimp's selection tool.
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

enum_start (background_remover)
   enum_value (JUSTEXTRACT,    "color",    N_("Color extract (best for hair)"))
   enum_value (TA2MODE,  "color-ta2",  N_("Color extract + threshold alpha"))
   enum_value (EDGESMOOTHMODE, "color-edge-smooth", N_("Color extract + edgesmooth"))
   enum_value (TA2EDGESMOOTHMODE,  "all",  N_("Color extract + threshold alpha and Edge Smooth"))
enum_end (backgroundremover)

property_enum (mode, _("Policy for color extraction"),
               backgroundremover, background_remover,
               TA2MODE)
  description (_("Certain modes do different things such as making extracting hair easier or additionally smoothing edges."))


property_color (color, _("Color to remove for extraction 1"), "na")
    description(_("The color range to make transparent."))

property_color (color2, _("Color to remove for extraction 2"), "na")
    description(_("The color range to make transparent."))

property_color (color3, _("Color to remove for extraction 3"), "na")
    description(_("The color range to make transparent."))

property_color (color4, _("Color to remove for extraction 4"), "na")
    description(_("The color range to make transparent."))

property_double (transparencythreshold, _("Transparency threshold"), 0.09)
    description(_("The limit below which colors become transparent."))
    value_range (0.0, 0.6)

property_double (transparencythreshold2, _("Transparency threshold 2"), 0.09)
    description(_("The limit below which colors become transparent."))
    value_range (0.0, 0.6)


property_double (opacitythreshold, _("Opacity threshold"), 0.9)
    description(_("The limit above which colors remain opaque."))
    value_range (0.0, 0.9)

property_double (opacitythreshold2, _("Opacity threshold 2"), 0.9)
    description(_("The limit above which colors remain opaque."))
    value_range (0.0, 0.9)


property_double (opacity1, _("Opacity 1 (Inside alphalock/replace composer)"), 7.0)
   description (_("The first opacity filter node that is inside a special blend mode that combines 'alpha lock' with 'replace'. (gegl:src-in). Src-In is a GEGL exclusive blend mode that this filter depends on and moving this opacity slider will modify the extraction abilities. "))
   value_range (1.0, 9.0)
   ui_range    (1.0, 9.0)
   ui_gamma    (3.0)

property_double (opacity2, _("Opacity 2 (Global Opacity)"), 1.0)
   description (_("The second opacity filter node that brings back artifact from the original image. "))
   value_range (1.0, 9.0)
   ui_range    (1.0, 9.0)
   ui_gamma    (3.0)

property_double (blur, _("Mild Gaussian Blur"), 0.0)
   description (_("Apply a mild gaussian blur"))
   value_range (0.0, 1.5)
   ui_range    (0.0, 1.5)
   ui_gamma    (3.0)
   ui_meta     ("unit", "pixel-distance")

property_double  (edgemedian, _("Median edges"), 73)
  value_range (0, 100)
  description (_("Apply a median blur only on the edges"))
ui_meta ("visible", "!mode {color, color-ta2}" )

property_double (edgegaus, _("Blur Edge"), 1)
   description (_("Apply a Gaussian Blur only on the edges of an image"))
   value_range (0.0, 3.0)
   ui_range    (0.24, 3.0)
   ui_gamma    (3.0)
   ui_meta     ("unit", "pixel-distance")
   ui_meta     ("axis", "x")
ui_meta ("visible", "!mode {color, color-ta2}" )


property_double (edgeopacity, _("Increase Opacity around edges"), 1.2)
    description (_("Increase the opacity only around the edges"))
    value_range (1, 6.0)
    ui_range    (1, 3.0)
ui_meta ("visible", "!mode {color, color-ta2}" )

property_double (alphadegree, _("Value"), 0.4)
    description ( _("Increase the Alpha channel intensity"))
    value_range    (0.0, 0.95)
  ui_steps      (0.01, 0.01)
ui_meta ("visible", "!mode {color, color-edge-smooth}" )


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     bgremoverbycolor
#define GEGL_OP_C_SOURCE bgremoverbycolor.c

#include "gegl-op.h"

typedef struct
{
 GeglNode *input;
 GeglNode *c2a;
 GeglNode *c2a2;
 GeglNode *c2a3;
 GeglNode *c2a4;
 GeglNode *alphalockreplace;
 GeglNode *opacity1;
 GeglNode *opacity2;
 GeglNode *opacity3;
 GeglNode *blur;
 GeglNode *median;
 GeglNode *idref;
 GeglNode *string;
 GeglNode *ta2;
 GeglNode *edgesmooth;
 GeglNode *replace;
 GeglNode *output;
}State;

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);

  State *state = o->user_data = g_malloc0 (sizeof (State));

  state->input    = gegl_node_get_input_proxy (gegl, "input");
  state->output   = gegl_node_get_output_proxy (gegl, "output");

  state->c2a = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-to-alpha",
                                  NULL);

  state->c2a2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-to-alpha",
                                  NULL);

  state->c2a3 = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-to-alpha",
                                  NULL);

  state->c2a4 = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-to-alpha",
                                  NULL);

  state->alphalockreplace = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-in",
                                  NULL);

  state->opacity1 = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

  state->opacity2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

  state->opacity3 = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

  state->blur = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
                                  NULL);

  state->median = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 0,
                                  NULL);

  state->idref = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);      

  state->replace = gegl_node_new_child (gegl,
                                  "operation", "gegl:src",
                                  NULL);      
#define syntaxhere \
"  median-blur radius=0 id=2 gimp:layer-mode layer-mode=split composite-mode=auto aux=[ ref=2  id=x opacity value=1 median-blur alpha-percentile=0 radius=1  gaussian-blur std-dev-x=0 std-dev-y=0 dropshadow x=0 y=0 radius=0.50  grow-radius=1 opacity=0.5 xor aux=[ ref=x ] color-overlay value=#ffffff  ] "\

  state->string = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", syntaxhere,
                                  NULL);
      
#define edgesmoothandthresholdalpha \
"  median-blur radius=0 id=2 gimp:layer-mode layer-mode=split composite-mode=auto aux=[ ref=2  id=x opacity value=1 median-blur alpha-percentile=0 radius=1  gaussian-blur std-dev-x=0 std-dev-y=0 dropshadow x=0 y=0 radius=0.50  grow-radius=1 opacity=0.5 xor aux=[ ref=x ] color-overlay value=#ffffff  ] "\

  state->edgesmooth = gegl_node_new_child (gegl,
                                  "operation", "lb:edgesmooth",
                                  NULL);

 state->ta2 = gegl_node_new_child (gegl,
                                  "operation", "lb:threshold-alpha",
                                  NULL);

 gegl_operation_meta_redirect (operation, "transparencythreshold", state->c2a, "transparency-threshold"); 
 gegl_operation_meta_redirect (operation, "opacitythreshold", state->c2a, "opacity-threshold"); 
 gegl_operation_meta_redirect (operation, "transparencythreshold2", state->c2a2, "transparency-threshold"); 
 gegl_operation_meta_redirect (operation, "opacitythreshold2", state->c2a2, "opacity-threshold"); 
 gegl_operation_meta_redirect (operation, "transparencythreshold", state->c2a3, "transparency-threshold"); 
 gegl_operation_meta_redirect (operation, "opacitythreshold", state->c2a3, "opacity-threshold");
 gegl_operation_meta_redirect (operation, "transparencythreshold", state->c2a4, "transparency-threshold"); 
 gegl_operation_meta_redirect (operation, "opacitythreshold", state->c2a4, "opacity-threshold");  
 gegl_operation_meta_redirect (operation, "color", state->c2a, "color"); 
 gegl_operation_meta_redirect (operation, "color2", state->c2a2, "color"); 
 gegl_operation_meta_redirect (operation, "color3", state->c2a3, "color");  
 gegl_operation_meta_redirect (operation, "color4", state->c2a4, "color"); 
 gegl_operation_meta_redirect (operation, "opacity1", state->opacity1, "value"); 
 gegl_operation_meta_redirect (operation, "opacity2", state->opacity2, "value"); 
 gegl_operation_meta_redirect (operation, "blur", state->blur, "std-dev-x"); 
 gegl_operation_meta_redirect (operation, "blur", state->blur, "std-dev-y"); 
 gegl_operation_meta_redirect (operation, "edgegaus", state->edgesmooth, "gaus");
 gegl_operation_meta_redirect (operation, "edgemedian", state->edgesmooth, "alpha-percentile2");
 gegl_operation_meta_redirect (operation, "edgeopacity", state->edgesmooth, "value");
 gegl_operation_meta_redirect (operation, "alphadegree", state->ta2, "alphadegree");

} 


static void update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;

switch (o->mode) {
        break;
    case JUSTEXTRACT:
  gegl_node_link_many (state->input, state->c2a, state->c2a2, state->c2a3, state->c2a4,  state->alphalockreplace, state->opacity2, state->string, state->blur,   state->median, state->output, NULL);
  gegl_node_link_many (state->input, state->opacity1,  NULL);
  gegl_node_connect (state->alphalockreplace, "aux", state->opacity1, "output");
        break;
    case TA2MODE:
  gegl_node_link_many (state->input, state->c2a, state->c2a2, state->c2a3, state->c2a4,  state->alphalockreplace, state->opacity2, state->string, state->blur, state->idref, state->replace, state->median, state->output, NULL);
  gegl_node_link_many (state->input, state->opacity1,  NULL);
  gegl_node_connect (state->alphalockreplace, "aux", state->opacity1, "output");
  gegl_node_link_many (state->idref, state->ta2,  NULL);
  gegl_node_connect (state->replace, "aux", state->ta2, "output");
        break;
    case EDGESMOOTHMODE:
  gegl_node_link_many (state->input, state->c2a, state->c2a2, state->c2a3, state->c2a4,  state->alphalockreplace, state->opacity2, state->string, state->blur, state->idref, state->replace, state->median, state->output, NULL);
  gegl_node_link_many (state->input, state->opacity1,  NULL);
  gegl_node_connect (state->alphalockreplace, "aux", state->opacity1, "output");
  gegl_node_link_many (state->idref, state->edgesmooth,  NULL);
  gegl_node_connect (state->replace, "aux", state->edgesmooth, "output");
        break;
    case TA2EDGESMOOTHMODE:
  gegl_node_link_many (state->input, state->c2a, state->c2a2, state->c2a3, state->c2a4, state->alphalockreplace, state->opacity2, state->string, state->blur, state->idref, state->replace, state->median, state->output, NULL);
  gegl_node_link_many (state->input, state->opacity1,  NULL);
  gegl_node_connect (state->alphalockreplace, "aux", state->opacity1, "output");
  gegl_node_link_many (state->idref, state->ta2, state->edgesmooth,  NULL);
  gegl_node_connect (state->replace, "aux", state->edgesmooth, "output");
    }
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
    "name",        "lb:background-remover",
    "title",       _("Remove Backgrounds by Color Range"),
    "reference-hash", "retroplayer1satmorning94playanotherlovesong",
    "description", _("This operation is suppose to be used with Gimp's selection tools and is only meant for simple backgrounds. It evidently works better on removing hair, cat whiskers and other fine structures then anything else; like a normal background. Make sure to use the dedicated mode for extracting hair and other thin structures.  "),
    "gimp:menu-path", "<Image>/Filters/Generic/",
    "gimp:menu-label", _("Extract simple backgrounds by color selection"),
    NULL);
}

#endif
