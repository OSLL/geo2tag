#!/bin/bash

pushd src/qjson

mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=`pwd`/../../internal/   ..
make
make install

popd




#qmake-qt4
#make

