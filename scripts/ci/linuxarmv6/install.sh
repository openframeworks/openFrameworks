#!/bin/bash
set -ev
sudo apt-get -y update
sudo apt-get -y install multistrap unzip
cd $(dirname $0)
multistrap -a armhf -d raspbian -f multistrap.conf
wget https://github.com/raspberrypi/tools/archive/master.zip -O raspberrytools.zip
unzip raspberrytools.zip
rm raspberrytools.zip

