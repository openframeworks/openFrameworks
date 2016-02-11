#!/bin/bash
set -ev
sudo apt-get -y update
sudo apt-get -y install multistrap unzip
#workaround for https://bugs.launchpad.net/ubuntu/+source/multistrap/+bug/1313787
sudo sed -i s/\$forceyes//g /usr/sbin/multistrap
ROOT=$( cd "$(dirname "$0")" ; pwd -P )
cd $ROOT
multistrap -a armhf -d raspbian -f multistrap.conf

wget http://ci.openframeworks.cc/rpi_toolchain.tar.bz2
tar xjf rpi_toolchain.tar.bz2
rm rpi_toolchain.tar.bz2

wget https://github.com/raspberrypi/firmware/archive/master.zip -O firmware.zip
unzip firmware.zip
cp -r firmware-master/opt raspbian/
rm -r firmware-master
rm firmware.zip

relativeSoftLinks(){
    for link in $(ls -la | grep "\-> /" | sed "s/.* \([^ ]*\) \-> \/\(.*\)/\1->\/\2/g"); do 
        lib=$(echo $link | sed "s/\(.*\)\->\(.*\)/\1/g"); 
        link=$(echo $link | sed "s/\(.*\)\->\(.*\)/\2/g"); 
        rm $lib
        ln -s ../../..$link $lib 
    done

    for f in *; do 
        grep " \/lib/" $f 2> /dev/null; 
        if [ $? -eq 0 ]; then 
            sed -i "s/ \/lib/ ..\/..\/..\/lib/g" $f
            sed -i "s/ \/usr/ ..\/..\/..\/usr/g" $f
        fi
    done
}

cd $ROOT/raspbian/usr/lib
relativeSoftLinks
cd $ROOT/raspbian/usr/lib/arm-linux-gnueabihf
relativeSoftLinks
cd $ROOT/raspbian/usr/lib/gcc/arm-linux-gnueabihf/4.9
