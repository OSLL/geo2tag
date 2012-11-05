#!/bin/sh

#scp *.deb devel@download.geo2tag.org:/var/www/geo2tag_repo/testing/binary_i386/
ssh       devel@download.geo2tag.org /home/devel/update_repo.sh
ssh       devel@download.geo2tag.org /home/devel/promote_to_stable.sh


