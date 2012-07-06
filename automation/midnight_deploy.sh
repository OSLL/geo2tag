#!/bin/bash

# This script should be runned by cron using next params:
# 59 23 * * * /path/midnight_deploy.sh

web_repo="geo2tag-web-repo"
app="geo2tag.war"
app_backup="geo2tag-backup.war"
dir_automation="$WEBGEO_HOME/automation"
dir_geo2tag="${dir_automation}/${web_repo}"
dir_log="${dir_automation}/webside_logs"


if [ -e $dir_geo2tag ]; then
	echo "geo2tag directory exists"	
else 
        echo "geo2tag directory does not exists"
        git clone git://github.com/OSLL/geo2tag.git ${web_repo}
fi 

cd $dir_geo2tag
git pull --all
git checkout web-devel
cp ${dir_automation}/local.properties ${dir_geo2tag}/src/webside/local.properties
cp ${dir_automation}/mail.properties ${dir_geo2tag}/src/webside/

# TEST AND BUILD
mkdir ${dir_log}
mv $CATALINA_HOME/webapps/${app} $CATALINA_HOME/webapps/${app_backup} 

cd ${dir_geo2tag}/src/webside/
date > ${dir_log}/test.log.txt
ant test >> ${dir_log}/test.log.txt
cp -rf  ${dir_geo2tag}/src/webside/reports/* ${dir_log}

date > ${dir_log}/build.log.txt
ant deploy-refresh-info >> ${dir_log}/build.log.txt

if [ -e $CATALINA_HOME/webapps/${app} ]; then
	date > ${dir_log}/deploy.log.txt
	echo "Deploy of new version of site successful." >> ${dir_log}/deploy.log.txt
	${dir_automation}/gen_index.sh
	ant -f ${dir_automation}/mail_sender.xml -Dsubject "(success) geo2tag-web - night build: build,test,deploy reports" -Dmessage "" -Dlogdir ${dir_log}
else 
	date > ${dir_log}/deploy.log.txt
	echo "Deploy of new version of site failed. Set stable version." >> ${dir_log}/deploy.log.txt
	mv $CATALINA_HOME/webapps/${app_backup} $CATALINA_HOME/webapps/${app}
	ant -f ${dir_automation}/mail_sender.xml -Dsubject "(fail) geo2tag-web - night build: build,test,deploy reports" -Dmessage "" -Dlogdir ${dir_log}
fi 
ant clean
git reset --hard
git clean -fxd
rm -rf ${dir_log}/*

${dir_automation}/clean_db.sh 
