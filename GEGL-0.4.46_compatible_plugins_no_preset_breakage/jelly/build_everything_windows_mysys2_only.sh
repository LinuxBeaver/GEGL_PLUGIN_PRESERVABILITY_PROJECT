#!/bin/bash

 cd compile_bevel_here/ && ./build_linux.sh && cd .. && cd compile_innerglow_here/ && ./build_linux.sh && cd .. && cd compile_glass_over_text_here/ && ./build_linux.sh && cd .. && cd compile_shiny_surface_here/ && ./build_linux.sh && cd .. && cd compile_zzstrokebevelimage_here/ && ./build_linux.sh && cd .. && cd compile_gegl_effects_here/ && ./build_linux.sh && cd .. && mv compile_bevel_here/build/bevel.dll bevel.dll && mv compile_innerglow_here/build/inner-glow.dll inner-glow.dll && mv compile_glass_over_text_here/build/glassovertext.dll glassovertext.dll && mv compile_shiny_surface_here/build/shinytext.dll shinytext.dll && mv compile_zzstrokebevelimage_here/build/zzstrokebevelimage.dll zzstrokebevelimage.dll && mv compile_gegl_effects_here/build/gegleffectspending.dll gegleffectspending.dll && rm -r compile_bevel_here/build && rm -r compile_gegl_effects_here/build && rm -r compile_glass_over_text_here/build && rm -r compile_innerglow_here/build && rm -r compile_shiny_surface_here/build && rm -r compile_zzstrokebevelimage_here/build 
