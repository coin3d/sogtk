
SUBDIRS = devices widgets viewers

INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src

BUILT_SOURCES = \
	SoGtkBasic.h \
	SoGtkRenderArea.h \
	SoGtkRenderArea.cpp

lib_LTLIBRARIES = libSoGtk.la

libSoGtk_la_SOURCES = \
	$(BUILT_SOURCES) \
	SoGtk.h \
	SoGtk.cpp \
	SoGtkTypedObject.h \
	SoGtkTypedObject.cpp \
	SoGtkComponent.h \
	SoGtkComponent.cpp \
	SoGtkGLWidget.h \
	SoGtkGLWidget.cpp \
	SoGtkGraphEditor.h \
	SoGtkGraphEditor.cpp

libSoGtk_la_LIBADD = \
        devices/libSoGtkDevices.la \
	viewers/libSoGtkViewers.la \
        widgets/libSoGtkWidgets.la

libSoGtkincdir = $(includedir)/Inventor/Gtk
libSoGtkinc_HEADERS = \
	SoGtkBasic.h \
	SoGtk.h \
	SoGtkTypedObject.h \
	SoGtkComponent.h \
	SoGtkGLWidget.h \
	SoGtkRenderArea.h \
	SoGtkGraphEditor.h

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
