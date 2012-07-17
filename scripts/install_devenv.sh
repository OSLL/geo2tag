#!/bin/sh
# Installation development environment for geo2tag
# Note: tested on debian


echo "Some commands will be executed with sudo, your local password maybe asked"

echo "step 1: install building tools"
sudo apt-get install build-essential libqt4-dev debhelper libxml2-dev libqjson-dev git


