PLUGIN_SOURCES = src/dr_meter_plugin_gui.c include/dr_meter_plugin_gui.h
PLUGIN_CPPFLAGS = -I$(srcdir)/include -I$(top_builddir)/dr_plugin/include -I$(top_builddir)/dr_meter/include
PLUGIN_LDFLAGS = -module -avoid-version
PLUGIN_LIBS = $(top_builddir)/dr_meter/libdrmeter.la

lib_LTLIBRARIES =

if HAS_GTK2
lib_LTLIBRARIES += ddb_dr_meter_gtk2.la
ddb_dr_meter_gtk2_la_SOURCES = $(PLUGIN_SOURCES)
ddb_dr_meter_gtk2_la_CPPFLAGS = $(PLUGIN_CPPFLAGS)
ddb_dr_meter_gtk2_la_CFLAGS = $(GTK2_CFLAGS)
ddb_dr_meter_gtk2_la_LDFLAGS = $(PLUGIN_LDFLAGS)
ddb_dr_meter_gtk2_la_LIBADD = $(GTK2_LIBS) $(PLUGIN_LIBS)
endif

if HAS_GTK3
lib_LTLIBRARIES += ddb_dr_meter_gtk3.la
ddb_dr_meter_gtk3_la_SOURCES = $(PLUGIN_SOURCES)
ddb_dr_meter_gtk3_la_CPPFLAGS = $(PLUGIN_CPPFLAGS)
ddb_dr_meter_gtk3_la_CFLAGS = $(GTK3_CFLAGS)
ddb_dr_meter_gtk3_la_LDFLAGS = $(PLUGIN_LDFLAGS)
ddb_dr_meter_gtk3_la_LIBADD = $(GTK3_LIBS) $(PLUGIN_LIBS)
endif
