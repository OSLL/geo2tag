#!/bin/bash

find ./ -name test.suite | xargs rm

dpkg-buildpackage -rfakeroot -j5



