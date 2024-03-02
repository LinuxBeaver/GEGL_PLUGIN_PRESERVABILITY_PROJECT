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
 * 2023, - Pixel Text, Beaver


Test this filter without installing by pasting this syntax into Gimp's GEGL graph. This filter works best with an image file upload


id=0 src-atop aux=[ ref=0 layer src=/home/USERNAME/Pictures/Magic/pattern_fills/pattern_plaid-pattern-2_1_3_0-0_0_1__ff8ab3_00e1ff.png   ]

pixelize size-x=14 size-y=14
median-blur radius=0
 id=1 dst-over aux=[ ref=1 translate x=5 y=4 levels out-high=0.2 saturation scale=1
id=x multiply aux=[ ref=x emboss depth=40 elevation=10] ]
gegl:opacity value=2
median-blur radius=0
 */


#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

enum_start (color_mode)
   enum_value (NOP_COLOR,    "nocolor",    N_("Original color mode"))
   enum_value (COLOR_ANY,  "color",  N_("Color overlay mode"))
enum_end (colormode)


property_enum (colorpolicy, _("Color Policy:"),
               colormode, color_mode,
               NOP_COLOR)
  description (_("Setting to keep the original image color or replace the original color with any color you want."))


property_color  (color, _("Color"), "#fe7e00")
ui_meta ("visible", "colorpolicy {color}")


/* This is Properties (part 1) where properties are shown in the GUI based on their definitions in Properties (part two)*/

property_int (pixelize, _("Control the Pixelize Effect"), 6)
   description (_("Pixelization of the text from gegl:pixelize ."))
   value_range (1, 25)
   ui_gamma    (3.0)

property_int (depth, _("Pixel Shadow Emboss Depth"), 40)
   description (_("Emboss Depth for the emboss effecting the text."))
   value_range (1, 60)
   ui_range    (1, 60)
   ui_gamma    (3.0)

property_double (elevation, _("Pixel Shadow Emboss Elevation"), 14.0)
   description (_("Emboss Elevation for the emboss effecting the text."))
   value_range (0.0, 60.0)
   ui_range    (0.0, 60.0)
   ui_gamma    (3.0)

property_double (x, _("X Pixel Shadow"), 5.0)
    description (_("Horizontal Movability of the Pixel Text's opaque shadow"))
    ui_range (-6.0, 6.0)
    ui_meta ("unit", "pixel-distance")
    ui_meta ("axis", "x")

property_double (y, _("Y Pixel Shadow"), 4.0)
    description (_("Vertical Movability of the Pixel Text's opaque shadow"))
    ui_range (-6.0, 6.0)
    ui_meta ("unit", "pixel-distance")
    ui_meta ("axis", "y")

property_file_path(image, _("Image overlay"), "")
    description (_("Optional image file overlay to be put over the text. Source image file path (png, jpg, raw, svg, bmp, tif, ...)"))

enum_start (emboss_blend_pixel)
   enum_value (BLEND_MODE_TYPE_OVERLAY,    "overlay",    N_("Overlay"))
   enum_value (BLEND_MODE_TYPE_MULTIPLY,  "multiply",  N_("Multiply"))
enum_end (embossblendpixel)

property_enum (switchblendmode, _("Blend mode of Pixel Text:"),
               embossblendpixel, emboss_blend_pixel,
               BLEND_MODE_TYPE_OVERLAY)
  description (_("Blend mode of pixel text filter"))


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     pixel_text
#define GEGL_OP_C_SOURCE pixel_text.c

#include "gegl-op.h"

/*This is where the nodes are listed without any properties defined. They do not have to be listed in correct order. */
typedef struct
{
 GeglNode *input;
 GeglNode *output;
 GeglNode *pixel;
 GeglNode *nopcolor;
 GeglNode *color;
 GeglNode *fix;
 GeglNode *fix2;
 GeglNode *behind;
 GeglNode *alphalock;
 GeglNode *offset;
 GeglNode *levels;
 GeglNode *saturation;
 GeglNode *multiply;
 GeglNode *overlay;
 GeglNode *idref1;
 GeglNode *idref2;
 GeglNode *opacity;
 GeglNode *emboss;
 GeglNode *layer;
}State;

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);

  State *state = o->user_data = g_malloc0 (sizeof (State));

  state->input    = gegl_node_get_input_proxy (gegl, "input");
  state->output   = gegl_node_get_output_proxy (gegl, "output");

/*This is where the nodes are listed (again) with the option to define some properties, They do not have to be listed in correct order*/

/*This node is Gimp's pixelize filter and does most of the work in making the text style"*/
  state->pixel = gegl_node_new_child (gegl,
                                  "operation", "gegl:pixelize",
                                  NULL);

/*These two fix nodes are median blur's at 0 radius that fix a bug relating to gegl:opacity corrupting the alpha channel"*/
  state->fix = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 0,
                                  NULL);

  state->fix2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 0,
                                  NULL);

/*DST Over is a GEGL blend mode that does the same thing as Gimp's behind blend mode."*/

  state->behind = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst-over",
                                  NULL);

