
sogtk.iwz:
	@perl -w $(top_srcdir)/build/geniwz.pl @VERSION@ \
	  $(libdir)/soqt@SOGTK_MAJOR_VERSION@.dll \
	  $(libdir)/soqt@SOGTK_MAJOR_VERSION@.lib \
	  `find $(includedir)/Inventor/Gtk -name "*.h"` > sogtk.iwz

ispkg: sogtk.iwz
	@IsxBuild.exe sogtk.iwz -E -O

