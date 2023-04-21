#!/bin/bash

# This script copy the needed files to compile every example via Make. 

if [ "$PLATFORM" == "" ]; then
	OS=$(uname)
	if [ "$OS" == "Linux" ]; then
		PLATFORM="linux"
	elif [ "$OS" == "Darwin" ]; then
		PLATFORM="osx"
	elif [ "${OS:0:5}" == "MINGW" ]; then
		PLATFORM="msys2"
	else
		# otherwise we are on windows and will download 32bit msys2
		PLATFORM="msys2"
	fi
fi

# ARCH=$(uname -m)

# PLATFORM=linux
# ARCH=aarch64

if [ "$ARCH" == "" ]; then
	if [ "$PLATFORM" == "linux" ]; then
		ARCH=$(uname -m)
		if [ "$ARCH" == "x86_64" ]; then
			GCC_VERSION=$(gcc -dumpversion | cut -f1 -d.)
			if [ $GCC_VERSION -eq 4 ]; then
				ARCH=64
			elif [ $GCC_VERSION -eq 5 ]; then
				ARCH=64
			else
				ARCH=64
			fi
		elif [ "$ARCH" == "armv7l" ]; then
			# Check for Raspberry Pi
			if [ -f /opt/vc/include/bcm_host.h ]; then
				ARCH=armv6l
			fi
		elif [ "$ARCH" == "i686" ] || [ "$ARCH" == "i386" ]; then
			cat << EOF
32bit linux is not officially supported anymore but compiling
the libraries using the build script in apothecary/scripts
should compile all the dependencies without problem
EOF
			exit 1
		fi
	elif [ "$PLATFORM" == "msys2" ]; then
		if [ "$MSYSTEM" == "MINGW64" ]; then
			ARCH=mingw64
		elif [ "$MSYSTEM" == "MINGW32" ]; then
			ARCH=mingw32
		elif [ "$MSYSTEM" == "UCRT64" ]; then
			ARCH=ucrt64
		elif [ "$MSYSTEM" == "CLANG64" ]; then
			ARCH=clang64
		fi
	fi
fi

# if [ "$PLATFORM" == "linux" ] && [ "$ARCH" == "64" ]; then
# 	ARCH=64
# fi

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PARENT_DIR="$(dirname "$DIR")"
MAKEFILE_PATH=$PARENT_DIR/templates/${PLATFORM}${ARCH}
echo "Makefile Path ${MAKEFILE_PATH}"

cd ${DIR}/../../examples

# echo "${MAKEFILE_PATH}"
if [ -d "${MAKEFILE_PATH}" ];
then
	for category in $(ls -1d *)
	do
		if [ $category != "addons" ] && [ $category != "ios" ] && [ $category != "android" ] && [[ -d $category ]]; then
			echo examples/${category}
			echo 
			cd ${category}
			for example in $(ls -1d *)
			do
				if [ -d ${example} ];
				then
					echo "	$example"
					cp ${MAKEFILE_PATH}/{Makefile,config.make} ${example}
					# echo $(pwd)
					# echo cp ${MAKEFILE_PATH}/Makefile ${category}/${example}/
					# echo cp ${MAKEFILE_PATH}/config.make ${category}/${example}/
				fi
			done
			cd ..
			echo 
		fi
	done
fi