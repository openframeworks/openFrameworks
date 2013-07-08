#! /bin/bash
#
# a low-level software library for pixel manipulation
# http://pixman.org/

VER=0.30.0

# download the source code and unpack it into LIB_NAME
function download() {
	curl -LO http://cairographics.org/releases/pixman-$VER.tar.gz
	tar -xf pixman-$VER.tar.gz
	mv pixman-$VER pixman
	rm pixman-$VER.tar.gz
	exit 0
}

# executed inside the build dir
function build() {

	./configure --prefix=$BUILD_DIR --disable-dependency-tracking --disable-gtk --disable-shared
	
	# only build & install lib, ignore demos/tests
	cd pixman
	make
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	
	# lib
	cd pixman
	make install
	
	# pkg-config info
	cd ../
	make install-pkgconfigDATA
}
