#!/bin/bash

tmpname=`tempfile`

echo $tmpname

cpp=`find ./ -name *.cpp`
h=`find ./ -name *.h`

files="${cpp} ${h} "

echo ${files}

for file in ${files};
do
	echo "Processing ${file}"
  bcpp -fnc ./conf/bcpp.cfg ${file} >${tmpname}
	cat ${tmpname} >${file}
done;
