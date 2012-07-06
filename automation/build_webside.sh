#!/bin/bash

# First parametr of this script - name of the branch, in with webside will be builded|tested

web_repo="geo2tag-web-repo"
app="geo2tag.war"
app_backup="geo2tag-backup.war"
dir_automation="$WEBGEO_HOME/automation"
dir_geo2tag="${dir_automation}/${web_repo}"
dir_log="${dir_automation}/webside_logs"

branch="$1"
last_commit="$2"

echo ${web_repo}
echo ${dir_geo2tag}

if [ "$branch" == "master" ]
then
# If branch is master, we neednt to build|test webside
exit 0
fi

if [ -e $dir_geo2tag ]; then
	echo "geo2tag directory exists"	
else 
	echo "geo2tag directory does not exists"
	git clone git://github.com/OSLL/geo2tag.git ${web_repo}
fi 

mkdir ${dir_log}

cd $dir_geo2tag
git pull --all > ${dir_log}/git.log.txt
git checkout ${branch} >>  ${dir_log}/git.log.txt
git reset --hard HEAD  >>  ${dir_log}/git.log.txt
cp ${dir_automation}/local.properties ${dir_geo2tag}/src/webside/local.properties
cp ${dir_automation}/mail.properties ${dir_geo2tag}/src/webside/

# TEST AND BUILD

cd ${dir_geo2tag}/src/webside/
date > ${dir_log}/test.log.txt
ant test >> ${dir_log}/test.log.txt
cp -rf  ${dir_geo2tag}/src/webside/reports/* ${dir_log}

if [ "$branch" == "web-devel" ] 
then 
	# if it is web-devel branch we should also make a deploy

	mv $CATALINA_HOME/webapps/${app} $CATALINA_HOME/webapps/${app_backup} 

	date > ${dir_log}/build.log.txt
	ant deploy-refresh-info >> ${dir_log}/build.log.txt		

	if [ -e $CATALINA_HOME/webapps/${app} ]; then
		date > ${dir_log}/deploy.log.txt
		echo "Deploy of new version of site successful." >> ${dir_log}/deploy.log.txt
		echo "" >> ${dir_log}/deploy.log.txt
		${dir_automation}/gen_index.sh
		ant -f ${dir_automation}/mail_sender.xml -Dsubject "(success) geo2tag-web web-devel ${last_commit}: integration reports" -Dmessage "" -Dlogdir ${dir_log}
	else 
		date > ${dir_log}/deploy.log.txt
		echo "Deploy of new version of site failed. Set stable version." >> ${dir_log}/deploy.log.txt
		echo "" >> ${dir_log}/deploy.log.txt
		mv $CATALINA_HOME/webapps/${app_backup} $CATALINA_HOME/webapps/${app}
		ant -f ${dir_automation}/mail_sender.xml -Dsubject "(fail) geo2tag-web web-devel ${last_commit}: integration reports" -Dmessage "" -Dlogdir ${dir_log}
	fi 

else
	# If it is not web-devel, then build, check war and send logs without deploy
	date > ${dir_log}/build.log.txt
	ant war >> ${dir_log}/build.log.txt	

	# If file *.war appears then build is succesful
	if [ -e ${dir_geo2tag}/src/webside/${app} ]
	then
		echo "Build of $branch successful." >> ${dir_log}/build.log.txt
 		ant -f ${dir_automation}/mail_sender.xml -Dsubject "(success) geo2tag-web $branch ${last_commit}: build and test reports" -Dmessage "" -Dlogdir ${dir_log}
	else
		echo "Build of $branch failed." >> ${dir_log}/build.log.txt
 		ant -f ${dir_automation}/mail_sender.xml -Dsubject "(fail) geo2tag-web $branch ${last_commit}: build and test reports" -Dmessage "" -Dlogdir ${dir_log}
	fi
fi

ant clean
git reset --hard
git clean -fxd
rm -rf ${dir_log}/*
