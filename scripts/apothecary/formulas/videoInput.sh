#! /bin/bash
#
# videoInput
# A video capture library for windows
# https://github.com/ofTheo/videoInput
#
# Visual Studio & Code Blocks projects are provided

FORMULA_TYPES=( "vs" "win_cb" )

# define the version
VER=master

# tools for git use
GIT_URL=https://github.com/ofTheo/videoInput.git
GIT_TAG=$VER

# download the source code and unpack it into LIB_NAME
function download() {
	git clone https://github.com/ofTheo/videoInput.git
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
		vs-build "videoInput.sln" Build Release
		vs-build "videoInput.sln" Build Debug

	elif [ "$TYPE" == "win_cb" ] ; then
		cd CodeBlocks-compileAsLib/videoInputLib
		# run CodeBlocks on videoInputLib.cpb somehow
		echoWarning "TODO: win_cb build"
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {

	# headers
	mkdir -p $1/include
	cp -Rv videoInputSrcAndDemos/libs/videoInput/videoInput.h $1/include

	if [ "$TYPE" == "vs" ] ; then
		mkdir -p $1/lib/$TYPE
		cp -v videoInputSrcAndDemos/VS-videoInputcompileAsLib/Debug/videoInputD.lib $1/lib/$TYPE/videoInputD.lib
		cp -v videoInputSrcAndDemos/VS-videoInputcompileAsLib/Release/videoInput.lib $1/lib/$TYPE/videoInput.lib

	else
		echoWarning "TODO: $TYPE copy"
	fi

	echoWarning "TODO: License Copy"
}

# executed inside the lib src dir
function clean() {
	
	if [ "$TYPE" == "vs" ] ; then
		echoWarning "TODO: clean vs"
	elif [ "$TYPE" == "win_cb" ] ; then
		echoWarning "TODO: clean win_cb"
	fi
}
