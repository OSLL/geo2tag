#!/bin/sh
# Installation development environment for geo2tag
# Note: tested on debian

#TODO add checks for clean directory

echo "Some commands will be executed with sudo, your local password maybe asked"

echo "step: install building tools"
sudo apt-get install --reinstall build-essential libqt4-dev debhelper libxml2-dev libqjson-dev libfcgi-dev git vim gitk libpq-dev postgresql-8.4 postgresql-client-8.4

echo "step: installation testing environment"
sudo /bin/sh 'echo "local all all trust" >>/etc/postgresql/8.4/main/pg_hba.conf'

echo "step: downloading sources"
#uncomment next line if you have commit access on github.com
##git clone git@github.com:OSLL/geo2tag.git

# read only version
git clone git://github.com/OSLL/geo2tag.git

# testing
#git clone ssh://192.168.56.1/home/kkv/ws/p/geo2tag/

echo "step: build geo2tag platform part"
cd geo2tag 
./clean_build.sh



