PREFIX ?= ${HOME}/.local/lib/deadbeef
OBJDIR := objects
SRCDIR := src
INCLDIR := include
ifeq ($(OS), Windows_NT)
	EXT := dll
else
	EXT := so
endif
PLUGIN := ddb_dr_meter.$(EXT)

OPTFLAG := -O3 -flto -DNDEBUG
CFLAGS += $(OPTFLAG) -std=c99 -Wall -Wextra -fPIC
CPPFLAGS += -I./$(INCLDIR)
ifdef DRMETER_DIR
	CPPFLAGS += -I$(DRMETER_DIR)/include
	LDFLAGS += -L$(DRMETER_DIR)/lib
endif
LDFLAGS += -ldrmeter
LDFLAGS += -flto
ifdef RPATH
	LDFLAGS += -Wl,-rpath,$(RPATH)
	RPATH_HASH := $(shell echo $RPATH | md5sum | awk '{print $1}')
endif

OBJECTS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(wildcard $(SRCDIR)/*.c))

.PHONY: all debug install clean

all: $(PLUGIN)

debug: OPTFLAG := -O0 -g -DDEBUG
debug: all

$(OBJECTS): | $(OBJDIR)
$(OBJDIR):
	mkdir -p $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLDIR)/%.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(PLUGIN): $(OBJECTS) $(RPATH_HASH)
	$(CC) -shared -o $@ $(OBJECTS) $(LDFLAGS)

$(RPATH_HASH):
	touch $@

install: $(PLUGIN)
	install -m 666 -D $(PLUGIN) $(DESTDIR)$(PREFIX)/$(PLUGIN)

clean:
	rm -f $(PLUGIN) $(OBJECTS)
