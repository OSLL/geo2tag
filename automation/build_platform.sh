#!/bin/bash

# download sources
# build deb
# backup previous version
# install already builded
# make tests
# if there are errors restore backup
# send logs to the maillist

# First parametr of this script - name of the branch, wich platform will be builded and tested
/
platform_repo="geo2tag-platform-repo"
dir_automation="$WEBGEO_HOME/automation"
dir_geo2tag="${dir_automation}/${platform_repo}"
dir_log="${dir_automation}/platform_logs"
dir_backup="${dir_automation}/platform_backup"
remote_repo="devel@download.geo2tag.org"
remote_dir_repo="${remote_repo}:/var/www/geo2tag_repo/testing/binary_i386/";
remote_update_repo_command="ssh ${remote_repo} sudo /home/devel/update_repo.sh";
remote_test_dir_clean="/var/www/geo2tag_repo/testing/*/*"
# How many packages should appear after deb building
valid_package_number="3"
branch="$1"
last_commit="$2"

# Errors
deb_build_error="Errors during debian packages building (no packages appeat after build). More info in build.log.txt"
wrong_package_number_error="Package number not equal to valid value ($valid_package_number). More info in build.log.txt"
deb_installation_error="Error during debian packages installation. More info in deploy.log.txt"
tests_failed_error="Integration testing not passed. More info in test.log.txt"

no_backup_packages_error="No backup pakcages found. More info in test.log.txt"
backup_installation_error="Error during backup packages installation. More info in test.log.txt"

### This function cleans automation folder from temporary files
clean_automation ()
{
	cd ${dir_geo2tag}
	dh_clean
	cd ${dir_automation}
	rm *.deb
	rm *.tar.gz
	rm *.dsc
	rm *.changes
	
	echo "" > ${dir_log}/build.log.txt
	echo "" > ${dir_log}/deploy.log.txt
	echo "" > ${dir_log}/test.log.txt

	cd ${dir_geo2tag}
	git reset --hard 
	git clean -fxd
}

### This function do webtests update and send letter with status of build
send_status_letter ()
{
	# update webtests page
	cd ${dir_automation}
	./update_webtests.sh $branch
	echo "$status \n $letter_body"
	if [ "$branch" == "master" ]
	then
		ant -f mail_sender.xml -Dsubject "($status) geo2tag-platform master ${last_commit}: integration reports " -Dmessage "$letter_body" -Dlogdir "platform_logs" 
	else
		ant -f mail_sender.xml -Dsubject "($status) geo2tag-platform $branch ${last_commit}: build and test reports " -Dmessage "$letter_body" -Dlogdir "platform_logs"
	fi
	clean_automation
	exit 0
}

# If branch name is "web-devel" then exit
if [ "$branch" == "web-devel" ]
then
	exit 0
fi

if [ -e $dir_geo2tag ]; then
        echo "geo2tag directory exists" 
else
        git clone git://github.com/OSLL/geo2tag.git ${platform_repo}
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

cp ${dir_automation}/local.properties ${dir_geo2tag}/src/webside/
rm ${dir_geo2tag}/stuff/geo2tag.war

cd ${dir_geo2tag}
dh_clean
# ./build_debs.sh instead 'dpkg-buildpackage -rfakeroot'
echo "Starting ./build_debs.sh `date -R`" >> ${dir_log}/build.log.txt 2>>${dir_log}/build.log.txt
./build_debs.sh >> ${dir_log}/build.log.txt 2>>${dir_log}/build.log.txt
echo "Finished ./build_debs.sh `date -R`" >> ${dir_log}/build.log.txt 2>>${dir_log}/build.log.txt
cp ./test.log ./test_summary.log ${dir_log}
founded_packages=`ls "${dir_automation}" | grep [.]deb | grep -v standalone | grep -v observer`;
package_count=`echo "${founded_packages}" | wc -w `;
# Check that build is success - that some packages appears 
if [  "$package_count" == "0" ] ;
then 
	status="fail";
	letter_body="$deb_build_error";
	echo "No packages builded - see logs above" >> ${dir_log}/build.log.txt
	send_status_letter
fi


echo "Succesful build, got next packages: $founded_packages" >> ${dir_log}/build.log.txt
echo "Valid packages number: $valid_package_number" >> ${dir_log}/build.log.txt

