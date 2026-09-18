#!/usr/bin/env bash

SCRIPT_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$SCRIPT_DIR" ]]; then SCRIPT_DIR="$PWD"; fi
. "$SCRIPT_DIR/../../dev/downloader.sh" 

if [ $EUID != 0 ]; then
	echo "this script must be run as root"
	echo ""
	echo "usage:"
	echo "sudo ./install_dependencies.sh"
	exit $exit_code
   exit 1
fi

ROOT=$(cd $(dirname $0); pwd -P)

pacman -S --needed make pkg-config gcc openal glew freeglut freeimage freetype2 cairo poco gstreamer gst-plugins-base gst-plugins-good gst-plugins-bad gst-libav raspberrypi-firmware gst-omx-rpi assimp libxcursor opencv assimp glfw-x11 uriparser curl pugixml brotli

downloader http://ci.openframeworks.cc/rtaudio-armv7hf.tar.bz2
tar xjf rtaudio-armv7hf.tar.bz2 -C /

exit_code=$?
if [ $exit_code != 0 ]; then
	echo "error installing packages, there could be an error with your internet connection, or you system might be too out of date (run pacman -Syu before running this script)""
	exit $exit_code
fi

# Update addon_config.mk files to use OpenCV 3, 4 or 5 depending on what's installed
addons_dir="$(readlink -f "$ROOT/../../../addons")"
if pkg-config opencv5 --exists; then
	echo "Updating ofxOpenCV to use openCV5"
	sed -i -E 's/(ADDON_PKG_CONFIG_LIBRARIES =.*)\bopencv[45]?\b/\1opencv5/' "$addons_dir/ofxOpenCv/addon_config.mk"
elif pkg-config opencv4 --exists; then
	echo "Updating ofxOpenCV to use openCV4"
	sed -i -E 's/(ADDON_PKG_CONFIG_LIBRARIES =.*)\bopencv[45]?\b/\1opencv4/' "$addons_dir/ofxOpenCv/addon_config.mk"
else
	echo "Updating ofxOpenCV to use openCV3"
	sed -i -E 's/(ADDON_PKG_CONFIG_LIBRARIES =.*)\bopencv[45]?\b/\1opencv/' "$addons_dir/ofxOpenCv/addon_config.mk"
fi
