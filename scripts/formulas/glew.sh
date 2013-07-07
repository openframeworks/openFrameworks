#! /bin/bash
#
# OpenGL Extensions Wrangler
# http://glew.sourceforge.net/

VER=1.9.0

SUPPORTED_TYPES=( "osx" "vs2010" "win_cb" )

# download the source code and unpack it into LIB_NAME
function download() {
	curl -LO http://downloads.sourceforge.net/project/glew/glew/$VER/glew-$VER.tgz
	tar -xf glew-$VER.tgz
	mv glew-$VER glew
}

# executed inside the build dir
function build() {

	if [ "$TYPE" == "osx" ] ; then
		# help from http://stackoverflow.com/questions/12229714/building-glew-for-mac-osx
		
		# 32 bit
		make clean; make glew.lib OPT="-arch i386"
		mv lib/libGLEW.a libGLEW-i386.a

		# 64 bit
		make clean; make glew.lib OPT="-arch x86_64"
		mv lib/libGLEW.a libGLEW-x86_64.a

		# link into universal lib
		lipo -c libGLEW-i386.a libGLEW-x86_64.a -o libGLEW.a

	elif [ "$TYPE" == "vs2010" ] ; then
		#cd build/vc10
		#MSBuild.exe glew_static.vcxproj
		echo "TODO: windows build here"

	elif [ "$TYPE" == "win_cb" ] ; then
		make glew.lib

	else
		echo "build not needed for $TYPE"
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {

	# headers
	mkdir -p $1/include
	cp -Rv include/* $1/include

	# libs
	if [ "$TYPE" == "osx" ] ; then
		mkdir -p $1/lib/$TYPE
		cp -v libGLEW.a $1/lib/$TYPE/glew.a

	elif [ "$TYPE" == "vs2010" ] ; then
		mkdir -p $1/lib/$TYPE
		cp -v lib/glew32s.lib $1/lib/$TYPE

	elif [ "$TYPE" == "win_cb" ] ; then
		mkdir -p $1/lib/$TYPE
		cp -v lib/glew32s.lib $1/lib/$TYPE
	fi
}
