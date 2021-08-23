PREFIX ?= /usr/local
OBJDIR := objects
SRCDIR := src
INCLDIR := include
LIBDIR := lib
ifeq ($(OS), Windows_NT)
	EXT := dll
else
	EXT := so
endif
LIBRARY := $(LIBDIR)/libdrmeter.$(EXT)

OPTFLAG := -O3 -flto -DNDEBUG
CFLAGS += $(OPTFLAG) -std=c99 -Wall -Wextra -fPIC -pthread
CPPFLAGS += -I./$(INCLDIR)
LDFLAGS += -lm -lpthread -Wl,-O1,--sort-common,--as-needed,-z,relro -flto

OBJECTS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(wildcard $(SRCDIR)/*.c))

.PHONY: all debug install clean

all: $(LIBRARY)

debug: OPTFLAG := -O0 -g -DDEBUG
debug: all

$(OBJECTS): | $(OBJDIR)
$(OBJDIR) $(LIBDIR):
	mkdir -p $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLDIR)/%.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(LIBRARY): $(OBJECTS) | $(LIBDIR)
	$(CC) -shared -o $@ $(OBJECTS) $(LDFLAGS)

install: $(LIBRARY)
	install -m 644 -D $(LIBRARY) $(DESTDIR)$(PREFIX)/$(LIBRARY)

clean:
	rm -f $(OBJECTS) $(LIBRARY)
