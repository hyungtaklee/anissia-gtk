CC ?= gcc
PKGCONFIG = $(shell which pkg-config)
CFLAGS = $(shell $(PKGCONFIG) --cflags gtk+-3.0)
LIBS = $(shell $(PKGCONFIG) --libs gtk+-3.0)
GLIB_COMPILE_RESOURCES = $(shell $(PKGCONFIG) --variable=glib_compile_resources gio-2.0)

SRC = anissiaapp.c cJSON.c requester.c anissiaapplist.c anissiaappwin.c  main.c
BUILT_SRC = resources.c

OBJS = $(BUILT_SRC:.c=.o) $(SRC:.c=.o)

all: anissiaapp

resources.c: anissiaapp.gresource.xml window.ui app-menu.ui
	$(GLIB_COMPILE_RESOURCES) anissiaapp.gresource.xml --target=$@ --sourcedir=. --generate-source

%.o: %.c
	$(CC) -c -o $(@F) $(CFLAGS) $<

anissiaapp: $(OBJS)
	$(CC) -o $(@F) $(OBJS) $(LIBS)

clean:
	rm -f $(BUILT_SRC)
	rm -f $(OBJS)
	rm -f anissiaapp
