#!/bin/bash

correct_result='"errno" : 0'; 

test_channel="test_$RANDOM";

sleep 15s

response_add_channel=`curl -d "{\"auth_token\":\"MMMMMMMMMM\", \"name\":\"$test_channel\", \"description\":\"\", \"url\":\"\", \"activeRadius\":30}"  http://localhost:81/service/addChannel`;
echo "$response_add_channel"
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

response_CI_test=`curl -d '{"login":"mark","password":"test"}'  http://localhost:81/service/login`;
if ! echo $response_CI_test | grep -q -s -F "$correct_result_login"  ; 
then
	echo "Fail at caseinsentivenes test"
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

test_altitude=$((RANDOM%100)).0;
test_time=`date +'%d %m %Y %H:%M:%S.300'`;
echo "test time = $test_time"
echo "Test alt:$test_altitude"
response_write_tag_test=`curl -d "{ \"auth_token\" : \"MMMMMMMMMM\", \"channel\" : \"$test_channel\", \"description\" : \"\", \"altitude\" : $test_altitude , \"latitude\" : 0.0,\"link\" : \"\", \"longitude\" : 0.0, \"time\" : \"$test_time\", \"title\" : \"\" }"  http://localhost:81/service/writeTag`;
echo "$response_write_tag_test "
if ! echo $response_write_tag_test | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at writeTag test"
        exit 1
fi

sleep 15s

response_altitude_test=`curl -d "{\"auth_token\":\"MMMMMMMMMM\",\"latitude\":0.0, \"longitude\":0.0, \"radius\":10.0}"  http://localhost:81/service/loadTags`;
echo "Alt test - $response_altitude_test "
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

response_575_bug=`curl -d "{\"email\":\"email1@test1.org\", \"login\":\"Vasja\",\"password\":\"VasjaPWD\"}"  http://localhost:81/service/registerUser`;
correct_result_575="{ \"errno\" : 17 }";
if ! echo $response_575_bug | grep -q -s -F "$correct_result_575"  ;
then
        echo "Fail at bug_575 test"
        exit 1
fi

response_add_user_test=`curl -d "{\"login\":\"$test_channel\",\"password\":\"test\",\"email\":\"11@11.ru\"}" http://localhost:81/service/addUser`;
if ! echo $response_add_user_test | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at addUser test"
        exit 1
fi

echo "$test_channel"
response_delete_user_test=`curl -d "{\"login\":\"$test_channel\",\"password\":\"test\"}" http://localhost:81/service/deleteUser`;
response_check_delete_test=`curl -d "{\"login\":\"$test_channel\",\"password\":\"test\"}" http://localhost:81/service/login`;
if ! echo $response_delete_user_test | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at deleteUser test"
        exit 1
fi
if  echo $response_check_delete_test | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at checkDelete test"
        exit 1
fi


echo "Success"
