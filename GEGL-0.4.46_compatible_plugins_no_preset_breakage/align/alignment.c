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
 * 2023 Beaver, Allignment Tool (BTW, this is my 70th GEGL plugin)

Test this filter without installing by pasting this GEGL Syntax inside Gimp's GEGL Graph filter


id=in 
color-overlay value=#ffffff dst-over aux=[ color value=#000000  ]   gimp:layer-mode layer-mode=color-erase opacity=1.00 aux=[ color value=#ffffff ]  crop 
xor aux=[
ref=in scale-ratio x=0.80 y=0.80 sampler=nohalo

 border-align x=0.5 y=0.5 
]


end of syntax


 */

#include "config.h"
#include <glib/gi18n-lib.h>


#ifdef GEGL_PROPERTIES

enum_start (scale_mode)
   enum_value (NOHALO,    "nohalo",    N_("No Halo"))
   enum_value (LOHALO,  "lohalo",  N_("Lo Halo"))
enum_end (scalemode)

property_enum (algorithm, _("Scale Mode"),
               scalemode, scale_mode,
               NOHALO)
  description (_("Select a scale algorithm. No Halo works best on scaling down images less then half and LoHalo works best on scaling down images more then half."))


property_double (scale, _("Scale"), 1.00)
    description (_("Scale Image. 1.00 is the default full size.")) 
    value_range (0.05, 1.0)
    ui_range    (0.05, 1.0)
  ui_steps      (0.1, 0.5)

property_double (x, _("Allign Amount Horizontal"), 0.5)
    description (_("Horizontal Allignment. 0.0 is the left border, 0.5 is center, and 1.0 is the right border.")) 
    value_range (0.0, 1.0)
    ui_range    (0.0, 1.0)
  ui_steps      (0.1, 0.5)

property_double (y, _("Allign Amount Vertical"), 0.5)
    description (_("Vertical Allignment. 0.0 is the left border, 0.5 is center, and 1.0 is the right border.")) 
    value_range (0.0, 1.0)
    ui_range    (0.0, 1.0)
  ui_steps      (0.1, 0.5)

/*
property_double (rotate, _("Rotate on Center"), 0.0)
    description(_("Angle to rotate (counter-clockwise)"))
    ui_range (-180.0, 180.0) */

/*This can be made normal code to restore the rotate on center option I decided to cancel.*/
#else

#define GEGL_OP_META
#define GEGL_OP_NAME     alignment
#define GEGL_OP_C_SOURCE alignment.c

#include "gegl-op.h"

typedef struct
{
 GeglNode *input;
 GeglNode *invertstring;
 GeglNode *erase;
 GeglNode *idref;
 GeglNode *idref2;
 GeglNode *scale1;
 GeglNode *scale2;
 GeglNode *align;
 GeglNode *alphalockreplace;
 GeglNode *nop;
 GeglNode *crop;
 GeglNode *rotate;
 GeglNode *output;
}State;

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);

  State *state = o->user_data = g_malloc0 (sizeof (State));

  state->input    = gegl_node_get_input_proxy (gegl, "input");
  state->output   = gegl_node_get_output_proxy (gegl, "output");

#define invert_transparency \
" color-overlay value=#ffffff dst-over aux=[ color value=#000000  ]   color-to-alpha  color=#ffffff  transparency-threshold=1 src-in aux=[ color value=#ffffff ]  crop   "\
/*This syntax string inverts transparency. NOTE I ALSO HAVE A PLUGIN THAT DOES THIS BUT I CHOOSE NOT TO SHIP WITH IT.*/ 

state->invertstring = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", invert_transparency,
                                  NULL);

  state->erase = gegl_node_new_child (gegl,
                                  "operation", "gegl:xor", 
                                  NULL);


  state->nop = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop", 
                                  NULL);

  state->alphalockreplace = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-in",  /*This is a GEGL exclusive blend mode that fuctions like Gimp's "lock alpha channel" and replace blend mode fused together.*/
                                  NULL);

  state->idref = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop", 
                                  NULL);

  state->idref2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop", 
                                  NULL);

  state->rotate = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop", /* "gegl:rotate-on-center", "sampler", 3, was the original node*/
                                  NULL);

  state->crop = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop", 
                                  NULL);

  state->scale1 = gegl_node_new_child (gegl,
                                  "operation", "gegl:scale-ratio", "sampler", 3, NULL); /*nohalo*/

  state->scale2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:scale-ratio", "sampler", 4, NULL); /*lohalo*/

  state->align = gegl_node_new_child (gegl,
                                  "operation", "gegl:border-align", 
                                  NULL);

 gegl_operation_meta_redirect (operation, "x", state->align,  "x"); 
 gegl_operation_meta_redirect (operation, "y", state->align,  "y"); 
 gegl_operation_meta_redirect (operation, "scale", state->scale1, "x"); 
 gegl_operation_meta_redirect (operation, "scale", state->scale1, "y"); 
 gegl_operation_meta_redirect (operation, "scale", state->scale2, "x"); 
 gegl_operation_meta_redirect (operation, "scale", state->scale2, "y"); 
  /* gegl_operation_meta_redirect (operation, "rotate", state->rotate, "degrees"); 
In early developing this filter had a rotate option that was scrapped */

} 

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;


  GeglNode *scale = state->scale1; /* the default */
  switch (o->algorithm) {
    case NOHALO: scale = state->scale1; break;
    case LOHALO: scale = state->scale2; break;
default: scale = state->scale1;

}
  gegl_node_link_many (state->input, state->idref2, state->alphalockreplace, state->crop, state->output,  NULL);
  gegl_node_connect (state->alphalockreplace, "aux", state->erase, "output");
  gegl_node_link_many (state->idref2, state->idref, state->invertstring, state->erase,  NULL);
  gegl_node_connect (state->erase, "aux", state->align, "output");
  gegl_node_link_many (state->idref,  scale, state->rotate, state->align,  NULL);
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
    "name",        "lb:align",
    "title",       _("Alignment Tool"),
    "reference-hash", "lookouthereitcomesauditnumber9",
    "description", _("Do an alignment with GEGL. (SELECT CLIP (clipping option) or this filter will have a glitch. "),
    "gimp:menu-path", "<Image>/Tools/Transform Tools",
    "gimp:menu-label", _("Align Image filter(use second clipping option)"),
    NULL);
}

#endif
