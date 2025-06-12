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
 * 2022 Beaver Color fill Trail
 */

/* 
GEGL Graph recreation -it isn't really impossible replicate its movability function in a graph 
as all five xy sliders are moving in synch.  You'd have to update all the xy=0's.

id=1
dst-over aux=[ ref=1
dropshadow x=0 y=0 radius=1 grow-radius=10 opacity=1 radius=0 color=#fd0002
dropshadow x=0 y=0 radius=1 grow-radius=10 opacity=1 radius=0 color=#fe7e00
dropshadow x=0 y=0 radius=1 grow-radius=10 opacity=1 radius=0 color=#ffff01
dropshadow x=0 y=0 radius=1 grow-radius=10 opacity=1 radius=0 color=#00fe01
dropshadow x=0 y=0 radius=1 grow-radius=10 opacity=1 radius=0 color=#00fe01
median-blur 
 ]
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

enum_start (gegl_dropshadow_grow_shapect)
  enum_value (GEGL_DROPSHADOW_GROW_SHAPE_SQUARECT,  "squarect",  N_("Square"))
  enum_value (GEGL_DROPSHADOW_GROW_SHAPE_CIRCLECT,  "circlect",  N_("Circle"))
  enum_value (GEGL_DROPSHADOW_GROW_SHAPE_DIAMONDCT, "diamondct", N_("Diamond"))
enum_end (GeglDropshadowGrowShapect)

property_enum   (shape, _("Grow shape"),
                 GeglDropshadowGrowShapect, gegl_dropshadow_grow_shapect,
                 GEGL_DROPSHADOW_GROW_SHAPE_CIRCLECT)
  description   (_("The shape to expand or contract the shadow in"))


property_double (x, _("X"), 13.0)
  description   (_("Horizontal shadow offset"))
  ui_range      (-300, 300.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "x")

property_double (y, _("Y"), 5.40)
  description   (_("Vertical shadow offset"))
  ui_range      (-300.0, 300.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "y")



property_double (gradius, _("Grow radius"), 0.0)
  value_range   (0.0, 25.0)
  ui_range      (00.0,25.0)
  ui_digits     (0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the shadow before blurring; a negative value will contract the shadow instead"))

property_color  (color, _("Color"), "#fd0002")

property_double (opacity, _("Opacity"), 1)
  value_range   (0.0, 1.0)
  ui_steps      (0.50, 1.0)

property_color  (color2, _("Color2"), "#fe7e00")

property_double (opacity2, _("Opacity2"), 1)
  value_range   (0.0, 1.0)
  ui_steps      (0.50, 1.0)

property_color  (color3, _("Color3"), "#ffff01")

property_double (opacity3, _("Opacity3"), 1)
  value_range   (0.0, 1.0)
  ui_steps      (0.50, 1.0)

property_color  (color4, _("Color4"), "#00fe01")

property_double (opacity4, _("Opacity4"), 1)
  value_range   (0.0, 1.0)
  ui_steps      (0.50, 1.0)

property_color  (color5, _("Color5"), "#00feff")

property_double (opacity5, _("Opacity5"), 1)
  value_range   (0.0, 1.0)
  ui_steps      (0.50, 1.0)


property_int  (radius, _("Median Blur to treat off color pixel outline bug"), 2)
  value_range (0, 4)
  ui_range    (0, 4)
  ui_meta     ("unit", "pixel-distance")
  description (_("Neighborhood radius, a negative value will calculate with inverted percentiles"))


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     ctrail
#define GEGL_OP_C_SOURCE ctrail.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *behind, *f1, *f2, *f3, *f4, *f5, *crop1, *crop2, *crop3, *crop4, *crop5, *median;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");

  f1    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow", "radius", 0.0,
                                  NULL);

  f2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow", "radius", 0.0,
                                  NULL);

  f3    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow", "radius", 0.0,
                                  NULL);

  f4    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow", "radius", 0.0,
                                  NULL);

  f5    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow", "radius", 0.0,
                                  NULL);



  median    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "alpha-percentile", 50.0,
                                  NULL);

  behind    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst-over",
                                  NULL);

  crop1   = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop",
                                  NULL);

  crop2   = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop",
                                  NULL);

  crop3   = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop",
                                  NULL);


  crop4   = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop",
                                  NULL);

  crop5   = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop",
                                  NULL);

/* The excessive GEGL crops are used after each gegl:color-overlay; which is internal to gegl:dropshadow. They are known to prevent a bug. 
Maybe they are useless here but without them there is a risk of the delayed color update bug. */



  gegl_node_link_many (input, behind, output, NULL);

  gegl_node_link_many (input, f1, crop1, f2, crop2, f3, crop3, f4, crop4, f5, crop5, median, NULL);
  gegl_node_connect (behind, "aux", median, "output");

  gegl_operation_meta_redirect (operation, "shape", f1, "grow-shape");
  gegl_operation_meta_redirect (operation, "shape", f2, "grow-shape");
  gegl_operation_meta_redirect (operation, "shape", f3, "grow-shape");
  gegl_operation_meta_redirect (operation, "shape", f4, "grow-shape");
  gegl_operation_meta_redirect (operation, "shape", f5, "grow-shape");
  gegl_operation_meta_redirect (operation, "gradius", f1, "grow-radius");
  gegl_operation_meta_redirect (operation, "gradius", f2, "grow-radius");
  gegl_operation_meta_redirect (operation, "gradius", f3, "grow-radius");
  gegl_operation_meta_redirect (operation, "gradius", f4, "grow-radius");
  gegl_operation_meta_redirect (operation, "gradius", f5, "grow-radius");
  gegl_operation_meta_redirect (operation, "opacity", f1,"opacity");
  gegl_operation_meta_redirect (operation, "opacity2", f2,"opacity");
  gegl_operation_meta_redirect (operation, "opacity3", f3,"opacity");
  gegl_operation_meta_redirect (operation, "opacity4", f4,"opacity");
  gegl_operation_meta_redirect (operation, "opacity5", f5,"opacity");
  gegl_operation_meta_redirect (operation, "color", f1, "color");
  gegl_operation_meta_redirect (operation, "color2", f2, "color");
  gegl_operation_meta_redirect (operation, "color3", f3, "color");
  gegl_operation_meta_redirect (operation, "color4", f4, "color");
  gegl_operation_meta_redirect (operation, "color5", f5, "color");
  gegl_operation_meta_redirect (operation, "x", f1, "x");
  gegl_operation_meta_redirect (operation, "x", f2, "x");
  gegl_operation_meta_redirect (operation, "x", f3, "x");
  gegl_operation_meta_redirect (operation, "x", f4, "x");
  gegl_operation_meta_redirect (operation, "x", f5, "x");
  gegl_operation_meta_redirect (operation, "y", f1, "y");
  gegl_operation_meta_redirect (operation, "y", f2, "y");
  gegl_operation_meta_redirect (operation, "y", f3, "y");
  gegl_operation_meta_redirect (operation, "y", f4, "y");
  gegl_operation_meta_redirect (operation, "y", f5, "y");
  gegl_operation_meta_redirect (operation, "radius", median, "radius");
  gegl_operation_meta_redirect (operation, "alpha_percentile", median, "alpha-percentile");



}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:color-trail",
    "title",       _("Color Fill Trail and MultiStroke (probably broke in Gimp 2.99/3)"),
    "categories",  "Artistic",
    "reference-hash", "41ag1g3b0917avx65493xc254001b2ac",
    "description", _("GEGL Color Fill Trail - Sliding a colors opacity to 0 will hide a trail/stroke."
                     ""),
    NULL);
}

#endif
