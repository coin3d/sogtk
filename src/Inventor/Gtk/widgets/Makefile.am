############################################################################
## SoGtk/src/Inventor/Gtk/widgets/Makefile.am

INCLUDES = -I$(top_srcdir)/src

DISTCLEANFILES = \
	$(BUILT_SOURCES)

BUILT_SOURCES = \
	SoAnyThumbWheel.h \
	SoAnyThumbWheel.cpp \
	SoAnyPopupMenu.h \
	SoAnyPopupMenu.cpp

noinst_LTLIBRARIES = libSoGtkWidgets.la

libSoGtkWidgets_la_SOURCES = \
	gtkthumbwheel.h \
	gtkthumbwheel.cpp \
	SoGtkPopupMenu.h \
	SoGtkPopupMenu.cpp

nodist_libSoGtkWidgets_la_SOURCES = \
	$(BUILT_SOURCES)

libSoGtkWidgets_la_LIBADD = 

############################################################################

SoAnyThumbWheel.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyThumbWheel.h.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Gtk/SoAnyThumbWheel.h )

SoAnyThumbWheel.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyThumbWheel.cpp.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Gtk/SoAnyThumbWheel.cpp )

SoAnyPopupMenu.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyPopupMenu.h.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Gtk/SoAnyPopupMenu.h )

SoAnyPopupMenu.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyPopupMenu.cpp.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Gtk/SoAnyPopupMenu.cpp )

############################################################################
