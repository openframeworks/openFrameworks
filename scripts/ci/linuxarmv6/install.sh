#!/bin/bash
set -ev
sudo apt-get -y update
sudo apt-get -y install multistrap unzip
#workaround for https://bugs.launchpad.net/ubuntu/+source/multistrap/+bug/1313787
sudo sed -i s/\$forceyes//g /usr/sbin/multistrap
cd $(dirname $0)
multistrap -a armhf -d raspbian -f multistrap.conf
wget https://github.com/raspberrypi/tools/archive/master.zip -O raspberrytools.zip
unzip raspberrytools.zip
rm raspberrytools.zip

wget https://github.com/raspberrypi/firmware/archive/master.zip -O firmware.zip
unzip firmware.zip
cp -r firmware-master/opt raspbian/
rm -r firmware-master
rm firmware.zip


