#!/bin/sh
#-----------------------------------------------------------------------------------

if [ -z "$1" ]; then echo 'Package name:'; read PACKAGE; else PACKAGE=$1; fi;
if [ -z "$2" ]; then echo 'Class name:'; read CLASS; else CLASS=$2; fi;

AUTHOR=`git config --get user.name`
EMAIL=`git config --get user.email`
YEAR=`date +%Y`

TESTFILE="${PACKAGE}_${CLASS}.cpp"
TESTMOC="${PACKAGE}_${CLASS}.moc"

TESTPRO="${CLASS}.pro"


mkdir ./${CLASS}

A="cat <<EOF
`cat qtest_template.pro`
EOF"

eval "$A" >${CLASS}/$TESTPRO

A="cat <<EOF
`cat qtest_template.cpp`
EOF"

eval "$A" >${CLASS}/$TESTFILE
