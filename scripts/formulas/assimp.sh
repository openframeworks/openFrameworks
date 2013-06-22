#! /bin/bash
#
# Open Asset Import Library
# https://github.com/assimp/assimp

# download the source code and unpack it into LIB_NAME
function download() {
	git clone https://github.com/assimp/assimp.git
}

# executed inside the build dir
function build() {
	if [ "$TYPE" == "osx" ] ; then
		cmake -G 'Unix Makefiles' -DASSIMP_BUILD_STATIC_LIB=1
		make assimp	
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {

	# headers
	mkdir -p $1/include
	cp -Rv include/assimp/* $1/include

	# libs
	if [ "$TYPE" == "osx" ] ; then
		mkdir -p $1/lib/osx
		cp -Rv lib/libassimp.a $1/lib/osx/assimp.a
	fi
}
