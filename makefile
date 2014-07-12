CFLAGS := -O2 -c -Wall -Werror -fPIC
LDFLAGS := -shared

SRCPATH := ./src
BUILDPATH := ./build

SOURCES := xif.c
HEADERS := xif.h
OBJECTS := $(addprefix $(BUILDPATH)/,$(SOURCES:%.c=%.o))
SOURCES := $(addprefix $(SRCPATH)/,$(SOURCES))

OUTFILE := libxif.so

PREFIX := /usr
LIBDIR := /lib
INCLDIR := /include
LIBPATH := $(DESTDIR)$(PREFIX)$(LIBDIR)
INCLPATH := $(DESTDIR)$(PREFIX)$(INCLDIR)

.PHONY : all install uninstall clean

all : $(OUTFILE)

$(OUTFILE) : $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^
	strip $@

$(BUILDPATH)/%.o : $(SRCPATH)/%.c
	$(CC) $(CFLAGS) -o $@ $^

install : $(LIBPATH)/$(OUTFILE) $(addprefix $(INCLPATH)/,$(HEADERS))

$(LIBPATH)/$(OUTFILE) : $(OUTFILE)
	install -Dm755 "$^" "$@"

$(INCLPATH)/%.h : $(SRCPATH)/%.h
	install -Dm755 "$^" "$@"

uninstall :
	rm -f "$(LIBPATH)/$(OUTFILE)"
	rm -f $(addsuffix ",$(addprefix "$(INCLPATH)/,$(HEADERS)))

clean :
	rm $(BUILDPATH)/*
	rm $(OUTFILE)
