export DRMETER_DIR=$(PWD)/dr_meter

TARGETS := all debug install clean
SUBDIRS := dr_meter dr_plugin dr_plugin_gui

$(TARGETS): $(SUBDIRS)
dr_plugin: dr_meter
dr_plugin_gui: dr_meter
$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TARGETS) $(SUBDIRS)
