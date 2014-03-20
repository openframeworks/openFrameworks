#!/bin/bash
version=$1
cd $(cat ~/.ofprojectgenerator/config)/scripts/dev

./create_package.sh linux $version
./create_package.sh linux64 $version
./create_package.sh linuxarmv6l $version
./create_package.sh linuxarmv7l $version
./create_package.sh win_cb $version
./create_package.sh vs $version
./create_package.sh ios $version
./create_package.sh osx $version
./create_package.sh android $version
mkdir /var/www/versions/v${version}
mv *.tar.gz /var/www/versions/v${version}
mv *.zip /var/www/versions/v${version}

wget http://openframeworks.cc/release_hook.php?version=${version} -O /dev/null
