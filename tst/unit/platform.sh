#!/bin/bash
#
# this file running tests.
#

./scripts/create_testdb.sh

TESTS=`find ./ -name test.suite`
COUNT=`echo ${TESTS} | wc -w`


echo "Found ${COUNT} test set(s): {"${TESTS}"}"
rm -rf ./test.log
rm -rf ./test_summary.log

for test in ${TESTS}; 
do
	${test} | tee --append "./test.log"  
done;


FAIL=`cat test.log | grep "FAIL" | wc -l`
PASS=`cat test.log | grep "PASS" | wc -l`

./scripts/remove_testdb.sh

echo "" >>test_summary.log
echo "See 'test.log' for results" >>test_summary.log
echo "=================================================================">>test_summary.log
echo "===  TESTING RESULTS Suites:${COUNT}   PASS/FAIL: ${PASS}/${FAIL}">>test_summary.log
echo "=================================================================">>test_summary.log

