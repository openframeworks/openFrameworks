#!/bin/bash
version=$1
cd $(cat ~/.ofprojectgenerator/config)/scripts/dev

./create_package.sh linux $version
./create_package.sh linux64 $version
./create_package.sh win_cb $version
./create_package.sh vs2010 $version
./create_package.sh ios $version
./create_package.sh osx $version
./create_package.sh android $version
mkdir /var/www/versions/$version
mv *.tar.gz /var/www/versions/$version
mv *.zip /var/www/versions/$version

wget http://openframeworks.cc/release_hook.php?version=${version} -O /dev/null
