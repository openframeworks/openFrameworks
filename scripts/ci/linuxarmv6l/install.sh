#!/bin/bash
set -e
set -o pipefail
# trap any script errors and exit
trap "trapError" ERR

trapError() {
	echo
	echo " ^ Received error ^"
	cat formula.log
	exit 1
}

installPackages(){
    sudo apt-get -y update
    sudo apt-get -y install multistrap unzip
    #workaround for https://bugs.launchpad.net/ubuntu/+source/multistrap/+bug/1313787
    sudo sed -i s/\$forceyes//g /usr/sbin/multistrap
}

createRaspbianImg(){
    #needed since Ubuntu 18.04 - allow non https repositories 
    mkdir -p raspbian/etc/apt/apt.conf.d/
    echo 'Acquire::AllowInsecureRepositories "true";' | sudo tee raspbian/etc/apt/apt.conf.d/90insecure
    multistrap -a armhf -d raspbian -f multistrap.conf
}

SCRIPT_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$SCRIPT_DIR" ]]; then SCRIPT_DIR="$PWD"; fi
. "$SCRIPT_DIR/../../dev/downloader.sh"

downloadToolchain(){
    wget https://github.com/openframeworks/openFrameworks/releases/download/tools/cross-gcc-9.4.0-pi_0-1.tar.gz
    tar xvf cross-gcc-9.4.0-pi_0-1.tar.gz
    mv cross-pi-gcc-9.4.0-0 rpi_toolchain
    rm cross-gcc-9.4.0-pi_0-1.tar.gz
}

downloadFirmware(){
    wget https://github.com/raspberrypi/firmware/archive/master.zip -O firmware.zip
    unzip firmware.zip
    cp -r firmware-master/opt raspbian/
    rm -r firmware-master
    rm firmware.zip
}

relativeSoftLinks(){
    for link in $(ls -la | grep "\-> /" | sed "s/.* \([^ ]*\) \-> \/\(.*\)/\1->\/\2/g"); do
        lib=$(echo $link | sed "s/\(.*\)\->\(.*\)/\1/g");
        link=$(echo $link | sed "s/\(.*\)\->\(.*\)/\2/g");
        rm $lib
        ln -s ../../..$link $lib
    done

    for f in *; do
        error=$(grep " \/lib/" $f > /dev/null 2>&1; echo $?)
        if [ $error -eq 0 ]; then
            sed -i "s/ \/lib/ ..\/..\/..\/lib/g" $f
            sed -i "s/ \/usr/ ..\/..\/..\/usr/g" $f
        fi
    done
}


ROOT=$( cd "$(dirname "$0")" ; pwd -P )
echo $ROOT
cd $ROOT
installPackages
createRaspbianImg
downloadToolchain
downloadFirmware

#needed for some of the toolchain libs
cp -rn rpi_toolchain/arm-linux-gnueabihf/libc/lib/* $ROOT/raspbian/usr/lib/
cp -rn rpi_toolchain/arm-linux-gnueabihf/libc/usr/lib/* $ROOT/raspbian/usr/lib/
cp -rn rpi_toolchain/arm-linux-gnueabihf/lib/* $ROOT/raspbian/usr/lib/

cd $ROOT/raspbian/usr/lib
relativeSoftLinks
cd $ROOT/raspbian/usr/lib/arm-linux-gnueabihf
relativeSoftLinks
