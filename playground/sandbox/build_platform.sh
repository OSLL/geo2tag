#!/bin/bash

# download sources
# build deb
# backup previous version
# install already builded
# make tests
# if there are errors restore backup
# send logs to the maillist

# First parametr of this script - name of the branch, wich platform will be builded and tested

dir_sandbox="$WEBGEO_HOME/sandbox"
dir_geo2tag="${dir_sandbox}/geo2tag"
dir_log="${dir_sandbox}/platform_logs"
dir_backup="${dir_sandbox}/platform_backup"
branch="$1"

# If branch name is "web-devel" then exit
if [ "$branch" == "web-devel" ]
then
	exit 0
fi

if [ -e $dir_geo2tag ]; then
        echo "geo2tag directory exists" 
else
        echo "geo2tag directory does not exists"
        git clone git://github.com/OSLL/geo2tag.git
fi

#CREATE LOGS
date > ${dir_log}/build.log.txt
date > ${dir_log}/deploy.log.txt
date > ${dir_log}/test.log.txt

# hack for determining git problems
cd $dir_geo2tag
#git stash >> ${dir_log}/build.log.txt 2>>${dir_log}/build.log.txt
git pull --all >> ${dir_log}/build.log.txt 2>>${dir_log}/build.log.txt
git checkout $branch >> ${dir_log}/build.log.txt 2>>${dir_log}/build.log.txt

mkdir ${dir_log}



#BUILD deb

cd ${dir_geo2tag}
dh_clean
dpkg-buildpackage -rfakeroot >> ${dir_log}/build.log.txt 2>>${dir_log}/build.log.txt
founded_packages=`ls "${dir_sandbox}" | grep deb | grep -v standalone | grep -v observer`;
echo "After building ${founded_packages}"
if [  $(echo "${founded_packages}" | wc -w ) == "2" ] ;
then

status="success";
# Build succesful

#UNIT TESTING

echo "Unit testing:"  >>${dir_log}/test.log.txt
${dir_sandbox}/geo2tag/run_tests.sh >>${dir_log}/test.log.txt 2>>${dir_log}/test.log.txt

# DEPLOY and TEST only if branch is devel
if [ "$branch" == "devel" ]
then

#DEPLOY
cd ${dir_sandbox}
ls
dpkg -i wikigps-libs_* wikigps-service_* >> ${dir_log}/deploy.log.txt 2>>${dir_log}/deploy.log.txt

#TEST
	if "${dir_sandbox}"/test_platform.sh 
	then
	# test cases passed, move installed debs to backup
		echo "Tests passed" >> ${dir_log}/test.log.txt
		status="success";
		cd "${dir_sandbox}"
		rm -rf "${dir_backup}"
		mkdir "${dir_backup}"
		mv -f ${dir_sandbox}/wikigps-libs_* "${dir_backup}"
		mv -f ${dir_sandbox}/wikigps-service_* "${dir_backup}"	
	else
	# test cases not passed, restore backup
		echo "Tests not passed" >> ${dir_log}/test.log.txt
		status="fail";
		cd "${dir_backup}"
		echo "Restore backup" >> ${dir_log}/test.log.txt
		dpkg -i `ls .` >> ${dir_log}/test.log.txt 2>>${dir_log}/test.log.txt
	fi
fi

else 
	status="fail";
fi

cd ${dir_geo2tag}
dh_clean
#git stash
cd ${dir_sandbox}
rm -rf wikigps*

#SEND EMAIL
echo "E-mailing!"
if [ "$branch" == "devel" ]
then
	ant -f mail_sender.xml -Dsubject "geo2tag-platform devel ($status): integration reports " -Dlogdir "platform_logs" 
else
	ant -f mail_sender.xml -Dsubject "geo2tag-platform $branch ($status): build and test reports " -Dlogdir "platform_logs"
fi
echo "" > ${dir_log}/build.log.txt
echo "" > ${dir_log}/deploy.log.txt
echo "" > ${dir_log}/test.log.txt

git reset --hard 
git clean -fxd

