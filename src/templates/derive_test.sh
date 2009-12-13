#!/bin/sh
# $Id$
#-----------------------------------------------------------------------------------

if [ -z "$1" ]; then echo 'Package name:'; read PACKAGE; else PACKAGE=$1; fi;
if [ -z "$2" ]; then echo 'Class name:'; read CLASS; else CLASS=$2; fi;

UUID=`uuidgen | tr 'abcdef-' 'ABCDEF_'`
NAME=TEST_${CLASS}_H
GUID=_${NAME}_${UUID}_INCLUDED_

FSUITE="TestSuite.h"
FTESTCPP="Test_"$CLASS".cpp"
FTEST="Test_"$CLASS".h"

A="cat <<EOF
`cat TestSuite.cpp.t`
EOF"

eval "$A" >$FSUITE

A="cat <<EOF
`cat Test_CClass.h.t`
EOF"

eval "$A" >$FTEST 
