
SUBDIRS = devices widgets viewers

INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/data

lib_LTLIBRARIES = libSoGtk@SUFFIX@.la

BUILT_SOURCES = \
	SoAny.h \
	SoAny.cpp \
	SoGtkBasic.h \
	SoGtkObject.h \
	SoGtkObject.cpp \
	SoGtkCursors.h \
	SoGtkCursors.cpp \
	SoGtkComponent.h \
	SoGtkGLWidget.h \
	SoGtkRenderArea.h \
	SoGtkRenderArea.cpp \
	SoAnyMaterialList.h \
	SoAnyMaterialList.cpp \
	SoAnyMaterialListBuiltins.cpp

############################################################################

libSoGtk@SUFFIX@_la_SOURCES = \
	$(BUILT_SOURCES) \
	SoGtk.h \
	SoGtk.cpp \
	SoGtkComponent.cpp \
	SoGtkGLWidget.cpp \
	SoGtkMaterialList.h \
	SoGtkMaterialList.cpp \
	SoGtkGraphEditor.h \
	SoGtkGraphEditor.cpp \
	SoGtkRoster.h \
	SoGtkRoster.cpp

libSoGtk@SUFFIX@_la_LIBADD = \
        devices/libSoGtkDevices.la \
	viewers/libSoGtkViewers.la \
        widgets/libSoGtkWidgets.la

libSoGtk@SUFFIX@_la_LDFLAGS = \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

libSoGtkincdir = $(includedir)/Inventor/Gtk
libSoGtkinc_HEADERS = \
	SoGtkBasic.h \
	SoGtk.h \
	SoGtkObject.h \
	SoGtkCursors.h \
	SoGtkComponent.h \
	SoGtkGLWidget.h \
	SoGtkRenderArea.h \
	SoGtkGraphEditor.h \
	SoGtkMaterialList.h \
	SoGtkRoster.h

############################################################################

SoGtkBasic.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiBasic.h.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoGtkBasic.h )

SoGtkObject.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiObject.h.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoGtkObject.h )

SoGtkObject.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiObject.cpp.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoGtkObject.cpp )

SoAny.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAny.h.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoAny.h )

SoAny.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAny.cpp.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoAny.cpp )

SoGtkCursors.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiCursors.h.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoGtkCursors.h )

SoGtkCursors.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiCursors.cpp.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoGtkCursors.cpp )

SoGtkComponent.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiComponent.h.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoGtkComponent.h )

SoGtkGLWidget.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiGLWidget.h.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoGtkGLWidget.h )

SoGtkRenderArea.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiRenderArea.h.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoGtkRenderArea.h )

SoGtkRenderArea.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiRenderArea.cpp.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoGtkRenderArea.cpp )

SoAnyMaterialList.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyMaterialList.h.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoAnyMaterialList.h )

SoAnyMaterialList.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyMaterialList.cpp.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoAnyMaterialList.cpp )

SoAnyMaterialListBuiltins.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyMaterialListBuiltins.cpp.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/SoAnyMaterialListBuiltins.cpp )

############################################################################

sogtk@SOGTK_MAJOR_VERSION@.dll: $(libSoGtk_la_OBJECTS) $(libSoGtk_la_DEPENDENCIES)
	@rm -f sogtk@SOGTK_MAJOR_VERSION@.dll && \
	if @BUILD_WITH_MSVC@; then \
	  echo "Linking DLL..."; \
	  $(CC) @DLLFLAGS@ /out:sogtk@SOGTK_MAJOR_VERSION@.dll $(LDFLAGS) \
	        `echo $(libSoGtk_la_OBJECTS) | sed 's%\.lo%.obj%g'` \
	        $(libSoGtk_la_LIBADD) $(LIBS); \
	else \
	  touch sogtk@SOGTK_MAJOR_VERSION@.dll; \
	fi

install-exec-hook: sogtk@SOGTK_MAJOR_VERSION@.dll
	@if test -s sogtk@SOGTK_MAJOR_VERSION@.dll; then \
	  echo "Installing sogtk@SOGTK_MAJOR_VERSION@.dll"; \
	  for e in dll lib pdb; do \
	    file="sogtk@SOGTK_MAJOR_VERSION@.$$e"; \
	    if test -s $$file; then \
	      $(INSTALL_DATA) $$file $(DESTDIR)$(libdir)/$$file; \
	    fi \
	  done \
	fi

uninstall: uninstall-am
	@for e in dll lib pdb; do \
	  file="$(DESTDIR)$(libdir)/sogtk@SOGTK_MAJOR_VERSION@.$$e"; \
	  if test -f $$file; then \
	    echo rm -f $$file; \
	    rm -f $$file; \
	  fi \
	done

