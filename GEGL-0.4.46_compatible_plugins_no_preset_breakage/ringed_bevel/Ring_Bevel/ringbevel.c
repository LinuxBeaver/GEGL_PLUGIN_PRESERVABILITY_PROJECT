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
 * 2023 Beaver, Ringed Bevel
You can test plugin without installing by pasting this syntax into Gimp's GEGL graph (This graph requires other plugins 'ssg' and 'custom-bevel' this filter ships with those two plugins)

levels out-low=0.00
id=1
dst-out aux=[ ref=1

median-blur radius=5 alpha-percentile=0
lb:ssg radius=0 stroke=7
lb:ssg radius=0 stroke=7
lb:ssg radius=0 stroke=7
lb:ssg radius=0 stroke=7

median-blur radius=0
 ]
lb:custom-bevel depth=4 gaus=1
dst-over aux=[ ref=1 lb:custom-bevel depth=1 gaus=1  ]

--end of syntax--

Fun fact about this plugin

This plugin is meant to be a GEGL counterpart to a script fu named LAYERFX's "bevel and emboss" with its inner bevel on double ring mode.
It isn't as simple as just making this part of my bevel filter, as it has a very different algorithm. Script fu can use selection tools in Gimp
that GEGL can't. That is why GEGL makes this kind of effect radically different, but hey at least it is possible to mimick the double ring inner bevel. :)


This filter is not a carbon copy of script fu "bevel and emboss" double ring. It has its own unique spin. But that is directly the inspiration.
 */


#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES


enum_start (ring_time)
  enum_value (onering,      "onering",
              N_("One Ring (half structure)"))
  enum_value (tworings,      "tworings",
              N_("Two Rings (one structure)"))
  enum_value (threerings,      "threerings",
              N_("Three Rings (one and a half structures)"))
  enum_value (fourrings,      "fourrings",
              N_("Four Rings (two structures)"))
enum_end (ringtime)
/* This ENUM list has a unique name to avoid conflicts with other plugins or Gimp filters*/

property_enum (rings, _("Amount of rings:"),
    ringtime, ring_time,
    fourrings)
   description  (_("Choose the amount of rings 1-4"))


property_int  (bevelsize, _("Size of the bevel"), 3)
  value_range (1, 9)
  ui_range    (1, 9)
  ui_meta     ("unit", "pixel-distance")
  description (_("Median radius to control the size of the bevel"))


property_int  (ringsize, _("Ring size"), 6)
  value_range (4, 9)
  ui_range    (4, 9)
  ui_meta     ("unit", "pixel-distance")
  description (_("Control the size of the rings. This also creates gaps when bottom bevel's opacity is low."))


property_double (azimuth, _("Azimuth of the bevel"), 67.0)
    description (_("Light angle (degrees)"))
    value_range (20, 90)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")

property_double (elevation, _("Elevation of the bevel"), 35.0)
    description (_("Elevation angle (degrees). This appears to rotate the brightest pixels."))
    value_range (7, 70)
    ui_meta ("unit", "degree")

property_int (depth, _("Depth and or detail of the bevel"), 15)
    description (_("Brings out depth and or detail of the bevel depending on the blend mode"))
    value_range (4, 65)

