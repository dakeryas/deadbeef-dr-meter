
![Build status](https://github.com/dakeryas/deadbeef-dr-meter/actions/workflows/ci.yml/badge.svg?branch=master)

## Description
Adds a "Dynamic Range" context menu to the selected playlist items. Upon
clicking, the dynamic range, peak, and total RMS for each selection item is
computed and a GTK dialogue is opened with results. The log can be saved and is
compatible with the DR online database.

<img src="https://user-images.githubusercontent.com/10599557/130930047-c293d3a1-157c-41bb-bddd-1d75ad32ee6f.png" width="876">
<img src="https://user-images.githubusercontent.com/10599557/130930091-6f2597f4-4936-4f13-b4c2-e1701b83bdb7.png" width="876">

## Installation
### Arch Linux
See the [Arch User Repository](https://aur.archlinux.org/packages/deadbeef-dr-meter-gtk3-git/).
### Other Linux distributions
Download the latest Ubuntu builds from
[Releases](https://github.com/dakeryas/deadbeef-dr-meter/releases) or from the
slightly out-of-sync [DeaDBeeF sourceforge](https://deadbeef.sourceforge.io/plugins.html),
or build from source.
### Windows
Download the latest builds from
[Releases](https://github.com/dakeryas/deadbeef-dr-meter/releases) or from the
slightly out-of-sync [DeaDBeeF sourceforge](https://deadbeef.sourceforge.io/plugins.html),
or build from source.

## Building from source
### Requirements
The build relies upon GNU make. By default, GTK is picked up via `pkg-config`
and the GTK3 version of the GUI plugin is built.

On Debian/Ubuntu, you should thus be covered with
```
sudo apt-get install build-essential pkg-config libgtk-3-dev
```
For non-standard GTK installs, the build configuration can be overriden via the
`GTKCFLAGS` and `GTKLIBS` variables.

### Installation
After having installed the aforementionned requirements and cloned this
repository, in the root folder of your local copy of the repository, type
```
make install
```
### Custom installation
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
### DR value formatting
The printing of the dynamic range values can be customised by changing the default
formatting string in "Plugins/Dynamic Range Meter", e.g.
```
DR%-5.2f %10.2f dB %8.2f dB
```
would print a left-justified five-character wide DR value with two digits after
the decimal point.

### Number of parallel threads
The maximum number of DR computing threads in the pool can be changed via the
`dr_meter.threads` variable.
