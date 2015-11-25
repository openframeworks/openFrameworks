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
VER=44459

# tools for git use
GIT_URL=
GIT_TAG=

# download the source code and unpack it into LIB_NAME
function download() {

	mkdir -p "fmodex"


	set -e

	if [ "$TYPE" == "osx" ]; then
			# download drive image for fmodex from fmod.org

			mkdir -p "fmodex/docs"

			echo "Manually download the fmod dmg from the following link. It requires a login."
			echo http://www.fmod.org/download/fmodex/api/Mac/fmodapi${VER}mac-installer.dmg
			echo "Once you have downloaded it. Copy it and place the dmg in apothecary/build/"
			#curl -O http://www.fmod.org/download/fmodex/api/Mac/fmodapi${VER}mac-installer.dmg
			# mount dmg
			hdiutil attach fmodapi${VER}mac-installer.dmg -quiet
			# copy contents into staging folder
			cp -R "/Volumes/FMOD Programmers API Mac/FMOD Programmers API/api/" "fmodex"
			cp -R "/Volumes/FMOD Programmers API Mac/FMOD Programmers API/documentation/" "fmodex/docs/"
			# remove installer dmg
			rm fmodapi${VER}mac-installer.dmg
			# unmount drive image
			hdiutil detach "/Volumes/FMOD Programmers API Mac/" -quiet 

			
	fi

	echo "downloaded fmodex"
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	: # noop
	# mount install
}

# executed inside the lib src dir
function build() {

	if [ "$TYPE" == "osx" ]; then

		cd lib
		install_name_tool -id "@executable_name/libfmodex.dylib" libfmodex.dylib
		cd ../

	else 
		echo "build not needed for $TYPE"

	fi

	
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	
	if [ "$TYPE" == "osx" ]; then
		# headers
		mkdir -p $1/include
		cp -Rv inc/* $1/include
		# library files
		cp -Rv lib/libfmodex.dylib $1/lib/$TYPE/

		cp -Rv lib/libfmodex.dylib $1/../../export/$TYPE/libs/
	fi

	# copy license files
	rm -rf $1/license # remove any older files if exists
	mkdir -p $1/license
	cp -v docs/LICENSE.TXT $1/license/

}

# executed inside the lib src dir
function clean() {
	: # noop
}
