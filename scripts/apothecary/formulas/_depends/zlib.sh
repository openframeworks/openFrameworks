#! /bin/bash
#
# A Massively Spiffy Yet Delicately Unobtrusive Compression Library
# http://zlib.net/

# define the version
VER=1.2.8

# tools for git use
GIT_URL=https://github.com/madler/zlib.git
GIT_TAG=v$VER

# download the source code and unpack it into LIB_NAME
function download() {
	curl -LO http://zlib.net/zlib-$VER.tar.gz
	tar -xf zlib-$VER.tar.gz
	mv zlib-$VER zlib
	rm zlib-$VER.tar.gz
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	: #noop
}

# executed inside the lib src dir
function build() {
	if [ "$TYPE" == "osx" ] ; then
		echo "build not needed for $TYPE"
	elif [ "$TYPE" == "vs" ] ; then
		if [ $ARCH == 32 ] ; then
			cmake -G "Visual Studio $VS_VER"
			vs-build "zlib.sln"
		elif [ $ARCH == 64 ] ; then
			cmake -G "Visual Studio $VS_VER Win64"
			vs-build "zlib.sln" Build "Release|x64"
		fi
	else
		./configure --static \
					--prefix=$BUILD_ROOT_DIR \
		make clean; 
		make
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	if [ "$TYPE" == "osx" ] ; then
		return
	elif [ "$TYPE" == "vs" ] ; then
		if [ $ARCH == 32 ] ; then
			cp -v Release/zlib.dll $1/../../export/$TYPE/Win32/Zlib.dll
		elif [ $ARCH == 64 ] ; then
			cp -v Release/zlib.dll $1/../../export/$TYPE/x64/Zlib.dll
		fi
		
	else
		make install
	fi
}

# executed inside the lib src dir
function clean() {
	if [ "$TYPE" == "vs" ] ; then
		vs-clean "zlib.sln"
	else
		make uninstall
		make clean
	fi
}
