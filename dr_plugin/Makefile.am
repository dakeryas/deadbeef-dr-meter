lib_LTLIBRARIES = ddb_dr_meter.la
ddb_dr_meter_la_SOURCES = src/dr_meter_plugin.c include/dr_meter_plugin.h
ddb_dr_meter_la_CPPFLAGS =  -I$(srcdir)/include -I$(top_srcdir)/dr_meter/include
ddb_dr_meter_la_LDFLAGS = -module -avoid-version
ddb_dr_meter_la_LIBADD = $(top_builddir)/dr_meter/libdrmeter.la
