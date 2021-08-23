PREFIX ?= ${HOME}/.local/lib/deadbeef
OBJDIR := objects
SRCDIR := src
INCLDIR := include
DRPLUGDIR := ../dr_plugin
GTK ?= 3
PLUGBASE := ddb_dr_meter_gtk
ifeq ($(OS), Windows_NT)
	EXT := dll
else
	EXT := so
endif
PLUGIN := $(PLUGBASE)$(GTK).$(EXT)

ifeq ($(GTK),2)
	GTKLIBNAME=gtk+-2.0
else
	GTKLIBNAME=gtk+-3.0
endif
GTKCFLAGS ?= $(shell pkg-config --cflags $(GTKLIBNAME))
GTKLIBS ?= $(shell pkg-config --libs $(GTKLIBNAME))

OPTFLAG := -O3 -flto -DNDEBUG
CFLAGS += $(OPTFLAG) -std=c99 -Wall -Wextra -fPIC
CFLAGS += $(GTKCFLAGS)
CPPFLAGS += -I./$(INCLDIR)
CPPFLAGS += -I$(DRPLUGDIR)/include
ifdef DRMETER_DIR
	CPPFLAGS += -I$(DRMETER_DIR)/include
	LDFLAGS += -L$(DRMETER_DIR)/lib
endif
LDFLAGS += $(GTKLIBS)
LDFLAGS += -ldrmeter -Wl,-O1,--sort-common,--as-needed -flto

RPATHCACHE_BASE := rpath_cache
ifdef RPATH
	LDFLAGS += -Wl,-rpath=$(RPATH)
	RPATHCACHE := $(RPATHCACHE_BASE).$(shell echo $(RPATH) | md5sum | cut -d ' ' -f 1)
endif
GTKCACHE_BASE := gtk_version_cache
GTKCACHE := $(GTKCACHE_BASE).$(GTK)

OBJECTS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(wildcard $(SRCDIR)/*.c))

.PHONY: all debug install clean

all: $(PLUGIN)

debug: OPTFLAG := -O0 -g -DDEBUG
debug: all

$(RPATHCACHE):
	rm -f $(RPATHCACHE_BASE).*
	touch $@

$(GTKCACHE):
	rm -f $(GTKCACHE_BASE).*
	touch $@

$(OBJECTS): | $(OBJDIR)
$(OBJDIR):
	mkdir -p $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLDIR)/%.h $(GTKCACHE)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(PLUGIN): $(OBJECTS) $(RPATHCACHE) $(GTKCACHE)
	$(CC) -shared -o $@ $(OBJECTS) $(LDFLAGS)

install: $(PLUGIN)
	install -m 644 -D $(PLUGIN) $(DESTDIR)$(PREFIX)/$(PLUGIN)

clean:
	rm -f $(PLUGBASE)*.so $(OBJECTS) $(RPATHCACHE_BASE).* $(GTKCACHE_BASE).*
