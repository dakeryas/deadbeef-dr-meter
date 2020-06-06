## Description
Adds a "Dynamic Range" context menu to the selected playlist items.
Upon clicking, the dynamic range, peak, and total RMS for each selection
item is computed and a GTK3 window is opened with results. The text is selectable and can be copied.
NB: This is work in progress.

## Requirements
You need GNU make. The computing plugin relies on a `drmeter` library which is also built and
installed via the top-level `Makefile`. The GTK3-based GUI plugin utilising the computing plugin and displaying its results in a window relies upon `pkg-config` to find
the `gtk+-3.0` library. On Debian/Ubuntu, you should thus be covered with
```
sudo apt-get install make libgtk-3-dev
```
The GNU Makefiles are Linux-specific for the moment but should be easy to port
to OSX or FreeBSD.

## Installion
After having cloned this repository, in the root folder type
```
make install
```
The default installation location of the plugins is
`${HOME}/.local/lib/deadbeef`, this may be overriden with the PREFIX GNUmake
variable, e.g.
```
make install PREFIX=/path/where/deadbeef/finds/plugins
```
