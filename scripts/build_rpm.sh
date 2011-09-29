#!/bin/bash
# Run this script in the root of geo2tag
version="0.1.7";
rm -rf  wikigps-service-$version
mkdir wikigps-service-$version
cp -r src/ config.pri wikigps-service-$version
tar -czfv wikigps-service-$version.tar.gz wikigps-service-$version
cp wikigps-service-$version.tar.gz ~/rpmbuild/SOURCES 
rpmbuild -ba wikigps-service.spec
