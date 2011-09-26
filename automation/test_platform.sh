#!/bin/bash

response=`curl -d '{"login":"Mark","password":"test"}'  http://localhost:81/service?query=login`;
correct_result='{ "auth_token" : "MMMMMMMMMM", "status" : "Ok", "status_description" : "Authorization was successful" }';
if [ "$response" != "$correct_result" ] ;
then
	echo "Fail"
	exit 1
fi
echo "Succes"

