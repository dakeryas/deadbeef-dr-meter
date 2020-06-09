PREFIX ?= ${HOME}/.local/lib
export DRMETER_DIR=$(PWD)/dr_meter

TARGETS := all debug install clean
SUBDIRS := dr_meter dr_plugin dr_plugin_gui

# for installation of plugins, copy libdrmeter and set rpath
ifneq (,$(findstring install, $(MAKECMDGOALS)))
	DRMETER_PREFIX := $(PREFIX)/deadbeef
	SET_DRMETER_PREFIX := PREFIX=$(DRMETER_PREFIX)
	SET_RPATH := RPATH=$(DRMETER_PREFIX)/lib
endif

$(TARGETS): $(SUBDIRS)
dr_meter:
	@$(MAKE) -C $@ $(MAKECMDGOALS) $(SET_DRMETER_PREFIX)
dr_plugin dr_plugin_gui: dr_meter
	@$(MAKE) -C $@ $(MAKECMDGOALS) $(SET_RPATH)

.PHONY: $(TARGETS) $(SUBDIRS)
