#! /bin/bash
#
# Free Type
# http://freetype.org/

VER=2.4.12

# download the source code and unpack it into LIB_NAME
function download() {
	curl -LO http://download.savannah.gnu.org/releases/freetype/freetype-$VER.tar.gz
	tar -xf freetype-$VER.tar.gz
	mv freetype-$VER freetype
	rm freetype*.tar.gz
}

# executed inside the build dir
function build() {
	
	if [ "$TYPE" == "osx" ] ; then

		# 32 bit
		./configure CFLAGS="-arch i386"
		make clean; make
		cp objs/.libs/libfreetype.a libfreetype-i386.a

		# 64 bit
		./configure CFLAGS="-arch x86_64"
		make clean; make
		cp objs/.libs/libfreetype.a libfreetype-x86_64.a

		# link into universal lib
		lipo -c libfreetype-i386.a libfreetype-x86_64.a -o libfreetype.a
	
	elif [ "$TYPE" == "vs2010" ] ; then
		echo "vs2010 build here"
	
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

	else
		# configure with arch
		if [ $ARCH ==  32 ] ; then
			./configure CFLAGS="-arch i386"
		elif [ $ARCH == 64 ] ; then
			./configure CFLAGS="-arch x86_64"
		fi
		make clean; make
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {

	# headers
	mkdir -p $1/include/freetype2 # not sure why we need the freetype2 subdir
	cp -Rv include/* $1/include/freetype2

	# lib
	mkdir -p $1/lib/$TYPE
	if [ "$TYPE" == "osx" -o "$TYPE" == "ios" ] ; then
		cp -v libfreetype.a $1/lib/$TYPE/freetype.a
	elif [ "$TYPE" == "vs2010" ] ; then
		echo "copy vs2010 here"
	else
		cp -v objs/.libs/libfreetype.a $1/lib/$TYPE/freetype.a
	fi
}
