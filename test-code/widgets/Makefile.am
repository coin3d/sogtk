
INCLUDES = -I$(top_srcdir)/src
LIBS = -lgtkgl `gtk-config --libs` -lstdc++

noinst_PROGRAMS = thumbwheel
thumbwheel_SOURCES = thumbwheel.c
thumbwheel_LDADD = \
	$(top_builddir)/src/Inventor/Gtk/widgets/libSoGtkWidgets.la \
	$(top_builddir)/src/Inventor/Gtk/common/libSoCommon.la
# thumbwheel_LDLIBS = $(LIBS)


