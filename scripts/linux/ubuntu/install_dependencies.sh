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
apt-get install libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev freeglut3-dev libasound2-dev libxmu-dev libxxf86vm-dev g++ libgl1-mesa-dev libglu1-mesa-dev libraw1394-dev libudev-dev libdrm-dev gstreamer0.10-ffmpeg libglew-dev libopenal-dev libsndfile-dev libfreeimage-dev libcairo2-dev libgtk2.0-dev python-lxml python-argparse libfreetype6-dev portaudio19-dev
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

