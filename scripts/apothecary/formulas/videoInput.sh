#! /bin/bash
#
# videoInput
# A video capture library for windows
# https://github.com/ofTheo/videoInput
#
# Visual Studio & Code Blocks projects are provided

FORMULA_TYPES=( "vs" "win_cb" )

# download the source code and unpack it into LIB_NAME
function download() {
	git clone https://github.com/ofTheo/videoInput.git -b update2013
}

# executed inside the lib src dir
function build() {

	cd videoInputSrcAndDemos

	if [ "$TYPE" == "vs" ] ; then
		cd VS-videoInputcompileAsLib
		# use MSBuild.exe on videoInput.sln
		echoWarning "TODO: vs build"

	elif [ "$TYPE" == "win_cb" ] ; then
		cd CodeBlocks-compileAsLib/videoInputLib
		# run CodeBlocks on videoInputLib.cpb somehow
		echoWarning "TODO: win_cb build"
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	echoWarning "TODO: copy"
}

# executed inside the lib src dir
function clean() {
	
	if [ "$TYPE" == "vs" ] ; then
		echoWarning "TODO: clean vs"
	
	elif [ "$TYPE" == "win_cb" ] ; then
		echoWarning "TODO: clean win_cb"
	fi
}
