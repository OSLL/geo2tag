#!/bin/bash

# This script should be run using crone
# */10 * * * * /path/check_scripts.sh

dir_automation="$WEBGEO_HOME/automation"

cd ${dir_automation}

#  Check that ruby and python processes exists
##  If started - exit 

isRubyRunned=`pgrep 'ruby'`;
if [[ -z "$isRubyRunned" ]]
then
#	echo "No ruby!"
        ruby1.8 refresh.rb 1>/dev/null 2>/dev/null &
fi

isPythonRunned=`pgrep 'python'`;
if [[ -z "$isPythonRunned" ]]
then
#	echo "No python!"
	nohup python ./WebRunner.py 1>/dev/null 2>/dev/null &
fi

