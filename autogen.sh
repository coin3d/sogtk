#! /bin/sh
# **************************************************************************
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
if test ! -f ./autogen.sh; then
  echo "unexpected problem with your shell - bailing out"
  exit 1
fi

PROJECT=SoGtk

MACRODIR=cfg/m4

SUBPROJECTS="$MACRODIR"
SUBPROJECTNAMES="SoGtkMacros"

AUTOMAKE_ADD=
if test "$1" = "--clean"; then
  rm -f config.h.in configure stamp-h*
  find . -name Makefile.in -print | xargs rm
  exit
elif test "$1" = "--add"; then
  AUTOMAKE_ADD=""
fi

echo "Checking the installed configuration tools..."

AUTOCONF_VER=2.49b
AUTOMAKE_VER=1.4a
LIBTOOL_VER=1.3.5

if test -z "`autoconf --version | grep \" $AUTOCONF_VER\" 2> /dev/null`"; then
  cat <<END

  You must have autoconf version $AUTOCONF_VER installed to
  generate configure information and Makefiles for $PROJECT.
END
    DIE=true
fi

if test -z "`automake --version | grep \" $AUTOMAKE_VER\" 2> /dev/null`"; then
  cat <<END

  You must have automake version $AUTOMAKE_VER installed to
  generate configure information and Makefiles for $PROJECT.

  The Automake version we are using is a development version
  "frozen" from the CVS repository at 2000-01-13. You can get
  it here:"

     ftp://ftp.sim.no/pub/coin/automake-1.4a-coin.tar.gz
END
    DIE=true
fi

if test -z "`libtool --version | grep \" $LIBTOOL_VER \" 2> /dev/null`"; then
  cat <<END

  You must have libtool version $LIBTOOL_VER installed to
  generate configure information and Makefiles for $PROJECT.

  Get ftp://ftp.gnu.org/pub/gnu/libtool/libtool-1.3.5.tar.gz
END
    DIE=true
fi

set $SUBPROJECTNAMES
num=1
for project in $SUBPROJECTS; do
  test -d $project || {
    cat <<END

  Could not find subdirectory '$project'.
  It was probably added after you initially fetched $PROJECT.
  To add the missing module, run 'cvs co $1' from the $PROJECT
  base directory.

  To do a completely fresh cvs checkout of the whole $PROJECT module,
  (if all else fails), remove $PROJECT and run:

    cvs -z3 -d :pserver:cvs@cvs.sim.no:/export/cvsroot co -P $PROJECT
END
    DIE=true
  }
  num=`expr $num + 1`
  shift
done

# abnormal exit?
${DIE=false} && echo "" && exit 1

# generate cfg/aclocal.m4
echo "Running aclocal..."
aclocal -I $MACRODIR --output=cfg/aclocal.m4

# generate config.h.in
echo "Running autoheader..."
autoheader -l cfg

# generate Makefile.in templates
echo "Running automake..."
echo "[ignore any \"directory should not contain '/'\" warning]"
cp cfg/aclocal.m4 .
automake
rm aclocal.m4

# generate configure
echo "Running autoconf..."
autoconf -l cfg

echo "Done."

