#!/bin/bash
sudo -u postgres psql --command="DROP DATABASE test_db;"
sudo -u postgres psql --command="DROP USER test_user;"
