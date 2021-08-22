## Description
Adds a "Dynamic Range" context menu to the selected playlist items.  Upon
clicking, the dynamic range, peak, and total RMS for each selection item is
computed and a GTK dialogue is opened with results. The log can be saved and
is compatible with the DR online database.

<img src="https://i.postimg.cc/HxKF3x92/Screenshot-20210817-225753.png" width="879">
<img src="https://i.postimg.cc/Qtbz3b8b/windows7.png" width="879">

## Requirements
### Linux
The rudimentary build system relies upon GNU make. The computing plugin relies
on a `drmeter` library which is also built and installed via the top-level
`Makefile`. The GUI plugin utilising the computing plugin and displaying its
results in a dialog relies upon `pkg-config` to find the GTK library for
linking. By default, the GTK3 version of the GUI plugin is built.

On Debian/Ubuntu, you should thus be covered with
```
sudo apt-get install build-essential pkg-config libgtk-3-dev
```
For non-standard GTK installs, the build configuration can be overriden via the
GTKCFLAGS and GTKLIBS variables.

### Windows
The plugin builds on Windows with MSYS2 (MinGW64...), the resulting binaries
were also tested on Windows 7 x64. See
[eadbeef-plugin-builder](https://github.com/DeaDBeeF-Player/deadbeef-plugin-builder)
for configration.

## Installation
After having installed the aforementionned requirements and cloned this
repository, in the root folder of your local copy of the repository, type
```
make install
```

## Custom installation
The default installation location of the plugins is
`${HOME}/.local/lib/deadbeef`, this may be overridden with the PREFIX GNUmake
variable, e.g.
```
make install PREFIX=/path/where/deadbeef/finds/plugins
```
You can also combine it with `DESTDIR` whilst retaining the `RUNPATH` (which is set
so that the plugins can find `libdrmeter.so`) set from the `PREFIX` value, e.g.
```
make install DESTDIR=$PWD/temp PREFIX=/usr/lib/x86_64-linux-gnu
```
If you insist on using GTK2, try from the root folder
```
make install GTK=2
```

## Runtime configuration
The printing of the dynamic range values can be customised by changing the default
formatting string in "Plugins/Dynamic Range Meter", e.g.
```
DR%-2.2f %10.2f dB %8.2f dB
```
would print the DR value with two significant digits.

The number of DR computing threads can be changed via the `dr_meter.threads` variable.
