############################################################################

INCLUDES = -I$(top_srcdir)/src
LIBS = 

noinst_LTLIBRARIES = libSoGtkViewers.la

BUILT_SOURCES = \
	SoGtkViewer.cpp \
	SoGtkViewer.h \
	SoAnyFullViewer.cpp \
	SoAnyFullViewer.h \
	SoAnyExaminerViewer.cpp \
	SoAnyExaminerViewer.h \
	SoAnyPlaneViewer.cpp \
	SoAnyPlaneViewer.h \
	SoAnyConstrainedViewer.cpp \
	SoAnyConstrainedViewer.h \
	SoAnyFlyViewer.cpp \
	SoAnyFlyViewer.h \
	SoAnyWalkViewer.cpp \
	SoAnyWalkViewer.h

libSoGtkViewers_la_SOURCES = \
	SoGtkFullViewer.cpp \
	SoGtkFullViewer.h \
	SoGtkFullViewerP.h \
	SoGtkExaminerViewer.cpp \
	SoGtkExaminerViewer.h \
	SoGtkPlaneViewer.cpp \
	SoGtkPlaneViewer.h \
	SoGtkConstrainedViewer.cpp \
	SoGtkConstrainedViewer.h \
	SoGtkFlyViewer.cpp \
	SoGtkFlyViewer.h \
	SoGtkWalkViewer.cpp \
	SoGtkWalkViewer.h

nodist_libSoGtkViewers_la_SOURCES = \
	$(BUILT_SOURCES)

DISTCLEANFILES = \
	$(BUILT_SOURCES)

############################################################################

SoGtkViewersincdir = $(includedir)/Inventor/Gtk/viewers

SoGtkViewersinc_HEADERS = \
	SoGtkViewer.h \
	SoGtkFullViewer.h \
	SoGtkExaminerViewer.h \
	SoGtkPlaneViewer.h \
	SoGtkConstrainedViewer.h \
	SoGtkFlyViewer.h \
	SoGtkWalkViewer.h

############################################################################

SoGtkViewer.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiViewer.h.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Gtk/viewers/SoGtkViewer.h )

SoGtkViewer.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiViewer.cpp.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Gtk/viewers/SoGtkViewer.cpp )

SoAnyFullViewer.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyFullViewer.h.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Gtk/viewers/SoAnyFullViewer.h )

SoAnyFullViewer.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyFullViewer.cpp.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Gtk/viewers/SoAnyFullViewer.cpp )

SoAnyExaminerViewer.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyExaminerViewer.h.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Gtk/viewers/SoAnyExaminerViewer.h )

SoAnyExaminerViewer.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyExaminerViewer.cpp.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Gtk/viewers/SoAnyExaminerViewer.cpp )

SoAnyPlaneViewer.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyPlaneViewer.h.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Gtk/viewers/SoAnyPlaneViewer.h )

SoAnyPlaneViewer.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyPlaneViewer.cpp.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Gtk/viewers/SoAnyPlaneViewer.cpp )

SoAnyConstrainedViewer.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyConstrainedViewer.h.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Gtk/viewers/SoAnyConstrainedViewer.h )

SoAnyConstrainedViewer.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyConstrainedViewer.cpp.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Gtk/viewers/SoAnyConstrainedViewer.cpp )

SoAnyFlyViewer.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyFlyViewer.h.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Gtk/viewers/SoAnyFlyViewer.h )

SoAnyFlyViewer.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyFlyViewer.cpp.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Gtk/viewers/SoAnyFlyViewer.cpp )

SoAnyWalkViewer.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyWalkViewer.h.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Gtk/viewers/SoAnyWalkViewer.h )

SoAnyWalkViewer.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyWalkViewer.cpp.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Gtk/viewers/SoAnyWalkViewer.cpp )

############################################################################
