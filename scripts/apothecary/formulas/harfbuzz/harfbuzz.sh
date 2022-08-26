#! /bin/bash
#
# Harfbuzz
# HarfBuzz OpenType text shaping engine
# http://harfbuzz.org/
#
# For osx, the system currently attempts to use a local version of pkg-config.

# array of build types supported by this formula
# you can delete this to implicitly support *all* types

FORMULA_TYPES=( "osx" )

FORMULA_DEPENDS=( "pkg-config" "libpng" "freetype" )

# tell apothecary we want to manually call the dependency commands
# as we set some env vars for osx the depends need to know about
FORMULA_DEPENDS_MANUAL=1

VER=1.1.3

# download the source code and unpack it into LIB_NAME
function download() {
	local fileName=harfbuzz-$VER

	curl -LO http://www.freedesktop.org/software/harfbuzz/release/$fileName.tar.bz2
	curl -LO http://www.freedesktop.org/software/harfbuzz/release/$fileName.tar.bz2.sha256

	if [ "$(shasum -a 256 -c $fileName.tar.bz2.sha256 | awk '{print $2}')" = "OK" ] ;  then
		tar -xf $fileName.tar.bz2
		mv $fileName harfbuzz
		rm $fileName.tar.bz2
		rm $fileName.tar.bz2.sha256
	else
		echoWarning "Invalid shasum for $fileName.tar.bz2."
	fi

	# manually download dependencies
	apothecaryDependencies download

}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	# manually prepare dependencies
	apothecaryDependencies prepare

	# Build and copy all dependencies in preparation
	apothecaryDepend build pkg-config
	apothecaryDepend copy pkg-config
	apothecaryDepend build libpng
	apothecaryDepend copy libpng
	apothecaryDepend build freetype
	apothecaryDepend copy freetype

	if [ "$TYPE" == "osx" ] ; then
		# create a zlib pkg-config entry for the system zlib ...
		cp -v $FORMULA_DIR/zlib.pc $BUILD_ROOT_DIR/lib/pkgconfig/
	fi
}


# executed inside the lib src dir
function build() {

	if [ "$TYPE" == "osx" ] ; then

		# Don't forget the CXXFLAGS for the c++ components.
		./configure PKG_CONFIG="$BUILD_ROOT_DIR/bin/pkg-config" \
					PKG_CONFIG_PATH="$BUILD_ROOT_DIR/lib/pkgconfig" \
					LDFLAGS="-arch i386 -stdlib=libstdc++ -arch x86_64 -Xarch_x86_64 -stdlib=libc++" \
					CFLAGS="-Os -arch i386 -stdlib=libstdc++ -arch x86_64 -Xarch_x86_64 -stdlib=libc++" \
					CXXFLAGS="-Os -arch i386 -stdlib=libstdc++ -arch x86_64 -Xarch_x86_64 -stdlib=libc++" \
					FREETYPE_CFLAGS="`$BUILD_ROOT_DIR/bin/./pkg-config --cflags freetype2 libpng`" \
					FREETYPE_LIBS="`$BUILD_ROOT_DIR/bin/./pkg-config --libs freetype2 libpng`" \
					--prefix=$BUILD_ROOT_DIR \
					--disable-dependency-tracking \
					--enable-introspection=yes \
					--enable-static=yes \
					--enable-shared=no \
					--enable-introspection=yes \
					--disable-gtk-doc \
					--disable-gtk-doc-html \
					--with-glib=no \
					--with-gobject=no \
					--with-cairo=no \
					--with-fontconfig=no \
					--with-icu=no \
					--with-graphite2=no \
					--with-uniscribe=no \
					--with-freetype=yes	\
					--with-coretext=yes

		make -j${PARALLEL_MAKE}
		make install

	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	if [ "$TYPE" == "osx" ] ; then
	mkdir -p $1/include/harfbuzz
	cp -Rv $BUILD_ROOT_DIR/include/harfbuzz/* $1/include/harfbuzz/

	mkdir -p $1/lib/$TYPE/
	cp -Rv $BUILD_ROOT_DIR/lib/libharfbuzz.a $1/lib/$TYPE/
	fi

	# copy license file
	rm -rf $1/license # remove any older files if exists
	mkdir -p $1/license
	cp -v COPYING $1/license/

}

# executed inside the lib src dir
function clean() {
	# manually clean dependencies
	apothecaryDependencies clean

	# clean the project
	make clean
}
