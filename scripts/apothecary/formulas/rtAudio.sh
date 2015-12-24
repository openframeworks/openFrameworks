#! /bin/bash
#
# RtAudio
# RealTime Audio input/output across Linux, Macintosh OS-X and Windows
# http://www.music.mcgill.ca/~gary/rtaudio/
#
# uses an autotools build system

FORMULA_TYPES=( "osx" "linux" "linux64" "vs" "msys2" )

#FORMULA_DEPENDS=( "pkg-config" )

# tell apothecary we want to manually call the dependency commands
# as we set some env vars for osx the depends need to know about
#FORMULA_DEPENDS_MANUAL=1

# define the version
VER=4.1.1

# tools for git use
GIT_URL=https://github.com/thestk/rtaudio
GIT_TAG=master

# download the source code and unpack it into LIB_NAME
function download() {
	#curl -O http://www.music.mcgill.ca/~gary/rtaudio/release/rtaudio-$VER.tar.gz
	curl -Lk $GIT_URL/archive/master.tar.gz -o  rtAudio-$GIT_TAG.tar.gz
	tar -xf rtaudio-$GIT_TAG.tar.gz
	mv rtaudio-$GIT_TAG rtAudio
	rm rtaudio-$GIT_TAG.tar.gz
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	: # noop
}

# executed inside the lib src dir
function build() {

	# The ./configure / MAKEFILE sequence is broken for OSX, making it 
	# impossible to create universal libs in one pass.  As a result, we compile
	# the project manually according to the author's page:
	# https://www.music.mcgill.ca/~gary/rtaudio/compiling.html

	if [ "$TYPE" == "osx" ] ; then

		# Compile the program
		/usr/bin/g++ -O2 \
					 -Wall \
					 -fPIC \
					 -stdlib=libc++ \
					 -arch i386 \
					 -Iinclude \
					 -DHAVE_GETTIMEOFDAY \
					 -D__MACOSX_CORE__ \
					 -c RtAudio.cpp \
					 -o RtAudio.o

		/usr/bin/ar ruv librtaudio.a RtAudio.o
		/usr/bin/ranlib librtaudio.a

		/usr/bin/g++ -O2 \
					 -Wall \
					 -fPIC \
					 -stdlib=libc++ \
					 -arch x86_64 \
					 -Iinclude \
					 -DHAVE_GETTIMEOFDAY \
					 -D__MACOSX_CORE__ \
					 -c RtAudio.cpp \
					 -o RtAudio.o

		/usr/bin/ar ruv librtaudio-x86_64.a RtAudio.o
		/usr/bin/ranlib librtaudio-x86_64.a

		lipo -c librtaudio.a librtaudio-x86_64.a -o librtaudio.a

	elif [ "$TYPE" == "vs" ] ; then
		local API="--with-wasapi --with-ds" # asio as well?
		if [ $ARCH == 32 ] ; then
			mkdir -p build_vs_32
			cd build_vs_32
			cmake .. -G "Visual Studio $VS_VER"  -DAUDIO_WINDOWS_WASAPI=ON -DAUDIO_WINDOWS_DS=ON -DAUDIO_WINDOWS_ASIO=ON
			vs-build "rtaudio_static.vcxproj"
			vs-build "rtaudio_static.vcxproj" Build "Debug"
		elif [ $ARCH == 64 ] ; then
			mkdir -p build_vs_64
			cd build_vs_64
			cmake .. -G "Visual Studio $VS_VER Win64" -DAUDIO_WINDOWS_WASAPI=ON -DAUDIO_WINDOWS_DS=ON -DAUDIO_WINDOWS_ASIO=ON
			vs-build "rtaudio_static.vcxproj" Build "Release|x64"
			vs-build "rtaudio_static.vcxproj" Build "Debug|x64"
		fi

	elif [ "$TYPE" == "msys2" ] ; then
		local API="--with-wasapi --with-ds" # asio as well?
		mkdir -p build
		cd build
		cmake .. -G "Unix Makefiles"  -DAUDIO_WINDOWS_WASAPI=ON -DAUDIO_WINDOWS_DS=ON -DAUDIO_WINDOWS_ASIO=ON -DCMAKE_C_COMPILER=/mingw32/bin/gcc.exe -DCMAKE_CXX_COMPILER=/mingw32/bin/g++.exe -DBUILD_TESTING=OFF
		make
	fi

	# clean up env vars
	# unset PKG_CONFIG PKG_CONFIG_PATH
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {

	# headers
	mkdir -p $1/include
	cp -v RtAudio.h $1/include
	#cp -v RtError.h $1/include #no longer a part of rtAudio

	# libs
	mkdir -p $1/lib/$TYPE
	if [ "$TYPE" == "vs" ] ; then
		if [ $ARCH == 32 ] ; then
			mkdir -p $1/lib/$TYPE/Win32
			cp -v build_vs_32/Release/rtaudio_static.lib $1/lib/$TYPE/Win32/rtAudio.lib
			cp -v build_vs_32/Debug/rtaudio_static.lib $1/lib/$TYPE/Win32/rtAudioD.lib
		elif [ $ARCH == 64 ] ; then
			mkdir -p $1/lib/$TYPE/x64
			cp -v build_vs_64/Release/rtaudio_static.lib $1/lib/$TYPE/x64/rtAudio.lib
			cp -v build_vs_64/Debug/rtaudio_static.lib $1/lib/$TYPE/x64/rtAudioD.lib
		fi
		

	elif [ "$TYPE" == "msys2" ] ; then
		cp -v build/librtaudio_static.a $1/lib/$TYPE/librtaudio.a
	
	else
		cp -v librtaudio.a $1/lib/$TYPE/rtaudio.a
	fi

	# copy license file
	rm -rf $1/license # remove any older files if exists
	mkdir -p $1/license
	cp -v readme $1/license/
}

# executed inside the lib src dir
function clean() {
	
	if [ "$TYPE" == "vs" ] ; then
		vs-clean "rtaudio_static.vcxproj"
	else
		make clean
	fi

	# manually clean dependencies
	#apothecaryDependencies clean
}
