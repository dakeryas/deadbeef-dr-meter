PREFIX := ${HOME}/.local
OBJDIR := objects
SRCDIR := src
INCLDIR := include
PLUGIN := ddb_dr_meter_gui.so

OPTFLAG := -O3 -flto -DNDEBUG
CFLAGS += $(OPTFLAG) -std=c99 -Wall -fPIC
CPPFLAGS += -I./$(INCLDIR) -I$(DRMETER_DIR)/include
CPPFLAGS += $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS += -flto
LDFLAGS += -L$(DRMETER_DIR)/lib -ldrmeter
LDFLAGS += $(shell pkg-config --libs gtk+-3.0)

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

$(PLUGIN): $(OBJECTS) ../dr_plugin/ddb_dr_meter.so
	$(CC) -shared -o $@ $^ $(LDFLAGS)

install: LDFLAGS += -Wl,-rpath=.
install: all
	install -m 666 -D $(PLUGIN) $(DESTDIR)$(PREFIX)/lib/deadbeef/$(PLUGIN)

clean:
	rm -f $(OBJECTS) $(PLUGIN)
