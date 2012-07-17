#!/bin/sh
# Installation development environment for geo2tag
# Note: tested on debian

#TODO add checks for clean directory

echo "Some commands will be executed with sudo, your local password maybe asked"

echo "step: install building tools"
sudo apt-get install build-essential libqt4-dev debhelper libxml2-dev libqjson-dev git

echo "step: installation testing environment"

echo "step: downloading sources"
#uncomment next line if you have commit access on github.com
##git clone git@github.com:OSLL/geo2tag.git
git clone git://github.com/OSLL/geo2tag.git

echo "step: build geo2tag platform part"
cd geo2tag 
./clean_build.sh



