#!/bin/bash
#
# this file running tests.
#
TESTS=`find ./ -name test.suite`
for test in ${TESTS}; 
do
	${test} -xml 
done;


FAIL=`cat test.log | grep "FAIL" | wc -l`
PASS=`cat test.log | grep "PASS" | wc -l`

su - postgres -c "dropdb test_db"
su - postgres -c "dropuser test_user"

echo "" >>test_summary.log
echo "See 'test.log' for results" >>test_summary.log
echo "=================================================================">>test_summary.log
echo "===  TESTING RESULTS Suites:${COUNT}   PASS/FAIL: ${PASS}/${FAIL}">>test_summary.log
echo "=================================================================">>test_summary.log

