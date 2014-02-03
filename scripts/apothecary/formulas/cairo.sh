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
	
	if [ USE_GIT==1 ] ; then
		GIT_ARGS=-g
	else
		GIT_ARGS=
	fi

	# dependencies (some commented for now as they might be needed for other platforms)
	
	# download dependencies here for when using git for cairo
	#$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $DEPENDS_BUILD_DIR $GIT_ARGS download $DEPENDS_FORMULA_DIR/zlib.sh
	$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $DEPENDS_BUILD_DIR $GIT_ARGS download $DEPENDS_FORMULA_DIR/libpng.sh
	$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $DEPENDS_BUILD_DIR $GIT_ARGS download $DEPENDS_FORMULA_DIR/pixman.sh
	$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $DEPENDS_BUILD_DIR $GIT_ARGS download $DEPENDS_FORMULA_DIR/pkg-config.sh

	#$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $DEPENDS_BUILD_DIR prepare $DEPENDS_FORMULA_DIR/zlib.sh
	$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $DEPENDS_BUILD_DIR prepare $DEPENDS_FORMULA_DIR/libpng.sh
	$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $DEPENDS_BUILD_DIR prepare $DEPENDS_FORMULA_DIR/pixman.sh
	$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $DEPENDS_BUILD_DIR prepare $DEPENDS_FORMULA_DIR/pkg-config.sh
}

# executed inside the lib src dir
function build() {

	# build dependencies and install into $BUILD_DIR/cairo/build
	#local buildDir=$BUILD_DIR/cairo/apothecary-build
	#rm -rf $buildDir/bin $buildDir/lib $buildDir/share
	
	# build a custom version of pkg-config
	$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $DEPENDS_BUILD_DIR build $DEPENDS_FORMULA_DIR/pkg-config.sh
	$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $DEPENDS_BUILD_DIR copy $DEPENDS_FORMULA_DIR/pkg-config.sh
	export PKG_CONFIG=$DEPENDS_BUILD_DIR/bin/pkg-config
	export PKG_CONFIG_PATH=$DEPENDS_BUILD_DIR/lib/pkgconfig

	# set flags for osx 32 & 64 bit fat lib
	if [ "$TYPE" == "osx" ] ; then
		export MACOSX_DEPLOYMENT_TARGET=$OSX_MIN_SDK_VER
   		export LDFLAGS="-arch i386 -arch x86_64 -isysroot $XCODE_DEV_ROOT/Platforms/MacOSX.platform/Developer/SDKs/MacOSX$OSX_SDK_VER.sdk"
   		export CFLAGS="-Os -arch i386 -arch x86_64 -isysroot $XCODE_DEV_ROOT/Platforms/MacOSX.platform/Developer/SDKs/MacOSX$OSX_SDK_VER.sdk"
	
	elif [ "$TYPE" == "vs" ] ; then
		make -f Makefile.win32
		echoWarning "TODO: vs build settings here?"
	
	elif [ "$YTYPE" == "win_cb" ] ; then
		echoWarning "TODO: win_cb build settings here?"
	fi

	# build dependencies (some commented for now as they might be needed for other platforms)
	#$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $DEPENDS_BUILD_DIR build $DEPENDS_FORMULA_DIR/zlib.sh
	$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $DEPENDS_BUILD_DIR build $DEPENDS_FORMULA_DIR/libpng.sh
	$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $DEPENDS_BUILD_DIR build $DEPENDS_FORMULA_DIR/pixman.sh

	# install dependencies to local build dir
	#$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $DEPENDS_BUILD_DIR copy $DEPENDS_FORMULA_DIR/zlib.sh
	$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $DEPENDS_BUILD_DIR copy $DEPENDS_FORMULA_DIR/libpng.sh
	$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $DEPENDS_BUILD_DIR copy $DEPENDS_FORMULA_DIR/pixman.sh

	# build cairo
	./configure --prefix=$DEPENDS_BUILD_DIR --disable-dependency-tracking --disable-xlib --disable-ft
	make install

	# clean up env vars
	unset PKG_CONFIG PKG_CONFIG_PATH CFLAGS LDFLAGS MACOSX_DEPLOYMENT_TARGET
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	
	cd apothecary-build

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
}

# executed inside the lib src dir
function clean() {

	# dependencies
	# don't clean pkg-config as we only need to use the built binary
	#$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $DEPENDS_BUILD_DIR clean $DEPENDS_FORMULA_DIR/zlib.sh
	$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $DEPENDS_BUILD_DIR clean $DEPENDS_FORMULA_DIR/libpng.sh
	$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $DEPENDS_BUILD_DIR clean $DEPENDS_FORMULA_DIR/pixman.sh
	
	# cairo
	make clean
}
