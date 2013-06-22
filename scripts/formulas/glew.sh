#! /bin/bash
#
# OpenGL Extensions Wrangler
# http://glew.sourceforge.net/

VER=1.9.0

# download the source code and unpack it into LIB_NAME
function download() {
	curl -LO http://downloads.sourceforge.net/project/glew/glew/$VER/glew-$VER.tgz
	tar -xf glew-$VER.tgz
	mv glew-$VER glew
}

# executed inside the build dir
function build() {
	if [ "$TYPE" == "vs2010" ] ; then
		cd build/vc10
		MSBuild.exe glew_static.vcxproj
	else
		make glew.lib
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {

	# headers
	mkdir -p $1/include
	cp -Rv include/* $1/include

	# libs
	if [ "$TYPE" == "osx" ] ; then
		mkdir -p $1/lib/osx
		cp -v lib/libGLEW.a $1/lib/osx/glew.a

	elif [ "$TYPE" == "win_cb" ] ; then
		mkdir -p $1/lib/win_cb
		cp -v lib/glew32s.lib $1/lib/win_cb

	elif [ "$TYPE" == "vs2010" ] ; then
		mkdir -p $1/lib/vs2010
		cp -v lib/glew32s.lib $1/lib/vs2010
	fi
}
