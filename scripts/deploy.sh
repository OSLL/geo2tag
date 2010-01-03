#!/bin/bash

dropdb -h 174.120.253.36 -U geo2tag geo2tag

createdb -h 174.120.253.36 -U geo2tag geo2tag
psql -h 174.120.253.36 -U geo2tag -f base.sql
