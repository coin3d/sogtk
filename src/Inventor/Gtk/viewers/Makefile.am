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

libSoGtkViewers_la_SOURCES = \
	SoGtkExaminerViewer.cpp \
	SoGtkExaminerViewer.h \
	SoGtkPlaneViewer.cpp \
	SoGtkPlaneViewer.h \
	SoGtkWalkViewer.cpp \
	SoGtkWalkViewer.h \
	FullViewer.cpp

nodist_libSoGtkViewers_la_SOURCES = \
	$(SoGui_src_Inventor_Gui_viewers_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_viewers_BuiltMetaFiles)

############################################################################

SoGtkViewersincdir = $(includedir)/Inventor/Gtk/viewers

SoGtkViewersinc_HEADERS = \
	SoGtkExaminerViewer.h \
	SoGtkPlaneViewer.h \
	SoGtkWalkViewer.h

nodist_SoGtkViewersinc_HEADERS = \
	SoGtkViewer.h \
	SoGtkConstrainedViewer.h \
	SoGtkFullViewer.h \
	SoGtkFlyViewer.h

############################################################################
