#!/bin/bash

# 1. build webside
# 2. build deb packages

dir_repo="$(pwd)"
dir_stuff=${dir_repo}/"stuff"
dir_webside=${dir_repo}/"src/webside"
war_webside=${dir_webside}/"geo2tag.war"

# 1.
cd ${dir_webside}
ant clean war
cp ${war_webside} ${dir_stuff}/

# 2.
cd ${dir_repo}
dh_clean
dpkg-buildpackage -rfakeroot
