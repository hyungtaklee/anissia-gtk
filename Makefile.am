GTK_CFLAGS =	 				\
	-I$(top_srcdir)				\
	$(GTK_DEP_CFLAGS)

GTK_LIBS = 					\
        $(top_builddir)/gtk/libgtk-3.la 	\
        $(top_builddir)/gdk/libgdk-3.la 	\
        $(GTK_DEP_LIBS)


noinst_PROGRAMS = anissiaapp

anissiaapp_CFLAGS = $(GTK_CFLAGS)
anissiaapp_LDADD = $(GTK_LIBS)
anissiaapp_SOURCES = 				\
	main.c 					\
	anissiaapp.c anissiaapp.h 		\
	anissiaappwin.c anissiaappwin.h 	\
	resources.c

BUILT_SOURCES = resources.c

resources.c: anissiaapp.gresource.xml window.ui
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) $(srcdir)/anissiaapp.gresource.xml \
		--target=$@ --sourcedir=$(srcdir) --generate-source

EXTRA_DIST = 					\
	window.ui 				\
	anissiaapp.gresource.xml

-include $(top_srcdir)/git.mk
