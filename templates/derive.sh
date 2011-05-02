#!/bin/sh
# $Id$

if [ -z "$1" ]; then echo 'Namespace:'; read PACKAGE ; else PACKAGE=$1 ; fi
if [ -z "$2" ]; then echo 'Class name:'; read NM ; else NM=$2 ; fi

UUID=`uuidgen | tr 'abcdef-' 'ABCDEF_'`
AUTHOR=${SRC_AUTHOR}

NAME=$NM
NAMEH=$NM.h
NAMECPP=$NM.cpp

PROJECT=
PROJECT_NAMESPACE=

A="cat <<EOF
`cat template.h`
EOF"

eval "$A" >$NAMEH

A="cat <<EOF
`cat template.cpp`
EOF"

eval "$A" >$NAMECPP

