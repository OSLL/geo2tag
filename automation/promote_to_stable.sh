#!/bin/bash

# This script copies content of testing repo to the stable repo

repo_path="/var/www/geo2tag_repo"
stable_repo="${repo_path}/stable"
testing_repo="${repo_path}/testing"
update_repo_script="/home/devel/update_repo.sh"

rm -rf ${stable_repo}/*/*
cp -r ${testing_repo}/* ${stable_repo}
${update_repo}
