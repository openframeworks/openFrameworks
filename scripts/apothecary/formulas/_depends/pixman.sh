#! /bin/bash
#
# a low-level software library for pixel manipulation
# http://pixman.org/

# define the version
VER=0.30.0

# tools for git use
GIT_URL=http://anongit.freedesktop.org/git/pixman.git
GIT_TAG=pixman-$VER

# download the source code and unpack it into LIB_NAME
function download() {
	curl -LO http://cairographics.org/releases/pixman-$VER.tar.gz
	tar -xf pixman-$VER.tar.gz
	mv pixman-$VER pixman
	rm pixman-$VER.tar.gz
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
				--disable-dependency-tracking \
				--disable-gtk \
				--disable-shared
	
	# only build & install lib, ignore demos/tests
	cd pixman
	make clean
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

# executed inside the lib src dir
function clean() {
	make uninstall
	make clean
}
