#! /bin/bash

VER=1.2.6.0

SRC=libusb-win32-bin-$VER

###
FORMULA_TYPES=( "vs" "win-cb" )

# download the source code and unpack it into LIB_NAME
function download() {
	### Debug
	# get source
	curl -LO http://downloads.sourceforge.net/project/libusb-win32/libusb-win32-releases/$VER/libusb-win32-bin-debug-$VER.zip
	unzip -oq libusb-win32-bin-debug-$VER.zip

	mv "libusb-win32-bin-$VER" libusb-debug
	
	# get source
	curl -LO http://downloads.sourceforge.net/project/libusb-win32/libusb-win32-releases/$VER/$SRC.zip
	unzip -oq $SRC.zip

	mv "libusb-win32-bin-$VER" libusb
	rm "libusb-win32-bin-$VER.zip"
	rm "libusb-win32-bin-debug-$VER.zip"
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	echo "Nothing to prepare for libusb"
}

# executed inside the lib src dir
function build() {
	echo "Nothing to build for libusb"
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	cd ..
	
	# make folders
	mkdir -p $1/include
	mkdir -p $1/lib/vs/Win32
	mkdir -p $1/lib/vs/x64
	mkdir -p $1/lib/msys2

	# copy libs
	cp -v libusb-debug/lib/msvc/libusb.lib $1/lib/vs/Win32/libusbd.lib
	cp -v libusb-debug/lib/msvc_x64/libusb.lib $1/lib/vs/x64/libusbd.lib
	cp -v libusb-debug/lib/msvc/libusb.lib $1/lib/msys2/libusbd.lib

	### Release

	# copy licenses, etc
	rm -rf $1/license # remove any older files if exists
    mkdir -p $1/license
	
	cp -v libusb/AUTHORS.txt $1/license/
	cp -v libusb/COPYING_LGPL.txt $1/license/
	cp -v libusb/README.txt $1/license/

	# copy headers
	cp -Rv libusb/include/* $1/include

	# copy libs
	cp -v libusb/lib/msvc/libusb.lib $1/lib/vs/Win32
	cp -v libusb/lib/msvc_x64/libusb.lib $1/lib/vs/x64
	cp -v libusb/lib/msvc/libusb.lib $1/lib/msys2
}

# executed inside the lib src dir
function clean() {

	# cleanup
	rm libusb
	rm libusb-debug	
}