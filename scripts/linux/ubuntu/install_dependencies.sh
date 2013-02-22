#!/bin/bash

if [ $EUID != 0 ]; then
	echo "this script must be run using sudo"
	echo ""
	echo "usage:"
	echo "sudo ./install_dependencies.sh"
	exit $exit_code
   exit 1
fi

apt-get update

GSTREAMER_VERSION=0.10
GSTREAMER_FFMPEG=gstreamer${GSTREAMER_VERSION}-ffmpeg

RET=$(apt-cache show -n libgstreamer1.0-dev &> /dev/null; echo $?)

if [ "$RET" -eq "0" ]; then
    echo selecting gstreamer 1.0
    GSTREAMER_VERSION=1.0
    GSTREAMER_FFMPEG=gstreamer${GSTREAMER_VERSION}-libav
fi


apt-get install freeglut3-dev libasound2-dev libxmu-dev libxxf86vm-dev g++ libgl1-mesa-dev libglu1-mesa-dev libraw1394-dev libudev-dev libdrm-dev libglew-dev libopenal-dev libsndfile-dev libfreeimage-dev libcairo2-dev libgtk2.0-dev python-lxml python-argparse libfreetype6-dev portaudio19-dev
apt-get install libgstreamer${GSTREAMER_VERSION}-dev libgstreamer-plugins-base${GSTREAMER_VERSION}-dev  ${GSTREAMER_FFMPEG} gstreamer${GSTREAMER_VERSION}-pulseaudio gstreamer${GSTREAMER_VERSION}-x gstreamer${GSTREAMER_VERSION}-plugins-bad gstreamer${GSTREAMER_VERSION}-alsa gstreamer${GSTREAMER_VERSION}-plugins-base gstreamer${GSTREAMER_VERSION}-plugins-good
exit_code=$?
if [ $exit_code != 0 ]; then
	echo "error installing packages, there could be an error with your internet connection"
	exit $exit_code
fi

cd ..
./compileOF.sh
exit_code=$?
if [ $exit_code != 0 ]; then
  exit $exit_code
fi

./compilePG.sh
exit_code=$?
if [ $exit_code != 0 ]; then
  exit $exit_code
fi

