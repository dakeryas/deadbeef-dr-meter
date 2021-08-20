export PREFIX ?= ${HOME}/.local/lib/deadbeef
DRMETER_DIR ?= $(PWD)/dr_meter

TARGETS := all debug install clean
SUBDIRS := dr_meter dr_plugin dr_plugin_gui

# for installation of plugins, copy libdrmeter and set rpath
ifneq (,$(findstring install, $(MAKECMDGOALS)))
ifndef RPATH
	SET_RPATH ?= RPATH=$(PREFIX)/lib
endif
endif

$(TARGETS): $(SUBDIRS)
dr_meter:
	@$(MAKE) -C $@ $(MAKECMDGOALS)
dr_plugin dr_plugin_gui: dr_meter
	@$(MAKE) -C $@ $(MAKECMDGOALS) DRMETER_DIR=$(DRMETER_DIR) $(SET_RPATH)

.PHONY: $(TARGETS) $(SUBDIRS)
