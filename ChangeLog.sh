#!/bin/sh
############################################################################
#
#  gen-changelog is a wrapper-script for generating ChangeLog files
#  using the cvs2cl script.
#

GUI=Gtk
cvs2cl=$HOME/store/cvs/cvs2cl/cvs2cl.pl
projectdir=$HOME/code/coin/src/So$GUI
headerfile=/tmp/So$GUI.header

############################################################################
cat > $headerfile <<ENDOFHEADER
ChangeLog
=========

This is the ChangeLog file for the So$GUI library.  This file is auto-
matically generated every night.  Entries are in reversed chronological
order.  See also the following ChangeLog files:

  ./conf-macros/ChangeLog 
  ./src/Inventor/$GUI/common/ChangeLog
  ./examples/ChangeLog

See http://www.red-bean.com/~kfogel/cvs2cl.shtml for information about the
script, cvs2cl.pl, used to generate this file.

============================================================================

ENDOFHEADER
############################################################################

cd $projectdir

# generate ChangeLog, but strip off uninteresting entries
cvs log | $cvs2cl --stdin --header $headerfile --separate-header --prune \
  --ignore 'ChangeLog$' \
  --ignore '(Makefile\.in|configure|aclocal\.m4|config\.sub|config\.guess)$' \
  --ignore '(ltconfig|ltmain\.sh|missing|mkinstalldirs|stamp-h.*|install-sh)$' \
  --ignore 'config\.h\.in$' \
  --ignore 'conf-macros/' \
  --ignore "src/Inventor/$GUI/common/" \
  --ignore "examples/"

rm ChangeLog.bak $headerfile

cvs commit -m "Automatic ChangeLog generation" ChangeLog

