#!/bin/bash

# init
init_dir=$(pwd)
$CATALINA_HOME/bin/shutdown.sh
/etc/init.d/lighttpd stop

# required software
sudo apt-get install unzip lsof git-core rubygems1.8 ruby1.8 ruby1.8-dev apache2
sudo gem install sinatra json test rack-test

# geo2tag infrastructure
export WEBGEO_HOME=/opt/geo2tag

cd $WEBGEO_HOME
dir_automation=$WEBGEO_HOME/automation
dir_3rd_party=$WEBGEO_HOME/3rd_party
dir_backup=$WEBGEO_HOME/backup
dir_repo=$WEBGEO_HOME/geo2tag-repo

# 1.
mkdir -p ${dir_backup}
cp -rf ${dir_automation} "${dir_backup}/automation$(date)"

# 2.
scp vood@zps.osll.spb.ru:~/3rd_party.tar.gz $WEBGEO_HOME
tar -zxvf $WEBGEO_HOME/3rd_party.tar.gz
rm $WEBGEO_HOME/3rd_party.tar.gz

# 3.
git clone git://github.com/OSLL/geo2tag.git ${dir_repo}
cd ${dir_repo}
git pull --all
git checkout master 
cp -rf ${dir_repo}/automation ${dir_automation}
#rm -rf ${dir_repo}

# 3.1.
cp ${dir_3rd_party}/mail.properties ${dir_automation}/mail.properties
cp ${dir_3rd_party}/mail.properties ${dir_automation}/platform_mail.properties

# 3.2.
cp -rf ${dir_3rd_party}/www /var/

# 4.
export JAVA_HOME=$WEBGEO_HOME/3rd_party/jdk1.6.0_20/
export PATH=${PATH}:$WEBGEO_HOME/3rd_party/jdk1.6.0_20/bin/
export CATALINA_HOME=$WEBGEO_HOME/3rd_party/apache-tomcat-7.0.19
export ANT_HOME=$WEBGEO_HOME/3rd_party/apache-ant-1.8.2
export PATH=${PATH}:$WEBGEO_HOME/3rd_party/apache-ant-1.8.2/bin/

sed -i '/WEBGEO_HOME/d' ~/.bashrc

echo 'export WEBGEO_HOME=/opt/geo2tag' 					>> ~/.bashrc
echo 'export JAVA_HOME=$WEBGEO_HOME/3rd_party/jdk1.6.0_20/' 		>> ~/.bashrc
echo 'export PATH=${PATH}:$WEBGEO_HOME/3rd_party/jdk1.6.0_20/bin/' 	>> ~/.bashrc
echo 'export CATALINA_HOME=$WEBGEO_HOME/3rd_party/apache-tomcat-7.0.19' >> ~/.bashrc
echo 'export ANT_HOME=$WEBGEO_HOME/3rd_party/apache-ant-1.8.2' 		>> ~/.bashrc
echo 'export PATH=${PATH}:$WEBGEO_HOME/3rd_party/apache-ant-1.8.2/bin/' >> ~/.bashrc

# 5.
file=${dir_automation}/local.properties

echo "gwt.sdk=${dir_3rd_party}/gwt-2.3.0"					 > ${file}
echo "junit.jar=${dir_3rd_party}/libs/junit-4.9b3.jar"				>> ${file}
echo "openlayers.jar=${dir_3rd_party}/libs/gwt-openlayers-client-0.5.jar"	>> ${file}
echo "gwt-visual.jar=${dir_3rd_party}/libs/gwt-visualization.jar"		>> ${file}
echo ""										>> ${file}
echo "server.url=http://62.76.179.81:8080"					>> ${file}
echo "server.username=name"							>> ${file}
echo "server.password=pass"							>> ${file}

# 6.
cron_file=${dir_automation}/crontab.properties
echo "SHELL=/bin/bash"								 > ${cron_file} 
echo "PATH=${PATH}"								>> ${cron_file}
echo "HOME=${dir_automation}"							>> ${cron_file}
echo "WEBGEO_HOME=${WEBGEO_HOME}"						>> ${cron_file}
echo "JAVA_HOME=$WEBGEO_HOME/3rd_party/jdk1.6.0_20/" 				>> ${cron_file}
echo "CATALINA_HOME=$WEBGEO_HOME/3rd_party/apache-tomcat-7.0.19"		>> ${cron_file}
echo "ANT_HOME=$WEBGEO_HOME/3rd_party/apache-ant-1.8.2"       			>> ${cron_file}
echo ""										>> ${cron_file}
echo "59 23 * * * ${dir_automation}/midnight_deploy.sh"				>> ${cron_file}
crontab ${cron_file}

# 7.
# Listening on 0.0.0.0:9494
cd ${dir_automation}
ruby1.8 refresh.rb &
echo "finish..."

# start tomcat
$CATALINA_HOME/bin/startup.sh

# start lighttpd
/etc/init.d/lighttpd start

#cd ${init_dir}
bash -i