enum_start (under_bevel_blend_mode)
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHT, "Hardlight",
              N_("HardLight"))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLY,      "Multiply",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_COLORDODGE,      "ColorDodge",
              N_("ColorDodge"))
  enum_value (GEGL_BLEND_MODE_TYPE_PLUS,      "Plus",
              N_("Plus"))
  enum_value (GEGL_BLEND_MODE_TYPE_DARKEN,      "Darken",
              N_("Darken"))
  enum_value (GEGL_BLEND_MODE_TYPE_LIGHTEN,      "Lighten",
              N_("Lighten"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAY,      "Overlay",
              N_("Overlay"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGE,      "GrainMerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOFTLIGHT,      "Softlight",
              N_("Soft Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITION,      "Addition",
              N_("Addition"))
enum_end (underbevelblendmode)

property_double (bottombevelopacity, _("Opacity of the bottom bevel."), 1.0)
    description (_("Opacity of the bottom bevel. 0 disables it entirely"))
    value_range (0.0, 1.0)


property_enum (blendbevel, _("Under bevel blend mode"),
    underbevelblendmode, under_bevel_blend_mode,
    GEGL_BLEND_MODE_TYPE_HARDLIGHT)
  description (_("Blend mode of the bevel under the rings"))


enum_start (ring_bevel_blend_mode)
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHTX, "Hardlight",
              N_("HardLight"))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLYX,      "Multiply",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_COLORDODGEX,      "ColorDodge",
              N_("ColorDodge"))
  enum_value (GEGL_BLEND_MODE_TYPE_PLUSX,      "Plus",
              N_("Plus"))
  enum_value (GEGL_BLEND_MODE_TYPE_DARKENX,      "Darken",
              N_("Darken"))
  enum_value (GEGL_BLEND_MODE_TYPE_LIGHTENX,      "Lighten",
              N_("Lighten"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAYX,      "Overlay",
              N_("Overlay"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEX,      "GrainMerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOFTLIGHTX,      "Softlight",
              N_("Soft Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITIONX,      "Addition",
              N_("Addition"))
enum_end (ringbevelblendmode)

property_enum (blendrings, _("Ring bevel blend mode"),
    ringbevelblendmode, ring_bevel_blend_mode,
    GEGL_BLEND_MODE_TYPE_HARDLIGHTX)
  description (_("Blend mode of the ring bevel, but it also appears to effect the bevel below it."))



enum_start (shape_of_ring)
  enum_value (GEGL_MEDIAN_BLUR_NEIGHBORHOOD_SQUARE,  "square",  N_("Square"))
  enum_value (GEGL_MEDIAN_BLUR_NEIGHBORHOOD_CIRCLE,  "circle",  N_("Circle"))
  enum_value (GEGL_MEDIAN_BLUR_NEIGHBORHOOD_DIAMOND, "diamond", N_("Diamond"))
enum_end (shapeofring)

property_enum (ringshape, _("Shape of ring"),
    shapeofring, shape_of_ring,
               GEGL_MEDIAN_BLUR_NEIGHBORHOOD_CIRCLE)
  description (_("Shape of the Rings that internally uses Median Blur's neighborhood setting (square, circle, diamond)"))


property_double (darkbevelslider, _("Dark color bevel fix"), 0.0)
    description (_("Elevation "))
    value_range (0.0, 0.12)
    ui_meta ("unit", "degree")
  description (_("Slide up if the Bevel is a dark color and the dark color will conform to the bevel. If not the dark color will not look proper. If this slider is up on a light color then it will modify it in a potential unwanted way."))

property_file_path(imageupload, _("Image Overlay (use grayscale text)"), "")
    description (_("Upload an image file. Only accepts (png, jpg, raw, svg, bmp, tif, ... If text isn't white this won't work proper.)"))


property_boolean (colormode, _("Recolor bevel"), FALSE)
  description    (_("Recolor Bevel with whatever color you want. By default this is disabled."))

property_color  (color, _("Color of bevel"), "#ff9f00")
    description (_("Optional Color Overlay for Ringed Bevel. Is only enabled if its checkbox is on."))
  ui_meta     ("sensitive", "colormode")




#else

#define GEGL_OP_META
#define GEGL_OP_NAME     ringbevel
#define GEGL_OP_C_SOURCE ringbevel.c

#include "gegl-op.h"

typedef struct
{
 GeglNode *input;
 GeglNode *levels;
 GeglNode *erase; 
 GeglNode *s1;
 GeglNode *s2;
 GeglNode *s3;
 GeglNode *s4;   
 GeglNode *nop1;
 GeglNode *nop2;
 GeglNode *nop3;
 GeglNode *nop4;        
 GeglNode *bevel;
 GeglNode *behind;
 GeglNode *median;
 GeglNode *mediandictator;
 GeglNode *bevel2; 
 GeglNode *over; 
 GeglNode *idref; 
 GeglNode *fix; 
 GeglNode *color;
 GeglNode *image;
 GeglNode *idrefimage;
 GeglNode *multiply;
 GeglNode *bevelopacity;     
 GeglNode *output;
}State;


static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);

  State *state = o->user_data = g_malloc0 (sizeof (State));


  state->input    = gegl_node_get_input_proxy (gegl, "input");
  state->output   = gegl_node_get_output_proxy (gegl, "output");

  state->image = gegl_node_new_child (gegl,
                                  "operation", "gegl:layer", 
                                  NULL);

  state->bevelopacity = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity", 
                                  NULL);

  state->idrefimage = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop", 
                                  NULL);

  state->multiply = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply", 
                                  NULL);

  state->bevel = gegl_node_new_child (gegl,
                                  "operation", "lb:custom-bevel", "depth", 4.0, "gaus", 1.0, 
                                  NULL);


  state->bevel2 = gegl_node_new_child (gegl,
                                  "operation", "lb:custom-bevel", "depth", 1.0, "gaus", 1.0, 
                                  NULL);

  state->mediandictator = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "alpha-percentile", 100.0,  
                                  NULL);

  state->erase = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst-out", 
                                  NULL);

  state->color = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay", 
                                  NULL);


  state->behind = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst-over", 
                                  NULL);

  state->idref = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop", 
                                  NULL);

  state->nop1 = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop", 
                                  NULL);

  state->nop2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop", 
                                  NULL);

  state->nop3 = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop", 
                                  NULL);

  state->nop4 = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop", 
                                  NULL);

  state->fix = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 0, 
                                  NULL);

  state->levels = gegl_node_new_child (gegl,
                                  "operation", "gegl:levels",
                                  NULL);

  state->s1 = gegl_node_new_child (gegl,
                                  "operation", "lb:ssg", "radius", 0, "stroke", 7.0,  
                                  NULL);

  state->s2 = gegl_node_new_child (gegl,
                                  "operation", "lb:ssg", "radius", 0, "stroke", 7.0,  
                                  NULL);

  state->s3 = gegl_node_new_child (gegl,
                                  "operation", "lb:ssg", "radius", 0, "stroke", 7.0,  
                                  NULL);

  state->s4 = gegl_node_new_child (gegl,
                                  "operation", "lb:ssg", "radius", 0, "stroke", 7.0,  
                                  NULL);


  state->median = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 5, "alpha-percentile", 0.0,  
                                  NULL);


