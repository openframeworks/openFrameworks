#! /bin/bash
#
# PortAudio
# Portable Cross-platform Audio I/O
# http://www.portaudio.com/
#
# build not currently needed on any platform

FORMULA_TYPES=( "osx" "vs" "msys2" )

# define the version
VER=stable_v19_20110326

# tools for git use
GIT_URL=
GIT_TAG=

# download the source code and unpack it into LIB_NAME
function download() {
	curl -O http://www.portaudio.com/archives/pa_$VER.tgz
	tar -xf pa_$VER.tgz
	rm pa_$VER.tgz
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	: # noop
}

# executed inside the lib src dir
function build() {
	echo "build not needed for $TYPE"
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	
	# headers
	mkdir -p $1/include
	cp -Rv include/* $1/include

	# copy license file
    rm -rf $1/license # remove any older files if exists
    mkdir -p $1/license
    cp -v LICENSE.txt $1/license/
}

# executed inside the lib src dir
function clean() {
	: # noop
}
