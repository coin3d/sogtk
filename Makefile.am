############################################################################
## SoGtk/Makefile.am
## $Id$

include src/Inventor/Gtk/common/Makefile.common

ACLOCAL_AMFLAGS = -I cfg/m4

############################################################################
# Automake conditional stuff

if BUILD_LIBRARY
LIBDIRS = data src
if BUILD_INTL
INTLDIRS = po intl
else
INTLDIRS =
endif
else
LIBDIRS =
INTLDIRS =
endif

if BUILD_HTMLPAGES
HTMLDIR = html
else
HTMLDIR =
endif

if BUILD_TESTS
TESTDIR = test-code
else
TESTDIR =
endif

SUBDIRS = . $(LIBDIRS) man $(HTMLDIR) $(INTLDIRS) $(TESTDIR)

############################################################################

BUILT_SOURCES = \
	$(SoGuiAllBuiltFiles)

CLEANFILES = \
	$(SoGui_BuiltMetaFiles)

DISTCLEANFILES = \
	$(SoGui_BuiltFiles) \
	$(SoGui_BuiltMetaFiles) \
	build/Makefile \
	build/so@gui@.doxygen \
	build/so@gui@.spec \
	intl/Makefile \
	intl/po2tbl.sed \
	so@gui@-@CONFIG@.cfg \
	so@gui@-config \
	po/POTFILES \
	po/Makefile.in \
	po/Makefile

EXTRA_DIST = \
	build/Makefile.am \
	build/Makefile.in \
	build/README \
	build/geniwz.pl \
	build/sogtk.doxygen.in \
	build/sogtk.spec.in \
	cfg/m4/sogtk.m4 \
	po/ChangeLog \
	po/Makefile.in.in \
	po/POTFILES.in \
	po/de.po \
	$(SoGuiCommonDistFiles)

if BUILD_LIBRARY

bin_SCRIPTS = so@gui@-config

m4datadir = $(datadir)/aclocal
# Don't replace "gtk" with "@gui@" here, as it will pøkk up
# for ``make dist''.
dist_m4data_DATA = cfg/m4/sogtk.m4

cfgdatadir = $(datadir)/Coin/conf
cfgdata_DATA = so@gui@-@CONFIG@.cfg

endif

all-local: built-sources

built-sources: $(BUILT_SOURCES)

############################################################################
