############################################################################
## SoGtk/Makefile.am
## $Id$

ACLOCAL_AMFLAGS = -I cfg/m4

############################################################################
# Automake conditional stuff

if BUILD_TESTS
TESTDIR = test-code
else
TESTDIR =
endif

if BUILD_INTL
INTLDIRS = po intl
else
INTLDIRS =
endif

############################################################################

SUBDIRS = . data src/Inventor/Gtk $(INTLDIRS) $(TESTDIR)

BUILT_SOURCES = \
	src/sogtkdefs.h \
	src/Inventor/Gtk/SoAny.h \
	src/Inventor/Gtk/SoAny.cpp \
	src/Inventor/Gtk/SoGtkBasic.h \
	src/Inventor/Gtk/SoGtkObject.h \
	src/Inventor/Gtk/SoGtkObject.cpp \
	src/Inventor/Gtk/SoGtkCursors.h \
	src/Inventor/Gtk/SoGtkCursors.cpp \
	src/Inventor/Gtk/SoGtkGLWidget.h \
	src/Inventor/Gtk/SoGtkRenderArea.h \
	src/Inventor/Gtk/SoGtkRenderArea.cpp \
	src/Inventor/Gtk/SoAnyMaterialList.h \
	src/Inventor/Gtk/SoAnyMaterialList.cpp \
	src/Inventor/Gtk/SoAnyMaterialListBuiltins.cpp \
	src/Inventor/Gtk/common/gl.h \
	src/Inventor/Gtk/devices/spwinput.h \
	src/Inventor/Gtk/devices/spwinput_x11.cpp \
	src/Inventor/Gtk/viewers/SoGtkViewer.h \
	src/Inventor/Gtk/viewers/SoGtkViewer.cpp \
	src/Inventor/Gtk/viewers/SoAnyFullViewer.h \
	src/Inventor/Gtk/viewers/SoAnyFullViewer.cpp \
	src/Inventor/Gtk/viewers/SoAnyExaminerViewer.h \
	src/Inventor/Gtk/viewers/SoAnyExaminerViewer.cpp \
	src/Inventor/Gtk/viewers/SoAnyPlaneViewer.h \
	src/Inventor/Gtk/viewers/SoAnyPlaneViewer.cpp \
	src/Inventor/Gtk/viewers/SoAnyConstrainedViewer.h \
	src/Inventor/Gtk/viewers/SoAnyConstrainedViewer.cpp \
	src/Inventor/Gtk/viewers/SoAnyFlyViewer.h \
	src/Inventor/Gtk/viewers/SoAnyFlyViewer.cpp \
	src/Inventor/Gtk/viewers/SoAnyWalkViewer.h \
	src/Inventor/Gtk/viewers/SoAnyWalkViewer.cpp \
	src/Inventor/Gtk/widgets/SoAnyThumbWheel.h \
	src/Inventor/Gtk/widgets/SoAnyThumbWheel.cpp \
	src/Inventor/Gtk/widgets/SoAnyPopupMenu.h \
	src/Inventor/Gtk/widgets/SoAnyPopupMenu.cpp

bin_SCRIPTS = so@gui@-config

EXTRA_DIST = \
	so@gui@.m4 \
	src/Inventor/Gtk/common/soguidefs.h.in \
	src/Inventor/Gtk/common/sogui-config.in \
	src/Inventor/Gtk/common/gl.h.in

m4datadir = $(datadir)/aclocal
m4data_DATA = so@gui@.m4

cfgdatadir = $(datadir)/Coin/conf
cfgdata_DATA = so@gui@-@CONFIG@.cfg

so@gui@-config: Makefile config.status $(srcdir)/src/Inventor/@Gui@/common/sogui-config.in
	@$(SHELL) config.status --file=$@:src/Inventor/@Gui@/common/sogui-config.in

Makefile: $(BUILT_SOURCES)

############################################################################

built-sources: $(BUILT_SOURCES)

