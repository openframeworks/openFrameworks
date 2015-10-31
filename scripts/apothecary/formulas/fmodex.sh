#! /bin/bash
#
# FmodEX 
# http://www.portaudio.com/
#
# This is not a build script, as fmodex is linked as a dynamic library.
# FmodEX is downloaded as a binary from the fmod.org website and copied 
# into the openFrameworks library directory.

FORMULA_TYPES=( "osx" "vs" "msys2" )

# define the version
VER=44221

# tools for git use
GIT_URL=
GIT_TAG=

# download the source code and unpack it into LIB_NAME
function download() {

	mkdir -p "fmodex"

	case $TYPE in
		"osx" )
			# download drive image for fmodex from fmod.org
			curl -O http://www.fmod.org/download/fmodex/api/Mac/fmodapi${VER}mac-installer.dmg
			# mount dmg
			hdiutil attach fmodapi${VER}mac-installer.dmg -quiet
			# copy contents into staging folder
			cp -R "/Volumes/FMOD Programmers API Mac/FMOD Programmers API/api/" "fmodex"
			# remove installer dmg
			rm fmodapi${VER}mac-installer.dmg
			# unmount drive image
			hdiutil detach "/Volumes/FMOD Programmers API Mac/" -quiet 
			;;
	esac

	echo "downloaded fmodex"
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	: # noop
	# mount install
}

# executed inside the lib src dir
function build() {
	echo "build not needed for $TYPE"
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	
	if [ "$TYPE" == "osx" ] ; then
		# headers
		mkdir -p $1/include
		cp -Rv inc/* $1/include
		# library files
		cp -Rv lib/libfmodex.dylib $1/lib/$TYPE/
	fi

	echoWarning "TODO: Copy License"
}

# executed inside the lib src dir
function clean() {
	: # noop
}
