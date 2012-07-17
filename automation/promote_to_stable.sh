#!/bin/bash

# This script copies content of testing repo to the stable repo

repo_path="/var/www/geo2tag_repo"
stable_repo="${repo_path}/stable"
testing_repo="${repo_path}/testing"
dir_automation="$WEBGEO_HOME/automation"
update_repo_script="${dir_automation}/update_repo.sh"

cp -r ${testing_repo}/* ${stable_repo}
${update_repo}
