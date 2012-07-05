#!/bin/bash

# This script should be runned by cron 
# */3 * * * * /path/check_deb_build_tasks.sh

dir_automation="$WEBGEO_HOME/automation"
dir_tasks="${dir_automation}/deb_build_tasks"
build_script="${dir_automation}/build_platform.sh"

#  Check that build_platform doesnt started
##  If started - exit 
isBuildingRunned=`pgrep '(build_platform|clean_db)'`;
if [[ -n "$isBuildingRunned" ]]
then 
	exit 0;
fi 

##  If not - while directory /opt/geo2tag/automation/build_tasks is not empty
##  call  build_remote.sh `cat /opt/geo2tag/automation/build_tasks/$i`

#build_tasks=`ls ${dir_tasks}`;

cd ${dir_automation}

for i in `ls -t -r ${dir_tasks}`
do
		
	current_task=`cat "${dir_tasks}/$i"`
	rm "${dir_tasks}/$i"
        tasks=`ls ${dir_tasks} | wc -w`
	queue=`ls -t -r ${dir_tasks} | grep -v $i`;
        ant -f mail_sender.xml -Dsubject "Starting platform build at $i branch ($tasks in queue)" -Dmessage "Queue: $queue " -Dlogdir "build_tasks" 
	## Run remote build
	${build_script} ${current_task}
done;

