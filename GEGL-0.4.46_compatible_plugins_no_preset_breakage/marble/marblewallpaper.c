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
 * 2023 - Marble Gimp Plugin by Beaver
If you paste this syntax in Gimp's GEGL Graph filter you can test this filter without installing
The syntax is also listed so users can study how this filter was made.


id=c clear aux=[ ref=c ]
gimp:layer-mode layer-mode=multiply  aux=[   noise-solid x-size=2.1 y-size=2.1 detail=15 seed=4136 gimp:layer-mode layer-mode=difference aux=[ noise-solid x-size=0.3 y-size=0.3 detail=15 seed=3433442   ] brightness-contrast contrast=1.1 brightness=0.60 levels out-high=1.4 ]

hard-light aux=[  noise-solid x-size=0.7 y-size=1.3 detail=15 seed=3224 
difference aux=[ noise-solid x-size=1.9 y-size=2.2 detail=15 seed=2432 opacity value=1.00  ] ]

  crop id=sl gimp:layer-mode layer-mode=screen blend-space=rgb-perceptual aux=[ ref=sl softglow glow-radius=50  brightness=0.42 sharpness=0.357  gimp:layer-mode layer-mode=overlay blend-space=rgb-perceptual opacity=0.71 aux=[ color value=#afafaf ] crop id=gaus gimp:layer-mode layer-mode=luminance opacity=0.65 aux=[ ref=gaus gaussian-blur std-dev-x=8 std-dev-y=8 ] levels in-high=1.2 out-high=1.0


--end of syntax---

 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES



/* Three GEGL Graphs are listed here and they are called later (graph0, graph1, and graph2) */

#define syntax0 \
" id=1 clear aux=[ ref=1 ] "\
/* This graph is an instruction to clear everything on canvas
The two graphs below are instructions to make the vast majority of the marble texture. */

#define syntax1 \
" gimp:layer-mode layer-mode=multiply  aux=[   noise-solid x-size=2.1 y-size=2.1 detail=15 seed=4136 gimp:layer-mode layer-mode=difference aux=[ noise-solid x-size=0.3 y-size=0.3 detail=15 seed=3433442   ] brightness-contrast contrast=1.1 brightness=0.60 levels out-high=1.4 ] "\
/* A bunch of filters are being chained together in various ways */

#define syntax2 \
" crop id=sl gimp:layer-mode layer-mode=screen blend-space=rgb-perceptual aux=[ ref=sl softglow glow-radius=50  brightness=0.42 sharpness=0.357  gimp:layer-mode layer-mode=overlay blend-space=rgb-perceptual opacity=0.71 aux=[ color value=#afafaf ] crop id=gaus gimp:layer-mode layer-mode=luminance opacity=0.65 aux=[ ref=gaus gaussian-blur std-dev-x=8 std-dev-y=8 ] levels in-high=1.2 out-high=1.0 "\
/* A bunch of filters are being chained together in various ways. The reason these three syntax chains are not combined is because they go in different orders. Each syntax chain is a node being chained in a particular order.*/

enum_start (marble_clownworld)
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLY, "multipy",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGE,      "grainmerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_HSLCOLOR,      "hslcolor",
              N_("HSL Color"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOFTLIGHT,      "softlight",
              N_("Soft Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_LCHCOLOR,      "lchcolor",
              N_("LCh Color"))
  enum_value (GEGL_BLEND_MODE_TYPE_LIGHTEN,      "Lighten",
              N_("Lighten Only"))
enum_end (marbleclownworld)
/* This is the first part of color overlay's blend mode switch. The ENUM list with all the blend modes needed by color overlay */


property_seed (seed, _("Seed for Marble Texture"), specifiednumber)
    description (_("Seed Texture of the Marble. Sometimes the seed will end up lousy. If that is the case just press seed again until you find a decent texture."))
/* This is the seed of gegl:noise-solid that internally makes a marble effect*/

property_enum (blend, _("Blend Mode of Color"),
    marbleclownworld, marble_clownworld,
    GEGL_BLEND_MODE_TYPE_SOFTLIGHT)
    description (_("Blend Mode Color Overlay of the Marble."))
/* This is the second part of color overlay. The user sees this in the GUI. It alllows them to switch color overlay's blend mode.
There is an instruction for softlight to be the default blend mode on startup. */

property_color (color, _("Color Overlay of the Marble"), "#2ef7b7")
/* This is gegl color overlay being displayed in the gui with a html color  */

property_double (opacity, _("Color Opacity (zero removes)"), 0.6)
    description (_("Opacity of the Marble's color overlay. Sliding this to 0 will remove the color overlay."))
    value_range (0.0, 1.0)
    ui_range    (0.0, 1.0)
/* This is the opacity meter for all of the blend modes */

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     marblewallpaper
#define GEGL_OP_C_SOURCE marblewallpaper.c

#include "gegl-op.h"

typedef struct
{
  GeglNode *input;
  GeglNode *graph0;
  GeglNode *graph1;
  GeglNode *graph2;
  GeglNode *hardlight;
  GeglNode *difference;
  GeglNode *noise1;
  GeglNode *noise2;
  GeglNode *nop;
  GeglNode *color;
  GeglNode *grainmerge;
  GeglNode *multiply;
  GeglNode *hslcolor;
  GeglNode *lchcolor;
  GeglNode *lighten;
  GeglNode *softlight;
  GeglNode *output;
}State;
/* This is a special way of listing GEGL nodes that will be defined below. The nodes below are defined with "state->" in front of them. So the graph can update with a blend mode switcher. */


static void attach (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);

  if (!o->seed )
    {
	o->seed = (1783907628);
    }
}

static void update_graph (GeglOperation *operation)
{

  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);

  State *state = o->user_data = g_malloc0 (sizeof (State));

  if (!state) return;

/* This is telling seed to start at a large number instead of default 0. It will still say 0 but it will be at this until the random seed is pressed. 0 is swapped for said number
in default zero had a lousy effect.*/
  if (!o->seed )
    {
	o->seed = (1783907628);
    }


  state->input    = gegl_node_get_input_proxy (gegl, "input");
  state->output   = gegl_node_get_output_proxy (gegl, "output");


  state->graph0 = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", syntax0,
                                  NULL);


  state->graph1 = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", syntax1,
                                  NULL);

  state->graph2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", syntax2,
                                  NULL);

  state->noise1 = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-solid", "x-size", 0.7, "y-size", 0.13, "detail", 15, "seed",
                                  NULL);

  state->noise2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-solid", "x-size", 1.9, "y-size", 2.2, "detail", 15,  "seed", 3342,
                                  NULL);
