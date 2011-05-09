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

echo ""
echo "============================================================="
echo "===                   END OF RUNNING   -> ${COUNT} "
echo "============================================================="
echo ""

cat ./test.log | grep "FAIL!" 
