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

VER=1.12.14

# download the source code and unpack it into LIB_NAME
function download() {

	# cairo
	curl -LO http://cairographics.org/releases/cairo-$VER.tar.xz
	tar -xf cairo-$VER.tar.xz
	mv cairo-$VER cairo
	rm cairo-$VER.tar.xz

	# dependencies (some commented for now as they might be needed for other platforms)
	local buildDir=$BUILD_DIR/cairo/apothecary-build
	mkdir -p $buildDir
	#$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $buildDir download $FORMULA_DIR/depends/zlib.sh
	$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $buildDir download $FORMULA_DIR/depends/libpng.sh
	$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $buildDir download $FORMULA_DIR/depends/pixman.sh
	#$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $buildDir download freetype
}

# executed inside the lib src dir
function build() {

	# build dependencies and install into $BUILD_DIR/cairo/build
	local buildDir=$BUILD_DIR/cairo/apothecary-build
	rm -rf $buildDir/bin $buildDir/lib $buildDir/share
	
	# build a custom version of pkg-config
	$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $buildDir update $FORMULA_DIR/depends/pkg-config.sh
	export PKG_CONFIG=$buildDir/bin/pkg-config
	export PKG_CONFIG_PATH=$buildDir/lib/pkgconfig

	# set flags for osx 32 & 64 bit fat lib
	if [ "$TYPE" == "osx" ] ; then
		export MACOSX_DEPLOYMENT_TARGET=$OSX_MIN_SDK_VER
   		export LDFLAGS="-arch i386 -arch x86_64 -isysroot $XCODE_DEV_ROOT/Platforms/MacOSX.platform/Developer/SDKs/MacOSX$OSX_SDK_VER.sdk"
   		export CFLAGS="-Os -arch i386 -arch x86_64 -isysroot $XCODE_DEV_ROOT/Platforms/MacOSX.platform/Developer/SDKs/MacOSX$OSX_SDK_VER.sdk"
	
	elif [ "$TYPE" == "vs" ] ; then
		echoWarning "TODO: vs build settings here?"
	
	elif [ "$YTYPE" == "win_cb" ] ; then
		echoWarning "TODO: win_cb build settings here?"
	fi

	# build and install dependencies (some commented for now as they might be needed for other platforms)
	#$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $buildDir update $FORMULA_DIR/depends/zlib.sh
	$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $buildDir update $FORMULA_DIR/depends/libpng.sh
	$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $buildDir update $FORMULA_DIR/depends/pixman.sh
	#$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $buildDir update freetype

	# build cairo
	./configure --prefix=$buildDir --disable-dependency-tracking --disable-xlib --disable-ft
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
	
	local buildDir=$BUILD_DIR/cairo/apothecary-build

	# dependencies
	# don't clean pkg-config as we only need to use the built binary
	#$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $buildDir clean $FORMULA_DIR/depends/zlib.sh
	$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $buildDir clean $FORMULA_DIR/depends/libpng.sh
	$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $buildDir clean $FORMULA_DIR/depends/pixman.sh
	#$APOTHECARY_DIR/apothecary -t $TYPE -a $ARCH -b $buildDir clean freetype
	
	# cairo
	make clean
}
