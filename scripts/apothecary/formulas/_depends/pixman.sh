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

	if [ "$TYPE" == "osx" ] ; then

		# these flags are used to create a fat 32/64 binary with i386->libstdc++, x86_64->libc++
		# see https://gist.github.com/tgfrerer/8e2d973ed0cfdd514de6
		local FAT_LDFLAGS="-arch i386 -arch x86_64 -stdlib=libstdc++ -Xarch_x86_64 -stdlib=libc++"

		./configure LDFLAGS="${FAT_LDFLAGS} " \
				CFLAGS="-O3 ${FAT_LDFLAGS}" \
				--prefix=$BUILD_ROOT_DIR \
				--disable-dependency-tracking \
				--disable-gtk \
				--disable-shared
	else
	./configure LDFLAGS="-arch i386 -arch x86_64" \
				CFLAGS="-O3 -arch i386 -arch x86_64" \
				--prefix=$BUILD_ROOT_DIR \
				--disable-dependency-tracking \
				--disable-gtk \
				--disable-shared
	fi

	
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
