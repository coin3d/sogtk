
SUBDIRS = devices widgets viewers

INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src

BUILT_SOURCES = \
	SoGtkBasic.h \
	SoGtkCursors.h \
	SoGtkCursors.cpp \
	SoGtkRenderArea.h \
	SoGtkRenderArea.cpp \
	SoAnyMaterialList.h \
	SoAnyMaterialList.cpp \
	SoAnyMaterialListBuiltins.cpp

lib_LTLIBRARIES = libSoGtk.la

libSoGtk_la_SOURCES = \
	$(BUILT_SOURCES) \
	SoGtk.h \
	SoGtk.cpp \
	SoGtkObject.h \
	SoGtkObject.cpp \
	SoGtkComponent.h \
	SoGtkComponent.cpp \
	SoGtkGLWidget.h \
	SoGtkGLWidget.cpp \
	SoGtkMaterialList.h \
	SoGtkMaterialList.cpp \
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
	SoGtkObject.h \
	SoGtkCursors.h \
	SoGtkComponent.h \
	SoGtkGLWidget.h \
	SoGtkRenderArea.h \
	SoGtkGraphEditor.h \
	SoGtkMaterialList.h

############################################################################

SoGtkBasic.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiBasic.h.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoGtkBasic.h )

SoGtkCursors.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiCursors.h.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoGtkCursors.h )

SoGtkCursors.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiCursors.cpp.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoGtkCursors.cpp )

SoGtkRenderArea.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiRenderArea.h.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoGtkRenderArea.h )

SoGtkRenderArea.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiRenderArea.cpp.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoGtkRenderArea.cpp )

SoAnyMaterialList.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyMaterialList.h.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoAnyMaterialList.h )

SoAnyMaterialList.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyMaterialList.cpp.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoAnyMaterialList.cpp )

SoAnyMaterialListBuiltins.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyMaterialListBuiltins.cpp.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoAnyMaterialListBuiltins.cpp )

############################################################################
