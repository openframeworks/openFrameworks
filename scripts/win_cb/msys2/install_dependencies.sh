#!/usr/bin/env bash
pacman -Sy
pacman -Su
pacman -S ca-certificates
pacman -Sy --needed make mingw-w64-i686-gcc mingw-w64-i686-glew mingw-w64-i686-freeglut mingw-w64-i686-FreeImage mingw-w64-i686-opencv mingw-w64-i686-assimp mingw-w64-i686-boost mingw-w64-i686-cairo mingw-w64-i686-clang mingw-w64-i686-gdb mingw-w64-i686-zlib  mingw-w64-i686-tools mingw-w64-i686-pkg-config 

# this would install gstreamer which can be used in mingw too
#pacman -Sy mingw-w64-i686-gst-libav mingw-w64-i686-gst-plugins-bad mingw-w64-i686-gst-plugins-base mingw-w64-i686-gst-plugins-good mingw-w64-i686-gst-plugins-ugly mingw-w64-i686-gstreamer

exit_code=$?
if [ $exit_code != 0 ]; then
	echo "error installing packages, there could be an error with your internet connection"
	exit $exit_code
fi
