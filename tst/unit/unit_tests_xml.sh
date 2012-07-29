#!/bin/bash
#
# this file running tests.
#

export LD_LIBRARY_PATH=./debian/build/lib/


TESTS=`find ./ -name utest.*`
for test in ${TESTS}; 
do
	${test} -xml 
done;
