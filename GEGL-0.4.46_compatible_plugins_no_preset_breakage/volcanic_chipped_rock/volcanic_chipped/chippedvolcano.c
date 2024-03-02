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
 * 2023 Beaver GEGL Chipped Rock and Volcanic Rock 

color-overlay value=#ffb247

id=1 gimp:layer-mode layer-mode=hardlight composite-mode=auto opacity=0.5 aux=[ ref=1 cell-noise iterations=15 seed=63333   ]
cubism tile-size=4

emboss type=bumpmap depth=3 elevation=70 azimuth=444


opacity value=4
median-blur radius=0

screen aux=[ ref=1
median-blur radius=15 alpha-percentile=0
gaussian-blur
opacity value=0.5
 ]

dropshadow x=0 y=0 opacity=0.9 grow-radius=1 radius=0

id=1 src-in aux=[ ref=1 id=char  gimp:layer-mode layer-mode=grain-merge blend-space=rgb-perceptual opacity=0.5 aux=[ ref=char distance-transform  ] opacity value=2   ] unsharp-mask scale=1 median-blur radius=0


 */


#define distancemapforblend \
" id=1 src-in aux=[ ref=1 id=2  gimp:layer-mode layer-mode=grain-merge blend-space=rgb-perceptual opacity=0.5 aux=[ ref=2 distance-transform  ] opacity value=2   ] unsharp-mask scale=1 median-blur radius=0 "\

#define coal \
" id=3 gimp:layer-mode layer-mode=multiply composite-mode=auto opacity=0.92 aux=[ ref=3 difference-of-gaussians radius1=9 radius2=3 ] gimp:desaturate mode=average gamma value=0.9   id=0 dst-out aux=[ ref=0  component-extract component=alpha   levels in-low=0.15  color-to-alpha opacity-threshold=0.6  ] id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0  "\

#define magma1 \
"  id=3 gimp:layer-mode layer-mode=multiply composite-mode=auto opacity=0.92 aux=[ ref=3 difference-of-gaussians radius1=9 radius2=3 ] gimp:desaturate mode=average gamma value=0.9   id=0 dst-out aux=[ ref=0  component-extract component=alpha   levels in-low=0.15  color-to-alpha opacity-threshold=0.6  ] id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0  gimp:desaturate mode=average gamma value=0.9  id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0 id=5 gimp:layer-mode layer-mode=overlay opacity=1 aux=[ ref=5 lb:glassovertext azimuth=300.44 depth=28 elevation=46 std-dev=3 blur=0.99400000000000002 color=#ff9a3f hyperopacity=1.5 ] id=6 gimp:layer-mode layer-mode=overlay opacity=1 aux=[ ref=6 lb:glassovertext azimuth=150.44 depth=28 elevation=46 std-dev=3 blur=0.99400000000000002 color=#ff823f hyperopacity=1.5 ] id=x gimp:layer-mode layer-mode=softlight opacity=0.8 aux=[  ref=x edge-laplace ] multiply value=1.5 gimp:layer-mode layer-mode=addition-legacy opacity=0.70   aux=[ lb:oceansurface  xnoise=9 ynoise=9  seed=2832664790 glow=90 color=#c65800 ] id=x2 plus aux=[ ref=x2 opacity value=0.7 ] levels in-low=0.03 id=magnatime gimp:layer-mode layer-mode=overlay composite-mode=auto opacity=0.8  aux=[ ref=magnatime component-extract  component=rgb-r   ] lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:edgesmooth  "\

