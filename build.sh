#!/bin/bash

pushd src/unixODBC-2.2.11

./configure --prefix `pwd`/../internal/
make
make install

popd
qmake-qt4
make

