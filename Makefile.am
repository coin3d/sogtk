############################################################################
## SoGtk/Makefile.am
## $Id$

############################################################################
# Automake conditional stuff

if BUILD_TESTS
TESTDIR = test-code
else
TESTDIR =
endif

SUBDIRS = . data src/Inventor/Gtk $(TESTDIR)

bin_SCRIPTS = sogtk-config

BUILT_SOURCES = \
	sogtkdefs.h \
	src/Inventor/Gtk/SoGtkBasic.h \
	src/Inventor/Gtk/SoGtkObject.h \
	src/Inventor/Gtk/SoGtkObject.cpp \
	src/Inventor/Gtk/SoGtkCursors.h \
	src/Inventor/Gtk/SoGtkCursors.cpp \
	src/Inventor/Gtk/SoGtkRenderArea.h \
	src/Inventor/Gtk/SoGtkRenderArea.cpp \
	src/Inventor/Gtk/SoAnyMaterialList.h \
	src/Inventor/Gtk/SoAnyMaterialList.cpp \
	src/Inventor/Gtk/SoAnyMaterialListBuiltins.cpp \
	src/Inventor/Gtk/viewers/SoGtkViewer.h \
	src/Inventor/Gtk/viewers/SoGtkViewer.cpp \
	src/Inventor/Gtk/viewers/SoAnyFullViewer.h \
	src/Inventor/Gtk/viewers/SoAnyFullViewer.cpp \
	src/Inventor/Gtk/viewers/SoAnyExaminerViewer.h \
	src/Inventor/Gtk/viewers/SoAnyExaminerViewer.cpp \
	src/Inventor/Gtk/viewers/SoAnyPlaneViewer.h \
	src/Inventor/Gtk/viewers/SoAnyPlaneViewer.cpp \
	src/Inventor/Gtk/widgets/SoAnyThumbWheel.h \
	src/Inventor/Gtk/widgets/SoAnyThumbWheel.cpp \
	src/Inventor/Gtk/widgets/SoAnyPopupMenu.h \
	src/Inventor/Gtk/widgets/SoAnyPopupMenu.cpp

EXTRA_DIST = cfg/m4/sogtk.m4

noinst_HEADERS = $(BUILT_SOURCES)

Makefile: $(BUILT_SOURCES)

############################################################################

sogtkdefs.h: $(top_srcdir)/sogtkdefs.h.in
	@./config.status --file=$@:sogtkdefs.h.in

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

src/Inventor/Gtk/viewers/SoGtkViewer.h: \
  $(srcdir)/src/Inventor/Gtk/common/SoGuiViewer.h.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoGuiViewer.h.in

src/Inventor/Gtk/viewers/SoGtkViewer.cpp: \
  $(srcdir)/src/Inventor/Gtk/common/SoGuiViewer.cpp.in
	@./config.status --file=$@:src/Inventor/Gtk/common/SoGuiViewer.cpp.in

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
