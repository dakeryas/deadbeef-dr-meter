PREFIX := /usr/local
OBJDIR := objects
SRCDIR := src
INCLDIR := include
LIBRARY := libdrmeter.so

OPTFLAG := -O3 -flto -DNDEBUG
CFLAGS += $(OPTFLAG) -std=c99 -Wall -fPIC -pthread
CPPFLAGS += -I./$(INCLDIR)
LDFLAGS += -flto -lpthread

OBJECTS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(wildcard $(SRCDIR)/*.c))

.PHONY: all debug install clean

all: $(LIBRARY)

debug: OPTFLAG := -O0 -g -DDEBUG
debug: all

$(OBJECTS): | $(OBJDIR)
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLDIR)/%.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(LIBRARY): $(OBJECTS)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

install: all
	install -m 666 -D $(LIBRARY) $(DESTDIR)$(PREFIX)/lib/$(LIBRARY)

clean:
	rm -f $(OBJECTS) $(LIBRARY)
