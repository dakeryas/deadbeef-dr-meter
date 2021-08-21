PREFIX ?= ${HOME}/.local/lib/deadbeef
OBJDIR := objects
SRCDIR := src
INCLDIR := include
PLUGIN := ddb_dr_meter.so

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

$(PLUGIN): $(OBJECTS)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

install: $(PLUGIN)
	install -m 666 -D $(PLUGIN) $(DESTDIR)$(PREFIX)/$(PLUGIN)

clean:
	rm -f $(PLUGIN) $(OBJECTS)
