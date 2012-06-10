#!/bin/bash

# This script updates data for web test page if branch 'master'
# First parametr of this script - name of the branch, wich platform will be builded and tested

platform_repo="geo2tag-platform-repo"
dir_automation="$WEBGEO_HOME/automation"
dir_geo2tag="${dir_automation}/${platform_repo}"

branch="$1"


# If branch name is "web-devel" then exit
if [ "$branch" == "master" ]
then
	cp ${dir_geo2tag}/automation/tests.xml ${dir_automation}/tests.xml
fi

