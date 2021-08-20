export PREFIX ?= ${HOME}/.local/lib/deadbeef
export DRMETER_DIR = $(PWD)/dr_meter

TARGETS := all debug install clean
SUBDIRS := dr_meter dr_plugin dr_plugin_gui

# for installation of plugins, copy libdrmeter and set rpath
ifneq (,$(findstring install, $(MAKECMDGOALS)))
	SET_RPATH ?= RPATH=$(PREFIX)/lib
endif

$(TARGETS): $(SUBDIRS)
dr_meter:
	@$(MAKE) -C $@ $(MAKECMDGOALS)
dr_plugin dr_plugin_gui: dr_meter
	@$(MAKE) -C $@ $(MAKECMDGOALS) $(SET_RPATH)

.PHONY: $(TARGETS) $(SUBDIRS)