#define magma2 \
"  id=3 gimp:layer-mode layer-mode=multiply composite-mode=auto opacity=0.92 aux=[ ref=3 difference-of-gaussians radius1=9 radius2=3 ] gimp:desaturate mode=average gamma value=0.9   id=0 dst-out aux=[ ref=0  component-extract component=alpha   levels in-low=0.15  color-to-alpha opacity-threshold=0.6  ] id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0  gimp:desaturate mode=average gamma value=0.9  id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0 id=5 gimp:layer-mode layer-mode=overlay opacity=1 aux=[ ref=5 lb:glassovertext azimuth=300.44 depth=28 elevation=46 std-dev=3 blur=0.99400000000000002 color=#ff9a3f hyperopacity=1.5 ] id=6 gimp:layer-mode layer-mode=overlay opacity=1 aux=[ ref=6 lb:glassovertext azimuth=150.44 depth=28 elevation=46 std-dev=3 blur=0.99400000000000002 color=#ff823f hyperopacity=1.5 ] id=x gimp:layer-mode layer-mode=softlight opacity=0.8 aux=[  ref=x edge-laplace ] multiply value=1.5 gimp:layer-mode layer-mode=addition-legacy opacity=0.70   aux=[ lb:oceansurface  xnoise=9 ynoise=9  seed=3520 glow=90 color=#c65800 ] id=x2 plus aux=[ ref=x2 opacity value=0.7 ] levels in-low=0.03 id=magnatime gimp:layer-mode layer-mode=overlay composite-mode=auto opacity=0.8  aux=[ ref=magnatime component-extract  component=rgb-r   ] lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:edgesmooth  "\

#define magma3 \
"  id=3 gimp:layer-mode layer-mode=multiply composite-mode=auto opacity=0.92 aux=[ ref=3 difference-of-gaussians radius1=9 radius2=3 ] gimp:desaturate mode=average gamma value=0.9   id=0 dst-out aux=[ ref=0  component-extract component=alpha   levels in-low=0.15  color-to-alpha opacity-threshold=0.6  ] id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0  gimp:desaturate mode=average gamma value=0.9  id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0 id=5 gimp:layer-mode layer-mode=overlay opacity=1 aux=[ ref=5 lb:glassovertext azimuth=300.44 depth=28 elevation=46 std-dev=3 blur=0.99400000000000002 color=#ff9a3f hyperopacity=1.5 ] id=6 gimp:layer-mode layer-mode=overlay opacity=1 aux=[ ref=6 lb:glassovertext azimuth=150.44 depth=28 elevation=46 std-dev=3 blur=0.99400000000000002 color=#ff823f hyperopacity=1.5 ] id=x gimp:layer-mode layer-mode=softlight opacity=0.8 aux=[  ref=x edge-laplace ] multiply value=1.5 gimp:layer-mode layer-mode=addition-legacy opacity=0.70   aux=[ lb:oceansurface  xnoise=9 ynoise=9  seed=73520 glow=90 color=#c65800 ] id=x2 plus aux=[ ref=x2 opacity value=0.7 ] levels in-low=0.03 id=magnatime gimp:layer-mode layer-mode=overlay composite-mode=auto opacity=0.8  aux=[ ref=magnatime component-extract  component=rgb-r   ] lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:edgesmooth  "\


#define magma4 \
"  id=3 gimp:layer-mode layer-mode=multiply composite-mode=auto opacity=0.92 aux=[ ref=3 difference-of-gaussians radius1=9 radius2=3 ] gimp:desaturate mode=average gamma value=0.9   id=0 dst-out aux=[ ref=0  component-extract component=alpha   levels in-low=0.15  color-to-alpha opacity-threshold=0.6  ] id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0  gimp:desaturate mode=average gamma value=0.9  id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0 id=5 gimp:layer-mode layer-mode=overlay opacity=1 aux=[ ref=5 lb:glassovertext azimuth=300.44 depth=28 elevation=46 std-dev=3 blur=0.99400000000000002 color=#ff9a3f hyperopacity=1.5 ] id=6 gimp:layer-mode layer-mode=overlay opacity=1 aux=[ ref=6 lb:glassovertext azimuth=150.44 depth=28 elevation=46 std-dev=3 blur=0.99400000000000002 color=#ff823f hyperopacity=1.5 ] id=x gimp:layer-mode layer-mode=softlight opacity=0.8 aux=[  ref=x edge-laplace ] multiply value=1.5 gimp:layer-mode layer-mode=addition-legacy opacity=0.70   aux=[ lb:oceansurface  xnoise=9 ynoise=9  seed=753345 glow=90 color=#c65800 ] id=x2 plus aux=[ ref=x2 opacity value=0.7 ] levels in-low=0.03 id=magnatime gimp:layer-mode layer-mode=overlay composite-mode=auto opacity=0.8  aux=[ ref=magnatime component-extract  component=rgb-r   ] lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:edgesmooth  "\


