#!/bin/bash
set -e
set -o pipefail
# trap any script errors and exit
trap "trapError" ERR

if [ -n "$MULTISTRAP_ARCH" ]; then
echo ${MULTISTRAP_ARCH}
else
export MULTISTRAP_ARCH=armhf
fi

trapError() {
	echo
	echo " ^ Received error ^"
	cat formula.log
	exit 1
}

# SCRIPT_DIR="${BASH_SOURCE%/*}"
# if [[ ! -d "$SCRIPT_DIR" ]]; then SCRIPT_DIR="$PWD"; fi
# . "$SCRIPT_DIR/../../dev/downloader.sh"

ROOT=$( cd "$(dirname "$0")" ; pwd -P )
echo $ROOT
cd $ROOT

mkdir -p raspbian/etc/apt/apt.conf.d/
echo 'Acquire::AllowInsecureRepositories "true";' | sudo tee raspbian/etc/apt/apt.conf.d/90insecure
multistrap -a ${MULTISTRAP_ARCH} -d raspbian -f multistrap.conf   

if [ ${MULTISTRAP_ARCH} = "armhf" ]; then
	git clone https://github.com/raspberrypi/userland --depth 1 raspbian/userland
fi
