#!/bin/bash

dir_automation="$WEBGEO_HOME/automation"
dir_tasks="${dir_automation}/deb_build_tasks"

echo "$1" > ${dir_tasks}/$1
