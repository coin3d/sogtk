############################################################################
# Usage:
#   SIM_AC_HAVE_SOGTK_IFELSE( IF-FOUND, IF-NOT-FOUND )
#
# Description:
#   This macro locates the SoGtk development system.  If it is found,
#   the set of variables listed below are set up as described and made
#   available to the configure script.
#
#   The $sim_ac_sogtk_desired variable can be set to false externally to
#   make SoGtk default to be excluded.
#
# Autoconf Variables:
# > $sim_ac_sogtk_desired     true | false (defaults to true)
# < $sim_ac_sogtk_avail       true | false
# < $sim_ac_sogtk_cppflags    (extra flags the preprocessor needs)
# < $sim_ac_sogtk_ldflags     (extra flags the linker needs)
# < $sim_ac_sogtk_libs        (link library flags the linker needs)
# < $sim_ac_sogtk_datadir     (location of SoGtk data files)
# < $sim_ac_sogtk_version     (the libSoGtk version)
#
# Authors:
#   Lars J. Aas, <larsa@sim.no>
#   Morten Eriksen, <mortene@sim.no>
#
# TODO:
#

AC_DEFUN([SIM_AC_HAVE_SOGTK_IFELSE], [
AC_PREREQ([2.14a])

# official variables
sim_ac_sogtk_avail=false
sim_ac_sogtk_cppflags=
sim_ac_sogtk_ldflags=
sim_ac_sogtk_libs=
sim_ac_sogtk_datadir=
sim_ac_sogtk_version=

# internal variables
: ${sim_ac_sogtk_desired=true}
sim_ac_sogtk_extrapath=

AC_ARG_WITH([sogtk],
  AC_HELP_STRING([--with-sogtk=DIR], [give prefix location of SoGtk]),
  [ case $withval in
    no)  sim_ac_sogtk_desired=false ;;
    yes) sim_ac_sogtk_desired=true ;;
    *)   sim_ac_sogtk_desired=true
         sim_ac_sogtk_extrapath=$withval ;;
    esac],
  [])

if $sim_ac_sogtk_desired; then
  sim_ac_path=$PATH
  test -z "$sim_ac_sogtk_extrapath" ||   ## search in --with-sogtk path
    sim_ac_path=$sim_ac_sogtk_extrapath/bin:$sim_ac_path
  test x"$prefix" = xNONE ||          ## search in --prefix path
    sim_ac_path=$sim_ac_path:$prefix/bin

  AC_PATH_PROG(sim_ac_sogtk_configcmd, sogtk-config, false, $sim_ac_path)
  if $sim_ac_sogtk_configcmd; then
    sim_ac_sogtk_cppflags=`$sim_ac_sogtk_configcmd --cppflags`
    sim_ac_sogtk_ldflags=`$sim_ac_sogtk_configcmd --ldflags`
    sim_ac_sogtk_libs=`$sim_ac_sogtk_configcmd --libs`
    sim_ac_sogtk_datadir=`$sim_ac_sogtk_configcmd --datadir`
    sim_ac_sogtk_version=`$sim_ac_sogtk_configcmd --version`
    AC_CACHE_CHECK(
      [whether libSoGtk is available],
      sim_cv_sogtk_avail,
      [sim_ac_save_cppflags=$CPPFLAGS
      sim_ac_save_ldflags=$LDFLAGS
      sim_ac_save_libs=$LIBS
      CPPFLAGS="$CPPFLAGS $sim_ac_sogtk_cppflags"
      LDFLAGS="$LDFLAGS $sim_ac_sogtk_ldflags"
      LIBS="$sim_ac_sogtk_libs $LIBS"
      AC_LANG_PUSH(C++)
      AC_TRY_LINK(
        [#include <Inventor/Gtk/SoGtk.h>],
        [(void)SoGtk::init((const char *)0L);],
        [sim_cv_sogtk_avail=true],
        [sim_cv_sogtk_avail=false])
      AC_LANG_POP
      CPPFLAGS=$sim_ac_save_cppflags
      LDFLAGS=$sim_ac_save_ldflags
      LIBS=$sim_ac_save_libs
    ])
    sim_ac_sogtk_avail=$sim_cv_sogtk_avail
  else
    locations=`IFS=:; for p in $sim_ac_path; do echo " -> $p/sogtk-config"; done`
    AC_MSG_WARN([cannot find 'sogtk-config' at any of these locations:
$locations])
  fi
fi

if $sim_ac_sogtk_avail; then
  ifelse([$1], , :, [$1])
else
  ifelse([$2], , :, [$2])
fi
]) # SIM_AC_HAVE_SOGTK_IFELSE()

