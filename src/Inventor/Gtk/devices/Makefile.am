## Process this file with automake to generate Makefile.in.

noinst_LTLIBRARIES = libSoGtkDevices.la

## Remove any linklibraries detected by configure, as they are not
## needed in the submodules -- only in the toplevel libSoQt build.
LIBS = 

INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src

## All source files used when building this submodule, including all
## headers.

libSoGtkDevices_la_SOURCES = \
	SoGtkDevice.cpp \
	SoGtkDevice.h \
	SoGtkInputFocus.cpp \
	SoGtkInputFocus.h \
	SoGtkKeyboard.cpp \
	SoGtkKeyboard.h \
	SoGtkMouse.cpp \
	SoGtkMouse.h \
	SoGtkSpaceball.cpp \
	SoGtkSpaceball.h

## Installation information.

libSoGtkdevicesincdir = $(includedir)/Inventor/Gtk/devices

libSoGtkdevicesinc_HEADERS = \
	$(top_srcdir)/src/Inventor/Gtk/devices/SoGtkDevice.h \
	$(top_srcdir)/src/Inventor/Gtk/devices/SoGtkInputFocus.h \
	$(top_srcdir)/src/Inventor/Gtk/devices/SoGtkKeyboard.h \
	$(top_srcdir)/src/Inventor/Gtk/devices/SoGtkMouse.h \
	$(top_srcdir)/src/Inventor/Gtk/devices/SoGtkSpaceball.h

