#!/bin/bash
#
# this qODfile running tests.
#

# LD_LIBRARY_PATH=./debian/build/lib/


TESTS=`find ./ -name utest.*`
for test in ${TESTS}; 
do
        ${test}
done;
