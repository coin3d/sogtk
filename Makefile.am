
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

SUBDIRS = src/Inventor/Gtk $(TESTDIR) $(EXAMPLEDIR)

BUILT_SOURCES = \
	src/Inventor/Gtk/SoGtkRenderArea.h \
	src/Inventor/Gtk/SoGtkRenderArea.cpp \
	src/Inventor/Gtk/viewers/SoGtkViewer.h \
	src/Inventor/Gtk/viewers/SoGtkViewer.cpp

EXTRA_DIST = conf-macros/sogtk.m4 conf-macros/modifierlist.m4

############################################################################

src/Inventor/Gtk/SoGtkRenderArea.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyRenderArea.h.in
	@CONFIG_HEADERS= CONFIG_FILES=$@:src/Inventor/Gtk/common/SoAnyRenderArea.h.in ./config.status

src/Inventor/Gtk/SoGtkRenderArea.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyRenderArea.cpp.in
	@CONFIG_HEADERS= CONFIG_FILES=$@:src/Inventor/Gtk/common/SoAnyRenderArea.cpp.in ./config.status

src/Inventor/Gtk/viewers/SoGtkViewer.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyViewer.h.in
	@CONFIG_HEADERS= CONFIG_FILES=$@:src/Inventor/Gtk/common/SoAnyViewer.h.in ./config.status

src/Inventor/Gtk/viewers/SoGtkViewer.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyViewer.cpp.in
	@CONFIG_HEADERS= CONFIG_FILES=$@:src/Inventor/Gtk/common/SoAnyViewer.cpp.in ./config.status

############################################################################
