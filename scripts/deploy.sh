#!/bin/bash

dropdb -h 127.0.0.1 -U geo2tag geo2tag

createdb -h 127.0.0.1 -U geo2tag geo2tag
psql -h 127.0.0.1 -U geo2tag -f base.sql geo2tag

