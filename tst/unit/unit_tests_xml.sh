#!/bin/bash
#
# this file running tests.
#
TESTS=`find ./ -name test.suite`
for test in ${TESTS}; 
do
	${test} -xml 
done;
