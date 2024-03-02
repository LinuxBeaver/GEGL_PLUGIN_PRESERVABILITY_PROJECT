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
 * 2022 Beaver (GEGL sand)
 */



/*

GEGL Graph of grains of sand to test without installing.

color-overlay value=#00f9ff
id=1
noise-spread amount-x=380 amount-y=150
cubism tile-size=2 seed=1

over aux=[ ref=1 color-overlay value=#ffffff  noise-spread amount-x=30 amount-y=30 cubism tile-size=2 seed=1 ]
 */


#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

property_double (lightness, _("Lightness"), 0.0)
   description  (_("Lightness adjustment"))
   value_range  (0.0, 15.0)

property_color (value, _("Color 1"), "#ffffff")
    description (_("The color to paint over the input"))



property_int    (amount_x, _("Horizontal spread 1"), 20)
    description (_("Horizontal spread amount"))
    value_range (15, 60)
    ui_meta     ("unit", "pixel-distance")
    ui_meta     ("axis", "x")

property_int    (amount_y, _("Vertical spread 1"), 20)
    description (_("Vertical spread amount"))
    value_range (15, 60)
    ui_meta     ("unit", "pixel-distance")
    ui_meta     ("axis", "y")

property_seed (seed, _("Random seed"), rand)

property_double (tilesize, _("Tile size"), 1.7)
    description (_("Average diameter of each tile (in pixels)"))
    value_range (1.5, 2)
    ui_meta     ("unit", "pixel-distance")


property_color (value2, _("Color 2"), "#ffffff")
    description (_("The color to paint over the input"))


property_int    (amount_x2, _("Horizontal spread 2"), 180)
    description (_("Horizontal spread amount"))
    value_range (100, 512)
    ui_meta     ("unit", "pixel-distance")
    ui_meta     ("axis", "x")

property_int    (amount_y2, _("Vertical spread 2"), 150)
    description (_("Vertical spread amount"))
    value_range (100, 512)
    ui_meta     ("unit", "pixel-distance")
    ui_meta     ("axis", "y")

property_seed (seed2, _("Random seed 2"), rand2)

property_double (tilesize2, _("Tile size 2"), 1.7)
    description (_("Average diameter of each tile (in pixels)"))
    value_range (1.5, 2)
    ui_meta     ("unit", "pixel-distance")


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     sand
#define GEGL_OP_C_SOURCE sand.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *color, *spread, *cubism, *over, *color2, *cubism2,  *lightness, *spread2;


  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");


  color   = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);

  color2   = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);


  cubism   = gegl_node_new_child (gegl,
                                  "operation", "gegl:cubism",
                                  NULL);


  cubism2   = gegl_node_new_child (gegl,
                                  "operation", "gegl:cubism",
                                  NULL);

  spread   = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-spread",
                                  NULL);


  spread2   = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-spread",
                                  NULL);


  over   = gegl_node_new_child (gegl,
                                  "operation", "gegl:over",
                                  NULL);


  lightness    = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);


gegl_node_link_many (input, color, spread, cubism, over, lightness, output, NULL);
gegl_node_link_many (input, color2, spread2, cubism2, NULL);
gegl_node_connect (over, "aux", cubism2, "output"); 
 
  gegl_operation_meta_redirect (operation, "value", color, "value");
  gegl_operation_meta_redirect (operation, "value2", color2, "value");
  gegl_operation_meta_redirect (operation, "amount_x", spread, "amount-x");
  gegl_operation_meta_redirect (operation, "amount_y", spread, "amount-y");
  gegl_operation_meta_redirect (operation, "amount_y2", spread2, "amount-y");
  gegl_operation_meta_redirect (operation, "amount_x2", spread2, "amount-x");
  gegl_operation_meta_redirect (operation, "seed", spread, "seed");
  gegl_operation_meta_redirect (operation, "seed2", spread2, "seed");
  gegl_operation_meta_redirect (operation, "tilesize", cubism, "tile-size");
  gegl_operation_meta_redirect (operation, "tilesize2", cubism2, "tile-size");
  gegl_operation_meta_redirect (operation, "lightness", lightness, "lightness");

}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:sand-text",
    "title",       _("Grains of Sand - Text Styling filter"),
    "reference-hash", "33doa01va13x9xn3v25sb2ac",
    "description", _("GEGL styles your text like grains of sand.  "
                     ""),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Grains of Sand..."),
    NULL);
}

#endif
