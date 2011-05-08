#!/bin/bash
#
# this file running tests.
#

TESTS=`find ./ -name test.suite`
COUNT=`echo ${TESTS} | wc -w`


echo "Found ${COUNT} test set(s): {"${TESTS}"}"
rm -rf ./test.log

for test in ${TESTS}; 
do
	${test} | tee --append "./test.log"  
done;


FAIL=`cat test.log | grep "FAIL" | wc -l`
PASS=`cat test.log | grep "PASS" | wc -l`

echo ""
echo "See 'test.log' for results"
echo "================================================================="
echo "===  TESTING RESULTS Suites:${COUNT}   PASS/FAIL: ${PASS}/${FAIL}"
echo "================================================================="

