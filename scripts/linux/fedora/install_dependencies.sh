#!/bin/bash

## thanks to Claudio for details on packages to install on fedora
if [ $EUID != 0 ]; then
	echo "this script must be run using sudo"
	echo ""
	echo "usage:"
	echo "sudo ./install_dependencies.sh"
	exit $exit_code
   exit 1
fi

yum install freeglut-devel alsa-lib-devel libXmu-devel libXxf86vm-devel gcc-c++ libraw1394-devel gstreamer-devel gstreamer-plugins-base-devel libudev-devel libtheora-devel libvorbis-devel openal-soft-devel libsndfile-devel python-lxml gstreamer-devel glew-devel flac-devel freeimage-devel cairo-devel jack-audio-connection-kit-devel portaudio-devel pulseaudio-libs-devel openssl-devel

exit_code=$?
if [ $exit_code != 0 ]; then
	echo "error installing packages, there could be an error with your internet connection"
	exit $exit_code
fi

