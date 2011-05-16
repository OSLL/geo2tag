#!/bin/bash

./erase_tests.sh
make distclean 
qmake && make -j5 && ./run_tests.sh