/* Properties for noise solid and other filters are being embedded. */

  state->hardlight = gegl_node_new_child (gegl,
                                  "operation", "gegl:hard-light", 
                                  NULL);

  state->difference = gegl_node_new_child (gegl,
                                  "operation", "gegl:difference", 
                                  NULL);

  state->nop = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop", 
                                  NULL);

  state->color = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay", 
                                  NULL);

/* Below are the blend mode numbers for Gimp's grain merge, multiply, lchcolor, lighten and softlight. If Gimp ever gets
new blend modes all these numbers will be off by one. Either 1 up or one down. (likely one up). This is an 
example of me predicting how my plugins will break  for future maintainers. 

It is not possible to just type in the words "grain merge" or "lchcolor" ect... it only works through numbers.*/
state->grainmerge = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 47, "composite-mode", 0, "blend-space", 1, NULL);

state->multiply = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 30, "composite-mode", 0, NULL);

state->lchcolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 26, "composite-mode", 0,  "blend-space", 3, NULL);

state->hslcolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 39, "composite-mode", 0, NULL);

state->lighten = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 36, "composite-mode", 0, NULL);


state->softlight = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 45, "composite-mode", 0, NULL);

  GeglNode *blendmode = state->softlight;
  switch (o->blend) {
    case GEGL_BLEND_MODE_TYPE_MULTIPLY: blendmode = state->multiply; break;
    case GEGL_BLEND_MODE_TYPE_GRAINMERGE: blendmode = state->grainmerge; break;
    case GEGL_BLEND_MODE_TYPE_HSLCOLOR: blendmode = state->hslcolor; break;
    case GEGL_BLEND_MODE_TYPE_SOFTLIGHT: blendmode = state->softlight; break;
    case GEGL_BLEND_MODE_TYPE_LCHCOLOR: blendmode = state->lchcolor; break;
    case GEGL_BLEND_MODE_TYPE_LIGHTEN: blendmode = state->lighten; break;

/* This is the third and final part relating to color overlay's blend mode switch. the state "blendmode" is instructed to update to one of these blend modes that the user specifies.
There is an instruction to make softlight the blend mode that appears on startup*/

default: blendmode = state->softlight;



}

  gegl_node_link_many (state->input, state->graph0, state->graph1, state->hardlight, state->graph2, state->nop, blendmode, state->output, NULL);
/* Most of the marble graph is here*/

  gegl_node_link_many (state->noise1, state->difference, NULL);
  gegl_node_connect (state->hardlight, "aux", state->difference, "output");
  gegl_node_connect (state->difference, "aux", state->noise2, "output");
/*A container is inside a container. 
Hardlight is connecting to Difference. Containing noise1 and difference, and Difference is connecting to noise 2 in its own container.*/
  

gegl_node_link_many (state->nop, state->color, NULL);
gegl_node_connect (blendmode, "aux", state->color, "output");
/*These are the nodes relating to the color overlayand its blend mode switch. They are at the end of the graph.
blendmode is the only node without a "state->" in front of it, because it is a wildcard for any blend mode the
user selections. */


  gegl_operation_meta_redirect (operation, "seed", state->noise1, "seed");
  gegl_operation_meta_redirect (operation, "color", state->color, "value");
/*Random Seed and Color Overlay's GUI properties defined here.*/


  gegl_operation_meta_redirect (operation, "opacity", state->multiply, "opacity");
  gegl_operation_meta_redirect (operation, "opacity", state->grainmerge, "opacity");
  gegl_operation_meta_redirect (operation, "opacity", state->lighten, "opacity");
  gegl_operation_meta_redirect (operation, "opacity", state->hslcolor, "opacity");
  gegl_operation_meta_redirect (operation, "opacity", state->lchcolor, "opacity");
  gegl_operation_meta_redirect (operation, "opacity", state->softlight, "opacity");
/*All six blend modes share the same opacity slider. The GUI has one slider that works for any blend mode.*/
}



static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;
GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);
  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_meta_class->update = update_graph;
operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
/*If this filter ever breaks try changing the name lb: to something else. This is because Gimp's team may want longer name spaces.*/
    "name",        "lb:marble",
    "title",       _("Marble"),
    "reference-hash", "marn6tjblklmare33jmanbfjr",
    "description", _("Render Marble Wallpaper"),
/*This is an instruction to put the filter in Gimp's menu. It only works in Gimp 2.99.16 and up. will not work in Gimp 2.10 as of August 28 2023*/
    "gimp:menu-path", "<Image>/Filters/Render/Fun",
    "gimp:menu-label", _("Marble..."),
    NULL);
}

#endif
