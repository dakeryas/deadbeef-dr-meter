## Description
Adds a "Dynamic Range" context menu to the selected playlist items.  Upon
clicking, the dynamic range, peak, and total RMS for each selection item is
computed and a GTK window is opened with results. The text is selectable and
can be copied.

[![deadbeef-dr-meter.png](https://i.postimg.cc/5tRw2KC2/deadbeef-dr-meter.png)](https://postimg.cc/Mc1csD5C)

NB: This is work in progress.

## Requirements
You need GNU make. The computing plugin relies on a `drmeter` library which is
also built and installed via the top-level `Makefile`. The GTK3-based GUI
plugin utilising the computing plugin and displaying its results in a window
relies upon `pkg-config` to find the `gtk+-3.0` library. On Debian/Ubuntu, you
should thus be covered with
```
sudo apt-get install make libgtk-3-dev
```
For the moment, the build also works with gtk+-2.0 but you need to swap the
gtk+-3.0 references for gtk+-2.0 in `dr_plugin_gui/Makefile`. Similarly, the
GNU Makefiles should be easy to port to OSX (or use macports) or FreeBSD.

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
