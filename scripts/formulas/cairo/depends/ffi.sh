#! /bin/bash
#
# A Portable Foreign Function Interface Library
# http://librarywebsite.com

VER=3.0.13

# download the source code and unpack it into LIB_NAME
function download() {
	curl -LO ftp://sourceware.org/pub/libffi/libffi-$VER.tar.gz
	tar -xf libffi-$VER.tar.gz
	mv libffi-$VER.tar.gz libffi
	rm libffi-$VER.tar.gz
}

# executed inside the build dir
function build() {

	local buildOpts="--enable-static=yes"

	if [ "$TYPE" == "osx" ] ; then

		# 32 bit
		./configure $buildOpts CFLAGS="-arch i386" LDFLAGS="-arch i386"
		make clean; make
		#cp .libs/libpng16.a libpng16-i386.a
		return

		# 64 bit
		./configure $buildOpts CFLAGS="-arch x86_64" LDFLAGS="-arch x86_64"
		make clean; make
		cp .libs/libpng16.a libpng16-x86_64.a

		# link into universal lib
		lipo -c libpng16-i386.a libpng16-x86_64.a -o libpng16.a
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	echo "copying"
}
