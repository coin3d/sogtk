############################################################################
## SoGtk/src/Inventor/Gtk/widgets/Makefile.am

include ../common/Makefile.common

INCLUDES = -I$(top_srcdir)/src

BUILT_SOURCES = \
	$(SoGuiAllBuiltSources)

CLEANFILES = \
	$(SoGui_src_Inventor_Gui_widgets_BuiltMetaFiles)

DISTCLEANFILES = \
	$(SoGui_src_Inventor_Gui_widgets_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_widgets_BuiltMetaFiles)

noinst_LTLIBRARIES = libSoGtkWidgets.la

libSoGtkWidgets_la_SOURCES = \
	gtkthumbwheel.h \
	gtkthumbwheel.cpp \
	GtkNativePopupMenu.h \
	GtkNativePopupMenu.cpp

nodist_libSoGtkWidgets_la_SOURCES = \
	$(SoGui_src_Inventor_Gui_widgets_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_widgets_BuiltMetaFiles)

libSoGtkWidgets_la_LIBADD = 

############################################################################
