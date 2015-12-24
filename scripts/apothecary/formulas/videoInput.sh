#! /bin/bash
#
# videoInput
# A video capture library for windows
# https://github.com/ofTheo/videoInput
#
# Visual Studio & Code Blocks projects are provided

FORMULA_TYPES=( "vs" "msys2" )

# define the version
VER=master

# tools for git use
GIT_URL=https://github.com/arturoc/videoInput.git
GIT_TAG=$VER

# download the source code and unpack it into LIB_NAME
function download() {
	git clone ${GIT_URL}
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	: # noop
}

# executed inside the lib src dir
function build() {

	cd videoInputSrcAndDemos

	if [ "$TYPE" == "vs" ] ; then
		cd VS-videoInputcompileAsLib
		if [ $ARCH == 32 ] ; then
			vs-build "videoInput.sln"
			vs-build "videoInput.sln" Build "Debug"
		elif [ $ARCH == 64 ] ; then
			vs-build "videoInput.sln" Build "Release|x64"
			vs-build "videoInput.sln" Build "Debug|x64"
		fi
	elif [ "$TYPE" == "msys2" ] ; then
		cd msys2
		make 
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {

	# headers
	mkdir -p $1/include
	cp -Rv videoInputSrcAndDemos/libs/videoInput/videoInput.h $1/include

	if [ "$TYPE" == "vs" ] ; then
		if [ $ARCH == 32 ] ; then
			mkdir -p $1/lib/$TYPE/Win32
			cp -v videoInputSrcAndDemos/VS-videoInputcompileAsLib/Debug/videoInputD.lib $1/lib/$TYPE/Win32/videoInputD.lib
			cp -v videoInputSrcAndDemos/VS-videoInputcompileAsLib/Release/videoInput.lib $1/lib/$TYPE/Win32/videoInput.lib
		elif [ $ARCH == 64 ] ; then
			mkdir -p $1/lib/$TYPE/x64
			cp -v videoInputSrcAndDemos/VS-videoInputcompileAsLib/x64/Debug/videoInputD.lib $1/lib/$TYPE/x64/videoInputD.lib
			cp -v videoInputSrcAndDemos/VS-videoInputcompileAsLib/x64/Release/videoInput.lib $1/lib/$TYPE/x64/videoInput.lib
		fi
		

	else
		mkdir -p $1/lib/$TYPE
		cp -v compiledLib/msys2/libvideoinput.a $1/lib/$TYPE/
	fi

	echoWarning "TODO: License Copy"
}

# executed inside the lib src dir
function clean() {
	
	if [ "$TYPE" == "vs" ] ; then
		cd videoInputSrcAndDemos/VS-videoInputcompileAsLib
		vs-clean "videoInput.sln"
	elif [ "$TYPE" == "msys2" ] ; then
		cd videoInputSrcAndDemos/msys2
		make clean
	fi
}