src/sogtkdefs.h: $(srcdir)/src/Inventor/Gtk/common/soguidefs.h.in
	@./config.status --file=$@:src/Inventor/Gtk/common/soguidefs.h.in

src/Inventor/Gtk/SoGtkBasic.h: \
  $(srcdir)/src/Inventor/Gtk/common/SoGuiBasic.h.in
	@./config.status --file=-:src/Inventor/Gtk/common/SoGuiBasic.h.in | \
	./config.status --header=$@:-

src/Inventor/Gtk/SoGtkObject.h: \
  $(srcdir)/src/Inventor/Gtk/common/SoGuiObject.h.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoGuiObject.h.in

src/Inventor/Gtk/SoGtkObject.cpp: \
  $(srcdir)/src/Inventor/Gtk/common/SoGuiObject.cpp.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoGuiObject.cpp.in

src/Inventor/Gtk/SoGtkCursors.h: \
  $(srcdir)/src/Inventor/Gtk/common/SoGuiCursors.h.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoGuiCursors.h.in

src/Inventor/Gtk/SoGtkCursors.cpp: \
  $(srcdir)/src/Inventor/Gtk/common/SoGuiCursors.cpp.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoGuiCursors.cpp.in

src/Inventor/Gtk/SoGtkGLWidget.h: \
  $(srcdir)/src/Inventor/Gtk/common/SoGuiGLWidget.h.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoGuiGLWidget.h.in

src/Inventor/Gtk/SoGtkRenderArea.h: \
  $(srcdir)/src/Inventor/Gtk/common/SoGuiRenderArea.h.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoGuiRenderArea.h.in

src/Inventor/Gtk/SoGtkRenderArea.cpp: \
  $(srcdir)/src/Inventor/Gtk/common/SoGuiRenderArea.cpp.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoGuiRenderArea.cpp.in

src/Inventor/Gtk/SoAnyMaterialList.h: \
  $(srcdir)/src/Inventor/Gtk/common/SoAnyMaterialList.h.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoAnyMaterialList.h.in

src/Inventor/Gtk/SoAnyMaterialList.cpp: \
  $(srcdir)/src/Inventor/Gtk/common/SoAnyMaterialList.cpp.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoAnyMaterialList.cpp.in

src/Inventor/Gtk/SoAnyMaterialListBuiltins.cpp: \
  $(srcdir)/src/Inventor/Gtk/common/SoAnyMaterialListBuiltins.cpp.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoAnyMaterialListBuiltins.cpp.in

src/Inventor/Gtk/common/gl.h: $(srcdir)/src/Inventor/Gtk/common/gl.h.in
	@if test -d src/Inventor/Gtk/common; then :; else \
	  echo " mkdir src/Inventor/Gtk/common"; \
	  mkdir src/Inventor/Gtk/common; \
	fi
	@./config.status --file=$@:src/Inventor/Gtk/common/gl.h.in

src/Inventor/Gtk/devices/spwinput.h: \
  $(srcdir)/src/Inventor/Gtk/common/spwinput.h.in
	@./config.status --file=$@:src/Inventor/Gtk/common/spwinput.h.in

src/Inventor/Gtk/devices/spwinput_x11.cpp: \
  $(srcdir)/src/Inventor/Gtk/common/spwinput_x11.cpp.in
	@./config.status --file=$@:src/Inventor/Gtk/common/spwinput_x11.cpp.in

src/Inventor/Gtk/viewers/SoGtkViewer.h: \
  $(srcdir)/src/Inventor/Gtk/common/SoGuiViewer.h.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoGuiViewer.h.in

src/Inventor/Gtk/viewers/SoGtkViewer.cpp: \
  $(srcdir)/src/Inventor/Gtk/common/SoGuiViewer.cpp.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoGuiViewer.cpp.in

src/Inventor/Gtk/SoAny.h: \
  $(srcdir)/src/Inventor/Gtk/common/SoAny.h.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoAny.h.in

