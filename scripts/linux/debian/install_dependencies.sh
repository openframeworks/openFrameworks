#!/usr/bin/env bash

if [ $EUID != 0 ]; then
	echo "this script must be run as root"
	echo ""
	echo "usage:"
	echo "sudo "$0
	exit $exit_code
   exit 1
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



echo "installing OF dependencies"
apt-get install freeglut3-dev libasound2-dev libxmu-dev libxxf86vm-dev g++ libgl1-mesa-dev libglu1-mesa-dev libraw1394-dev libudev-dev libdrm-dev libglew-dev libopenal-dev libsndfile-dev libfreeimage-dev libcairo2-dev libfreetype6-dev libssl-dev libpulse-dev libusb-1.0-0-dev libgtk${GTK_VERSION}-dev libopencv-dev libegl1-mesa-dev libgles1-mesa-dev libgles2-mesa-dev libassimp-dev librtaudio-dev libboost-filesystem-dev libglfw3-dev  liburiparser-dev libcurl4-openssl-dev libpugixml-dev
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

if [ -f /opt/vc/include/bcm_host.h ]; then
    echo "detected Raspberry Pi"
    echo "installing gstreamer omx and poco"
    apt-get install  gstreamer${GSTREAMER_VERSION}-omx libpoco-dev
fi

OS_CODENAME=$(cat /etc/os-release | grep VERSION= | sed "s/VERSION\=\"\(.*\)\"/\1/")

if [ "$OS_CODENAME" = "7 (wheezy)" ]; then
    echo "detected wheezy, installing g++4.8 for c++11 compatibility"
    apt-get install g++-4.8
	echo "setting gcc-${CXX_VER} as default compiler"
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 1 --force
    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 1 --force
fi

