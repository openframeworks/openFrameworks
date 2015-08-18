#!/usr/bin/env bash

if [ $EUID != 0 ]; then
	echo "this script must be run using sudo"
	echo ""
	echo "usage:"
	echo "sudo "$0
	exit $exit_code
   exit 1
fi

MAJOR_VERSION=$(lsb_release -r | cut -f2 -d: | cut -f1 -d. | sed "s/\t//g")
MINOR_VERSION=$(lsb_release -r | cut -f2 -d: | cut -f2 -d.)

echo "Running on ubuntu ${MAJOR_VERSION}.${MINOR_VERSION}"

if [ $(expr $MAJOR_VERSION \< 12 ) -eq 1 ]; then
    echo "Your ubuntu version is too old try using an older version of openFrameworks or updating your system"
    exit 1
elif [ $(expr $MAJOR_VERSION \< 13 ) -eq 1 ]; then
    add-apt-repository ppa:ubuntu-toolchain-r/test --yes
    add-apt-repository ppa:gstreamer-developers/ppa --yes
    add-apt-repository ppa:boost-latest/ppa --yes
    CXX_VER=-4.9
    BOOST_VER=1.55
elif [ $(expr $MAJOR_VERSION \< 14 ) -eq 1 ]; then
    add-apt-repository ppa:ubuntu-toolchain-r/test --yes
    add-apt-repository ppa:boost-latest/ppa --yes
    CXX_VER=-4.9
    BOOST_VER=1.55
else
    CXX_VER=
    BOOST_VER=
fi

apt-get update

GSTREAMER_VERSION=0.10
GSTREAMER_FFMPEG=gstreamer${GSTREAMER_VERSION}-ffmpeg

echo "detecting latest gstreamer version"
apt-cache show libgstreamer1.0-dev
exit_code=$?
if [ $exit_code = 0 ]; then
    echo selecting gstreamer 1.0
    GSTREAMER_VERSION=1.0
    GSTREAMER_FFMPEG=gstreamer${GSTREAMER_VERSION}-libav
fi

GTK_VERSION=2.0
echo "detecting latest gtk version"
apt-cache show libgtk-3-dev
exit_code=$?
if [ $exit_code = 0 ]; then
    echo selecting gtk 3
    GTK_VERSION=-3
fi

#checking for distrib tagged xserver-xorg
XTAG=$(dpkg -l |grep xserver-xorg-core|grep ii|awk '{print $2}'|sed "s/xserver-xorg-core//")
if [ ! -z $XTAG ]
then
	read -p " installing OF dependencies with "${XTAG}" packages, confirm Y/N ? " -n 1 -r
	if [[ $REPLY =~ ^[Yy]$ ]]
	then
		echo
		echo "installation of OF dependencies with "${XTAG}" packages confirmed"
	else
		XTAG="" 
	fi
fi

echo "installing OF dependencies"
apt-get install freeglut3-dev libasound2-dev libxmu-dev libxxf86vm-dev g++${CXX_VER} libgl1-mesa-dev${XTAG} libglu1-mesa-dev libraw1394-dev libudev-dev libdrm-dev libglew-dev libopenal-dev libsndfile-dev libfreeimage-dev libcairo2-dev libfreetype6-dev libssl-dev libpulse-dev libusb-1.0-0-dev libgtk${GTK_VERSION}-dev  libopencv-dev libassimp-dev librtaudio-dev libboost-filesystem${BOOST_VER}-dev
exit_code=$?
if [ $exit_code != 0 ]; then
    echo "error installing dependencies, there could be an error with your internet connection"
    echo "if the error persists, please report an issue in github: http://github.com/openframeworks/openFrameworks/issues"
	exit $exit_code
fi


echo "installing gstreamer"
apt-get install libgstreamer${GSTREAMER_VERSION}-dev libgstreamer-plugins-base${GSTREAMER_VERSION}-dev  ${GSTREAMER_FFMPEG} gstreamer${GSTREAMER_VERSION}-pulseaudio gstreamer${GSTREAMER_VERSION}-x gstreamer${GSTREAMER_VERSION}-plugins-bad gstreamer${GSTREAMER_VERSION}-alsa gstreamer${GSTREAMER_VERSION}-plugins-base gstreamer${GSTREAMER_VERSION}-plugins-good
exit_code=$?
if [ $exit_code != 0 ]; then
	echo "error installing gstreamer, there could be an error with your internet connection"
    echo "if the error persists, please report an issue in github: http://github.com/openframeworks/openFrameworks/issues"
	exit $exit_code
fi

if [ $(expr $MAJOR_VERSION \< 13 ) -eq 1 ]; then
    echo "detected ubuntu 12.xx setting gcc-${CXX_VER} as default compiler" 
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.6 20
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc${CXX_VER} 50
    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.6 20
    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++${CXX_VER} 50
fi
