
SUBDIRS = devices widgets viewers

INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src

BUILT_SOURCES = \
	SoGtkBasic.h \
	SoGtkRenderArea.h \
	SoGtkRenderArea.cpp

lib_LTLIBRARIES = libSoGtk.la

libSoGtk_la_SOURCES = \
	$(BUILT_SOURCES) \
	SoGtk.cpp \
	SoGtk.h \
	SoGtkComponent.cpp \
	SoGtkComponent.h \
	SoGtkGLWidget.cpp \
	SoGtkGLWidget.h \
	SoGtkRenderAreaExtra.cpp

libSoGtk_la_LIBADD = \
        devices/libSoGtkDevices.la \
	viewers/libSoGtkViewers.la \
        widgets/libSoGtkWidgets.la

############################################################################

SoGtkBasic.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiBasic.h.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoGtkBasic.h )

SoGtkRenderArea.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiRenderArea.h.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoGtkRenderArea.h )

SoGtkRenderArea.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiRenderArea.cpp.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoGtkRenderArea.cpp )

############################################################################