#define magma5 \
"  id=3 gimp:layer-mode layer-mode=multiply composite-mode=auto opacity=0.92 aux=[ ref=3 difference-of-gaussians radius1=9 radius2=3 ] gimp:desaturate mode=average gamma value=0.9   id=0 dst-out aux=[ ref=0  component-extract component=alpha   levels in-low=0.15  color-to-alpha opacity-threshold=0.6  ] id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0  gimp:desaturate mode=average gamma value=0.9  id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0 id=5 gimp:layer-mode layer-mode=overlay opacity=1 aux=[ ref=5 lb:glassovertext azimuth=300.44 depth=28 elevation=46 std-dev=3 blur=0.99400000000000002 color=#ff9a3f hyperopacity=1.5 ] id=6 gimp:layer-mode layer-mode=overlay opacity=1 aux=[ ref=6 lb:glassovertext azimuth=150.44 depth=28 elevation=46 std-dev=3 blur=0.99400000000000002 color=#ff823f hyperopacity=1.5 ] id=x gimp:layer-mode layer-mode=softlight opacity=0.8 aux=[  ref=x edge-laplace ] multiply value=1.5 gimp:layer-mode layer-mode=addition-legacy opacity=0.70   aux=[ lb:oceansurface  xnoise=9 ynoise=9  seed=94334 glow=90 color=#c65800 ] id=x2 plus aux=[ ref=x2 opacity value=0.7 ] levels in-low=0.03 id=magnatime gimp:layer-mode layer-mode=overlay composite-mode=auto opacity=0.8  aux=[ ref=magnatime component-extract  component=rgb-r   ] lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:edgesmooth  "\



#define magma6 \
"  id=3 gimp:layer-mode layer-mode=multiply composite-mode=auto opacity=0.92 aux=[ ref=3 difference-of-gaussians radius1=9 radius2=3 ] gimp:desaturate mode=average gamma value=0.9   id=0 dst-out aux=[ ref=0  component-extract component=alpha   levels in-low=0.15  color-to-alpha opacity-threshold=0.6  ] id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0  gimp:desaturate mode=average gamma value=0.9  id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0 id=5 gimp:layer-mode layer-mode=overlay opacity=1 aux=[ ref=5 lb:glassovertext azimuth=300.44 depth=28 elevation=46 std-dev=3 blur=0.99400000000000002 color=#ff9a3f hyperopacity=1.5 ] id=6 gimp:layer-mode layer-mode=overlay opacity=1 aux=[ ref=6 lb:glassovertext azimuth=150.44 depth=28 elevation=46 std-dev=3 blur=0.99400000000000002 color=#ff823f hyperopacity=1.5 ] id=x gimp:layer-mode layer-mode=softlight opacity=0.8 aux=[  ref=x edge-laplace ] multiply value=1.5 gimp:layer-mode layer-mode=addition-legacy opacity=0.70   aux=[ lb:oceansurface  xnoise=9 ynoise=9  seed=9354 glow=90 color=#c65800 ] id=x2 plus aux=[ ref=x2 opacity value=0.7 ] levels in-low=0.03 id=magnatime gimp:layer-mode layer-mode=overlay composite-mode=auto opacity=0.8  aux=[ ref=magnatime component-extract  component=rgb-r   ] lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:edgesmooth  "\

#define magma7 \
"  id=3 gimp:layer-mode layer-mode=multiply composite-mode=auto opacity=0.92 aux=[ ref=3 difference-of-gaussians radius1=9 radius2=3 ] gimp:desaturate mode=average gamma value=0.9   id=0 dst-out aux=[ ref=0  component-extract component=alpha   levels in-low=0.15  color-to-alpha opacity-threshold=0.6  ] id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0  gimp:desaturate mode=average gamma value=0.9  id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0 id=5 gimp:layer-mode layer-mode=overlay opacity=1 aux=[ ref=5 lb:glassovertext azimuth=300.44 depth=28 elevation=46 std-dev=3 blur=0.99400000000000002 color=#ff9a3f hyperopacity=1.5 ] id=6 gimp:layer-mode layer-mode=overlay opacity=1 aux=[ ref=6 lb:glassovertext azimuth=150.44 depth=28 elevation=46 std-dev=3 blur=0.99400000000000002 color=#ff823f hyperopacity=1.5 ] id=x gimp:layer-mode layer-mode=softlight opacity=0.8 aux=[  ref=x edge-laplace ] multiply value=1.5 gimp:layer-mode layer-mode=addition-legacy opacity=0.70   aux=[ lb:oceansurface  xnoise=9 ynoise=9  seed=122354 glow=90 color=#c65800 ] id=x2 plus aux=[ ref=x2 opacity value=0.7 ] levels in-low=0.03 id=magnatime gimp:layer-mode layer-mode=overlay composite-mode=auto opacity=0.8  aux=[ ref=magnatime component-extract  component=rgb-r   ] lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:edgesmooth  "\

