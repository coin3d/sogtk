# **************************************************************************
## Process this file with automake to generate Makefile.in.

noinst_LTLIBRARIES = libSoGtkDevices.la

## Remove any linklibraries detected by configure, as they are not
## needed in the submodules -- only in the toplevel libSoQt build.
LIBS = 

INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src

BUILT_SOURCES = \
	spwinput_x11.cpp \
	spwinput.h

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

libSoGtkdevicesincdir = $(includedir)/Inventor/Gtk/devices

libSoGtkdevicesinc_HEADERS = \
	$(top_srcdir)/src/Inventor/Gtk/devices/SoGtkDevice.h \
	$(top_srcdir)/src/Inventor/Gtk/devices/SoGtkInputFocus.h \
	$(top_srcdir)/src/Inventor/Gtk/devices/SoGtkKeyboard.h \
	$(top_srcdir)/src/Inventor/Gtk/devices/SoGtkMouse.h \
	$(top_srcdir)/src/Inventor/Gtk/devices/SoGtkSpaceball.h

# **************************************************************************

spwinput.h: $(top_srcdir)/src/Inventor/Gtk/common/spwinput.h.in
	@( cd $(top_builddir);
	   $(MAKE) src/Inventor/Gtk/devices/spwinput.h )

spwinput_x11.cpp: $(top_srcdir)/src/Inventor/Gtk/common/spwinput_x11.cpp.in
	@( cd $(top_builddir);
	   $(MAKE) src/Inventor/Gtk/devices/spwinput_x11.cpp )

