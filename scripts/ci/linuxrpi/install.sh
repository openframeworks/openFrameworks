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
	#workaround for https://bugs.launchpad.net/ubuntu/+source/multistrap/+bug/1313787
	sudo sed -i s/\$forceyes//g /usr/sbin/multistrap
}

downloadFirmware(){
	# FIXME: only if it is not aarch64
	git clone https://github.com/raspberrypi/userland --depth 1
	# cp -r userland/opt raspbian/
}

createRaspbianImg(){
	if [ -n "$MULTISTRAP_ARCH" ]; then
	echo ${MULTISTRAP_ARCH}
	else
	# export MULTISTRAP_ARCH=arm64
	export MULTISTRAP_ARCH=armhf
	fi
	#needed since Ubuntu 18.04 - allow non https repositories 
	mkdir -p raspbian/etc/apt/apt.conf.d/
	echo 'Acquire::AllowInsecureRepositories "true";' | sudo tee raspbian/etc/apt/apt.conf.d/90insecure
	# multistrap -a arm64 -d raspbian -f multistrap.conf
	multistrap -a ${MULTISTRAP_ARCH} -d raspbian -f multistrap.conf    
}

SCRIPT_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$SCRIPT_DIR" ]]; then SCRIPT_DIR="$PWD"; fi
. "$SCRIPT_DIR/../../dev/downloader.sh"

ROOT=$( cd "$(dirname "$0")" ; pwd -P )
echo $ROOT
cd $ROOT
# installPackages
createRaspbianImg
downloadFirmware
