#!/bin/bash

dropdb -h zps.spb.su -U geo2tag geo2tag

createdb -h zps.spb.su -U geo2tag geo2tag
psql -h zps.spb.su -U geo2tag -f base.sql
