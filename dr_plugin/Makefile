PREFIX := ${HOME}/.local
OBJDIR := objects
SRCDIR := src
INCLDIR := include
PLUGIN := ddb_dr_meter.so

OPTFLAG := -O3 -flto -DNDEBUG
CFLAGS += $(OPTFLAG) -std=c99 -Wall -fPIC -pthread
CPPFLAGS += -I./$(INCLDIR)
LDFLAGS += -flto -lpthread

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
	install -m 666 -D $(PLUGIN) $(PREFIX)/lib/deadbeef

clean:
	rm -f $(OBJECTS) $(PLUGIN)