src/Inventor/Gtk/SoAny.cpp: \
  $(srcdir)/src/Inventor/Gtk/common/SoAny.cpp.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoAny.cpp.in

src/Inventor/Gtk/viewers/SoAnyFullViewer.h: \
  $(srcdir)/src/Inventor/Gtk/common/SoAnyFullViewer.h.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoAnyFullViewer.h.in

src/Inventor/Gtk/viewers/SoAnyFullViewer.cpp: \
  $(srcdir)/src/Inventor/Gtk/common/SoAnyFullViewer.cpp.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoAnyFullViewer.cpp.in

src/Inventor/Gtk/viewers/SoAnyExaminerViewer.h: \
  $(srcdir)/src/Inventor/Gtk/common/SoAnyExaminerViewer.h.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoAnyExaminerViewer.h.in

src/Inventor/Gtk/viewers/SoAnyExaminerViewer.cpp: \
  $(srcdir)/src/Inventor/Gtk/common/SoAnyExaminerViewer.cpp.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoAnyExaminerViewer.cpp.in

src/Inventor/Gtk/viewers/SoAnyPlaneViewer.h: \
  $(srcdir)/src/Inventor/Gtk/common/SoAnyPlaneViewer.h.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoAnyPlaneViewer.h.in

src/Inventor/Gtk/viewers/SoAnyPlaneViewer.cpp: \
  $(srcdir)/src/Inventor/Gtk/common/SoAnyPlaneViewer.cpp.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoAnyPlaneViewer.cpp.in

src/Inventor/Gtk/viewers/SoAnyConstrainedViewer.h: \
  $(srcdir)/src/Inventor/Gtk/common/SoAnyConstrainedViewer.h.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoAnyConstrainedViewer.h.in

src/Inventor/Gtk/viewers/SoAnyConstrainedViewer.cpp: \
  $(srcdir)/src/Inventor/Gtk/common/SoAnyConstrainedViewer.cpp.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoAnyConstrainedViewer.cpp.in

src/Inventor/Gtk/viewers/SoAnyFlyViewer.h: \
  $(srcdir)/src/Inventor/Gtk/common/SoAnyFlyViewer.h.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoAnyFlyViewer.h.in

src/Inventor/Gtk/viewers/SoAnyFlyViewer.cpp: \
  $(srcdir)/src/Inventor/Gtk/common/SoAnyFlyViewer.cpp.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoAnyFlyViewer.cpp.in

src/Inventor/Gtk/viewers/SoAnyWalkViewer.h: \
  $(srcdir)/src/Inventor/Gtk/common/SoAnyWalkViewer.h.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoAnyWalkViewer.h.in

src/Inventor/Gtk/viewers/SoAnyWalkViewer.cpp: \
  $(srcdir)/src/Inventor/Gtk/common/SoAnyWalkViewer.cpp.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoAnyWalkViewer.cpp.in

src/Inventor/Gtk/widgets/SoAnyThumbWheel.h: \
  $(srcdir)/src/Inventor/Gtk/common/SoAnyThumbWheel.h.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoAnyThumbWheel.h.in

src/Inventor/Gtk/widgets/SoAnyThumbWheel.cpp: \
  $(srcdir)/src/Inventor/Gtk/common/SoAnyThumbWheel.cpp.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoAnyThumbWheel.cpp.in

src/Inventor/Gtk/widgets/SoAnyPopupMenu.h: \
  $(srcdir)/src/Inventor/Gtk/common/SoAnyPopupMenu.h.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoAnyPopupMenu.h.in

src/Inventor/Gtk/widgets/SoAnyPopupMenu.cpp: \
  $(srcdir)/src/Inventor/Gtk/common/SoAnyPopupMenu.cpp.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoAnyPopupMenu.cpp.in

############################################################################

doxygen-doc:
	@( cd build; @sim_ac_doxygen_exe@ so@gui@.doxygen )

doxygen-docs:
	@( cd build; @sim_ac_doxygen_exe@ so@gui@.doxygen )

