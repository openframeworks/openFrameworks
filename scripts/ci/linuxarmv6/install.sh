#!/bin/bash
set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
cd ~
multistrap -a armhf -d raspbian -f multistrap.conf
wget https://github.com/raspberrypi/tools/archive/master.zip -O raspberrytools.zip
unzip raspberrytools.zip
rm raspberrytools.zip

