
noinst_LTLIBRARIES = libSoGtkViewers.la

LIBS = 

INCLUDES = -I$(top_srcdir)/src

libSoGtkViewers_la_SOURCES = \
	SoGtkViewer.cpp \
	SoGtkViewer.h
#	SoGtkFullViewer.cpp \
#	SoGtkFullViewer.h \
#	SoGtkExaminerViewer.cpp \
#	SoGtkExaminerViewer.h \
#	SoGtkPlaneViewer.cpp \
#	SoGtkPlaneViewer.h

libSoGtkViewersincdir = $(includedir)/Inventor/Gtk/viewers

libSoGtkViewersinc_HEADERS = \
	$(top_srcdir)/src/Inventor/Gtk/viewers/SoGtkViewer.h
#	$(top_srcdir)/src/Inventor/Gtk/viewers/SoGtkFullViewer.h \
#	$(top_srcdir)/src/Inventor/Gtk/viewers/SoGtkExaminerViewer.h
#	$(top_srcdir)/src/Inventor/Gtk/viewers/SoGtkPlaneViewer.h

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

