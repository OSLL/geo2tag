#!/bin/bash

correct_result='"errno" : 0'; 

test_channel="geo2tag_demo_channel";


GEO2TAG_INSTANCE="http://demo.geo2tag.org/service"

#sleep 15s

response_login_test=`curl -d '{"login":"Mark","password":"test"}' ${GEO2TAG_INSTANCE}/login`;
echo $ "Login test - $response_login_test"
auth_token=`echo $response_login_test | grep -Po '"\w+"' | grep -v "auth_token" | grep -v "errno"`;
correct_result_login="\"errno\" : 0";
#correct_result_login="{ \"auth_token\" : \"mmmmmmmmmm\", \"errno\" : 0 }";
if ! echo $response_login_test | grep -q -s -F "$correct_result_login"  ; 
then
	echo "Fail at login test"
	exit 1
fi

response_CI_test=`curl -d '{"login":"mark","password":"test"}'  ${GEO2TAG_INSTANCE}/login`;
echo "CI test - $response_CI_test"
if ! echo $response_CI_test | grep -q -s -F "$correct_result_login"  ; 
then
	echo "Fail at caseinsentivenes test"
	exit 1
fi

response_add_channel=`curl -d "{\"auth_token\":"$auth_token", \"name\":\"$test_channel\", \"description\":\"\", \"url\":\"\", \"activeRadius\":30}"  ${GEO2TAG_INSTANCE}/addChannel`;
echo "Add channel test - $response_add_channel"
if ! echo $response_add_channel | grep -q -s -F "$correct_result"  ;
then
	echo "Fail at addChannel test, may failed, expected"
#	exit 1
fi

response_subscribe=`curl -d "{\"auth_token\":"$auth_token", \"channel\":\"$test_channel\"}" ${GEO2TAG_INSTANCE}/subscribe`;
echo "Subscribe test - $response_subscribe"
if ! echo $response_subscribe | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at subscribe test"
        exit 1
fi


test_altitude=$((RANDOM%100)).0;
test_time=`date +'%d %m %Y %H:%M:%S.300'`;
echo "test time = $test_time"
echo "Test alt:$test_altitude"
response_write_tag_test=`curl -d "{ \"auth_token\" : "$auth_token", \"channel\" : \"$test_channel\", \"description\" : \"\", \"altitude\" : $test_altitude , \"latitude\" : 0.0,\"link\" : \"\", \"longitude\" : 0.0, \"time\" : \"$test_time\", \"title\" : \"\" }"  ${GEO2TAG_INSTANCE}/writeTag`;
echo "Write tag test - $response_write_tag_test"
if ! echo $response_write_tag_test | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at writeTag test"
        exit 1
fi

#sleep 15s

response_altitude_test=`curl -d "{\"auth_token\":"$auth_token",\"latitude\":0.0, \"longitude\":0.0, \"radius\":10.0}"  ${GEO2TAG_INSTANCE}/loadTags`;
echo "Alt test - $response_altitude_test "
if ! echo $response_altitude_test | grep -q -s -F "$test_altitude"  ;
then
        echo "Fail at altitude test"
        exit 1
fi

response_unsubscribe=`curl -d "{\"auth_token\":"$auth_token", \"channel\":\"$test_channel\"}" ${GEO2TAG_INSTANCE}/unsubscribe`;
echo "Unsubscribe test - $response_unsubscribe"
if ! echo $response_unsubscribe | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at unsubscribe test"
        exit 1
fi


response_add_user_test=`curl -d "{\"login\":\"$test_channel\",\"password\":\"test\",\"email\":\"11@11.ru\"}" ${GEO2TAG_INSTANCE}/addUser`;
echo "Add user test - $response_add_user_test"
if ! echo $response_add_user_test | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at addUser test -- expected, continue"
#        exit 1
fi

echo "$test_channel"
response_delete_user_test=`curl -d "{\"login\":\"$test_channel\",\"password\":\"test\"}" ${GEO2TAG_INSTANCE}/deleteUser`;
echo "Delete user test - $response_delete_user_test"
response_check_delete_test=`curl -d "{\"login\":\"$test_channel\",\"password\":\"test\"}" ${GEO2TAG_INSTANCE}/login`;
echo "Check delete test - $response_check_delete_test"
if ! echo $response_delete_user_test | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at deleteUser test -- expected, continue"
#        exit 1
fi
if  echo $response_check_delete_test | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at checkDelete test --  expected, continue"
#        exit 1
fi

response_quit_session_test=`curl -d "{\"auth_token\":"$auth_token"}"  ${GEO2TAG_INSTANCE}/quitSession`;
echo "Quit session test - $response_quit_session_test"
correct_result_quit_session='{ "errno" : 0 }';
if ! echo $response_quit_session_test | grep -q -s -F "$correct_result_quit_session"  ; 
then
	echo "Fail at quitSession test"
	exit 1
fi


random_number=$((RANDOM%1000000));
rand_user=user_$random_number;
rand_email=email_$random_number;
response_add_rand_user_test=`curl -d "{\"login\":\"$rand_user\",\"password\":\"test\",\"email\":\"$rand_email\"}" ${GEO2TAG_INSTANCE}/addUser`;
echo "Add random user test - $response_add_rand_user_test"
if ! echo $response_add_rand_user_test | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at add random user test"
        exit 1
fi


response_restore_password_test=`curl -d "{\"email\":\"$rand_email\"}"  ${GEO2TAG_INSTANCE}/restorePassword`;
echo $response_restore_password_test;
echo $ "Restore Password test - $response_restore_password_test"
correct_result_restore_password="\"errno\" : 0";
if ! echo $response_restore_password_test | grep -q -s -F "$correct_result_restore_password"  ; 
then
        echo "Fail at restorePassword test"
        exit 1
fi



echo "Success"

