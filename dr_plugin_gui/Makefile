PREFIX ?= ${HOME}/.local/lib/deadbeef
OBJDIR := objects
SRCDIR := src
INCLDIR := include
DRPLUGDIR := ../dr_plugin
GTK ?= 3
GTKCACHEBASE := gtk_version_cache_
GTKCACHE := $(GTKCACHEBASE)$(GTK)
PLUGBASE := ddb_dr_meter_gtk
PLUGIN := $(PLUGBASE)$(GTK).so

ifeq ($(GTK),2)
	GTKLIB=gtk+-2.0
else
	GTKLIB=gtk+-3.0
endif

OPTFLAG := -O3 -flto -DNDEBUG
CFLAGS ?= $(OPTFLAG) -std=c99 -Wall -Wextra -fPIC
CPPFLAGS += $(shell pkg-config --cflags $(GTKLIB))
CPPFLAGS += -I./$(INCLDIR)
CPPFLAGS += -I$(DRPLUGDIR)/include
ifdef DRMETER_DIR
	CPPFLAGS += -I$(DRMETER_DIR)/include
	LDFLAGS += -L$(DRMETER_DIR)/lib
endif
LDFLAGS += $(shell pkg-config --libs $(GTKLIB))
LDFLAGS += -ldrmeter
LDFLAGS += -flto
ifdef RPATH
	LDFLAGS += -Wl,-rpath,$(RPATH)
endif

OBJECTS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(wildcard $(SRCDIR)/*.c))

.PHONY: all debug install clean

all: $(PLUGIN)

debug: OPTFLAG := -O0 -g -DDEBUG
debug: all

$(GTKCACHE):
	rm -f $(GTKCACHEBASE)*
	touch $(GTKCACHE)

$(OBJECTS): | $(OBJDIR)
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLDIR)/%.h $(GTKCACHE)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(PLUGIN): $(OBJECTS) $(GTKCACHE)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

install: $(PLUGIN)
	install -m 666 -D $(PLUGIN) $(DESTDIR)$(PREFIX)/$(PLUGIN)

clean:
	rm -f $(PLUGBASE)*.so $(OBJECTS) $(GTKCACHEBASE)*
