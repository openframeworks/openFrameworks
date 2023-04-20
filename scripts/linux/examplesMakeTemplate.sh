#!/bin/bash

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

ARCH=$(uname -m)

# PLATFORM=linux
# ARCH=aarch64

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PARENT_DIR="$(dirname "$DIR")"
MAKEFILE_PATH=$PARENT_DIR/templates/${PLATFORM}${ARCH}
cd ${DIR}/../../examples

# echo "${MAKEFILE_PATH}"
if [ -d "${MAKEFILE_PATH}" ];
then
	for category in $(ls -1d *)
	do
		if [ $category != "addons" ] && [ $category != "ios" ] && [ $category != "android" ] && [[ -d $category ]]; then
			echo examples/${category}
			echo 
			for example in $(ls -1d *)
			do
				echo "	$example"
				# echo cp ${MAKEFILE_PATH}/{Makefile,config.make} ${category}/${example}
			done
			echo 
		fi
	done
fi