/*This is a median blur's radius being called to make the bevel larger, I can't use custom bevel's size because that doesn't work in enlarging both custom bevels*/
    gegl_operation_meta_redirect (operation, "bevelsize", state->mediandictator, "radius");

/*These list the azimuth, elevation and depth of my custom bevel plugin.*/
    gegl_operation_meta_redirect (operation, "azimuth", state->bevel, "azimuth");
    gegl_operation_meta_redirect (operation, "elevation", state->bevel, "elevation");
    gegl_operation_meta_redirect (operation, "depth", state->bevel, "depth");

/*This controls the size of ALL ssg nodes*/
    gegl_operation_meta_redirect (operation, "ringsize", state->s1, "stroke");
    gegl_operation_meta_redirect (operation, "ringsize", state->s2, "stroke");
    gegl_operation_meta_redirect (operation, "ringsize", state->s3, "stroke");
    gegl_operation_meta_redirect (operation, "ringsize", state->s4, "stroke");

/*This controls the blend mode of the bevel and the rings*/
    gegl_operation_meta_redirect (operation, "blendrings", state->bevel, "blendmode");
    gegl_operation_meta_redirect (operation, "blendbevel", state->bevel2, "blendmode");


/*This controls the shape of the rings. It is lb:ssg grow-shape=square,circle,diamond*/
    gegl_operation_meta_redirect (operation, "ringshape", state->s1, "grow-shape");
    gegl_operation_meta_redirect (operation, "ringshape", state->s2, "grow-shape");
    gegl_operation_meta_redirect (operation, "ringshape", state->s3, "grow-shape");
    gegl_operation_meta_redirect (operation, "ringshape", state->s4, "grow-shape");

