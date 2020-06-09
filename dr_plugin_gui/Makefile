PREFIX ?= ${HOME}/.local/lib
OBJDIR := objects
SRCDIR := src
INCLDIR := include
DRPLUG_DIR := ../dr_plugin
PLUGIN := ddb_dr_meter_gui.so

OPTFLAG := -O3 -flto -DNDEBUG
CFLAGS ?= $(OPTFLAG) -std=c99 -Wall -fPIC
CPPFLAGS += $(shell pkg-config --cflags gtk+-3.0)
CPPFLAGS += -I./$(INCLDIR)
CPPFLAGS += -I$(DRPLUG_DIR)/include
ifdef DRMETER_DIR
	CPPFLAGS += -I$(DRMETER_DIR)/include
	LDFLAGS += -L$(DRMETER_DIR)/lib
endif
LDFLAGS += $(shell pkg-config --libs gtk+-3.0)
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
	rm -f $(PLUGIN) $(OBJECTS)
