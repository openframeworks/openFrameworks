#!/bin/bash
set -ev
SCRIPTS_DIR=$(dirname "$0")
cd ~
sudo apt-get update
sudo apt-get install multistrap unzip
multistrap -a armhf -d raspbian -f $SCRIPTS_DIR/multistrap.conf
wget https://github.com/raspberrypi/tools/archive/master.zip -O raspberrytools.zip
unzip raspberrytools.zip
rm raspberrytools.zip

