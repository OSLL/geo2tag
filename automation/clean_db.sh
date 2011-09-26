#!/bin/bash

dir_automation="$WEBGEO_HOME/automation"
dir_geo2tag="${dir_automation}/geo2tag"
dir_log="${dir_automation}/platform_logs"
dir_backup="${dir_automation}/platform_backup"


#CREATE LOGS
mkdir ${dir_log}
date > ${dir_log}/build.log.txt
date > ${dir_log}/deploy.log.txt
date > ${dir_log}/test.log.txt

/etc/init.d/lighttpd stop >> ${dir_log}/build.log.txt 2>>${dir_log}/build.log.txt
$CATALINA_HOME/bin/shutdown.sh >> ${dir_log}/build.log.txt 2>>${dir_log}/build.log.txt
sudo -u postgres dropdb geo2tag >> ${dir_log}/build.log.txt 2>>${dir_log}/build.log.txt
sudo -u postgres dropuser geo2tag >> ${dir_log}/build.log.txt 2>>${dir_log}/build.log.txt


#DEPLOY
dpkg -i ${dir_backup}/wikigps-libs_*  >> ${dir_log}/deploy.log.txt 2>>${dir_log}/deploy.log.txt
dpkg -i ${dir_backup}/wikigps-service_*  >> ${dir_log}/deploy.log.txt 2>>${dir_log}/deploy.log.txt
$CATALINA_HOME/bin/startup.sh >> ${dir_log}/deploy.log.txt 2>>${dir_log}/deploy.log.txt
# check, does db appears on machine
db=`sudo -u postgres psql -A -q -t -c "select datname from pg_database" template1 | grep geo2tag`;
if [[ -n "$db" ]]
then
	status="success"
else
	status="fail_1"
fi

#TEST
if ${dir_automation}/test_platform.sh >> ${dir_log}/test.log.txt 2>>${dir_log}/test.log.txt
then
        # test cases passed
	status="success"
else
	# test cases not passed
	status="fail_2"
fi

# suscribe user "tracker" for "default" channel
curl -d '{"auth_token":"82dfa68f508feb7f6139327cdb837e69","channel":"default"}'  http://localhost:81/service?query=subscribe ;

#SEND EMAIL
cd ${dir_automation}
ant -f mail_sender.xml -Dsubject "geo2tag-platform night build($status): build, test, deploy reports " -Dlogdir "platform_logs"

#CLEAN LOGS
echo "" > ${dir_log}/build.log.txt
echo "" > ${dir_log}/deploy.log.txt
echo "" > ${dir_log}/test.log.txt

