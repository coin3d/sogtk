############################################################################

include ../common/Makefile.common

INCLUDES = -I$(top_srcdir)/src
LIBS = 

noinst_LTLIBRARIES = libSoGtkViewers.la

BUILT_SOURCES = \
	$(SoGuiAllBuiltFiles)

CLEANFILES = \
	$(SoGui_src_Inventor_Gui_viewers_BuiltMetaFiles)

DISTCLEANFILES = \
	$(SoGui_src_Inventor_Gui_viewers_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_viewers_BuiltMetaFiles)

EXTRA_DIST = \
	SoNativeFlyViewer.cpp

libSoGtkViewers_la_SOURCES = \
	SoGtkFullViewer.cpp \
	SoGtkFullViewer.h \
	SoGtkFullViewerP.h \
	SoGtkExaminerViewer.cpp \
	SoGtkExaminerViewer.h \
	SoGtkPlaneViewer.cpp \
	SoGtkPlaneViewer.h \
	SoGtkWalkViewer.cpp \
	SoGtkWalkViewer.h

nodist_libSoGtkViewers_la_SOURCES = \
	$(SoGui_src_Inventor_Gui_viewers_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_viewers_BuiltMetaFiles)

############################################################################

SoGtkViewersincdir = $(includedir)/Inventor/Gtk/viewers

SoGtkViewersinc_HEADERS = \
	SoGtkFullViewer.h \
	SoGtkExaminerViewer.h \
	SoGtkPlaneViewer.h \
	SoGtkWalkViewer.h

nodist_SoGtkViewersinc_HEADERS = \
	SoGtkViewer.h \
	SoGtkConstrainedViewer.h \
	SoGtkFlyViewer.h

############################################################################
