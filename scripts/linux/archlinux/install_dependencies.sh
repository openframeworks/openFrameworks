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

pacman -Sy --needed make pkg-config gcc openal glew freeglut freeimage gstreamer gst-plugins-base gst-plugins-good gst-plugins-bad gst-libav opencv libxcursor assimp boost glfw-x11 uriparser curl pugixml

exit_code=$?
if [ $exit_code != 0 ]; then
	echo "error installing packages, there could be an error with your internet connection"
	exit $exit_code
fi

echo ""
echo ""
echo "NOTE FOR RTAUDIO"
echo "====================="
echo "OpenFramworks requires rtaudio. This package is not in the official repositories and has to be installed via aur. https://aur.archlinux.org/packages/rtaudio/ You can do it manually or use an aur helper like yaourt to do it for you."
echo ""
read -p "Press any key to continue... " -n1 -s

