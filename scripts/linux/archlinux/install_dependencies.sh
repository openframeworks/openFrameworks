#!/bin/bash

if [ $EUID != 0 ]; then
	echo "this script must be run as root"
	echo ""
	echo "usage:"
	echo "su -"
	echo "./install_dependencies.sh"
	exit $exit_code
   exit 1
fi

pacman -Sy --needed make pkg-config gcc openal python-lxml glew freeglut freeimage jack gstreamer gst-plugins-base gst-plugins-good gst-plugins-bad gst-libav portaudio

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
