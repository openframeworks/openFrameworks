#! /bin/bash
#
# Free Type
# cross platform ttf/optf font loder
# http://freetype.org/
#
# an autotools project

FORMULA_TYPES=( "osx" "vs" "win_cb" "ios" "android" )

# define the version
VER=2.4.12

# tools for git use
GIT_URL=http://git.savannah.gnu.org/r/freetype/freetype2.git
GIT_TAG=VER-2-4-12

# download the source code and unpack it into LIB_NAME
function download() {
	curl -LO http://download.savannah.gnu.org/releases/freetype/freetype-$VER.tar.gz
	tar -xf freetype-$VER.tar.gz
	mv freetype-$VER freetype
	rm freetype*.tar.gz
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	: # noop
}

# executed inside the lib src dir
function build() {
	
	if [ "$TYPE" == "osx" ] ; then

		export CFLAGS="-arch i386 -arch x86_64"

		# 32 + 64 bit
		./configure --prefix=$BUILD_ROOT_DIR

		make clean; 
		make;
		make install;

		unset CFLAGS
	
	elif [ "$TYPE" == "vs" ] ; then
   		make clean
        # Force config: auto detection is wrong
        cp -v builds/win32/w32-vcc.mk config.mk
        make
	
	elif [ "$TYPE" == "win_cb" ] ; then
		# configure with arch
		if [ $ARCH ==  32 ] ; then
			./configure CFLAGS="-arch i386"
		elif [ $ARCH == 64 ] ; then
			./configure CFLAGS="-arch x86_64"
		fi
		
		make clean; 
		make

	elif [ "$TYPE" == "ios" ] ; then
		# http://shift.net.nz/2010/09/compiling-freetype-for-iphoneios/
		# http://stackoverflow.com/questions/6425643/compiling-freetype-for-iphone
	
		# armv7
		./configure --without-bzip2 --prefix=/usr/local/iphone --host=arm-apple-darwin --enable-static=yes --enable-shared=no \
			CC=$XCODE_DEV_ROOT/Platforms/iPhoneOS.platform/Developer/usr/bin/gcc \
			CFLAGS="-arch armv7 -pipe -std=c99 -Wno-trigraphs -fpascal-strings -O2 -Wreturn-type -Wunused-variable -fmessage-length=0 -fvisibility=hidden -miphoneos-version-min=$IOS_MIN_SDK_VER -I$XCODE_DEV_ROOT/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$IOS_SDK_VER.sdk/usr/include/libxml2 -isysroot $XCODE_DEV_ROOT/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$IOS_SDK_VER.sdk" \
			AR=$XCODE_DEV_ROOT/Platforms/iPhoneOS.platform/Developer/usr/bin/ar \
			LDFLAGS="-arch armv7 -isysroot $XCODE_DEV_ROOT/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$IOS_SDK_VER.sdk -miphoneos-version-min=$IOS_MIN_SDK_VER"
		make clean; make
		cp objs/.libs/libfreetype.a libfreetype-armv7.a

		# armv7s
		./configure --without-bzip2 --prefix=/usr/local/iphone --host=arm-apple-darwin --enable-static=yes --enable-shared=no \
			CC=$XCODE_DEV_ROOT/Platforms/iPhoneOS.platform/Developer/usr/bin/gcc \
			CFLAGS="-arch armv7s -pipe -std=c99 -Wno-trigraphs -fpascal-strings -O2 -Wreturn-type -Wunused-variable -fmessage-length=0 -fvisibility=hidden -miphoneos-version-min=$IOS_MIN_SDK_VER -I$XCODE_DEV_ROOT/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$IOS_SDK_VER.sdk/usr/include/libxml2 -isysroot $XCODE_DEV_ROOT/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$IOS_SDK_VER.sdk" \
			AR=$XCODE_DEV_ROOT/Platforms/iPhoneOS.platform/Developer/usr/bin/ar \
			LDFLAGS="-arch armv7s -isysroot $XCODE_DEV_ROOT/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$IOS_SDK_VER.sdk -miphoneos-version-min=$IOS_MIN_SDK_VER"
		make clean; make
		cp objs/.libs/libfreetype.a libfreetype-armv7s.a

		# simulator
		./configure CFLAGS="-arch i386"
		make clean; make
		cp objs/.libs/libfreetype.a libfreetype-i386.a

		# link into universal lib
		lipo -create libfreetype-armv7.a libfreetype-armv7s.a libfreetype-i386.a -output libfreetype.a

	elif [ "$TYPE" == "android" ] ; then
		echoWarning "TODO: build android"
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {

	# copy headers 
	mkdir -p $1/include/freetype2/freetype/config

	# copy files from the build root
	cp -Rv $BUILD_ROOT_DIR/include/freetype2/* $1/include/freetype2/
	cp -Rv $BUILD_ROOT_DIR/include/ft2build.h $1/include/freetype2/

	# copy lib
	mkdir -p $1/lib/$TYPE

	if [ "$TYPE" == "osx" -o "$TYPE" == "ios" ] ; then
		# TODO use the lib prefix.  
		# Required CoreOF.xcconfig mod.
		# Required ios CoreOF.xcconfig mod.
		cp -v $BUILD_ROOT_DIR/lib/libfreetype.a $1/lib/$TYPE/freetype.a
	elif [ "$TYPE" == "vs" ] ; then
		echoWarning "TODO: copy vs lib"
	elif [ "$TYPE" == "win_cb" ] ; then
		#cp -v objs/.libs/libfreetype.a $1/lib/$TYPE/freetype.a
		echoWarning "TODO: copy win_cb lib"
	elif [ "$TYPE" == "android" ] ; then
		echoWarning "TODO: copy android lib"
	fi
}

# executed inside the lib src dir
function clean() {

	if [ "$TYPE" == "vs" ] ; then
		echoWarning "TODO: clean vs"
	
	elif [ "$TYPE" == "android" ] ; then
		echoWarning "TODO: clean android"
	
	else
		make clean
		rm -f *.a *.lib
	fi
}