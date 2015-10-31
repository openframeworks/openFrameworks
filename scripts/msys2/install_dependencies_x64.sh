#!/usr/bin/env bash

function usage {
	echo usage: 
    echo ./install_dependencies.sh [--help] [--noconfirm]
    echo --help:
    echo display this message
    echo 
    echo --noconfirm:
    echo install packages without user confirmation
}

#Analyse script arguments
while [[ $# > 0 ]] ; do
	arg=$1
	shift
	if [ "$arg" == "--noconfirm" ]; then
		confirm=--noconfirm
		continue
	fi
	if [ "$arg" == "--help" ]; then
		usage
		exit 1
	fi
	echo Invalid argument : $arg
	usage
	exit 1
done

pacman -Sy
pacman -Su
pacman -S $confirm ca-certificates
pacman -Sy $confirm --needed make mingw-w64-x86_64-gcc mingw-w64-x86_64-glew mingw-w64-x86_64-freeglut mingw-w64-x86_64-FreeImage mingw-w64-x86_64-opencv mingw-w64-x86_64-assimp mingw-w64-x86_64-boost mingw-w64-x86_64-cairo mingw-w64-x86_64-clang mingw-w64-x86_64-gdb mingw-w64-x86_64-zlib  mingw-w64-x86_64-tools mingw-w64-x86_64-pkg-config 
pacman -Sy $confirm --needed mingw-w64-x86_64-poco mingw-w64-x86_64-glfw
# this would install gstreamer which can be used in mingw too
#pacman -Sy mingw-w64-i686-gst-libav mingw-w64-i686-gst-plugins-bad mingw-w64-i686-gst-plugins-base mingw-w64-i686-gst-plugins-good mingw-w64-i686-gst-plugins-ugly mingw-w64-i686-gstreamer

exit_code=$?
if [ $exit_code != 0 ]; then
	echo "error installing packages, there could be an error with your internet connection"
	exit $exit_code
fi
