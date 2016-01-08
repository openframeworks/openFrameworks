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


arch=i686

pacman -Sy
pacman -Su
pacman -S $confirm ca-certificates
pacman -Sy $confirm --needed make mingw-w64-$arch-gcc mingw-w64-$arch-glew mingw-w64-$arch-freeglut mingw-w64-$arch-FreeImage mingw-w64-$arch-opencv mingw-w64-$arch-assimp mingw-w64-$arch-boost mingw-w64-$arch-cairo mingw-w64-$arch-clang mingw-w64-$arch-gdb mingw-w64-$arch-zlib  mingw-w64-$arch-tools mingw-w64-$arch-pkg-config mingw-w64-$arch-poco mingw-w64-$arch-glfw

# this would install gstreamer which can be used in mingw too
#pacman -Sy mingw-w64-$arch-gst-libav mingw-w64-$arch-gst-plugins-bad mingw-w64-$arch-gst-plugins-base mingw-w64-$arch-gst-plugins-good mingw-w64-$arch-gst-plugins-ugly mingw-w64-$arch-gstreamer

exit_code=$?
if [ $exit_code != 0 ]; then
	echo "error installing packages, there could be an error with your internet connection"
	exit $exit_code
fi
