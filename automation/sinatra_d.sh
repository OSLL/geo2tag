#!/bin/bash

dir_automation=$WEBGEO_HOME/automation
sinPID=`ps aux | grep "refresh.rb" | grep -v grep | awk {'print $2'}`
target=$1

echo "${dir_automation}"
echo "${sinPID}"


if [ "$target" == "start" ] ; then
	if [[ "$sinPID" > 0 ]] ; then
		echo "sinatra already running";
	else
		/etc/init.d/lighttpd stop
		cd ${dir_automation}
		ruby1.8 refresh.rb &
		/etc/init.d/lighttpd stop
		
		echo "sinatra started ...";
	fi	
fi

if [ "$target" == "stop" ] ; then
	if [[ "$sinPID" > 0 ]] ; then
		kill -9 $sinPID
		echo "sinatra stopped ...";
	else
		echo "sinatra not running";
	fi	

fi

if [ "$target" == "restart" ] ; then
	kill -9 $sinPID
	/etc/init.d/lighttpd stop
	cd ${dir_automation}
	ruby1.8 refresh.rb &
	/etc/init.d/lighttpd start
	
	echo "sinatra restarted ...";
fi

