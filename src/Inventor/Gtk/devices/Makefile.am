# **************************************************************************
## Process this file with automake to generate Makefile.in.

include ../common/Makefile.common

noinst_LTLIBRARIES = libSoGtkDevices.la

## Remove any linklibraries detected by configure, as they are not
## needed in the submodules -- only in the toplevel libSoQt build.
LIBS = 

INCLUDES = -I$(top_srcdir)/src

BUILT_SOURCES = \
	$(SoGuiAllBuiltFiles)

CLEANFILES = \
	$(SoGui_src_Inventor_Gui_devices_BuiltMetaFiles)

DISTCLEANFILES = \
	$(SoGui_src_Inventor_Gui_devices_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_devices_BuiltMetaFiles)

# **************************************************************************

libSoGtkDevices_la_SOURCES = \
	SoGtkDevice.h \
	SoGtkDevice.cpp \
	SoGtkInputFocus.h \
	SoGtkInputFocus.cpp \
	SoGtkKeyboard.h \
	SoGtkKeyboard.cpp \
	SoGtkMouse.h \
	SoGtkMouse.cpp \
	SoGtkSpaceball.h \
	SoGtkSpaceball.cpp
#	spwinput.h \
#	spwinput_x11.cpp

# **************************************************************************

SoGtkdevicesincdir = $(includedir)/Inventor/Gtk/devices
SoGtkdevicesinc_HEADERS = \
	SoGtkDevice.h \
	SoGtkInputFocus.h \
	SoGtkKeyboard.h \
	SoGtkMouse.h \
	SoGtkSpaceball.h

# **************************************************************************
