#!/bin/bash
#WEBGEO_HOME="/opt/geo2tag"
#CATALINA_HOME="/opt/geo2tag/3rd_party/apache-tomcat-7.0.19"
platform_repo="geo2tag-platform-repo"
dir_automation="$WEBGEO_HOME/automation"
dir_geo2tag="${dir_automation}/${platform_repo}"
dir_log="${dir_automation}/platform_logs"
dir_backup="${dir_automation}/platform_backup"


#CREATE LOGS
mkdir ${dir_log}
date > ${dir_log}/build.log.txt
date > ${dir_log}/deploy.log.txt
date > ${dir_log}/test.log.txt

/etc/init.d/lighttpd stop >> ${dir_log}/build.log.txt 2>>${dir_log}/build.log.txt
$CATALINA_HOME/bin/shutdown.sh >> ${dir_log}/build.log.txt 2>>${dir_log}/build.log.txt
su - postgres -c "dropdb geo2tag" >> ${dir_log}/build.log.txt 2>>${dir_log}/build.log.txt
su - postgres -c "dropuser geo2tag" >> ${dir_log}/build.log.txt 2>>${dir_log}/build.log.txt

#DEPLOY
dpkg -i ${dir_backup}/libgeo2tag_*  >> ${dir_log}/deploy.log.txt 2>>${dir_log}/deploy.log.txt
echo "n" | dpkg -i ${dir_backup}/geo2tag_*  >> ${dir_log}/deploy.log.txt 2>>${dir_log}/deploy.log.txt
$CATALINA_HOME/bin/startup.sh >> ${dir_log}/deploy.log.txt 2>>${dir_log}/deploy.log.txt
# check, does db appears on machine
db=`sudo -u postgres psql -A -q -t -c "select datname from pg_database" template1 | grep geo2tag`;
if [[ -n "$db" ]]
then
	status="success"
else
	status="fail"
fi
echo $status
#exit 0
sleep 10s
#TEST
if ${dir_automation}/test_platform.sh >> ${dir_log}/test.log.txt 2>>${dir_log}/test.log.txt
then
        # test cases passed
	status="success"
else
	# test cases not passed
	status="fail"
fi

# suscribe user "tracker" for "default" channel
curl -d '{"auth_token":"82dfa68f508feb7f6139327cdb837e69","channel":"default"}'  http://localhost:81/service/subscribe ;

#SEND EMAIL
cd ${dir_automation}
test_summary=`cat ${dir_log}/test_summary.log `;
ant -f mail_sender.xml -Dsubject "($status) geo2tag-platform night build : build, test, deploy reports " -Dmessage "$test_summary" -Dlogdir "platform_logs"

#CLEAN LOGS
echo "" > ${dir_log}/build.log.txt
echo "" > ${dir_log}/deploy.log.txt
echo "" > ${dir_log}/test.log.txt

#CLEAN DB ON THE FEDORA MACHINE
#${dir_automation}/clean_remote.sh
