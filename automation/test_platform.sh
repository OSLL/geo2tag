#!/bin/bash

correct_result='"errno" : 0'; 

test_channel="test_$RANDOM";

response_add_channel=`curl -d "{\"auth_token\":\"MMMMMMMMMM\", \"name\":\"$test_channel\", \"description\":\"\", \"url\":\"\", \"activeRadius\":30}"  http://localhost:81/service/addChannel`;
if ! echo $response_add_channel | grep -q -s -F "$correct_result"  ;
then
	echo "Fail at addChannel test"
	exit 1
fi

response_subscribe=`curl -d "{\"auth_token\":\"MMMMMMMMMM\", \"channel\":\"$test_channel\"}" http://localhost:81/service/subscribe`;
if ! echo $response_subscribe | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at subscribe test"
        exit 1
fi


response_login_test=`curl -d '{"login":"Mark","password":"test"}'  http://localhost:81/service/login`;
correct_result_login='{ "auth_token" : "MMMMMMMMMM", "errno" : 0 }';
if ! echo $response_login_test | grep -q -s -F "$correct_result_login"  ; 
then
	echo "Fail at login test"
	exit 1
fi


response_incorrect_json_test=`curl -d '{"login":"Markpassword":"test"}'  http://localhost:81/service/login`;
correct_result_incorrect_json='{ "errno" : 9 }';
if ! echo $response_incorrect_json_test | grep -q -s -F "$correct_result_incorrect_json"  ; 
then
	echo "Fail at incorrect json test"
	exit 1
fi


response_incorrect_url_test=`curl -d ''  http://localhost:81/service/incorrect_url`;
correct_result_incorrect_url='{ "errno" : 8 }';
if ! echo $response_incorrect_url_test | grep -q -s -F "$correct_result_incorrect_url"  ; 
then
	echo "Fail at incorrect url test"
	exit 1
fi

test_altitude=$((RANDOM%100)).$((RANDOM%1000));
echo $test_altitude
response_write_tag_test=`curl -d "{ \"auth_token\" : \"MMMMMMMMMM\", \"channel\" : \"$test_channel\", \"description\" : \"\", \"altitude\" : $test_altitude , \"latitude\" : 0.0,\"link\" : \"\", \"longitude\" : 0.0, \"time\" : \"04 03 2011 15:33:47.630\", \"title\" : \"\" }"  http://localhost:81/service/writeTag`;
echo "$response_write_tag_test "
if ! echo $response_write_tag_test | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at writeTag test"
        exit 1
fi


response_altitude_test=`curl -d "{\"auth_token\":\"MMMMMMMMMM\",\"latitude\":0.0, \"longitude\":0.0, \"radius\":10.0}"  http://localhost:81/service/loadTags`;
if ! echo $response_altitude_test | grep -q -s -F "$test_altitude"  ;
then
        echo "Fail at altitude test"
        exit 1
fi


response_unsubscribe=`curl -d "{\"auth_token\":\"MMMMMMMMMM\", \"channel\":\"$test_channel\"}" http://localhost:81/service/unsubscribe`;
if ! echo $response_unsubscribe | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at unsubscribe test"
        exit 1
fi


echo "Succes"
