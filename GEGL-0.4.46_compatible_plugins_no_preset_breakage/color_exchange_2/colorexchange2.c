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
 * 2023 Beaver GEGL Color Exchange 2
 */


#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

/*
GEGL GRAPH TO TEST WITHOUT INSTALLING. Feed this syntax to Gimp's GEGL graph and get a static preview of this plugin.

id=1 xor aux=[ ref=1
color-to-alpha color=#ffffff opacity-threshold=0.5 ]
color-overlay value=#88e6ff
 */

property_color (colorselect, _("From Color"), "#ffffff")
   description  (_("Select the color that you want to recolor here. Use Gimp's color picker."))

property_color (colorreplace, _("To Color"), "#88e6ff")
   description  (_("The new color is selected here"))


property_double (colorthreshold, _("Color to Alpha's opacity threshold"), 0.6)
   description  (_("Color to Alpha's Opacity Threshold"))
  ui_range (0.5, 1.0)
   value_range  (0.2, 0.8)
  ui_steps      (0.2, 0.8)

property_double (colorthreshold2, _("Color to Alpha's transparency threshold."), 0.0)
   description  (_("Color Threshold"))
  ui_range (0.0, 1.0)
   value_range  (0.0, 0.7)
  ui_steps      (0.0, 0.7)

property_double (blur, _("Apply mild blur"), 0.0)
   description  (_("A mild blur that may help smooth the pixel effect created by an internal color to alpha."))
  ui_range (0.0, 3.0)
   value_range  (0.0, 3.0)
  ui_steps      (0.0, 3.0)

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     colorexchange2
#define GEGL_OP_C_SOURCE colorexchange2.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *erase, *blur, *color, *c2a;
  GeglColor *white_color_embedded = gegl_color_new ("#ffffff");

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");


   erase   = gegl_node_new_child (gegl,
                                  "operation", "gegl:xor",
                                  NULL);

   color   = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);

   c2a   = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-to-alpha", "color", white_color_embedded,
                                  NULL);

   blur   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
                                  NULL);


  gegl_operation_meta_redirect (operation, "colorselect", c2a, "color");
  gegl_operation_meta_redirect (operation, "colorreplace", color, "value");
  gegl_operation_meta_redirect (operation, "colorthreshold", c2a, "opacity-threshold");
  gegl_operation_meta_redirect (operation, "colorthreshold2", c2a, "transparency-threshold");
  gegl_operation_meta_redirect (operation, "blur", blur, "std-dev-x");
  gegl_operation_meta_redirect (operation, "blur", blur, "std-dev-y");


  gegl_node_link_many (input, erase, color, blur, output, NULL);
  gegl_node_connect (erase, "aux", c2a, "output");
  gegl_node_link_many (input, c2a, NULL);

}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:color-exchange",
    "title",       _("Color Exchange 2"),
    "reference-hash", "b35dgxsfexchna77ge5xchan0laefe2g4f1b2a",
    "description", _("An alternative color exchange plugin that expects the user to use Gimp's built in blending options. On the 'Normal' blend mode this will behave like a normal color swap. On HSV Hue and LCh Hue blend modes it will function as a hue rotation. On blend modes like HSL Color, LCh Color and Overlay it will become a deep color overlay."
                     ""),
    "gimp:menu-path", "<Image>/Colors/Map",
    "gimp:menu-label", _("Color Exchange 2..."),
    NULL);
}

#endif
