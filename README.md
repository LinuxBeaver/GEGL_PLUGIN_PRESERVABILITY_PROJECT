## This is a special repo featuring all my plugins meant for GEGL-0.4.46 and up
![image](https://github.com/LinuxBeaver/GEGL_PLUGIN_PRESERVABILITY_PROJECT/assets/78667207/1d50c68d-8d05-4b67-be90-96a47650ee82)

In 2025-2026 GEGL will break all plugins of mine that use **gegl_node_connect_from** with **gegl_node_connect** (that is the majority of them) This special repo contains the fix, but if Gimp is using versions of GEGL earlier then 0.4.46 it can't read gegl_node_connect so Ubuntu 20.04-22.04 and even Fedora 36-38 users won't be able to use my plugins in default.
This special repo contains the source code of all my plugins that work on GEGL-0.4.46 up.

GEGL 0.446 can make both **gegl_node_connect** from and **gegl_node_connect** plugins but earlier versions can only use **gegl_node_connect_from**

Compiling is very easy using build_linux.sh and similar scripts. They auto generate binaries but require the following packages

**gegl**

**meson**

**ninja**

**mysys2** (if one is on Windows)

But we also have precompiled binaries for Windows and Linux

## Location to put binaries

**Windows**

 C:\Users\(USERNAME)\AppData\Local\gegl-0.4\plug-ins
 
 **Linux** 

 /home/(USERNAME)/.local/share/gegl-0.4/plug-ins
 
**Linux (Flatpak includes Chromebook)**

 /home/(USERNAME)/.var/app/org.gimp.GIMP/data/gegl-0.4/plug-ins

THEN RESTART GIMP AND GO TO GEGL OPERATIONS and look for Modify Selection in Gimp 2.99.16+ it will be in the main menu in Selection
Gimp 2.99+ users will see this in filters>text styling>custom bevel
