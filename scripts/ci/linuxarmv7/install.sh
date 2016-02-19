#!/bin/bash
#set -e
#set -o pipefail
# trap any script errors and exit
trap "trapError" ERR

trapError() {
	echo
	echo " ^ Received error ^"
	exit 1
}

createArchImg(){
    sudo ./arch-bootstrap.sh archlinux
}

downloadToolchain(){
    #wget http://archlinuxarm.org/builder/xtools/x-tools7h.tar.xz
    #tar xf x-tools7h.tar.xz
    #rm x-tools7h.tar.xz
    sudo apt-get -y update
    sudo apt-get install -y gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf libasound2-dev
}

downloadFirmware(){
    wget https://github.com/raspberrypi/firmware/archive/master.zip -O firmware.zip
    unzip firmware.zip
    cp -r firmware-master/opt archlinux/
    rm -r firmware-master
    rm firmware.zip
}


relativeSoftLinks(){
    rel_link=$1
    escaped_rel_link=$2
    for link in $(ls -la | grep "\-> /" | sed "s/.* \([^ ]*\) \-> \/\(.*\)/\1->\/\2/g"); do 
        lib=$(echo $link | sed "s/\(.*\)\->\(.*\)/\1/g"); 
        link=$(echo $link | sed "s/\(.*\)\->\(.*\)/\2/g"); 
        rm $lib
        ln -s ${rel_link}/${link} $lib 
    done

    for f in *; do 
        error_lib=$(grep " \/lib/" $f > /dev/null 2>&1; echo $?) 
        error_usr=$(grep " \/usr/" $f > /dev/null 2>&1; echo $?) 
        if [ $error_lib -eq 0 ] || [ $error_usr -eq 0 ]; then 
            sed -i "s/ \/lib/ $escaped_rel_link\/lib/g" $f
            sed -i "s/ \/usr/ $escaped_rel_link\/usr/g" $f
        fi
    done
}



ROOT=$( cd "$(dirname "$0")" ; pwd -P )
echo $ROOT
cd $ROOT
createArchImg
downloadToolchain
downloadFirmware

cd $ROOT/archlinux/usr/lib
relativeSoftLinks "../.." "..\/.."
#cd $ROOT/archlinux/usr/lib/arm-unknown-linux-gnueabihf
#relativeSoftLinks  "../../.." "..\/..\/.."
#cd $ROOT/raspbian/usr/lib/gcc/arm-unknown-linux-gnueabihf/5.3
#relativeSoftLinks  "../../../.." "..\/..\/..\/.."