/*SRC-ATOP is a GEGL exclusive blend mode that does something LIKE Gimp's "lock alpha channel" option. This blend mode is unique to GEGL"*/

  state->alphalock = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);

/*This node "translate" is a gegl move tool similar to "offset"*/

  state->offset = gegl_node_new_child (gegl,
                                  "operation", "gegl:translate",
                                  NULL);

  state->levels = gegl_node_new_child (gegl,
                                  "operation", "gegl:levels", "out-high", 0.2,
                                  NULL);

  state->saturation = gegl_node_new_child (gegl,
                                  "operation", "gegl:saturation", "scale", 1.1,
                                  NULL);

/*These two nodes multiply and overlay will be called to replace "blendmodechange"*/
  state->multiply = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply", 
                                  NULL);

  state->overlay = gegl_node_new_child (gegl,
                                  "operation", "gegl:overlay", 
                                  NULL);
/*These two nop nodes function as independent id and refs*/


  state->idref1 = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  state->idref2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  state->nopcolor = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  state->color = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);

  state->emboss = gegl_node_new_child (gegl,
                                  "operation", "gegl:emboss",
                                  NULL);

  state->layer = gegl_node_new_child (gegl,
                                  "operation", "gegl:layer",
                                  NULL);

  state->opacity = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity", "value", 2.0,
                                  NULL);

/* This is Properties (part 2) where properties are defined

These are sliders to control the pixelize x and y sliders as one.*/
 gegl_operation_meta_redirect (operation, "pixelize", state->pixel, "size-x"); 
 gegl_operation_meta_redirect (operation, "pixelize", state->pixel, "size-y"); 
 gegl_operation_meta_redirect (operation, "depth", state->emboss, "depth"); 
 gegl_operation_meta_redirect (operation, "elevation", state->emboss, "elevation"); 
 gegl_operation_meta_redirect (operation, "x", state->offset, "x"); 
 gegl_operation_meta_redirect (operation, "y", state->offset, "y"); 
 gegl_operation_meta_redirect (operation, "image", state->layer, "src"); 
 gegl_operation_meta_redirect (operation, "color", state->color, "value"); 
} 


static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;

/* This is where the stateless blendmodechange gets swapped for multiply or overlay.
"switchblendmode will be its property name in the GUI" */
  GeglNode *blendmodechange = state->multiply; /* the default node is multiply  */
  switch (o->switchblendmode) {
    case BLEND_MODE_TYPE_OVERLAY: blendmodechange = state->overlay; break;
    case BLEND_MODE_TYPE_MULTIPLY: blendmodechange = state->multiply; break;
default: blendmodechange = state->overlay;

 }

/* This is where the stateless colorsetting gets swapped for default color mode or any color mode.*/
  GeglNode *colorsetting = state->nopcolor;
  switch (o->colorpolicy) {
    case NOP_COLOR: colorsetting = state->nopcolor; break;
    case COLOR_ANY: colorsetting = state->color; break;
default: colorsetting = state->nopcolor;
}


/* This is the GEGL Graph itself and nodes do have to be in the correct order.

"blendmodechange" lacks a state-> because it will be either or blend mode we want (overlay or multiply)*/
  gegl_node_link_many (state->input, colorsetting, state->alphalock, state->pixel, state->fix, state->idref1, state->behind, state->idref2, blendmodechange, state->opacity, state->fix2, state->output, NULL);

  gegl_node_link_many (state->idref2, state->emboss,  NULL);
/*idref2 also know as gegl:nop connects to emboss because it is the last node in the second link_many. It is sending a image of the graph's current state to be embossed. That is why it is seen
before and after "blend mode change"*/
  gegl_node_connect (blendmodechange, "aux", state->emboss, "output");
/*blendmodechange the only node without a state-> is connecting to emboss so it can be a blend mode switcher of either "gegl:overlay or gegl:multiply"*/
  gegl_node_connect (blendmodechange, "aux", state->emboss, "output");
/*The lock alpha channel blend mode (gegl:src-atop) is connecting to gegl:layer src=imagepathhere.jpg for an image file upload ability*/
  gegl_node_connect (state->alphalock, "aux", state->layer, "output");
/*Idref1 (The first gegl:nop) exist to make a duplicate of pixelize's effect and put it behind the original image and then tweak it with levels and saturation to darken it.
It connects to levels because that is the last node.*/
  gegl_node_link_many (state->idref1,  state->offset, state->saturation, state->levels,  NULL);
  gegl_node_connect (state->behind, "aux", state->levels, "output");
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
/* This is where the operation name, title name, description, menu path and menu label go.*/
    "name",        "lb:pixel-text",
    "title",       _("Pixel Text"),
    "reference-hash", "action52avgnmeme",
    "description", _("Make retro gaming text"),
    "gimp:menu-path", "<Image>/Filters/Text Styling/",
    "gimp:menu-label", _("Pixel Text..."),
    NULL);
}

#endif