#define magma8 \
"  id=3 gimp:layer-mode layer-mode=multiply composite-mode=auto opacity=0.92 aux=[ ref=3 difference-of-gaussians radius1=9 radius2=3 ] gimp:desaturate mode=average gamma value=0.9   id=0 dst-out aux=[ ref=0  component-extract component=alpha   levels in-low=0.15  color-to-alpha opacity-threshold=0.6  ] id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0  gimp:desaturate mode=average gamma value=0.9  id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0 id=5 gimp:layer-mode layer-mode=overlay opacity=1 aux=[ ref=5 lb:glassovertext azimuth=300.44 depth=28 elevation=46 std-dev=3 blur=0.99400000000000002 color=#ff9a3f hyperopacity=1.5 ] id=6 gimp:layer-mode layer-mode=overlay opacity=1 aux=[ ref=6 lb:glassovertext azimuth=150.44 depth=28 elevation=46 std-dev=3 blur=0.99400000000000002 color=#ff823f hyperopacity=1.5 ] id=x gimp:layer-mode layer-mode=softlight opacity=0.8 aux=[  ref=x edge-laplace ] multiply value=1.5 gimp:layer-mode layer-mode=addition-legacy opacity=0.70   aux=[ lb:oceansurface  xnoise=9 ynoise=9  seed=3563554 glow=90 color=#c65800 ] id=x2 plus aux=[ ref=x2 opacity value=0.7 ] levels in-low=0.03 id=magnatime gimp:layer-mode layer-mode=overlay composite-mode=auto opacity=0.8  aux=[ ref=magnatime component-extract  component=rgb-r   ] lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:edgesmooth  "\



#define magma9 \
"  id=3 gimp:layer-mode layer-mode=multiply composite-mode=auto opacity=0.92 aux=[ ref=3 difference-of-gaussians radius1=9 radius2=3 ] gimp:desaturate mode=average gamma value=0.9   id=0 dst-out aux=[ ref=0  component-extract component=alpha   levels in-low=0.15  color-to-alpha opacity-threshold=0.6  ] id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0  gimp:desaturate mode=average gamma value=0.9  id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0 id=5 gimp:layer-mode layer-mode=overlay opacity=1 aux=[ ref=5 lb:glassovertext azimuth=300.44 depth=28 elevation=46 std-dev=3 blur=0.99400000000000002 color=#ff9a3f hyperopacity=1.5 ] id=6 gimp:layer-mode layer-mode=overlay opacity=1 aux=[ ref=6 lb:glassovertext azimuth=150.44 depth=28 elevation=46 std-dev=3 blur=0.99400000000000002 color=#ff823f hyperopacity=1.5 ] id=x gimp:layer-mode layer-mode=softlight opacity=0.8 aux=[  ref=x edge-laplace ] multiply value=1.5 gimp:layer-mode layer-mode=addition-legacy opacity=0.70   aux=[ lb:oceansurface  xnoise=9 ynoise=9  seed=57454 glow=90 color=#c65800 ] id=x2 plus aux=[ ref=x2 opacity value=0.7 ] levels in-low=0.03 id=magnatime gimp:layer-mode layer-mode=overlay composite-mode=auto opacity=0.8  aux=[ ref=magnatime component-extract  component=rgb-r   ] lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:edgesmooth  "\

