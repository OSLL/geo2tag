#!/bin/bash

dropdb -h 127.0.0.1 -U geo2tag demo

createdb -h 127.0.0.1 -U geo2tag demo
psql -h 127.0.0.1 -U geo2tag -f base.sql demo
