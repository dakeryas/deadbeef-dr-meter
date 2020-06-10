PREFIX ?= ${HOME}/.local/lib
OBJDIR := objects
SRCDIR := src
INCLDIR := include
DRPLUGDIR := ../dr_plugin
GTK ?= 3
PLUGBASE := ddb_dr_meter_gtk
PLUGIN := $(PLUGBASE)$(GTK).so

ifeq ($(GTK),2)
	GTKLIB=gtk+-2.0
else
	GTKLIB=gtk+-3.0
endif

OPTFLAG := -O3 -flto -DNDEBUG
CFLAGS ?= $(OPTFLAG) -std=c99 -Wall -fPIC
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

$(OBJECTS): | $(OBJDIR)
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLDIR)/%.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(PLUGIN): $(OBJECTS)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

install: $(PLUGIN)
	install -m 666 -D $(PLUGIN) $(DESTDIR)$(PREFIX)/deadbeef/$(PLUGIN)

clean:
	rm -f $(PLUGBASE)*.so $(OBJECTS)
