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

}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	./autogen.sh

	# We place the downloads in the prepare section in case 
	# cairo was already was already downloaded.

	# manually download dependencies
	apothecaryDependencies download

	# manually prepare dependencies
	apothecaryDependencies prepare
}

# executed inside the lib src dir
function build() {
	
	# manually build pkg-config first
	apothecaryDepend build pkg-config
	apothecaryDepend copy pkg-config

	# we're using a custom version of pkg-config, so let the cairo build system know
	export PKG_CONFIG=$BUILD_ROOT_DIR/bin/pkg-config
	export PKG_CONFIG_PATH=$BUILD_ROOT_DIR/lib/pkgconfig

	# set flags for osx 32 & 64 bit fat lib
	if [ "$TYPE" == "osx" ] ; then
		export MACOSX_DEPLOYMENT_TARGET=$OSX_MIN_SDK_VER
   		export LDFLAGS="-arch i386 -arch x86_64 -isysroot $XCODE_DEV_ROOT/Platforms/MacOSX.platform/Developer/SDKs/MacOSX$OSX_SDK_VER.sdk"
   		export CFLAGS="-Os -arch i386 -arch x86_64 -isysroot $XCODE_DEV_ROOT/Platforms/MacOSX.platform/Developer/SDKs/MacOSX$OSX_SDK_VER.sdk"
	fi

	apothecaryDepend build libpng
	apothecaryDepend copy libpng
	apothecaryDepend build pixman
	apothecaryDepend copy pixman
	apothecaryDepend build freetype
	apothecaryDepend copy freetype


	if [ "$TYPE" == "vs" ] ; then
		make -f Makefile.win32
	else 
		./configure --prefix=$BUILD_ROOT_DIR \
				--disable-gtk-doc \
				--disable-gtk-doc-html \
				--disable-gtk-doc-pdf \
				--disable-full-testing \
				--disable-dependency-tracking \
				--disable-xlib \
				--disable-qt 

		make
		make install
	fi

	# clean up env vars
	unset PKG_CONFIG PKG_CONFIG_PATH

	if [ "$TYPE" == "osx" ] ; then
		unset MACOSX_DEPLOYMENT_TARGET CFLAGS LDFLAGS
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {

	# change to prefix location
	cd $BUILD_ROOT_DIR

	# headers
	mkdir -p $1/include
	cp -Rv include/* $1/include

	# lib
	mkdir -p $1/lib/$TYPE
	if [ "$TYPE" == "vs" ] ; then
		echoWarning "copy vs lib"

	elif [ "$TYPE" == "osx" -o "$TYPE" == "win_cb" ] ; then
		if [ "$TYPE" == "osx" ] ; then
			cp -v lib/libcairo-script-interpreter.a $1/lib/$TYPE/cairo-script-interpreter.a
		fi
		cp -v lib/libcairo.a $1/lib/$TYPE/cairo.a
		cp -v lib/libpixman-1.a $1/lib/$TYPE/pixman-1.a
	fi

	cd -
}

# executed inside the lib src dir
function clean() {
	
	# manually clean dependencies
	apothecaryDependencies clean

	# cairo
	make clean
}
