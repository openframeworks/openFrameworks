#!/usr/bin/env bash

if [ $EUID != 0 ]; then
	echo "this script must be run using sudo"
	echo ""
	echo "usage:"
	echo "sudo "$0
	exit $exit_code
    exit 1
fi

if [ "$1" == "-y" ]; then
    FORCE_YES=-y
else
    FORCE_YES=""
fi

ROOT=$(cd $(dirname $0); pwd -P)

function installPackages {
    for pkg in $@; do
        echo "Installing ${pkg}"
        dpkg-query -W -f=' ' ${pkg} 2> /dev/null
        if [ $? -eq 0 ]; then
            echo "Already installed"
        else
            error="$(apt-get install -y --dry-run ${pkg})"
            exit_code=$?
            echo "$error" | grep Remv > /dev/null
            if [ $? -eq 0 ]; then
                apt-get install ${FORCE_YES} ${pkg}
                exit_code=$?
                if [ $exit_code != 0 ]; then
                    echo "error installing ${pkg}, there could be an error with your internet connection"
                    echo "if the error persists, please report an issue in github: http://github.com/openframeworks/openFrameworks/issues"
                    exit $exit_code
                fi
            elif [ $exit_code -eq 0 ]; then
                apt-get -y -qq install ${pkg}
                exit_code=$?
                if [ $exit_code != 0 ]; then
                    echo "error installing ${pkg}, there could be an error with your internet connection"
                    echo "if the error persists, please report an issue in github: http://github.com/openframeworks/openFrameworks/issues"
                    exit $exit_code
                fi
            else
                echo "error installing ${pkg}"
                echo $error
                echo "this seems an error with your distribution repositories but you can also"
                echo "report an issue in the openFrameworks github: http://github.com/openframeworks/openFrameworks/issues"
                exit $exit_code
            fi
        fi
    done
    echo
    echo "All packages were installed correctly"
    echo
}

# Ubuntu derivatives supported: ElementaryOS
# Loads: NAME, VERSION, ID, ID_LIKE, PRETTY_NAME,
# 	VERSION_ID, HOME_URL, SUPPORT_URL, BUG_REPORT_URL, VERSION_CODENAME, UBUNTU_CODENAME
source /etc/os-release

if [ "$ID" = "elementary" ]; then
	# Gets ubuntu base version
	RELEASE=$(lsb_release -r -u)
elif [ "$ID" = "linuxmint" ]; then
	# Gets ubuntu base version
	RELEASE=$(cat /etc/upstream-release/lsb-release | grep DISTRIB_RELEASE | cut -d "=" -f2)
else
	RELEASE=$(lsb_release -r)
fi

MAJOR_VERSION=$(echo $RELEASE | cut -f2 -d: | cut -f1 -d. | sed "s/\t//g")
MINOR_VERSION=$(echo $RELEASE | cut -f2 -d: | cut -f2 -d.)

echo "Running on ubuntu ${MAJOR_VERSION}.${MINOR_VERSION}"

if [ $MAJOR_VERSION -lt 12 ]; then
    echo "Your ubuntu version is too old try using an older version of openFrameworks or updating your system"
    exit 1
elif [ $MAJOR_VERSION -lt 13 ]; then
    add-apt-repository ppa:ubuntu-toolchain-r/test --yes
    add-apt-repository ppa:gstreamer-developers/ppa --yes
    add-apt-repository ppa:boost-latest/ppa --yes
    CXX_VER=-4.9
    BOOST_VER=1.55
elif [[ $MAJOR_VERSION -lt 14 || ($MAJOR_VERSION -eq 14 && $MINOR_VERSION -eq 4) ]]; then
    add-apt-repository ppa:ubuntu-toolchain-r/test --yes
    add-apt-repository ppa:boost-latest/ppa --yes
    CXX_VER=-4.9
    BOOST_VER=1.55
else
    CXX_VER=
    BOOST_VER=
fi

apt-get update

if [ -x "$(command -v /usr/lib/update-notifier/apt-check)" ]; then
	REGULAR_UPDATES=$(/usr/lib/update-notifier/apt-check 2>&1 | cut -d ';' -f 1)
	SECURITY_UPDATES=$(/usr/lib/update-notifier/apt-check 2>&1 | cut -d ';' -f 2)
else
	# apt-check is not installed.
	REGULAR_UPDATES=0
	SECURITY_UPDATES=0
fi

if [ "$1" != "-y" ]; then
    if [ $REGULAR_UPDATES -ne 0 ] || [ $SECURITY_UPDATES -ne 0 ]; then
        read -p "Your system is not updated, that can create problems when installing the OF dependencies. Do you want to update all the packages now? [Y/n]"
        if [[ $REPLY =~ ^[Nn]$ ]]; then
            read -p "Do you want to try installing the OF dependencies anyway?"
            if [[ $REPLY =~ ^[Nn]$ ]]; then
                exit 0
            fi
        else
            apt-get dist-upgrade
        fi
    fi
fi

GSTREAMER_VERSION=0.10
GSTREAMER_FFMPEG=gstreamer${GSTREAMER_VERSION}-ffmpeg

