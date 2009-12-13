#!/bin/bash

lat=59.9391
long=30.3116

while true; do
  shifting=`echo "scale=10; ($RANDOM - $RANDOM) / 1000000.23" | bc`
  echo "Shifting is $shifting"
  nlat=`echo "scale=10; $lat + $shifting" | bc`
  shifting=`echo "scale=10; ($RANDOM - $RANDOM) / 1000000.23" | bc`
  nlong=`echo "scale=10; $long + $shifting" | bc`
  echo $nlat > $1
  echo $nlong > $2
  echo "Updating latitude to $nlat, longitude to $nlong"
  lat=$nlat
  long=$nlong
  sleep 10;
done;





