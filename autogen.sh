#! /bin/sh
############################################################################
# Regenerate all files which are constructed by the autoconf, automake
# and libtool tool-chain. Note: only developers should need to use
# this script.
#
# Authors:
#   Lars J. Aas <larsa@sim.no>
#   Morten Eriksen <mortene@sim.no>
#

directory=`echo "$0" | sed -e 's/[^\/]*$//g'`;
cd $directory
if ! test -f ./autogen.sh; then
  echo "unexpected problem with your shell - bailing out"
  exit 1
fi
DIE=false

PROJECT=SoGtk

MACRODIR=conf-macros

SUBPROJECTS="$MACRODIR"
SUBPROJECTNAMES="$MACRODIR"

AUTOMAKE_ADD=
if test "$1" = "--clean"; then
  rm -f aclocal.m4 \
	config.guess \
	config.h.in \
	config.sub \
	configure \
	depcomp \
	install-sh \
	ltconfig \
	ltmain.sh \
	missing \
	mkinstalldirs \
	stamp-h*
  find . -name Makefile.in -print | \
        egrep -v '^\./(examples|ivexamples)/' | xargs rm
  exit
elif test "$1" = "--add"; then
  AUTOMAKE_ADD="--add-missing --gnu --copy"
fi

echo "Checking the installed configuration tools..."

AUTOCONF_VER=2.49a
AUTOMAKE_VER=1.4a
LIBTOOL_VER=1.3.5

if test -z "`autoconf --version | grep \" $AUTOCONF_VER\" 2> /dev/null`"; then
    echo
    echo "You must have autoconf version $AUTOCONF_VER installed to"
    echo "generate configure information and Makefiles for $PROJECT."
    echo ""
    DIE=true
fi

if test -z "`automake --version | grep \" $AUTOMAKE_VER\" 2> /dev/null`"; then
    echo
    echo "You must have automake version $AUTOMAKE_VER installed to"
    echo "generate configure information and Makefiles for $PROJECT."
    echo ""
    echo "The Automake version we are using is a development version"
    echo "\"frozen\" from the CVS repository at 2000-01-13. You can get"
    echo "it here:"
    echo ""
    echo "   ftp://ftp.sim.no/pub/coin/automake-1.4a-coin.tar.gz"
    echo ""
    DIE=true
fi

if test -z "`libtool --version | grep \" $LIBTOOL_VER \" 2> /dev/null`"; then
    echo
    echo "You must have libtool version $LIBTOOL_VER installed to"
    echo "generate configure information and Makefiles for $PROJECT."
    echo ""
    echo "Get ftp://ftp.gnu.org/pub/gnu/libtool/libtool-1.3.5.tar.gz"
    echo ""
    DIE=true
fi


set $SUBPROJECTNAMES
num=1
for project in $SUBPROJECTS; do
  test -d $project || {
    echo
    echo "Could not find subdirectory '$project'."
    echo "It was probably added after you initially fetched $PROJECT."
    echo "To add the missing module, run 'cvs co $1' from the $PROJECT"
    echo "base directory."
    echo ""
    echo "To do a completely fresh cvs checkout of the whole $PROJECT module,"
    echo "(if all else fails), remove $PROJECT and run:"
    echo "  cvs -z3 -d :pserver:cvs@cvs.sim.no:/export/cvsroot co -P $PROJECT"
    echo ""
    DIE=true
  }
  num=`expr $num + 1`
  shift
done

$DIE && exit 1

echo "Running aclocal (generating aclocal.m4)..."
aclocal -I $MACRODIR

echo "Running autoheader (generating config.h.in)..."
autoheader

echo "Running automake (generating the Makefile.in files)..."
echo "[ignore any \"directory should not contain '/'\" warning]"
automake $AUTOMAKE_ADD

echo "Running autoconf (generating ./configure and the Makefile files)..."
autoconf

echo "Done: Now run './configure' and 'make install' to build $PROJECT."

