Ring Text Gimp plugin
works in both Gimp 2.10 and Gimp 2.99

![image preview](images/2.png)


In the folder "images" their is the image file sand.png that this calls.
Remember, to make text while or grayscale before uploading an image file.

## Directory to put Binaries (They do NOT go in the normal plugins folder)

**Windows**

 C:\Users\(USERNAME)\AppData\Local\gegl-0.4\plug-ins
 
 **Linux** 

 /home/(USERNAME)/.local/share/gegl-0.4/plug-ins
 
 **Linux (Flatpak includes Chromebook)**

 /home/(USERNAME)/.var/app/org.gimp.GIMP/data/gegl-0.4/plug-ins

Then Restart Gimp and go to GEGL Operations and look for "Ring Text" in the drop down list
Gimp 2.99.16+ users will find the filter in Filters>Text Styling>Ring Text. 2.10 
users will only see it in the GEGL operations drop down list.


This plugin ships with one other plugins of mine that are useful stand alones on their own. 
named SSG (stroke shadow glow).

![image preview](images/dep_preview.png)

## Compiling and Installing

### Linux

To compile and install you will need the GEGL header files (`libgegl-dev` on
Debian based distributions or `gegl` on Arch Linux) and meson (`meson` on
most distributions).

```bash
meson setup --buildtype=release build
ninja -C build

```

If you have an older version of gegl you may need to copy to `~/.local/share/gegl-0.3/plug-ins`
instead (on Ubuntu 18.04 for example).

BEAVER RECOMMENDS YOU USE A MODERN VERSION OF GEGL. NO GUARANTEE DATED VERSIONS OF GIMP WILL WORK WITH THIS PLUGIN 

### Windows

The easiest way to compile this project on Windows is by using msys2.  Download
and install it from here: https://www.msys2.org/

Open a msys2 terminal with `C:\msys64\mingw64.exe`.  Run the following to
install required build dependencies:

```bash
pacman --noconfirm -S base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-meson mingw-w64-x86_64-gegl
```

Then build the same way you would on Linux:

```bash
meson setup --buildtype=release build
ninja -C build
```

## More Image Previews of this amazing plugin

![image preview](images/7.png)



