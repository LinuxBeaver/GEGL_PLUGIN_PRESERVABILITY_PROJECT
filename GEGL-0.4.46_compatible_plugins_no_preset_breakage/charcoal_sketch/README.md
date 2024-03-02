### Sketched Image Gimp Plugin 
 
Charcoal Sketch a Gimp GEGL Plugin that does a charcoal sketch effect with lines and a colored pencil mode.

Download procompiled binaries here
/releases

## Location to put binaries

## Windows
![image preview](preview1.png)

 
## Linux 
 /home/(USERNAME)/.local/share/gegl-0.4/plug-ins
 
## Linux (Flatpak includes Chromebook)
 /home/(USERNAME)/.var/app/org.gimp.GIMP/data/gegl-0.4/plug-ins

then restart Gimp and go to GEGL Operation and look for Sketch Charcoal Effect in the drop down list. If you use Gimp 2.99.16+ you will see this filter in Filters>Artistic>Sketched Image.


## Compiling and Installing

### Linux

To compile and install you will need the GEGL header files (`libgegl-dev` on
Debian based distributions or `gegl` on Arch Linux) and meson (`meson` on
most distributions).

```bash
meson setup --buildtype=release build
ninja -C build

```
 

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

### for technical Gimp users reading this.
 
PLEASE CONSIDER MAKING A GEGL FILTER AND KNOW THE META

I have been writing GEGL syntax in Gimp for almost two and a half years and making GEGL plugins for  Gimp for almost one and a half years. No one else on planet Earth is doing this stuff. As I lose interest in Libregraphics I'd appreciate if others made GEGL plugins via arranging GEGL nodes like I do. I am personally upset that some users on Gimp Chat are more interested in scripting GEGL with python then using GEGL directly (like I do). They won't be able to take advantage of GEGL composers, and that is a huge deal breaker - as that is what makes GEGL far more powerful then traditional alpha to logo and script fu stuff. There is also a current issue python to GEGL tech is having where python calling GEGL can only use (gegl:) name space operations and not (svg: gimp: or lb:) name space ones. lb: is my name space btw.  and without gimp: name spaces GEGL cannot use Gimp only blend modes. So please be informed about python to GEGL tech and consider making GEGL plugins my way until python gets better. My way of making GEGL plugins will be non-destructive in Gimp 3.2.


