## Description
Adds a "Dynamic Range" context menu to the selected playlist items.  Upon
clicking, the dynamic range, peak, and total RMS for each selection item is
computed and a GTK window is opened with results. The text is selectable and
can be copied.

[![deadbeef-dr-meter.png](https://i.postimg.cc/5tRw2KC2/deadbeef-dr-meter.png)](https://postimg.cc/Mc1csD5C)

NB: This is work in progress.

## Requirements
Building is achieved with GNU make. The computing plugin relies on a `drmeter`
library which is also built and installed via the top-level `Makefile`. The GUI
plugin utilising the computing plugin and displaying its results in a dialog
relies upon `pkg-config` to find the GTK library when linking. By default, the
GTK3 version of the GUI plugin is built (GTK2 is somewhat supported). On
Debian/Ubuntu, you should thus be covered with
```
sudo apt-get install build-essential pkg-config libgtk-3-dev
```
The Makefiles should be easy to port to OSX or FreeBSD.

## Installation
After having installed the aforementionned requirements and cloned this repository, in the root folder of your local copy of the repository, type
```
make install
```
The default installation location of the plugins is
`${HOME}/.local/lib/deadbeef`, this may be overridden with the PREFIX GNUmake
variable, e.g.
```
make install PREFIX=/path/where/deadbeef/finds/plugins
```
You can also combined it with `DESTDIR`, e.g.
```
make install DESTDIR=$PWD/temp PREFIX=/usr/lib/x86_64-linux-gnu
```
If you insist on using GTK2, try from the root folder
```
make clean
make install GTK=2
```
