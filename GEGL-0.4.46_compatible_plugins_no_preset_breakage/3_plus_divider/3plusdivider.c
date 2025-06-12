/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the graph3e that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <https://www.gnu.org/licenses/>.
 *
 * Credit to Øvind Kolas (pippin) for major GEGL contributions
 * 2023 Beaver, Pie divider

Static preview this plugin without installing by pasting this syntax inside Gimp's GEGL Graph filter. n-segs is the amount of dividers and r-angle is the rotation of dividers. 
Drop Shadow's radius is the growth size of the divider.

 over aux=[  color value=#000000  ] crop
mirrors n-segs=4  r-angle=90
color-overlay value=#ffffff dst-over aux=[ color value=#000000  ]  gimp:layer-mode layer-mode=color-erase opacity=1.00 aux=[ color value=#ffffff ] crop
dropshadow x=0 y=0 radius=0 grow-radius=10 opacity=2
color-overlay value=#11005d
 gegl:opacity value=9 median-blur radius=0

 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES


#define color_fill \
" over aux=[  color value=#000000  ] crop "\



#define invert_transparency \
" color-overlay value=#ffffff dst-over aux=[ color value=#000000  ]  gimp:layer-mode layer-mode=color-erase opacity=1.00 aux=[ color value=#ffffff ] crop "\

#define hyper_opacity_and_fix \
" gegl:opacity value=8.5 median-blur radius=0 "\


property_int (dividers, _("Amount of Dividers"), 4)
   description (_("How many dividers are present. Three is the minimum number and twelve is the max. Due to how this plugin works it is not possible to go below 3. "))
   value_range (3, 12)
   ui_range    (3, 12)



property_double (rotate, _("Rotate Dividers"), 90)
   description (_("Rotate dividers."))
   value_range (90, 360)
   ui_range    (90, 360)
   ui_gamma    (3.0)


property_double (grow, _("Grow Divider Border"), 10)
   description (_("Rotate dividers"))
   value_range (1, 50)
   ui_range    (1, 20)
   ui_gamma    (3.0)

property_color  (color, _("Color of Divider"), "#000000")
    description (_("Change the color of the divider."))

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     3plusdivider
#define GEGL_OP_C_SOURCE 3plusdivider.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *graph, *graph2, *grow, *kali, *graph3, *color,  *output;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");

  graph = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", color_fill, 
                                  NULL);

  graph2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", invert_transparency, 
                                  NULL);

  graph3 = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", hyper_opacity_and_fix, 
                                  NULL);

  grow = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow", "x", 0.0, "y", 0.0, "radius", 0.0, "opacity", 2.0,
                                  NULL);

  kali = gegl_node_new_child (gegl,
                                  "operation", "gegl:mirrors", "n-seg", 4, "r-angle", 90.0,
                                  NULL);

  color = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);

  gegl_node_link_many (input, graph, kali, graph2, grow, color, graph3, output, NULL);

  gegl_operation_meta_redirect (operation, "rotate", kali, "r-angle");
  gegl_operation_meta_redirect (operation, "dividers", kali, "n-segs");
  gegl_operation_meta_redirect (operation, "grow", grow, "grow-radius");
  gegl_operation_meta_redirect (operation, "color", color, "value");

}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:pie",
    "title",       _("Pie Divider"),
    "reference-hash", "knsedakfpieafdigks44loaa",
    "description", _("Render equally spaced dividers in the style of a sliced pie. Though the default setting is a generic four corners structure. If it is rotated it will match the 'pie slices' look. Once a divider is made a user can manually put transformed images inside of it. "),
    "gimp:menu-path", "<Image>/Filters/Render/Fun",
    "gimp:menu-label", _("Pie Divider..."),
    NULL);
}

#endif
