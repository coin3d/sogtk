
include common/Makefile.common

SUBDIRS = devices widgets viewers

INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/data

lib_LTLIBRARIES = libSoGtk@SUFFIX@.la

BUILT_SOURCES = \
	$(SoGuiAllBuiltFiles)

CLEANFILES = \
	$(SoGui_src_Inventor_Gui_BuiltFiles)

DISTCLEANFILES = \
	$(SoGui_src_Inventor_Gui_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_BuiltMetaFiles)

############################################################################

libSoGtk@SUFFIX@_la_SOURCES = \
	SoGtk.cpp \
	SoGtkComponent.cpp \
	SoGtkGLWidget.cpp \
	SoGtkGraphEditor.h \
	SoGtkGraphEditor.cpp \
	SoGtkRoster.h \
	SoGtkRoster.cpp

nodist_libSoGtk@SUFFIX@_la_SOURCES = \
	$(SoGui_src_Inventor_Gui_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_BuiltMetaFiles)

libSoGtk@SUFFIX@_la_LIBADD = \
        devices/libSoGtkDevices.la \
	viewers/libSoGtkViewers.la \
        widgets/libSoGtkWidgets.la

libSoGtk@SUFFIX@_la_LDFLAGS = \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

############################################################################

SoGtkincdir = $(includedir)/Inventor/Gtk

SoGtkinc_HEADERS = \
	SoGtkGraphEditor.h \
	SoGtkRoster.h

nodist_SoGtkinc_HEADERS = \
	SoGtk.h \
	SoGtkBasic.h \
	SoGtkObject.h \
	SoGtkCursor.h \
	SoGtkComponent.h \
	SoGtkGLWidget.h \
	SoGtkRenderArea.h

install-SoGtkincHEADERS: $(SoGtkinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(SoGtkincdir)
	@list='$(SoGtkinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  src="$$d$$p"; dst="$(DESTDIR)$(SoGtkincdir)/$$f"; \
	  if cmp -s "$$src" "$$dst"; then :; else \
	    echo " $(INSTALL_HEADER) $$src $$dst"; \
	    $(INSTALL_HEADER) "$$src" "$$dst"; \
	  fi \
	done

install-nodist_SoGtkincHEADERS: $(nodist_SoGtkinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(SoGtkincdir)
	@list='$(nodist_SoGtkinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  src="$$d$$p"; dst="$(DESTDIR)$(SoGtkincdir)/$$f"; \
	  if cmp -s "$$src" "$$dst"; then :; else \
	    echo " $(INSTALL_HEADER) $$src $$dst"; \
	    $(INSTALL_HEADER) "$$src" "$$dst"; \
	  fi \
	done

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

############################################################################