#define magma10 \
"  id=3 gimp:layer-mode layer-mode=multiply composite-mode=auto opacity=0.92 aux=[ ref=3 difference-of-gaussians radius1=9 radius2=3 ] gimp:desaturate mode=average gamma value=0.9   id=0 dst-out aux=[ ref=0  component-extract component=alpha   levels in-low=0.15  color-to-alpha opacity-threshold=0.6  ] id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0  gimp:desaturate mode=average gamma value=0.9  id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0 id=5 gimp:layer-mode layer-mode=overlay opacity=1 aux=[ ref=5 lb:glassovertext azimuth=300.44 depth=28 elevation=46 std-dev=3 blur=0.99400000000000002 color=#ff9a3f hyperopacity=1.5 ] id=6 gimp:layer-mode layer-mode=overlay opacity=1 aux=[ ref=6 lb:glassovertext azimuth=150.44 depth=28 elevation=46 std-dev=3 blur=0.99400000000000002 color=#ff823f hyperopacity=1.5 ] id=x gimp:layer-mode layer-mode=softlight opacity=0.8 aux=[  ref=x edge-laplace ] multiply value=1.5 gimp:layer-mode layer-mode=addition-legacy opacity=0.70   aux=[ lb:oceansurface  xnoise=9 ynoise=9  seed=35654 glow=90 color=#c65800 ] id=x2 plus aux=[ ref=x2 opacity value=0.7 ] levels in-low=0.03 id=magnatime gimp:layer-mode layer-mode=overlay composite-mode=auto opacity=0.8  aux=[ ref=magnatime component-extract  component=rgb-r   ] lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:edgesmooth  "\

#define magma11 \
"  id=3 gimp:layer-mode layer-mode=multiply composite-mode=auto opacity=0.92 aux=[ ref=3 difference-of-gaussians radius1=9 radius2=3 ] gimp:desaturate mode=average gamma value=0.9   id=0 dst-out aux=[ ref=0  component-extract component=alpha   levels in-low=0.15  color-to-alpha opacity-threshold=0.6  ] id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0  gimp:desaturate mode=average gamma value=0.9  id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0 id=5 gimp:layer-mode layer-mode=overlay opacity=1 aux=[ ref=5 lb:glassovertext azimuth=300.44 depth=28 elevation=46 std-dev=3 blur=0.99400000000000002 color=#ff9a3f hyperopacity=1.5 ] id=6 gimp:layer-mode layer-mode=overlay opacity=1 aux=[ ref=6 lb:glassovertext azimuth=150.44 depth=28 elevation=46 std-dev=3 blur=0.99400000000000002 color=#ff823f hyperopacity=1.5 ] id=x gimp:layer-mode layer-mode=softlight opacity=0.8 aux=[  ref=x edge-laplace ] multiply value=1.5 gimp:layer-mode layer-mode=addition-legacy opacity=0.70   aux=[ lb:oceansurface  xnoise=9 ynoise=9  seed=53426 glow=90 color=#c65800 ] id=x2 plus aux=[ ref=x2 opacity value=0.7 ] levels in-low=0.03 id=magnatime gimp:layer-mode layer-mode=overlay composite-mode=auto opacity=0.8  aux=[ ref=magnatime component-extract  component=rgb-r   ] lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:edgesmooth  "\

#define magma12 \
"  id=3 gimp:layer-mode layer-mode=multiply composite-mode=auto opacity=0.92 aux=[ ref=3 difference-of-gaussians radius1=9 radius2=3 ] gimp:desaturate mode=average gamma value=0.9   id=0 dst-out aux=[ ref=0  component-extract component=alpha   levels in-low=0.15  color-to-alpha opacity-threshold=0.6  ] id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0  gimp:desaturate mode=average gamma value=0.9  id=4 gimp:layer-mode layer-mode=overlay opacity=0.45 aux=[ ref=4 color-overlay value=white  ] median-blur radius=0 id=5 gimp:layer-mode layer-mode=overlay opacity=1 aux=[ ref=5 lb:glassovertext azimuth=300.44 depth=28 elevation=46 std-dev=3 blur=0.99400000000000002 color=#ff9a3f hyperopacity=1.5 ] id=6 gimp:layer-mode layer-mode=overlay opacity=1 aux=[ ref=6 lb:glassovertext azimuth=150.44 depth=28 elevation=46 std-dev=3 blur=0.99400000000000002 color=#ff823f hyperopacity=1.5 ] id=x gimp:layer-mode layer-mode=softlight opacity=0.8 aux=[  ref=x edge-laplace ] multiply value=1.5 gimp:layer-mode layer-mode=addition-legacy opacity=0.70   aux=[ lb:oceansurface  xnoise=9 ynoise=9  seed=89452 glow=90 color=#c65800 ] id=x2 plus aux=[ ref=x2 opacity value=0.7 ] levels in-low=0.03 id=magnatime gimp:layer-mode layer-mode=overlay composite-mode=auto opacity=0.8  aux=[ ref=magnatime component-extract  component=rgb-r   ] lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:edgesmooth  "\


