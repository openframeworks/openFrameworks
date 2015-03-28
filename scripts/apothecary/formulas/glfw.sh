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
	# curl -Lk https://github.com/glfw/glfw/archive/$GIT_TAG.tar.gz -o glfw-$GIT_TAG.tar.gz
	# temporary fix until https://github.com/openframeworks/openFrameworks/issues/3621 is resolved.
	GIT_TAG=feature-keysUnicode
	curl -Lk https://github.com/arturoc/glfw/archive/feature-keysUnicode.tar.gz -o glfw-$GIT_TAG.tar.gz
	# end fix
	tar -xf glfw-$GIT_TAG.tar.gz
	mv glfw-$GIT_TAG glfw
	rm glfw*.tar.gz
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	: # noop
}

# executed inside the lib src dir
function build() {
	rm -f CMakeCache.txt

	if [ "$TYPE" == "vs" ] ; then
		cmake -G "Visual Studio $VS_VER"
		vs-build "GLFW.sln"
	else
		# *nix build system

		# NOTE: DGLFW_BUILD_UNIVERSAL will be ignored on non OSX systems.
		# Rather than creating a seperate if/then branch, we just let cmake
		# ignore the warning on non OSX platforms.
		cmake . -DCMAKE_INSTALL_PREFIX=$BUILD_ROOT_DIR \
				-DGLFW_BUILD_DOCS=OFF \
				-DGLFW_BUILD_TESTS=OFF \
				-DGLFW_BUILD_EXAMPLES=OFF \
				-DBUILD_SHARED_LIBS=OFF \
				-DGLFW_BUILD_UNIVERSAL=ON 

 		make clean
 		make
 		make install
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	# prepare headers directory if needed
	mkdir -p $1/include/GLFW

	# prepare libs directory if needed
	mkdir -p $1/lib/$TYPE

	if [ "$TYPE" == "vs" ] ; then
	    cp -Rv include/* $1/include
		cp -v src/Release/glfw3.lib $1/lib/$TYPE/glfw3.lib
	else
		# Standard *nix style copy.
		# copy headers
		cp -Rv $BUILD_ROOT_DIR/include/GLFW/* $1/include/GLFW/
		# copy lib
		cp -Rv $BUILD_ROOT_DIR/lib/libglfw3.a $1/lib/$TYPE/
	fi

	# copy license file
    cp -v COPYING.txt $1/
}

# executed inside the lib src dir
function clean() {
	if [ "$TYPE" == "vs" ] ; then
		rm -f *.lib
	else
		make clean
	fi
}