/*Slide up if Bevel color is dark. This allows the bevel to work on dark colors*/
    gegl_operation_meta_redirect (operation, "darkbevelslider", state->levels, "out-low");

/*Recolor node that only kicks in with the checkbox*/
    gegl_operation_meta_redirect (operation, "color", state->color, "value");

/*Image upload option*/
    gegl_operation_meta_redirect (operation, "imageupload", state->image, "src");

/*Disable bottom bevel (bevel 2)*/
    gegl_operation_meta_redirect (operation, "bottombevelopacity", state->bevelopacity, "value");

/*Let's analysis what is going on below

The same idref is called 3 times. The first is a bookmark (the id=bookmark) the second is a (ref=bookmark) inside the erase blend mode. THe third is a (ref=bookmark) inside the behind blend mode. 
Remember. idref is a gegl:nop (no operation). 

There are two blend modes being called "erase blend mode" gegl:dst-out, and the behind blend mode and "gegl:dst-over" The blend modes that the bevels call are baked inside custom-bevel's .c file 
not this. They are being called by bevel1 and bevel2, but so is every other property name of custom bevel.

Fix is a median blur radius=0 at the end that resets internal values in the graph from a known bug where the drop shadow filter glitches out. median blur at 0 makes 0 changes to an image. 


This shows all the nodes and composers "erase" and "behind"
  gegl_node_link_many (state->input, state->mediandictator, state->levels, state->idref, state->erase,  state->bevel, state->behind, state->output, NULL);


This is the content inside the erase blend mode. Their can be up to 3 more SSGs but this graph only has 1; s1.  

  gegl_node_link_many (state->idref, state->median, state->s1, (potentially three other SSGs) state->fix, NULL);
  gegl_node_connect (state->erase, "aux", state->fix, "output");



This is the content inside the behind blend mode.
  gegl_node_link_many (state->idref, state->bevel2,   NULL);
  gegl_node_connect (state->behind, "aux", state->bevel2, "output");

*/

} 

static void update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;

if (o->colormode) 

switch (o->rings) {
        break;
    case onering:
  gegl_node_link_many (state->input, state->color, state->mediandictator, state->levels, state->idref, state->erase,  state->bevel, state->behind,   state->multiply,  state->output, NULL);
  gegl_node_link_many (state->idref, state->median, state->s1, state->fix, NULL);
  gegl_node_connect (state->erase, "aux", state->fix, "output");
  gegl_node_link_many (state->idref, state->bevel2, state->bevelopacity,   NULL);
  gegl_node_connect (state->behind, "aux", state->bevelopacity, "output");
  gegl_node_connect (state->multiply, "aux", state->image, "output");
        break;
    case tworings:
  gegl_node_link_many (state->input, state->color, state->mediandictator,  state->levels, state->idref, state->erase, state->bevel,  state->behind, state->multiply, state->output, NULL);
  gegl_node_link_many (state->idref, state->median, state->s1, state->s2,   state->fix, NULL);
  gegl_node_connect (state->erase, "aux", state->fix, "output");
  gegl_node_link_many (state->idref, state->bevel2, state->bevelopacity,   NULL);
  gegl_node_connect (state->behind, "aux", state->bevelopacity, "output");
  gegl_node_connect (state->multiply, "aux", state->image, "output");
        break;
    case threerings:
  gegl_node_link_many (state->input, state->color, state->mediandictator, state->levels, state->idref, state->erase,   state->bevel, state->behind,  state->multiply, state->output, NULL);
  gegl_node_link_many (state->idref, state->median, state->s1, state->s2, state->s3,   state->fix, NULL);
  gegl_node_connect (state->erase, "aux", state->fix, "output");
  gegl_node_link_many (state->idref, state->bevel2, state->bevelopacity,   NULL);
  gegl_node_connect (state->behind, "aux", state->bevelopacity, "output");
  gegl_node_connect (state->multiply, "aux", state->image, "output");
        break;
    case fourrings:
  gegl_node_link_many (state->input, state->color, state->mediandictator, state->levels, state->idref, state->erase,   state->bevel,  state->behind,   state->multiply, state->output, NULL);
  gegl_node_link_many (state->idref, state->median, state->s1, state->s2, state->s3, state->s4,  state->fix, NULL);
  gegl_node_connect (state->erase, "aux", state->fix, "output");
  gegl_node_link_many (state->idref, state->bevel2, state->bevelopacity,   NULL);
  gegl_node_connect (state->behind, "aux", state->bevelopacity, "output");
  gegl_node_connect (state->multiply, "aux", state->image, "output");

    }

