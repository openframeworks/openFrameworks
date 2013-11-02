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

function prebuild() {
	# copy in build script and CMake toolchains adapted from Assimp
	if [ "$OS" == "osx" ] ; then
		mkdir -p build
		cp -Rv $FORMULA_DIR/ios build
		chmod +x build/ios/build_ios.sh
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
		cmake -G 'Unix Makefiles' $buildOpts -DCMAKE_C_FLAGS="-arch i386" -DCMAKE_CXX_FLAGS="-arch i386" .
		make
		mv libtess2.a libtess2-i386.a

		# 64 bit
		rm -f CMakeCache.txt
		cmake -G 'Unix Makefiles' $buildOpts -DCMAKE_C_FLAGS="-arch x86_64" -DCMAKE_CXX_FLAGS="-arch x86_64" .
		make
		mv libtess2.a libtess2-x86_64.a

		# link into universal lib
		lipo -c libtess2-i386.a libtess2-x86_64.a -o libtess2.a

	elif [ "$TYPE" == "vs" ] ; then
		#cmake -G "Visual Studio" --build build/$TYPE .
		# call MSBuild on the generated sln here
		echoWarning "TODO: vs build"

	elif [ "$TYPE" == "ios" ] ; then
		cd build/ios
		./build_ios.sh # build fat lib

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
		#cp -v libtess2.lib $1/lib/$TYPE/tess2.lib
		echoWarning "TODO: copy vs lib"

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
		echoWarning "TODO: clean vs"
	
	elif [ "$TYPE" == "android" ] ; then
		echoWarning "TODO: clean android"

	else
		make clean
		rm -f CMakeCache.txt *.a *.lib
	fi
}
