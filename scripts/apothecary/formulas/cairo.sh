#! /bin/bash
#
# Cairo
# 2D graphics library with support for multiple output devices
# http://www.cairographics.org/
#
# has an autotools build system and requires pkg-config, libpng, & pixman,
# dependencies have their own formulas in cairo/depends
#
# following http://www.cairographics.org/end_to_end_build_for_mac_os_x,
# we build and install dependencies into a subfolder of cairo by setting the 
# prefix (install location) and use a custom copy of pkg-config which returns
# the dependent lib cflags/ldflags for that prefix (cairo/apothecary-build)

FORMULA_TYPES=( "osx" "vs" "win_cb" )

FORMULA_DEPENDS=( "pkg-config" "libpng" "pixman" "freetype" )

# tell apothecary we want to manually call the dependency commands
# as we set some env vars for osx the depends need to know about
FORMULA_DEPENDS_MANUAL=1

# define the version
VER=1.12.14

# tools for git use
GIT_URL=http://anongit.freedesktop.org/git/cairo
GIT_TAG=$VER

# download the source code and unpack it into LIB_NAME
function download() {
	curl -LO http://cairographics.org/releases/cairo-$VER.tar.xz
	tar -xf cairo-$VER.tar.xz
	mv cairo-$VER cairo
	rm cairo-$VER.tar.xz

	# manually download dependencies
	apothecaryDependencies download
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	# generate the configure script if it's not there
	if [ ! -f configure ] ; then
		./autogen.sh
	fi

	# manually prepare dependencies
	apothecaryDependencies prepare

	# Build and copy all dependencies in preparation
	apothecaryDepend build pkg-config
	apothecaryDepend copy pkg-config
	apothecaryDepend build libpng
	apothecaryDepend copy libpng
	apothecaryDepend build pixman
	apothecaryDepend copy pixman
	apothecaryDepend build freetype
	apothecaryDepend copy freetype

}

# executed inside the lib src dir
function build() {

	if [ "$TYPE" == "vs" ] ; then
		make -f Makefile.win32
	else 
		./configure PKG_CONFIG="$BUILD_ROOT_DIR/bin/pkg-config" \
					PKG_CONFIG_PATH="$BUILD_ROOT_DIR/lib/pkgconfig" \
					LDFLAGS="-arch i386 -arch x86_64" \
					CFLAGS="-Os -arch i386 -arch x86_64" \
					--prefix=$BUILD_ROOT_DIR \
					--disable-gtk-doc \
					--disable-gtk-doc-html \
					--disable-gtk-doc-pdf \
					--disable-full-testing \
					--disable-dependency-tracking \
					--disable-xlib \
					--disable-qt 

		make -j
		make install
	fi

}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {

	# make the path in the libs dir
	mkdir -p $1/include/cairo

	# copy the cairo headers
	cp -Rv $BUILD_ROOT_DIR/include/cairo/* $1/include/cairo

	# make the path in the libs dir
	mkdir -p $1/include/libpng16

	# copy the cairo headers
	cp -Rv $BUILD_ROOT_DIR/include/libpng16/* $1/include/libpng16

	# make the path in the libs dir
	mkdir -p $1/include/pixman-1

	# copy the cairo headers
	cp -Rv $BUILD_ROOT_DIR/include/pixman-1/* $1/include/pixman-1

	# copy the png symlinks
	cp -v $BUILD_ROOT_DIR/include/png* $1/include/

	# make the libs path 
	mkdir -p $1/lib/$TYPE

	if [ "$TYPE" == "vs" ] ; then
		echoWarning "copy vs lib"

	elif [ "$TYPE" == "osx" -o "$TYPE" == "win_cb" ] ; then
		if [ "$TYPE" == "osx" ] ; then
			cp -v $BUILD_ROOT_DIR/lib/libcairo-script-interpreter.a $1/lib/$TYPE/libcairo-script-interpreter.a
		fi
		cp -v $BUILD_ROOT_DIR/lib/libcairo.a $1/lib/$TYPE/cairo.a
		cp -v $BUILD_ROOT_DIR/lib/libpixman-1.a $1/lib/$TYPE/pixman-1.a
	fi

}

# executed inside the lib src dir
function clean() {
	
	# manually clean dependencies
	apothecaryDependencies clean

	# cairo
	make clean
}
