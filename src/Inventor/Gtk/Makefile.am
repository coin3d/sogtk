
SUBDIRS = common devices widgets

INCLUDES = -I$(top_srcdir)/src

lib_LTLIBRARIES = libSoGtk.la

libSoGtk_la_SOURCES = \
	SoGtk.cpp \
	SoGtk.h \
	SoGtkComponent.cpp \
	SoGtkComponent.h \
	SoGtkGLWidget.cpp \
	SoGtkGLWidget.h \
	SoGtkRenderArea.cpp \
	SoGtkRenderArea.h

libSoGtk_la_LIBADD = \
        common/libSoCommon.la \
        devices/libSoGtkDevices.la \
        widgets/libSoGtkWidgets.la