else 

switch (o->rings) {
        break;
    case onering:
  gegl_node_link_many (state->input, state->mediandictator, state->levels, state->idref, state->erase,  state->bevel, state->behind,  state->multiply,  state->output, NULL);
  gegl_node_link_many (state->idref, state->median, state->s1, state->fix, NULL);
  gegl_node_connect (state->erase, "aux", state->fix, "output");
  gegl_node_link_many (state->idref, state->bevel2, state->bevelopacity,   NULL);
  gegl_node_connect (state->behind, "aux", state->bevelopacity, "output");
  gegl_node_connect (state->multiply, "aux", state->image, "output");
        break;
    case tworings:
  gegl_node_link_many (state->input, state->mediandictator,  state->levels, state->idref, state->erase, state->bevel, state->behind, state->multiply,  state->output, NULL);
  gegl_node_link_many (state->idref, state->median, state->s1, state->s2, state->fix, NULL);
  gegl_node_connect (state->erase, "aux", state->fix, "output");
  gegl_node_link_many (state->idref, state->bevel2, state->bevelopacity,   NULL);
  gegl_node_connect (state->behind, "aux", state->bevelopacity, "output");
  gegl_node_connect (state->multiply, "aux", state->image, "output");
        break;
    case threerings:
  gegl_node_link_many (state->input, state->mediandictator, state->levels, state->idref, state->erase,   state->bevel, state->behind,  state->multiply, state->output, NULL);
  gegl_node_link_many (state->idref, state->median, state->s1, state->s2, state->s3,   state->fix, NULL);
  gegl_node_connect (state->erase, "aux", state->fix, "output");
  gegl_node_link_many (state->idref, state->bevel2, state->bevelopacity,   NULL);
  gegl_node_connect (state->behind, "aux", state->bevelopacity, "output");
  gegl_node_connect (state->multiply, "aux", state->image, "output");
        break;
    case fourrings:
  gegl_node_link_many (state->input, state->mediandictator, state->levels, state->idref, state->erase,   state->bevel, state->behind,  state->multiply, state->output, NULL);
  gegl_node_link_many (state->idref, state->median, state->s1, state->s2, state->s3, state->s4,   state->fix, NULL);
  gegl_node_connect (state->erase, "aux", state->fix, "output");
  gegl_node_link_many (state->idref, state->bevel2, state->bevelopacity,   NULL);
  gegl_node_connect (state->behind, "aux", state->bevelopacity, "output");
  gegl_node_connect (state->multiply, "aux", state->image, "output");


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
    "name",        "lb:ringbevel",
    "title",       _("Ring Bevel"),
    "reference-hash", "r34jghavetheringrightherejfj3",
    "description", _("Make a Ringed Bevel effect"),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Ringed Bevel..."),
    NULL);
}

#endif
