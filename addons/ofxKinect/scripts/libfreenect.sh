#! /bin/bash


SRC=libfreenect

GIT_URL=https://github.com/wizgrav/libfreenect
GIT_TAG=master

# download the source code and unpack it into LIB_NAME
function download() {
	# get latest source
	
	curl -Lk $GIT_URL/archive/master.tar.gz -o  libfreenect-$GIT_TAG.tar.gz
	tar -xf libfreenect-$GIT_TAG.tar.gz
	mv libfreenect-$GIT_TAG libfreenect
	rm libfreenect-$GIT_TAG.tar.gz
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	echo "The current working directory: $PWD"
	# remove uneeded makefiles
	rm src/CMakeLists.txt
	rm src/libfreenect.pc.in

	# we dont need freenect audio
	rm include/libfreenect-audio.h
	rm src/audio.c
	rm src/loader.c

	# no python either
	rm src/fwfetcher.py
}

# executed inside the lib src dir
function build() {
	echo "Nothing to build for freenect"
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	# make folders
	mkdir -p $1/include
	mkdir -p $1/src
	mkdir -p $1/platform

	# copy licenses, etc
	rm -rf $1/license # remove any older files if exists
    mkdir -p $1/license
	
	cp -v APACHE20 $1/license/
	cp -v CONTRIB $1/license/
	cp -v GPL2 $1/license/

	# copy headers
	cp -Rv include/* $1/include

	# copy sources
	cp -Rv src/* $1/src

	# copy platform specfic stuff
	cp -Rv platform/linux $1/platform
	cp -Rv platform/windows $1/platform
}

# executed inside the lib src dir
function clean() {
	# cleanup
	rm -rf $SRC
}
