
SUBDIRS =

INCLUDES = -I$(top_srcdir)/src

noinst_LTLIBRARIES = libSoGtkWidgets.la

libSoGtkWidgets_la_SOURCES = \
	$(top_srcdir)/src/Inventor/Gtk/common/ThumbWheel.cpp \
	$(top_srcdir)/src/Inventor/Gtk/common/ThumbWheel.h \
	gtkthumbwheel.cpp \
	gtkthumbwheel.h

libSoGtkWidgets_la_LIBADD = 

ThumbWheel.lo: $(top_srcdir)/src/Inventor/Gtk/common/ThumbWheel.cpp
	source='$^' object='$@' libtool=yes \
	depfile='$(DEPDIR)/$*.Plo' tmpdepfile='$(DEPDIR)/$*.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LTCXXCOMPILE) -c -o $@ $^

