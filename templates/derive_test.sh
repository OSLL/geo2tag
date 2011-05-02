#!/bin/sh
# $Id$
#-----------------------------------------------------------------------------------

if [ -z "$1" ]; then echo 'Package name:'; read PACKAGE; else PACKAGE=$1; fi;
if [ -z "$2" ]; then echo 'Class name:'; read CLASS; else CLASS=$2; fi;

UUID=`uuidgen | tr 'abcdef-' 'ABCDEF_'`
NAME=TEST_${CLASS}_H
GUID=_${NAME}_${UUID}_INCLUDED_
AUTHOR=`git config --get user.name`
EMAIL=`git config --get user.email`
YEAR=`date +%Y`

FSUITE="main.cpp"
FTESTCPP=$CLASS"_Test.cpp"
FTEST=$CLASS"_Test.h"

A="cat <<EOF
`cat TestSuite.cpp.t`
EOF"

eval "$A" >$FSUITE

A="cat <<EOF
`cat Test_CClass.h.t`
EOF"

eval "$A" >$FTEST 
