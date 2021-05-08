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
confirm="yes"
while [[ $# > 0 ]] ; do
	arg=$1
	shift
	if [ "$arg" == "--noconfirm" ]; then
		confirm="no"
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

# List of MSYS packages to be installed
msyspackages="make rsync zip unzip wget"

# List of MINGW packages to be installed (without prefix)
mingwPackages="assimp boost cairo curl freeglut FreeImage gcc gdb glew glfw \
			  harfbuzz libsndfile libusb libxml2 mpg123 ntldd-git openal opencv \
			  pkg-config poco tools zlib"

# Build the full list of packages adding prefix to MINGW packages
packages=${msyspackages}
for pkg in ${mingwPackages}; do
	packages="$packages  $MINGW_PACKAGE_PREFIX-$pkg"
done

# Install packages
if [[ "${confirm}" == "yes" ]]; then
	for pkg in ${packages}; do
		pacman -Su --confirm --needed ${pkg}
	done
else
	pacman -Su --noconfirm --needed ${packages}
fi


# this would install gstreamer which can be used in mingw too
#pacman -Su ${MINGW_PACKAGE_PREFIX}-gst-libav ${MINGW_PACKAGE_PREFIX}-gst-plugins-bad ${MINGW_PACKAGE_PREFIX}-gst-plugins-base ${MINGW_PACKAGE_PREFIX}-gst-plugins-good ${MINGW_PACKAGE_PREFIX}-gst-plugins-ugly ${MINGW_PACKAGE_PREFIX}-gstreamer

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
