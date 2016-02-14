#!/usr/bin/env bash

if [ $EUID != 0 ]; then
	echo "this script must be run as root"
	echo ""
	echo "usage:"
	echo "su -"
	echo "./install_dependencies.sh"
	exit $exit_code
   exit 1
fi

pacman -Sy --needed make pkg-config gcc openal python-lxml glew freeglut freeimage gstreamer gst-plugins-base gst-plugins-good gst-plugins-bad gst-libav raspberrypi-firmware gst-omx-rpi assimp rtaudio boost

exit_code=$?
if [ $exit_code != 0 ]; then
	echo "error installing packages, there could be an error with your internet connection"
	exit $exit_code
fi

export LC_ALL=C
GCC_MAJOR_GT_4=$(expr `gcc -dumpversion | cut -f1 -d.` \> 4)
if [ $GCC_MAJOR_GT_4 -eq 1 ]; then
    echo
    echo
    echo "It seems you are running gcc 5 or later, due to incomatible ABI with previous versions"
    echo "we need to recompile poco. This will take a while"
    read -p "Press any key to continue... " -n1 -s
    
	sys_cores=$(getconf _NPROCESSORS_ONLN)
	if [ $sys_cores -gt 1 ]; then
		cores=$(($sys_cores-1))
	else
		cores=1
	fi
	
    DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
    cd ${DIR}/../../apothecary
    ./apothecary -j${cores} update poco
fi