#define magmarealtime \
"  id=magnatime gimp:layer-mode layer-mode=overlay composite-mode=auto opacity=0.8  aux=[ ref=magnatime component-extract  component=rgb-r   ] lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:threshold-alpha alphadegree=9.5 lb:edgesmooth id=normalbeveldodge gimp:layer-mode layer-mode=dodge opacity=0.15 composite-mode=auto aux=[ ref=normalbeveldodge  lb:bevel radius1=4  ] median-blur radius=0 "\



#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES



enum_start (rock_select)
  enum_value (chippedrock, "chippedrock",
              N_("Chipped Rock"))
  enum_value (volcanicnolava, "volcanicnolava",
              N_("Volcanic Rock  (No Lava)"))
  enum_value (volcaniclava, "volcaniclava",
              N_("Volcanic Rock with Lava"))
enum_end (rockselect)


property_enum (rocktype, _("Rock Type"),
    rockselect, rock_select,
    volcaniclava)
  description (_("Choose between chipped rock, volcanic rock and volcanic rock with lava."))


property_color  (colorrock, _("Rock's Color"), "#ffb247")
  description   (_("The rock's color"))
ui_meta ("visible", "!rocktype {volcanicnolava, volcaniclava}" )



property_int  (sizecontrol, _("Size control of rock text"), 0)
  value_range (0, 10)
  ui_range    (0, 10)
  ui_meta     ("unit", "pixel-distance")
  description (_("Size Control of all rocks"))


property_int (depth, _("Depth"), 3)
    description (_("Emboss depth -Brings out depth and detail of the rock"))
    value_range (1, 20)



property_double (elevation, _("Elevation"), 70.0)
    description (_("Elevation angle of the rock.)"))
    value_range (20, 140)
    ui_meta ("unit", "degree")
  ui_steps      (0.1, 1.50)




property_double (lavaintensity, _("Intensify Lava"), 0.3)
    description (_("This slider will intensify the lava."))
    value_range (0.0, 1.0)
  ui_steps      (0.01, 0.50)
ui_meta ("visible", "rocktype {volcaniclava}" )

property_seed (rockseed, _("Rock Seed"), rand)
    description (_("Seed for the structure of the rock"))



enum_start (lava_preset)
  enum_value (LAVA1, "lava1",
              N_("Lava 1"))
  enum_value (LAVA2, "lava2",
              N_("Lava 2"))
  enum_value (LAVA3, "lava3",
              N_("Lava 3"))
  enum_value (LAVA4, "lava4",
              N_("Lava 4"))
  enum_value (LAVA5, "lava5",
              N_("Lava 5"))
  enum_value (LAVA6, "lava6",
              N_("Lava 6"))
  enum_value (LAVA7, "lava7",
              N_("Lava 7"))
  enum_value (LAVA8, "lava8",
              N_("Lava 8"))
  enum_value (LAVA9, "lava9",
              N_("Lava 9"))
  enum_value (LAVA10, "lava10",
              N_("Lava 10"))
  enum_value (LAVA11, "lava11",
              N_("Lava 11"))
  enum_value (LAVA12, "lava12",
              N_("Lava 12"))
enum_end (lavapreset)

property_enum(lavapreset, _("Lava Preset"),
    lavapreset, lava_preset,
    LAVA1)
    description (_("This list contains twelve different seeds for the lava."))
ui_meta ("visible", "rocktype {volcaniclava}" )




#else

#define GEGL_OP_META
#define GEGL_OP_NAME     chippedvolcano
#define GEGL_OP_C_SOURCE chippedvolcano.c

#include "gegl-op.h"

