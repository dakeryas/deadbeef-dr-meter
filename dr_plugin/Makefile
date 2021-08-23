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
LDFLAGS += -ldrmeter -Wl,-O1,--sort-common,--as-needed -flto

RPATHCACHE_BASE := rpath_cache
ifdef RPATH
	LDFLAGS += -Wl,-rpath=$(RPATH)
	RPATHCACHE := $(RPATHCACHE_BASE).$(shell echo $(RPATH) | md5sum | cut -d ' ' -f 1)
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

$(PLUGIN): $(OBJECTS) $(RPATHCACHE)
	$(CC) -shared -o $@ $(OBJECTS) $(LDFLAGS)

$(RPATHCACHE):
	rm -f $(RPATHCACHE_BASE).*
	touch $@

install: $(PLUGIN)
	install -m 644 -D $(PLUGIN) $(DESTDIR)$(PREFIX)/$(PLUGIN)

clean:
	rm -f $(PLUGIN) $(OBJECTS) $(RPATHCACHE_BASE).*
