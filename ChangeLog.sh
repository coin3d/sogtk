#!/bin/sh
############################################################################
#
#  ChangeLog.sh-changelog is a wrapper-script for generating ChangeLog files
#  using the cvs2cl script.
#

moduledir=${0%/[^/]*}
module=SoGtk
GUI=Gtk
cvs2cl=$HOME/code/cvs/src/cvs2cl/cvs2cl.pl
headerfile=/tmp/$module.header

cd $moduledir

############################################################################
cat > $headerfile <<ENDOFHEADER
ChangeLog
=========

This is the ChangeLog file for the $module library.  This file is auto-
matically generated every night.  Entries are in reversed chronological
order.  See also the following ChangeLog files:

  ./src/Inventor/$GUI/common/ChangeLog
  ./cfg/m4/ChangeLog 

See http://www.red-bean.com/~kfogel/cvs2cl.shtml for information about the
script, cvs2cl.pl, used to generate this file.

============================================================================

ENDOFHEADER
############################################################################


# generate ChangeLog, but strip off uninteresting entries
cvs log | $cvs2cl --stdin --header $headerfile --separate-header --prune \
  --ignore 'ChangeLog$' \
  --ignore '(Makefile\.in|configure|aclocal\.m4|config\.sub|config\.guess)$' \
  --ignore '(ltconfig|ltmain\.sh|missing|mkinstalldirs|stamp-h.*|install-sh)$' \
  --ignore 'config\.h\.in$' \
  --ignore "src/Inventor/$GUI/common/" \
  --ignore 'cfg/m4/' \
  --ignore "data/"

rm ChangeLog.bak $headerfile

cvs commit -m "Automatic ChangeLog generation" ChangeLog

