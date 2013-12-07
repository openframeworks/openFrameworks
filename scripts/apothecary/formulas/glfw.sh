#! /bin/bash
#
# GLFW
# creating windows with OpenGL contexts and managing input and events
# http://www.glfw.org
#
# uses a CMake build system

FORMULA_TYPES=( "osx" "linux" "linux64" "vs" "win_cb" "ios" "android" )

# define the version
VER=3.0.4

# tools for git use
GIT_URL=https://github.com/glfw/glfw.git
GIT_TAG=$VER

# download the source code and unpack it into LIB_NAME
function download() {
	curl -Lk https://github.com/glfw/glfw/archive/$VER.tar.gz -o glfw-$VER.tar.gz
	tar -xf glfw-$VER.tar.gz
	mv glfw-$VER glfw
	rm glfw*.tar.gz
}

# executed inside the lib src dir
function build() {
	
	if [ "$TYPE" == "vs" ] ; then
		cmake -G "Visual Studio $VS_VER"
		vs-build "GLFW.sln"

	else
		echoWarning "TODO: $TYPE build"

	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	# headers
	mkdir -p $1/include
	cp -Rv include/* $1/include

	# libs
	if [ "$TYPE" == "vs" ] ; then
		mkdir -p $1/lib/$TYPE
		cp -v src/Release/glfw3.lib $1/lib/$TYPE/glfw3.lib

	else
		echoWarning "TODO: copy"

	fi
}

# executed inside the lib src dir
function clean() {
	if [ "$TYPE" == "vs" ] ; then
		rm -f *.lib
	else
		echoWarning "TODO: clean"
	fi
}
