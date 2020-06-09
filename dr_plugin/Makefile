PREFIX ?= ${HOME}/.local/lib
OBJDIR := objects
SRCDIR := src
INCLDIR := include
PLUGIN := ddb_dr_meter.so

OPTFLAG := -O3 -flto -DNDEBUG
CFLAGS ?= $(OPTFLAG) -std=c99 -Wall -fPIC
CPPFLAGS += -I./$(INCLDIR) -I$(DRMETER_DIR)/include
ifdef RPATH
	LDFLAGS ?= -Wl,-rpath,$(RPATH)
else
	LDFLAGS ?= -Wl,-rpath,$(DRMETER_DIR)/lib
endif
LDFLAGS += -flto
LDFLAGS += -L$(DRMETER_DIR)/lib -ldrmeter

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
