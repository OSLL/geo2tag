#!/bin/bash
sudo -u postgres createuser -s test_user
sudo -u postgres createdb -O test_user test_db
psql test_db -U test_user < ./scripts/base.sql
