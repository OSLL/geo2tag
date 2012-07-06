#!/bin/bash

cd /var/www/geo2tag_repo/testing

dpkg-scanpackages binary_i386 /dev/null | gzip -9c > binary_i386/Packages.gz
dpkg-scanpackages binary_amd64 /dev/null | gzip -9c > binary_amd64/Packages.gz
dpkg-scansources source /dev/null | gzip -9c > source/Sources.gz

cd /var/www/geo2tag_repo/stable

dpkg-scanpackages binary_i386 /dev/null | gzip -9c > binary_i386/Packages.gz
dpkg-scanpackages binary_amd64 /dev/null | gzip -9c > binary_amd64/Packages.gz
dpkg-scansources source /dev/null | gzip -9c > source/Sources.gz

