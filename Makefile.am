
if BUILD_TESTS
TESTDIR = test-code
else
TESTDIR =
endif

if BUILD_EXAMPLES
EXAMPLEDIR = examples
else
EXAMPLEDIR =
endif

if BUILD_IV_EXAMPLES
IVEXAMPLEDIR = ivexamples
else
IVEXAMPLEDIR = 
endif

SUBDIRS = src/Inventor/Gtk $(TESTDIR) $(EXAMPLEDIR) $(IVEXAMPLEDIR)

BUILT_SOURCES = \
	src/Inventor/Gtk/SoGtkBasic.h \
	src/Inventor/Gtk/SoGtkRenderArea.h \
	src/Inventor/Gtk/SoGtkRenderArea.cpp \
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

EXTRA_DIST = conf-macros/sogtk.m4 conf-macros/modifierlist.m4

############################################################################

src/Inventor/Gtk/SoGtkBasic.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiBasic.h.in
	@CONFIG_HEADERS=$@:SoGtkBasic.h.in CONFIG_FILES=$(top_srcdir)/SoGtkBasic.h.in:src/Inventor/Gtk/common/SoGuiBasic.h.in ./config.status
	@echo "removing $(top_srcdir)/SoGtkBasic.h.in (temporary file)"
	@rm $(top_srcdir)/SoGtkBasic.h.in

src/Inventor/Gtk/SoGtkRenderArea.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiRenderArea.h.in
	@CONFIG_HEADERS= \
	CONFIG_FILES=$@:src/Inventor/Gtk/common/SoGuiRenderArea.h.in \
	./config.status

src/Inventor/Gtk/SoGtkRenderArea.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiRenderArea.cpp.in
	@CONFIG_HEADERS= \
	CONFIG_FILES=$@:src/Inventor/Gtk/common/SoGuiRenderArea.cpp.in \
	./config.status

src/Inventor/Gtk/viewers/SoGtkViewer.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiViewer.h.in
	@CONFIG_HEADERS= \
	CONFIG_FILES=$@:src/Inventor/Gtk/common/SoGuiViewer.h.in \
	./config.status

src/Inventor/Gtk/viewers/SoGtkViewer.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiViewer.cpp.in
	@CONFIG_HEADERS= \
	CONFIG_FILES=$@:src/Inventor/Gtk/common/SoGuiViewer.cpp.in \
	./config.status

src/Inventor/Gtk/viewers/SoAnyFullViewer.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyFullViewer.h.in
	@CONFIG_HEADERS= \
	CONFIG_FILES=$@:src/Inventor/Gtk/common/SoAnyFullViewer.h.in \
	./config.status

src/Inventor/Gtk/viewers/SoAnyFullViewer.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyFullViewer.cpp.in
	@CONFIG_HEADERS= \
	CONFIG_FILES=$@:src/Inventor/Gtk/common/SoAnyFullViewer.cpp.in \
	./config.status

src/Inventor/Gtk/viewers/SoAnyExaminerViewer.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyExaminerViewer.h.in
	@CONFIG_HEADERS= \
	CONFIG_FILES=$@:src/Inventor/Gtk/common/SoAnyExaminerViewer.h.in \
	./config.status

src/Inventor/Gtk/viewers/SoAnyExaminerViewer.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyExaminerViewer.cpp.in
	@CONFIG_HEADERS= \
	CONFIG_FILES=$@:src/Inventor/Gtk/common/SoAnyExaminerViewer.cpp.in \
	./config.status

src/Inventor/Gtk/viewers/SoAnyPlaneViewer.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyPlaneViewer.h.in
	@CONFIG_HEADERS= \
	CONFIG_FILES=$@:src/Inventor/Gtk/common/SoAnyPlaneViewer.h.in \
	./config.status

src/Inventor/Gtk/viewers/SoAnyPlaneViewer.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyPlaneViewer.cpp.in
	@CONFIG_HEADERS= \
	CONFIG_FILES=$@:src/Inventor/Gtk/common/SoAnyPlaneViewer.cpp.in \
	./config.status

src/Inventor/Gtk/widgets/SoAnyThumbWheel.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyThumbWheel.h.in
	@CONFIG_HEADERS= \
	CONFIG_FILES=$@:src/Inventor/Gtk/common/SoAnyThumbWheel.h.in \
	./config.status

src/Inventor/Gtk/widgets/SoAnyThumbWheel.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyThumbWheel.cpp.in
	@CONFIG_HEADERS= \
	CONFIG_FILES=$@:src/Inventor/Gtk/common/SoAnyThumbWheel.cpp.in \
	./config.status

src/Inventor/Gtk/widgets/SoAnyPopupMenu.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyPopupMenu.h.in
	@CONFIG_HEADERS= \
	CONFIG_FILES=$@:src/Inventor/Gtk/common/SoAnyPopupMenu.h.in \
	./config.status

src/Inventor/Gtk/widgets/SoAnyPopupMenu.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyPopupMenu.cpp.in
	@CONFIG_HEADERS= \
	CONFIG_FILES=$@:src/Inventor/Gtk/common/SoAnyPopupMenu.cpp.in \
	./config.status

############################################################################
