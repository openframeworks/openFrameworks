#! /bin/bash
#
# the official PNG reference library
# http://libpng.org/pub/png/libpng.html

# define the version
MAJOR_VER=16/older-releases
VER=1.6.2

# tools for git use
GIT_URL=http://git.code.sf.net/p/libpng/code
GIT_TAG=v$VER

# download the source code and unpack it into LIB_NAME
function download() {
	curl -LO https://downloads.sourceforge.net/project/libpng/libpng$MAJOR_VER/$VER/libpng-$VER.tar.gz
	tar -xf libpng-$VER.tar.gz
	mv libpng-$VER libpng
	rm libpng-$VER.tar.gz
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	# generate the configure script if it's not there
	if [ ! -f configure ] ; then
		./autogen.sh
	fi
}

# executed inside the lib src dir
function build() {
	
	./configure LDFLAGS="-arch i386 -arch x86_64" \
				CFLAGS="-Os -arch i386 -arch x86_64" \
				--prefix=$BUILD_ROOT_DIR \
				--disable-dependency-tracking

	make clean
	make
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	make install
}

# executed inside the lib src dir
function clean() {
	make uninstall
	make clean
}
