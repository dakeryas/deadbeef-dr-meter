PREFIX ?= ${HOME}/.local/lib
OBJDIR := objects
SRCDIR := src
INCLDIR := include
PLUGIN := ddb_dr_meter.so

OPTFLAG := -O3 -flto -DNDEBUG
CFLAGS ?= $(OPTFLAG) -std=c99 -Wall -fPIC
CPPFLAGS += -I./$(INCLDIR) -I$(DRMETER_DIR)/include
LDFLAGS += -L$(DRMETER_DIR)/lib -ldrmeter

OBJECTS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(wildcard $(SRCDIR)/*.c))

.PHONY: all debug install clean clean_plugin

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

install: LDFLAGS += -Wl,-rpath='$$ORIGIN/lib'
install: clean_plugin $(PLUGIN)
	install -m 666 -D $(PLUGIN) $(DESTDIR)$(PREFIX)/deadbeef/$(PLUGIN)

clean_plugin:
	rm -f $(PLUGIN)

clean: clean_plugin
	rm -f $(OBJECTS)
