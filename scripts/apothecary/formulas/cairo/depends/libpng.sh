#! /bin/bash
#
# the official PNG reference library
# http://libpng.org/pub/png/libpng.html

VER=1.6.2

# download the source code and unpack it into LIB_NAME
function download() {
	curl -LO http://sourceforge.net/projects/libpng/files/libpng16/older-releases/$VER/libpng-$VER.tar.gz
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
