#! /bin/sh
############################################################################
#
#  ChangeLog.sh is a script for generating ChangeLog files using cvs2cl.
#

me=`echo $0 | sed 's:^.*/::g'`
wd=`echo $0 | sed 's:/[^/]*$::'`
basedir=`echo $wd | sed 's:/cfg$::'`

module=SoGtk
GUI=Gtk
headerfile=/tmp/$module.header
cvs2cl=$HOME/code/misc/src/cvs2cl/cvs2cl.pl
test -f $cvs2cl || cvs2cl=cvs2cl.pl

cd $basedir

############################################################################
cat > $headerfile <<ENDOFHEADER
ChangeLog
=========

This is the ChangeLog file for the $module library.  This file is auto-
matically generated every night.  Entries are in reversed chronological
order, and starts at January 1st, 2001.  See also the following ChangeLog
files:

  ./ChangeLog.2000
  ./src/Inventor/$GUI/common/ChangeLog
  ./cfg/m4/ChangeLog 
  ./data/ChangeLog 

See http://www.red-bean.com/~kfogel/cvs2cl.shtml for information about the
script, cvs2cl.pl, used to generate this file.

============================================================================

ENDOFHEADER
############################################################################

# generate ChangeLog, but strip off uninteresting entries
cvs log -d"00:00:00 Jan 1 2001<00:00:00 Jan 1 2002" |
  $cvs2cl --stdin --header $headerfile --separate-header --prune \
  --ignore 'ChangeLog$' \
  --ignore '(Makefile\.in|configure|aclocal\.m4|config\.sub|config\.guess)$' \
  --ignore '(ltconfig|ltmain\.sh|missing|mkinstalldirs|stamp-h.*|install-sh)$' \
  --ignore 'config\.h\.in$' \
  --ignore "src/Inventor/$GUI/common/" \
  --ignore 'cfg/m4/' \
  --ignore "data/"

rm ChangeLog.bak $headerfile

cvs commit -m "Automatic ChangeLog generation" ChangeLog

