
SUBDIRS = Inventor

BUILT_SOURCES = \
	so@gui@defs.h

DISTCLEANFILES = \
	$(BUILT_SOURCES)

so@gui@defs.h: $(srcdir)/Inventor/@Gui@/common/soguidefs.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

