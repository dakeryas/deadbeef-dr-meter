PREFIX ?= ${HOME}/.local
export DRMETER_DIR=$(PWD)/dr_meter

TARGETS := all debug install clean
SUBDIRS := dr_meter dr_plugin dr_plugin_gui

ifeq ($(MAKECMDGOALS),install)
	DRMETER_PREFIX := PREFIX=$(PREFIX)/lib/deadbeef
endif

$(TARGETS): $(SUBDIRS)
dr_meter:
	$(MAKE) -C $@ $(MAKECMDGOALS) $(DRMETER_PREFIX)
dr_plugin dr_plugin_gui: dr_meter
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TARGETS) $(SUBDIRS)
