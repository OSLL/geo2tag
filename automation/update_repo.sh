#!/bin/bash

cd /var/www/geo2tag_repo

dpkg-scanpackages binary /dev/null | gzip -9c > binary/Packages.gz
dpkg-scansources source /dev/null | gzip -9c > source/Sources.gz