if [  "$package_count" == "$valid_package_number" ] ;
then

	# Build succesful

	#UNIT TESTING

	echo "Unit testing:"  >>${dir_log}/test.log.txt
	${dir_geo2tag}/tst/unit/platform.sh >>${dir_log}/test.log.txt 2>>${dir_log}/test.log.txt

	# DEPLOY and TEST only if branch is master
	if [ "$branch" == "master" ] 
	then

		#DEPLOY
		cp ${dir_geo2tag}/automation/test_platform.sh ${dir_automation}
		cd ${dir_automation}
		echo "Starting deploy" >> ${dir_log}/deploy.log.txt 
		date -R >> ${dir_log}/deploy.log.txt
		echo "Stoping daemons:" >> ${dir_log}/deploy.log.txt 
		/etc/init.d/lighttpd stop  >> ${dir_log}/deploy.log.txt 2>>${dir_log}/deploy.log.txt		
		/etc/init.d/postgresql stop  >> ${dir_log}/deploy.log.txt 2>>${dir_log}/deploy.log.txt
		/etc/init.d/postgresql start  >> ${dir_log}/deploy.log.txt 2>>${dir_log}/deploy.log.txt
		echo "Removing database:" >> ${dir_log}/deploy.log.txt
		sudo -u postgres dropdb geo2tag >> ${dir_log}/deploy.log.txt 2>>${dir_log}/deploy.log.txt
		sudo -u postgres dropuser geo2tag >> ${dir_log}/deploy.log.txt 2>>${dir_log}/deploy.log.txt
		echo "Start packages install: " >> ${dir_log}/deploy.log.txt
		if ! echo "n" | dpkg -i geo2tag_*deb libgeo2tag_*deb geo2tag-webside*deb >> ${dir_log}/deploy.log.txt 2>>${dir_log}/deploy.log.txt
		then
			status="fail"
			letter_body="$deb_installation_error"
			echo "Installation failed, see logs above." >> ${dir_log}/deploy.log.txt
			send_status_letter		
		fi
		/etc/init.d/lighttpd start	
                echo "Deploy finished, starting testing" >> ${dir_log}/deploy.log.txt
                date -R >> ${dir_log}/deploy.log.txt
		#TEST
		test_result=`${dir_automation}/test_platform.sh`;
                echo "Testing finished" >> ${dir_log}/deploy.log.txt
                date -R >> ${dir_log}/deploy.log.txt

		echo "Integration tests:\n $test_result" >>${dir_log}/test.log.txt
		if ! echo $test_result | grep -i fail
		then
		# test cases passed, move installed debs to backup
			echo "Copying of debs to remote server"  >> ${dir_log}/test.log.txt
			ls ${dir_automation}/*.deb >> ${dir_log}/test.log.txt 2 >> ${dir_log}/test.log.txt
			# copy *.deb to repo
			ssh ${remote_repo} rm -rf ${remote_test_dir_clean}
			scp ${dir_automation}/*.deb ${remote_dir_repo} 
			${remote_update_repo_command} 
			echo "Tests passed. Current commit succesfuly integrated." >> ${dir_log}/test.log.txt
			cd "${dir_automation}"
			rm -rf "${dir_backup}"
			mkdir "${dir_backup}"
			mv -f ${dir_automation}/libgeo2tag_*deb "${dir_backup}"
			mv -f ${dir_automation}/geo2tag_*deb "${dir_backup}"	
			mv -f ${dir_automation}/geo2tag-webside*deb "${dir_backup}"	
		else
		# test cases not passed, restore backup
			echo "Tests not passed" >> ${dir_log}/test.log.txt
			status="fail";
			letter_body="$tests_failed_error"
			cd "${dir_backup}"
			echo "Restore backup" >> ${dir_log}/test.log.txt

			if ! echo "n" | dpkg -i `ls .` >> ${dir_log}/test.log.txt 2>>${dir_log}/test.log.txt
			then
				letter_body="$letter_body \n $backup_installation_error"
				echo "Errors during backup packages install, see logs above." >> ${dir_log}/test.log.txt 
			fi
			send_status_letter
		fi
	fi
else 
	status="fail";
	letter_body="$wrong_package_number_error"
	send_status_letter
fi

status="success"
letter_body=`cat "${dir_log}/test_summary.log"  `
send_status_letter
