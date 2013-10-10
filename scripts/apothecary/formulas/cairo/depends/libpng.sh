#! /bin/bash
#
# the official PNG reference library
# http://libpng.org/pub/png/libpng.html

# define the version
MAJOR_VER=16
VER=1.6.2

# tools for git use
GIT_URL=
GIT_TAG=libpng-$VER

# download the source code and unpack it into LIB_NAME
function download() {
	curl -LO https://downloads.sourceforge.net/project/libpng/libpng$MAJORVER/$VER/libpng-$VER.tar.gz
	tar -xf libpng-$VER.tar.gz
	mv libpng-$VER libpng
	rm libpng-$VER.tar.gz
}

# executed inside the lib src dir
function build() {
	./configure --prefix=$BUILD_DIR --disable-dependency-tracking
	make clean; make
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