echo "detecting latest gstreamer version"
apt-cache show libgstreamer1.0-dev
exit_code=$?
if [ $exit_code = 0 ]; then
    echo selecting gstreamer 1.0
    GSTREAMER_VERSION=1.0
    GSTREAMER_FFMPEG=gstreamer${GSTREAMER_VERSION}-libav
fi

GTK_VERSION=2.0
echo "detecting latest gtk version"
apt-cache show libgtk-3-dev
exit_code=$?
if [ $exit_code = 0 ]; then
    echo selecting gtk 3
    GTK_VERSION=-3
fi

#checking for distrib tagged xserver-xorg
XTAG=$(dpkg -l |grep xserver-xorg-core|grep ii|awk '{print $2}'|sed "s/xserver-xorg-core//")
if [ ! -z $XTAG ]
then
	read -p " installing OF dependencies with "${XTAG}" packages, confirm Y/N ? " -n 1 -r
	if [[ $REPLY =~ ^[Yy]$ ]]; then
		echo
		echo "installation of OF dependencies with "${XTAG}" packages confirmed"
	else
		XTAG=""
	fi
fi

#check if glfw3 exists
apt-cache show libglfw3-dev
exit_code=$?
if [ $exit_code = 0 ]; then
    GLFW_PKG=libglfw3-dev
else
    echo installing glfw from source
    GLFW_VER=32f38b97d544eb2fd9a568e94e37830106417b51

    # tools for git use
    GLFW_GIT_TAG=$GLFW_VER
    apt-get install -y -qq libxrandr-dev libxinerama-dev libxcursor-dev cmake
    wget https://github.com/glfw/glfw/archive/$GLFW_GIT_TAG.tar.gz -O glfw-$GLFW_GIT_TAG.tar.gz
    tar -xf glfw-$GLFW_GIT_TAG.tar.gz
	mv glfw-$GLFW_GIT_TAG glfw
	rm glfw-$GLFW_GIT_TAG.tar.gz
	cd glfw
    mkdir -p build
    cd build
    cmake .. -DGLFW_BUILD_DOCS=OFF \
	-DGLFW_BUILD_TESTS=OFF \
	-DGLFW_BUILD_EXAMPLES=OFF \
	-DBUILD_SHARED_LIBS=OFF \
	-DCMAKE_BUILD_TYPE=Release
    make clean
    make
    make install
    cd ../..
    rm -rf glfw
    GLFW_PKG=
fi

PACKAGES="curl libjack-jackd2-0 libjack-jackd2-dev freeglut3-dev libasound2-dev libxmu-dev libxxf86vm-dev g++${CXX_VER} libgl1-mesa-dev${XTAG} libglu1-mesa-dev libraw1394-dev libudev-dev libdrm-dev libglew-dev libopenal-dev libsndfile-dev libfreeimage-dev libcairo2-dev libfreetype6-dev libssl-dev libpulse-dev libusb-1.0-0-dev libgtk${GTK_VERSION}-dev  libopencv-dev libassimp-dev librtaudio-dev libboost-filesystem${BOOST_VER}-dev libgstreamer${GSTREAMER_VERSION}-dev libgstreamer-plugins-base${GSTREAMER_VERSION}-dev  ${GSTREAMER_FFMPEG} gstreamer${GSTREAMER_VERSION}-pulseaudio gstreamer${GSTREAMER_VERSION}-x gstreamer${GSTREAMER_VERSION}-plugins-bad gstreamer${GSTREAMER_VERSION}-alsa gstreamer${GSTREAMER_VERSION}-plugins-base gstreamer${GSTREAMER_VERSION}-plugins-good gdb ${GLFW_PKG} liburiparser-dev libcurl4-openssl-dev libpugixml-dev libgconf-2-4 libgtk2.0-0 libpoco-dev"

echo "installing OF dependencies"
echo "OF needs to install the following packages using apt-get:"
echo ${PACKAGES}
if [ "$1" != "-y" ]; then
    read -p "Do you want to continue? [Y/n] "
    if [[ $REPLY =~ ^[Nn]$ ]]; then
        exit 0
    fi

    echo
    echo "Installing..."
    echo
fi
installPackages ${PACKAGES}

if [[ $MAJOR_VERSION -lt 18 ]]; then
    cp $ROOT/../extra/poco_config.mk $ROOT/../../../addons/ofxPoco/addon_config.mk
fi

if [[ $MAJOR_VERSION -lt 14 || ($MAJOR_VERSION -eq 14 && $MINOR_VERSION -eq 4) ]]; then
    echo "detected ubuntu default gcc too old for compatibility with c++11"
	echo "OF needs at least ${CXX_VER} as default compiler, we can install this now"
	echo "or you will need to setup this manually before compiling"
	if [ "$1" != "-y" ]; then
	    read -p "Do you want to set gcc/g++ ${CXX_VER} as default now? [Y/n] "
	    if [[ $REPLY =~ ^[Nn]$ ]]; then
	        exit 0
	    fi
	fi
	echo "setting gcc-${CXX_VER} as default compiler"
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc${CXX_VER} 1 --force
    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++${CXX_VER} 1 --force
fi

# Update addon_config.mk files to use OpenCV 3 or 4 depending on what's installed
addons_dir="$(readlink -f "$ROOT/../../../addons")"
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