typedef struct
{
 GeglNode *input;
 GeglNode *cubism;
 GeglNode *gaussian;
 GeglNode *fix;
 GeglNode *fix2;
 GeglNode *median;
 GeglNode *median2;
 GeglNode *emboss;
 GeglNode *opacity;
 GeglNode *opacity2;
 GeglNode *outline;
 GeglNode *cellnoise;
 GeglNode *defaultcolor;
 GeglNode *string;
 GeglNode *string2;   
 GeglNode *idref; 
 GeglNode *idref2;
 GeglNode *idref3;  
 GeglNode *screen;
 GeglNode *hardlight;
 GeglNode *lava1;
 GeglNode *lava2;
 GeglNode *lava3;
 GeglNode *lava4;
 GeglNode *lava5;
 GeglNode *lava6;
 GeglNode *lava7;
 GeglNode *lava8;
 GeglNode *lava9;
 GeglNode *lava10;
 GeglNode *lava11;
 GeglNode *lava12;
 GeglNode *lavarealtime;
 GeglNode *replace;
 GeglNode *replace2;
 GeglNode *sizecontrol;
 GeglNode *output;
}State;

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglColor *chipawaycolor = gegl_color_new ("#ffb247");

  State *state = o->user_data = g_malloc0 (sizeof (State));





  state->input    = gegl_node_get_input_proxy (gegl, "input");
  state->output   = gegl_node_get_output_proxy (gegl, "output");

   state->sizecontrol   = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "alpha-percentile", 100.0,   
                                  NULL);


   state->cubism   = gegl_node_new_child (gegl,
                                  "operation", "gegl:cubism", "tile-size", 4.0,   
                                  NULL);

   state->gaussian   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur", "std-dev-x", 1.5, "std-dev-y", 1.5,   
                                  NULL);

   state->fix   = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 0,   
                                  NULL);

   state->fix2   = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 0,   
                                  NULL);


   state->median   = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur",  "radius", 0,   
                                  NULL);


   state->median2   = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 15, "alpha-percentile", 0.0,
                                  NULL);


   state->emboss   = gegl_node_new_child (gegl,
                                  "operation", "gegl:emboss", "type", 1, "depth", 3, "elevation", 70.0, "azimuth", 90.0, 
                                  NULL);


   state->opacity   = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity", "value", 4.0,
                                  NULL);

   state->opacity2   = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity", "value", 0.5,
                                  NULL);


   state->outline   = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow", "x", 0.0, "y", 0.0, "opacity", 0.9, "grow-radius", 1.0, "radius", 1.0, 
                                  NULL);

   state->cellnoise   = gegl_node_new_child (gegl,
                                  "operation", "gegl:cell-noise", "iterations", 15,   
                                  NULL);


   state->defaultcolor   = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay", "value", chipawaycolor,   
                                  NULL);


   state->screen   = gegl_node_new_child (gegl,
                                  "operation", "gegl:screen",   
                                  NULL);


   state->idref   = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",   
                                  NULL);


   state->idref2   = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",   
                                  NULL);

   state->idref3   = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",   
                                  NULL);


   state->string   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", distancemapforblend,   
                                  NULL);

   state->string2   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", coal,   
                                  NULL);


   state->lava1   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", magma1,   
                                  NULL);

   state->lava2   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", magma2,   
                                  NULL);


   state->lava3   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", magma3,   
                                  NULL);

   state->lava4   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", magma4,   
                                  NULL);

   state->lava5   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", magma5,   
                                  NULL);

   state->lava6   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", magma6,   
                                  NULL);

   state->lava7   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", magma7,   
                                  NULL);

   state->lava8   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", magma8,   
                                  NULL);

   state->lava9   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", magma9,   
                                  NULL);

   state->lava10   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", magma10,   
                                  NULL);

   state->lava11   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", magma11,   
                                  NULL);


   state->lava12   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", magma12,   
                                  NULL);

   state->lavarealtime   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", magmarealtime,   
                                  NULL);


state->replace = gegl_node_new_child (gegl,
                                    "operation", "gimp:layer-mode", "layer-mode", 62, "composite-mode", 0, "opacity", 0.9,   NULL);


