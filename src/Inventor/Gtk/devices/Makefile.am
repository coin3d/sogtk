# **************************************************************************
## Process this file with automake to generate Makefile.in.

noinst_LTLIBRARIES = libSoGtkDevices.la

## Remove any linklibraries detected by configure, as they are not
## needed in the submodules -- only in the toplevel libSoQt build.
LIBS = 

INCLUDES = -I$(top_srcdir)/src

BUILT_SOURCES = \
	spwinput_x11.cpp \
	spwinput.h

DISTCLEANFILES = \
	$(BUILT_SOURCES)

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

spwinput.h: $(top_srcdir)/src/Inventor/Gtk/common/spwinput.h.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Gtk/devices/spwinput.h )

spwinput_x11.cpp: $(top_srcdir)/src/Inventor/Gtk/common/spwinput_x11.cpp.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Gtk/devices/spwinput_x11.cpp )

