#!/bin/bash

dir_sandbox="$WEBGEO_HOME/sandbox"
dir_geo2tag="${dir_sandbox}/geo2tag"
dir_log="${dir_sandbox}/platform_logs"
dir_backup="${dir_sandbox}/platform_backup"


#CREATE LOGS
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
	status="fail"
fi

#TEST
if ${dir_sandbox}/test_platform.sh >> ${dir_log}/test.log.txt 2>>${dir_log}/test.log.txt
then
        # test cases passed
	status="success"
else
	# test cases not passed
	status="fail"
fi

# suscribe user "tracker" for "default" channel
curl -d '{"auth_token":"82dfa68f508feb7f6139327cdb837e69","channel":"default"}'  http://localhost:81/service?query=subscribe ;

#SEND EMAIL
cd ${dir_sandbox}
ant -f mail_sender.xml -Dsubject "geo2tag-platform night build($status): build, test, deploy reports " -Dlogdir "platform_logs"

#CLEAN LOGS
echo "" > ${dir_log}/build.log.txt
echo "" > ${dir_log}/deploy.log.txt
echo "" > ${dir_log}/test.log.txt

