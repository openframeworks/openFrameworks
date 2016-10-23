#!/usr/bin/env bash
#
# KissFFT
# "Keep It Simple, Stupid" Fast Fourier Transform
# http://sourceforge.net/projects/kissfft/
#
# has a Makefile
FORMULA_TYPES=( "linux" "linux64" "linuxarmv6l" "linuxarmv7l" )

# define the version
VER=130
VER_UNDERSCORE=1_3_0

# tools for git use
GIT_URL=
GIT_TAG=v$VER

# download the source code and unpack it into LIB_NAME
function download() {
	wget http://downloads.sourceforge.net/project/kissfft/kissfft/v$VER_UNDERSCORE/kiss_fft$VER.tar.gz
	tar -xf kiss_fft$VER.tar.gz
	mv kiss_fft$VER kiss
	rm kiss_fft$VER.tar.gz
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	cp -Rv $FORMULA_DIR/Makefile .
}

# executed inside the lib src dir
function build() {
    make  -j${PARALLEL_MAKE} TARGET_DIR=$TYPE
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	# headers
	mkdir -p $1/include
	cp -v kiss_fft.h $1/include
	cp -v tools/kiss_fftr.h $1/include

	mkdir -p $1/lib/$TYPE
	cp -v lib/$TYPE/libkiss.a $1/lib/$TYPE/libkiss.a

	# copy license file
	rm -rf $1/license # remove any older files if exists
	mkdir -p $1/license
	cp -v COPYING $1/license/
}

# executed inside the lib src dir
function clean() {
	
	if [ "$TYPE" == "linux" -o "$TYPE" == "linux64" ] ; then
		make clean
		rm -f *.a
	fi
}

