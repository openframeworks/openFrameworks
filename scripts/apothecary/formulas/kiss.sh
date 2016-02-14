#! /bin/bash
#
# KissFFT
# "Keep It Simple, Stupid" Fast Fourier Transform
# http://sourceforge.net/projects/kissfft/
#
# has a Makefile
FORMULA_TYPES=( "osx" "linux" "linux64" "msys2" "ios" "android" )

# define the version
VER=130
VER_UNDERSCORE=1_3_0

# tools for git use
GIT_URL=
GIT_TAG=v$VER

# download the source code and unpack it into LIB_NAME
function download() {
	curl -LO http://downloads.sourceforge.net/project/kissfft/kissfft/v$VER_UNDERSCORE/kiss_fft$VER.tar.gz
	tar -xf kiss_fft$VER.tar.gz
	mv kiss_fft$VER kiss
	rm kiss_fft$VER.tar.gz
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	if [ "$TYPE" == "linux" ] ; then
		cp -Rv $FORMULA_DIR/Makefile.linux .
	elif [ "$TYPE" == "linux64" ] ; then
		cp -Rv $FORMULA_DIR/Makefile.linux64 .
	fi
}

# executed inside the lib src dir
function build() {

	if [ "$TYPE" == "linux" ] ; then
		make  -j${PARALLEL_MAKE} -f Makefile.linux	
	elif [ "$TYPE" == "linux64" ] ; then
		make  -j${PARALLEL_MAKE} -f Makefile.linux64	
	else
		echo "build not needed for $TYPE"
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {

	# headers
	mkdir -p $1/include
	cp -v kiss_fft.h $1/include
	cp -v tools/kiss_fftr.h $1/include

	# source
	mkdir -p $1/src
	cp -v kiss_fft.c $1/src
	cp -v tools/kiss_fftr.c $1/src
	cp -v _kiss_fft_guts.h $1/src

	# libs
	if [ "$TYPE" == "linux" ] ; then
		mkdir -p $1/lib/linux
		cp -v libkiss.a $1/lib/linux/libkiss.a

	elif [ "$TYPE" == "linux64" ] ; then
		mkdir -p $1/lib/linux64
		cp -v libkiss.a $1/lib/linux64/libkiss.a
	fi

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
