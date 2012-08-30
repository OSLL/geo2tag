#!/bin/bash
#
# this file running tests.
#

export LD_LIBRARY_PATH=./debian/build/lib/
export PWD="123"

echo "Type your postgres password"
su - postgres -c "createuser -s test_user"
echo "Type your postgres password"
su - postgres -c "createdb -O test_user test_db"
psql test_db -U test_user < ./scripts/base.sql

TESTS=`find ./ -name test.suite -o -name utest.*`
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

echo "Type your postgres password"
su - postgres -c "dropdb test_db"
echo "Type your postgres password"
su - postgres -c "dropuser test_user"

echo "" >>test_summary.log
echo "See 'test.log' for results" >>test_summary.log
echo "=================================================================">>test_summary.log
echo "===  TESTING RESULTS Suites:${COUNT}   PASS/FAIL: ${PASS}/${FAIL}">>test_summary.log
echo "=================================================================">>test_summary.log

