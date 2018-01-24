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

downloader() { if command -v curl 2>/dev/null; then curl -LO --retry 20 -O --progress $1 $2 $3 2> /dev/null; else wget $1 $2 $3 2> /dev/null; fi; }

pacman -Sy --needed make pkg-config gcc openal glew freeglut freeimage freetype2 cairo poco gstreamer gst-plugins-base gst-plugins-good gst-plugins-bad gst-libav raspberrypi-firmware gst-omx-rpi assimp boost libxcursor opencv assimp glfw-x11  uriparser curl pugixml

downloader http://ci.openframeworks.cc/rtaudio-armv7hf.tar.bz2
tar xjf rtaudio-armv7hf.tar.bz2 -C /

exit_code=$?
if [ $exit_code != 0 ]; then
	echo "error installing packages, there could be an error with your internet connection"
	exit $exit_code
fi
