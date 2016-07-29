#!/usr/bin/env bash

## thanks to Claudio for details on packages to install on fedora
if [ $EUID != 0 ]; then
	echo "this script must be run using sudo"
	echo ""
	echo "usage:"
	echo "sudo "$0
	exit $exit_code
   exit 1
fi

yum install freeglut-devel alsa-lib-devel libXmu-devel libXxf86vm-devel gcc-c++ libraw1394-devel gstreamer1-devel gstreamer1-plugins-base-devel libudev-devel libtheora-devel libvorbis-devel openal-soft-devel libsndfile-devel python-lxml glew-devel flac-devel freeimage-devel cairo-devel pulseaudio-libs-devel openssl-devel libusbx-devel gtk2-devel libXrandr-devel libXi-devel opencv-devel libX11-devel assimp-devel rtaudio-devel boost-devel gtk3-devel libglfw3-devel

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