state->replace2 = gegl_node_new_child (gegl,
                                    "operation", "gimp:layer-mode", "layer-mode", 62, "composite-mode", 0, "opacity", 0.9,  NULL);


state->hardlight = gegl_node_new_child (gegl,
                                  "operation", "gimp:layer-mode", "layer-mode", 44, "composite-mode", 0, "opacity", 0.5, "blend-space", 2, NULL);


gegl_operation_meta_redirect (operation, "seed", state->cubism, "seed");
gegl_operation_meta_redirect (operation, "sizecontrol", state->sizecontrol, "radius");
gegl_operation_meta_redirect (operation, "colorrock", state->defaultcolor, "value");
gegl_operation_meta_redirect (operation, "depth", state->emboss, "depth");
gegl_operation_meta_redirect (operation, "elevation", state->emboss, "elevation");
gegl_operation_meta_redirect (operation, "lavaintensity", state->replace2, "opacity");
gegl_operation_meta_redirect (operation, "rockseed", state->cubism, "seed");


} 

static void update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;

  GeglNode *magma = state->lava1; /* the default */
  switch (o->lavapreset) {
    case LAVA1: magma = state->lava1; break;
    case LAVA2: magma = state->lava2; break;
    case LAVA3: magma = state->lava3; break;
    case LAVA4: magma = state->lava4; break;
    case LAVA5: magma = state->lava5; break;
    case LAVA6: magma = state->lava6; break;
    case LAVA7: magma = state->lava7; break;
    case LAVA8: magma = state->lava8; break;
    case LAVA9: magma = state->lava9; break;
    case LAVA10: magma = state->lava10; break;
    case LAVA11: magma = state->lava11; break;
    case LAVA12: magma = state->lava12; break;
default: magma = state->lava1;

}


switch (o->rocktype) {
        break;
    case chippedrock:
  gegl_node_link_many (state->input, state->sizecontrol, state->defaultcolor, state->idref, state->hardlight, state->cubism, state->median, state->emboss, state->opacity, state->fix, state->screen, state->outline, state->fix2,  state->output, NULL);
  gegl_node_connect (state->hardlight, "aux", state->cellnoise, "output");
  gegl_node_link_many (state->idref, state->median2, state->gaussian, state->opacity2, NULL);
  gegl_node_connect (state->screen, "aux", state->opacity2, "output");
        break;
    case volcanicnolava:
  gegl_node_link_many (state->input, state->sizecontrol, state->defaultcolor, state->idref, state->hardlight, state->cubism, state->median, state->emboss, state->opacity, state->fix, state->screen, state->outline, state->fix2, state->string2,  state->output, NULL);
  gegl_node_connect (state->hardlight, "aux", state->cellnoise, "output");
  gegl_node_link_many (state->idref, state->median2, state->gaussian, state->opacity2, NULL);
  gegl_node_connect (state->screen, "aux", state->opacity2, "output");
        break;
    case volcaniclava:
  gegl_node_link_many (state->input, state->sizecontrol, state->defaultcolor, state->idref, state->hardlight, state->cubism, state->median, state->emboss, state->opacity, state->fix, state->screen, state->outline, state->fix2, state->idref2, state->replace, magma, state->idref3, state->replace2,  state->output, NULL);
  gegl_node_connect (state->hardlight, "aux", state->cellnoise, "output");
  gegl_node_link_many (state->idref, state->median2, state->gaussian, state->opacity2, NULL);
  gegl_node_connect (state->screen, "aux", state->opacity2, "output");
  gegl_node_connect (state->replace, "aux", state->string2, "output");
  gegl_node_connect (state->replace2, "aux", state->lavarealtime, "output");
  gegl_node_link_many (state->idref2, state->string2, NULL);
  gegl_node_link_many (state->idref3, state->lavarealtime, NULL);
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
    "name",        "lb:volcanicrock",
    "title",       _("Volcanic Rock / Chipped Rock"),
    "reference-hash", "fddcfuturefunkvaporpop",
    "description", _("This filter has 3 modes (Chipped Rock, Volcanic Rock, and Lava Rock). Classic Gimp fans should know that 'chipped rock' is inspired by the script fu 'chip away'. "
                     ""),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Chipped Rock and Volcanic Rock"),
    NULL);
}

#endif
