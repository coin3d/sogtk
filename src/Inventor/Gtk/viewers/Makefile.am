
INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src
LIBS = 

noinst_LTLIBRARIES = libSoGtkViewers.la

libSoGtkViewers_la_SOURCES = \
	$(top_builddir)/src/Inventor/Gtk/viewers/SoGtkViewer.cpp \
	$(top_builddir)/src/Inventor/Gtk/viewers/SoGtkViewer.h \
	$(top_srcdir)/src/Inventor/Gtk/common/SoAnyFullViewer.cpp \
	$(top_srcdir)/src/Inventor/Gtk/common/SoAnyFullViewer.h \
	SoGtkViewerExtra.cpp \
	SoGtkFullViewer.cpp \
	SoGtkFullViewer.h \
	$(top_srcdir)/src/Inventor/Gtk/common/SoAnyExaminerViewer.cpp \
	$(top_srcdir)/src/Inventor/Gtk/common/SoAnyExaminerViewer.h \
	SoGtkExaminerViewer.cpp \
	SoGtkExaminerViewer.h
#	$(top_srcdir)/src/Inventor/Gtk/common/SoAnyPlaneViewer.cpp \
#	$(top_srcdir)/src/Inventor/Gtk/common/SoAnyPlaneViewer.h \
#	SoGtkPlaneViewer.cpp \
#	SoGtkPlaneViewer.h

libSoGtkViewersincdir = $(includedir)/Inventor/Gtk/viewers

libSoGtkViewersinc_HEADERS = \
	$(top_builddir)/src/Inventor/Gtk/viewers/SoGtkViewer.h \
	$(top_srcdir)/src/Inventor/Gtk/viewers/SoGtkFullViewer.h \
	$(top_srcdir)/src/Inventor/Gtk/viewers/SoGtkExaminerViewer.h
#	$(top_srcdir)/src/Inventor/Gtk/viewers/SoGtkPlaneViewer.h

############################################################################

SoGtkViewer.h: $(top_srcdir)/src/Inventor/Gtk/common/SoAnyViewer.h.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/viewers/SoGtkViewer.h )

SoGtkViewer.cpp: $(top_srcdir)/src/Inventor/Gtk/common/SoAnyViewer.cpp.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Gtk/viewers/SoGtkViewer.cpp )

############################################################################

SoAnyFullViewer.lo: $(top_srcdir)/src/Inventor/Gtk/common/SoAnyFullViewer.cpp
	source='$(top_srcdir)/src/Inventor/Gtk/common/SoAnyFullViewer.cpp' \
	object='$@' libtool=yes \
	depfile='$(DEPDIR)/$*.Plo' tmpdepfile='$(DEPDIR)/$*.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LTCXXCOMPILE) -c -o $@ \
	$(top_srcdir)/src/Inventor/Gtk/common/SoAnyFullViewer.cpp

SoAnyExaminerViewer.lo: $(top_srcdir)/src/Inventor/Gtk/common/SoAnyExaminerViewer.cpp
	source='$(top_srcdir)/src/Inventor/Gtk/common/SoAnyExaminerViewer.cpp' \
	object='$@' libtool=yes \
	depfile='$(DEPDIR)/$*.Plo' tmpdepfile='$(DEPDIR)/$*.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LTCXXCOMPILE) -c -o $@ \
	$(top_srcdir)/src/Inventor/Gtk/common/SoAnyExaminerViewer.cpp

SoAnyPlaneViewer.lo: $(top_srcdir)/src/Inventor/Gtk/common/SoAnyPlaneViewer.cpp
	source='$(top_srcdir)/src/Inventor/Gtk/common/SoAnyPlaneViewer.cpp' \
	object='$@' libtool=yes \
	depfile='$(DEPDIR)/$*.Plo' tmpdepfile='$(DEPDIR)/$*.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LTCXXCOMPILE) -c -o $@ \
	$(top_srcdir)/src/Inventor/Gtk/common/SoAnyPlaneViewer.cpp

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

