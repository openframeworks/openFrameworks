#! /bin/bash
#
# tess2
# Game and tools oriented refactored version of GLU tesselator
# https://code.google.com/p/libtess2/
#
# has no build system, only an old Xcode project
# we follow the Homebrew approach which is to use CMake via a custom CMakeLists.txt
# on ios, use some build scripts adapted from the Assimp project

# define the version
VER=1.0

# tools for git use
GIT_URL=https://github.com/memononen/libtess2.git
GIT_TAG=master

# download the source code and unpack it into LIB_NAME
function download() {
	curl -LO https://libtess2.googlecode.com/files/libtess2-$VER.zip
	unzip -oq libtess2-$VER.zip
	mv libtess2 tess2
	rm libtess2-$VER.zip
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	
	# check if the patch was applied, if not then patch
	if patch -p1 -u -N --dry-run --silent < $FORMULA_DIR/tess2.patch 2>/dev/null ; then
		patch -p1 -u -N  < $FORMULA_DIR/tess2.patch
	fi

	# copy in build script and CMake toolchains adapted from Assimp
	if [ "$OS" == "osx" ] ; then
		mkdir -p build
		cp -Rv $FORMULA_DIR/ios build
		chmod +x build/ios/build.sh
	fi
}

# executed inside the lib src dir
function build() {
	
	# use CMake for the build using CMakeLists.txt from HomeBrew since the original source doesn't have one
	# see : https://github.com/mxcl/homebrew/pull/19634/files
	cp -v $FORMULA_DIR/CMakeLists.txt .
	
	if [ "$TYPE" == "osx" ] ; then

		local buildOpts="--build build/$TYPE"

		# 32 bit
		rm -f CMakeCache.txt
		cmake -G 'Unix Makefiles' \
			$buildOpts \
			-DCMAKE_C_FLAGS="-arch i386 -arch x86_64" \
			-DCMAKE_CXX_FLAGS="-arch i386 -arch x86_64" \
			.
		make clean 
		make

	elif [ "$TYPE" == "vs" ] ; then
		cmake -G "Visual Studio $VS_VER"
		vs-build "tess2.sln"

	elif [ "$TYPE" == "ios" ] ; then
		cd build/ios
		./build.sh # build fat lib

	elif [ "$TYPE" == "android" ] ; then
		echoWarning "TODO: android build"

	else 
		cmake -G "Unix Makefiles" --build build/$TYPE ../../
		make
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	
	# headers
	mkdir -p $1/include
	cp -v Include/* $1/include

	# source
	mkdir -p $1/Source
	cp -v Source/* $1/Source

	# lib
	mkdir -p $1/lib/$TYPE
	if [ "$TYPE" == "vs" ] ; then 
		cp -v Release/tess2.lib $1/lib/$TYPE/tess2.lib

	elif [ "$TYPE" == "ios" ] ; then 
		cp -v libtess2.a $1/lib/$TYPE/tess2.a

	elif [ "$TYPE" == "android" ] ; then
		echoWarning "TODO: copy android lib"

	else
		cp -v libtess2.a $1/lib/$TYPE/tess2.a
	fi
}

# executed inside the lib src dir
function clean() {

	if [ "$TYPE" == "vs" ] ; then
		rm -f CMakeCache.txt *.lib
	
	elif [ "$TYPE" == "android" ] ; then
		echoWarning "TODO: clean android"

	else
		make clean
		rm -f CMakeCache.txt *.a *.lib
	fi
}
