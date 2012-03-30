#!/bin/bash

# download sources
# build deb
# backup previous version
# install already builded
# make tests
# if there are errors restore backup
# send logs to the maillist

# First parametr of this script - name of the branch, wich platform will be builded and tested

platform_repo="geo2tag-platform-repo"
dir_automation="$WEBGEO_HOME/automation"
dir_geo2tag="${dir_automation}/${platform_repo}"
dir_log="${dir_automation}/platform_logs"
dir_backup="${dir_automation}/platform_backup"

branch="$1"
last_commit="$2"
AIROMO_FLAG="$3"

if [ "$AIROMO_FLAG" == "AIROMO" ] ; then
        platform_repo="airomo-platform-repo"
        dir_geo2tag="${dir_automation}/${platform_repo}"
fi

# If branch name is "web-devel" then exit
if [ "$branch" == "web-devel" ]
then
	exit 0
fi

if [ -e $dir_geo2tag ]; then
        echo "geo2tag directory exists" 
else
	if [ "$AIROMO_FLAG" == "AIROMO" ] ; then
                git clone ssh://airomo@repo.osll.spb.ru/git/airomo ${platform_repo}
        else
                git clone git://github.com/OSLL/geo2tag.git ${platform_repo}
        fi
fi

#CREATE LOGS
mkdir -p ${dir_log}
date > ${dir_log}/build.log.txt
date > ${dir_log}/deploy.log.txt
date > ${dir_log}/test.log.txt

# hack for determining git problems
cd $dir_geo2tag
#git stash >> ${dir_log}/build.log.txt 2>>${dir_log}/build.log.txt
git pull --all >> ${dir_log}/build.log.txt 2>>${dir_log}/build.log.txt
git checkout $branch >> ${dir_log}/build.log.txt 2>>${dir_log}/build.log.txt
git pull origin $branch >> ${dir_log}/build.log.txt 2>>${dir_log}/build.log.txt
#BUILD deb

cd ${dir_geo2tag}
dh_clean
dpkg-buildpackage -rfakeroot >> ${dir_log}/build.log.txt 2>>${dir_log}/build.log.txt
cp ./test.log ./test_summary.log ${dir_log}
founded_packages=`ls "${dir_automation}" | grep deb | grep -v standalone | grep -v observer`;
echo "After building ${founded_packages}"
if [  $(echo "${founded_packages}" | wc -w ) == "2" ] ;
then

	status="success";
	# Build succesful

	#UNIT TESTING

	echo "Unit testing:"  >>${dir_log}/test.log.txt
	${dir_geo2tag}/tst/unit/platform.sh >>${dir_log}/test.log.txt 2>>${dir_log}/test.log.txt

	# DEPLOY and TEST only if branch is devel
	if [ "$branch" == "devel" ] && [ "$AIROMO_FLAG" != "AIROMO" ]
	then

		#DEPLOY
		cp ${dir_geo2tag}/automation/test_platform.sh ${dir_automation}
		cd ${dir_automation}
		ls
		echo "n" | dpkg -i wikigps-libs_* wikigps-service_* >> ${dir_log}/deploy.log.txt 2>>${dir_log}/deploy.log.txt

		#TEST
                test_result=`${dir_automation}/test_platform.sh`;
		echo "Integration tests $test_result" >>${dir_log}/test.log.txt
                if ! echo $test_result | grep -i fail
                then
		# test cases passed, move installed debs to backup
			echo "Tests passed" >> ${dir_log}/test.log.txt
			status="success";
			cd "${dir_automation}"
			rm -rf "${dir_backup}"
			mkdir "${dir_backup}"
			mv -f ${dir_automation}/wikigps-libs_* "${dir_backup}"
			mv -f ${dir_automation}/wikigps-service_* "${dir_backup}"	
		else
		# test cases not passed, restore backup
			echo "Tests not passed" >> ${dir_log}/test.log.txt
			status="fail";
			cd "${dir_backup}"
			echo "Restore backup" >> ${dir_log}/test.log.txt
			echo "n" | dpkg -i `ls .` >> ${dir_log}/test.log.txt 2>>${dir_log}/test.log.txt
		fi
	fi
else 
	status="fail";
fi

cd ${dir_geo2tag}
dh_clean
#git stash
cd ${dir_automation}
rm -rf wikigps*

#SEND EMAIL
echo "E-mailing!"
test_summary=`cat "${dir_log}/test_summary.log"  `;
echo $test_summary
if [ "$branch" == "devel" ]
then
	ant -f mail_sender.xml -Dsubject "($status) geo2tag-platform devel ${AIROMO_FLAG} ${last_commit}: integration reports " -Dmessage "$test_summary" -Dlogdir "platform_logs" 
else
	ant -f mail_sender.xml -Dsubject "($status) geo2tag-platform $branch ${AIROMO_FLAG} ${last_commit}: build and test reports " -Dmessage "$test_summary" -Dlogdir "platform_logs"
fi
echo "" > ${dir_log}/build.log.txt
echo "" > ${dir_log}/deploy.log.txt
echo "" > ${dir_log}/test.log.txt

cd ${dir_geo2tag}
git reset --hard 
git clean -fxd
