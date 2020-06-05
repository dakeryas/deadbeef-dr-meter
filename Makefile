.PHONY: all debug install clean

all:
	$(MAKE) -C dr_plugin/
	$(MAKE) -C dr_plugin_gui/

debug:
	$(MAKE) -C dr_plugin/ debug
	$(MAKE) -C dr_plugin_gui/ debug

install:
	$(MAKE) -C dr_plugin/ install
	$(MAKE) -C dr_plugin_gui/ install

clean:
	$(MAKE) -C dr_plugin/ clean
	$(MAKE) -C dr_plugin_gui/ clean
