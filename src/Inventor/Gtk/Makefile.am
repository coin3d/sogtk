
SUBDIRS = devices viewers widgets

INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src

lib_LTLIBRARIES = libSoGtk.la

libSoGtk_la_SOURCES = \
	SoGtk.cpp \
	SoGtk.h \
	SoGtkComponent.cpp \
	SoGtkComponent.h \
	SoGtkGLWidget.cpp \
	SoGtkGLWidget.h \
	$(top_builddir)/src/Inventor/Gtk/SoGtkRenderArea.cpp \
	$(top_builddir)/src/Inventor/Gtk/SoGtkRenderArea.h \
	SoGtkRenderAreaExtra.cpp

libSoGtk_la_LIBADD = \
        devices/libSoGtkDevices.la \
	viewers/libSoGtkViewers.la \
        widgets/libSoGtkWidgets.la

############################################################################

SoGtkRenderArea.h: $(top_srcdir)/src/Inventor/Gtk/common/SoAnyRenderArea.h.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoGtkRenderArea.h )

SoGtkRenderArea.cpp: $(top_srcdir)/src/Inventor/Gtk/common/SoAnyRenderArea.cpp.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoGtkRenderArea.cpp )

############################################################################
