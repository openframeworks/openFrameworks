#!/usr/bin/env bash

script_dir="$( dirname "$(readlink -f "$0")" )"

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

NOT_HAS_PATH=$(cmd /c "echo %PATH%" | grep mingw32\\bin > /dev/null; echo $?)
if [ "$NOT_HAS_PATH" -ne "0" ]; then
	cd /
	MSYS2_ROOT=$(pwd)
	MSYS2_ROOT=$(cygpath -w $MSYS2_ROOT)
	setx PATH "%PATH%;${MSYS2_ROOT}mingw32\\bin;${MSYS2_ROOT}usr\\bin\\"
	echo "set path to ${MSYS2_ROOT}mingw32\\bin;${MSYS2_ROOT}usr\\bin\\"
fi

#Install packages
if [ -z ${confirm+x} ]; then
	pacman -S $confirm --needed ca-certificates
	if [ -z ${APPVEYOR+x} ]; then
		pacman -S $confirm --needed wget rsync unzip make ${MINGW_PACKAGE_PREFIX}-gcc ${MINGW_PACKAGE_PREFIX}-ntldd-git
	fi
	pacman -S $confirm --needed ${MINGW_PACKAGE_PREFIX}-glew \
		${MINGW_PACKAGE_PREFIX}-freeglut \
		${MINGW_PACKAGE_PREFIX}-FreeImage \
		${MINGW_PACKAGE_PREFIX}-opencv \
		${MINGW_PACKAGE_PREFIX}-assimp \
		${MINGW_PACKAGE_PREFIX}-boost \
		${MINGW_PACKAGE_PREFIX}-cairo \
		${MINGW_PACKAGE_PREFIX}-clang \
		${MINGW_PACKAGE_PREFIX}-gdb \
		${MINGW_PACKAGE_PREFIX}-zlib \
		${MINGW_PACKAGE_PREFIX}-tools \
		${MINGW_PACKAGE_PREFIX}-pkg-config \
		${MINGW_PACKAGE_PREFIX}-poco \
		${MINGW_PACKAGE_PREFIX}-glfw \
		${MINGW_PACKAGE_PREFIX}-libusb \
		${MINGW_PACKAGE_PREFIX}-harfbuzz \
		${MINGW_PACKAGE_PREFIX}-poco \
		${MINGW_PACKAGE_PREFIX}-curl \
		${MINGW_PACKAGE_PREFIX}-libxml2
else
	pacman -S $confirm --needed ${MINGW_PACKAGE_PREFIX}-harfbuzz
	pacman -S $confirm --needed ca-certificates
	if [ -z ${APPVEYOR+x} ]; then
		pacman -S $confirm --needed wget
		pacman -S $confirm --needed rsync
		pacman -S $confirm --needed unzip
		pacman -S $confirm --needed make
		pacman -S $confirm --needed ${MINGW_PACKAGE_PREFIX}-gcc
		pacman -S $confirm --needed ${MINGW_PACKAGE_PREFIX}-ntldd-git
	fi
	pacman -S $confirm --needed ${MINGW_PACKAGE_PREFIX}-glew
	pacman -S $confirm --needed ${MINGW_PACKAGE_PREFIX}-freeglut
	pacman -S $confirm --needed ${MINGW_PACKAGE_PREFIX}-FreeImage
	pacman -S $confirm --needed ${MINGW_PACKAGE_PREFIX}-opencv
	pacman -S $confirm --needed ${MINGW_PACKAGE_PREFIX}-assimp
	pacman -S $confirm --needed ${MINGW_PACKAGE_PREFIX}-boost
	pacman -S $confirm --needed ${MINGW_PACKAGE_PREFIX}-cairo
	pacman -S $confirm --needed ${MINGW_PACKAGE_PREFIX}-clang
	pacman -S $confirm --needed ${MINGW_PACKAGE_PREFIX}-gdb
	pacman -S $confirm --needed ${MINGW_PACKAGE_PREFIX}-zlib
	pacman -S $confirm --needed ${MINGW_PACKAGE_PREFIX}-tools
	pacman -S $confirm --needed ${MINGW_PACKAGE_PREFIX}-pkg-config
	pacman -S $confirm --needed ${MINGW_PACKAGE_PREFIX}-poco
	pacman -S $confirm --needed ${MINGW_PACKAGE_PREFIX}-glfw
	pacman -S $confirm --needed ${MINGW_PACKAGE_PREFIX}-libusb
	pacman -S $confirm --needed ${MINGW_PACKAGE_PREFIX}-poco
	pacman -S $confirm --needed ${MINGW_PACKAGE_PREFIX}-curl
	pacman -S $confirm --needed ${MINGW_PACKAGE_PREFIX}-libxml2
fi


# this would install gstreamer which can be used in mingw too
#pacman -Sy ${MINGW_PACKAGE_PREFIX}-gst-libav ${MINGW_PACKAGE_PREFIX}-gst-plugins-bad ${MINGW_PACKAGE_PREFIX}-gst-plugins-base ${MINGW_PACKAGE_PREFIX}-gst-plugins-good ${MINGW_PACKAGE_PREFIX}-gst-plugins-ugly ${MINGW_PACKAGE_PREFIX}-gstreamer

exit_code=$?
if [ $exit_code != 0 ]; then
	echo "error installing packages, there could be an error with your internet connection"
	exit $exit_code
fi


# Update addon_config.mk files to use OpenCV 3 or 4 depending on what's installed
addons_dir="$(readlink -f "$script_dir/../../addons")"
$(pkg-config opencv4 --exists)
exit_code=$?
if [ $exit_code != 0 ]; then
	echo "Updating ofxOpenCV to use openCV3"
	sed -i -E 's/ADDON_PKG_CONFIG_LIBRARIES =(.*)opencv4(.*)$/ADDON_PKG_CONFIG_LIBRARIES =\1opencv\2/' "$addons_dir/ofxOpenCv/addon_config.mk"
else
	echo "Updating ofxOpenCV to use openCV4"
	sed -i -E 's/ADDON_PKG_CONFIG_LIBRARIES =(.*)opencv\s/ADDON_PKG_CONFIG_LIBRARIES =\1opencv4 /g' "$addons_dir/ofxOpenCv/addon_config.mk"
	sed -i -E 's/ADDON_PKG_CONFIG_LIBRARIES =(.*)opencv$/ADDON_PKG_CONFIG_LIBRARIES =\1opencv4/g' "$addons_dir/ofxOpenCv/addon_config.mk"
fi
