
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
	SoGtk.h \
	SoGtkComponent.h \
	SoGtkGLWidget.h \
	SoGtkRenderArea.h \
	SoGtkRenderArea.cpp \
	SoAnyMaterialList.h \
	SoAnyMaterialList.cpp \
	SoAnyMaterialListBuiltins.cpp

DISTCLEANFILES = \
	$(BUILT_SOURCES) \
	common/gl.h

############################################################################

libSoGtk@SUFFIX@_la_SOURCES = \
	SoGtk.cpp \
	SoGtkComponent.cpp \
	SoGtkGLWidget.cpp \
	SoGtkMaterialList.h \
	SoGtkMaterialList.cpp \
	SoGtkGraphEditor.h \
	SoGtkGraphEditor.cpp \
	SoGtkRoster.h \
	SoGtkRoster.cpp

nodist_libSoGtk@SUFFIX@_la_SOURCES = \
	$(BUILT_SOURCES)

libSoGtk@SUFFIX@_la_LIBADD = \
        devices/libSoGtkDevices.la \
	viewers/libSoGtkViewers.la \
        widgets/libSoGtkWidgets.la

libSoGtk@SUFFIX@_la_LDFLAGS = \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

############################################################################

SoGtkincdir = $(includedir)/Inventor/Gtk
SoGtkinc_HEADERS = \
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

SoGtkBasic.h: $(srcdir)/common/SoGuiBasic.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoGtkObject.h: $(srcdir)/common/SoGuiObject.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoGtkObject.cpp: $(srcdir)/common/SoGuiObject.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAny.h: $(srcdir)/common/SoAny.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAny.cpp: $(srcdir)/common/SoAny.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoGtkCursors.h: $(srcdir)/common/SoGuiCursors.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoGtkCursors.cpp: $(srcdir)/common/SoGuiCursors.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoGtk.h: $(srcdir)/common/SoGui.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoGtkComponent.h: $(srcdir)/common/SoGuiComponent.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoGtkGLWidget.h: $(srcdir)/common/SoGuiGLWidget.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoGtkRenderArea.h: $(srcdir)/common/SoGuiRenderArea.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoGtkRenderArea.cpp: $(srcdir)/common/SoGuiRenderArea.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyMaterialList.h: $(srcdir)/common/SoAnyMaterialList.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyMaterialList.cpp: $(srcdir)/common/SoAnyMaterialList.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyMaterialListBuiltins.cpp: $(srcdir)/common/SoAnyMaterialListBuiltins.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

############################################################################

if BUILD_WITH_MSVC

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

uninstall-local: uninstall-am
	@for e in dll lib pdb; do \
	  file="$(DESTDIR)$(libdir)/sogtk@SOGTK_MAJOR_VERSION@.$$e"; \
	  if test -f $$file; then \
	    echo rm -f $$file; \
	    rm -f $$file; \
	  fi \
	done

else

install-exec-hook:
uninstall-local: uninstall-am

endif

