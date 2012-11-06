#!/bin/sh

LOCAL_IP=10.10.1.250
LOCAL_IF=eth1

sudo /sbin/ifconfig ${LOCAL_IF}  ${LOCAL_IP} netmask 255.255.255.0 up
