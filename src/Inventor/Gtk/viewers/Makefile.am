
INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src
LIBS = 

noinst_LTLIBRARIES = libSoGtkViewers.la

BUILT_SOURCES = \
	SoGtkViewer.cpp \
	SoGtkViewer.h \
	SoAnyFullViewer.cpp \
	SoAnyFullViewer.h \
	SoAnyExaminerViewer.cpp \
	SoAnyExaminerViewer.h

libSoGtkViewers_la_SOURCES = \
	$(BUILT_SOURCES) \
	SoGtkViewerExtra.cpp \
	SoGtkFullViewer.cpp \
	SoGtkFullViewer.h \
	SoGtkExaminerViewer.cpp \
	SoGtkExaminerViewer.h

libSoGtkViewersincdir = $(includedir)/Inventor/Gtk/viewers

libSoGtkViewersinc_HEADERS = \
	$(top_builddir)/src/Inventor/Gtk/viewers/SoGtkViewer.h \
	$(top_srcdir)/src/Inventor/Gtk/viewers/SoGtkFullViewer.h \
	$(top_srcdir)/src/Inventor/Gtk/viewers/SoGtkExaminerViewer.h
#	$(top_srcdir)/src/Inventor/Gtk/viewers/SoGtkPlaneViewer.h

############################################################################

SoGtkViewer.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiViewer.h.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Gtk/viewers/SoGtkViewer.h )

SoGtkViewer.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoGuiViewer.cpp.in
	@( cd $(top_builddir);
	   $(MAKE) src/Inventor/Gtk/viewers/SoGtkViewer.cpp )

SoAnyFullViewer.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyFullViewer.h.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Gtk/viewers/SoAnyFullViewer.h )

SoAnyFullViewer.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyFullViewer.cpp.in
	@( cd $(top_builddir);
	   $(MAKE) src/Inventor/Gtk/viewers/SoAnyFullViewer.cpp )

SoAnyExaminerViewer.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyExaminerViewer.h.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Gtk/viewers/SoAnyExaminerViewer.h )

SoAnyExaminerViewer.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyExaminerViewer.cpp.in
	@( cd $(top_builddir);
	   $(MAKE) src/Inventor/Gtk/viewers/SoAnyExaminerViewer.cpp )

SoAnyPlaneViewer.h: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyPlaneViewer.h.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Gtk/viewers/SoAnyPlaneViewer.h )

SoAnyPlaneViewer.cpp: \
  $(top_srcdir)/src/Inventor/Gtk/common/SoAnyPlaneViewer.cpp.in
	@( cd $(top_builddir);
	   $(MAKE) src/Inventor/Gtk/viewers/SoAnyPlaneViewer.cpp )

############################################################################
## FIXME: this is a temporary workaround for a bug in Automake. The
## bug is fixed in the latest Automake CVS, but not in any "official"
## release (?). Last checked with Automake 1.4a. 19990912 mortene.

install-libSoGtkViewersincHEADERS: $(libSoGtkViewersinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libSoGtkViewersincdir)
	@list='$(libSoGtkViewersinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libSoGtkViewersincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libSoGtkViewersincdir)/$$f; \
	done

uninstall-libSoGtkViewersincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libSoGtkViewersinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libSoGtkViewersincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libSoGtkViewersincdir)/$$f; \
	done

