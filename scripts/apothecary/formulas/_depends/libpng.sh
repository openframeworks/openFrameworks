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
	
	if [ "$TYPE" == "osx" ] ; then

		# these flags are used to create a fat 32/64 binary with i386->libstdc++, x86_64->libc++
		# see https://gist.github.com/tgfrerer/8e2d973ed0cfdd514de6
		local FAT_LDFLAGS="-arch i386 -arch x86_64 -stdlib=libstdc++ -Xarch_x86_64 -stdlib=libc++"
	
		./configure LDFLAGS="${FAT_LDFLAGS} " \
				CFLAGS="-O3 ${FAT_LDFLAGS}" \
				--prefix=$BUILD_ROOT_DIR \
				--disable-dependency-tracking

	else
	./configure LDFLAGS="-arch i386 -arch x86_64" \
				CFLAGS="-Os -arch i386 -arch x86_64" \
				--prefix=$BUILD_ROOT_DIR \
				--disable-dependency-tracking
	fi


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
