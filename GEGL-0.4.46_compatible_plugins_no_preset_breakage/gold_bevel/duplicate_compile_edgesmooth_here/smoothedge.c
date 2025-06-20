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
 * 2022 Edge Smooth
 */

/*  
Rough Recreation of GEGL Graph. This may not be 100% accurate but it is good enough. Using
this method you can run the filter without installing it.
 
id=1  gegl:over aux=[ ref=1  xor aux=[   median-blur radius=2.4 alpha-percentile=2 ]

id=2  gegl:dst-atop aux=[  ref=2 median-blur radius=2 alpha-percentile=-1 gaussian-blur std-dev-x=2 std-dev-y=2 opacity value=2.7 median-blur radius= percentile=2  alpha-percentile=73  ]
 */


#include "config.h"
#include <glib/gi18n-lib.h>


#ifdef GEGL_PROPERTIES

#define TUTORIAL \
"  id=1  gegl:over aux=[ ref=1  xor aux=[   median-blur radius=2.4 alpha-percentile=2    ]  "\


/*It is mid 2023 and I still don't know how to hide these from the GEGL Graph. Everything else is easy to hide though*/


property_double  (alpha_percentile2, _("Median edges"), 73)
  value_range (0, 100)
  description (_("Apply a median blur only on the edges"))


property_double (gaus, _("Blur Edge"), 1)
   description (_("Apply a Gaussian Blur only on the edges of an image"))
   value_range (0.0, 3.0)
   ui_range    (0.24, 3.0)
   ui_gamma    (3.0)
   ui_meta     ("unit", "pixel-distance")
   ui_meta     ("axis", "x")


property_double (value, _("Increase Opacity around edges"), 1.2)
    description (_("Increase the opacity only around the edges"))
    value_range (1, 6.0)
    ui_range    (1, 3.0)



#else

#define GEGL_OP_META
#define GEGL_OP_NAME     smoothedge
#define GEGL_OP_C_SOURCE smoothedge.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *graph, *behind, *median, *median2, *fixgraph,  *gaussian, *opacity;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");



  median    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 2, "alpha-percentile", 2.0,
                                  NULL);

  median2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 2, "percentile", 1.0, 
                                  NULL);

  fixgraph    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 0,
                                  NULL);

/*  
This "fixgraph" is for Gimp's non-destructive future. Median Blur at 0 makes no modifications to an image but solves a 
bug by resetting gegl opacity. egl:opacity adjust the global opacity of an image; resulting in filters like "drop shadow" behaving
in a damaged way because there global opacity is way to high. Median blur radius=0 resets its global opacity.
 */


  gaussian    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
                                  NULL);

  opacity    = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

  behind    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst-atop",
                                  NULL);

  graph    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", TUTORIAL,
                                  NULL);


gegl_node_link_many(input, graph, behind, fixgraph, output, NULL);
gegl_node_link_many(input,  median, gaussian, opacity, median2,  NULL);
gegl_node_connect (behind, "aux", median2, "output"); 

  gegl_operation_meta_redirect (operation, "gaus", gaussian, "std-dev-x");
  gegl_operation_meta_redirect (operation, "gaus", gaussian, "std-dev-y");
  gegl_operation_meta_redirect (operation, "alpha_percentile2", median2, "alpha-percentile");
  gegl_operation_meta_redirect (operation, "value", opacity, "value");
  gegl_operation_meta_redirect (operation, "string", graph, "string");
}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:edgesmooth",
    "title",       _("Rough Edge Smoother"),
    "categories",  "EdgeSmoother",
    "reference-hash", "45ed5656a11bgxxdt27730vaefe2g4f1b2ac",
    "description", _("GEGL will apply a median blur and a few other things around a transparent images edges to repair them. "
                     ""),
    NULL);
}

#endif
