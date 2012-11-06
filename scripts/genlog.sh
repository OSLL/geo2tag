#!/bin/bash

# This script adds new record into changelog 
# Usage
#1	genlog.sh -m "message" 
#2	genlog.sh -f file_with_message
#3	genlog.sh 

# In case 3 last commit message where taken as a changelog message

changelog="debian/changelog";
geo2tag_version=`cat VERSION`;
message="";
branchname=`git branch | grep '\*' | cut -d' ' -f2`


echo "changelog: ${changelog}"
echo "base version: ${geo2tag_version}"
echo "branch: ${branchname}"
echo "extra message: $message"


if [ "$#" == "2" ]
then
	if [ "$1" == "-m" ]
	then
		# Get message from command line
		message="$2";

	elif [ "$1" == "-f" ]
	then
		# Get message from file
		message=`cat $2`;

	else
	        echo "Invalid first argument(should be -f or -m)."
	        exit 1

	fi

elif [ "$#" == "0" ]
then
	# Get message from last commit
	message=`git log -1 --pretty=%B`;

else
	echo "Invalid argument number(0 or 2)."
	exit 1
fi

# Getting verison last part from current date
version_last_part="`date +%s`-b${branchname}";
curr_date=`date -R`;
new_record="geo2tag ($geo2tag_version-$version_last_part) unstable; urgency=low\n\n  * $message\n\n -- Open Source and Linux lab  <geo2tag-support@osll.spb.ru>  $curr_date\n"

echo "${new_record}" >./.changelog_record

# Adding record as a first line
sed -i 1i"$new_record" $changelog
