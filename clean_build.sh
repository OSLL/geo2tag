#!/bin/bash

./scripts/create_testdb.sh

find ./ -name test.suite | xargs rm

dpkg-buildpackage -rfakeroot -j5

cat ./test_summary.log

./scripts/remove_testdb.sh

