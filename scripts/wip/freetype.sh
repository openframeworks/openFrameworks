#! /bin/bash
#
# Free Type
# http://freetype.org/

VER=2.4.12

SRC=freetype-$VER
DEST=../freetype

###

# get first arg
if [ $# -gt 0 ] ; then
	OS=$1
else
	OS=`./ostype.sh`
fi

# change to the dir of this script
cd $(dirname $0)

# get source
curl -LO http://download.savannah.gnu.org/releases/freetype/$SRC.tar.gz
tar -xf $SRC.tar.gz

# copy headers
mkdir -p $DEST/include
cp -Rv $SRC/include/* $DEST/include

# build & copy libs
if [ "$OS" == "vs2010" ] ; then
	echo "vs2010"
else

	cd $SRC
	./configure CFLAGS="-arch i386"
	make
	cd -

	if [ "$OS" == "osx" ] ; then
		mkdir -p $DEST/lib/osx
		cp -v $SRC/objs/.libs/libfreetype.a $DEST/lib/osx/freetype.a

	elif [ "$OS" == "win_cb" ] ; then
		mkdir -p $DEST/lib/win_cb
		cp -v $SRC/objs/.libs/libfreetype.a $DEST/lib/win_cb/freetype.a
	
	elif [ "$OS" == "ios" ] ; then
		# http://librocket.com/wiki/documentation/BuildingFreeTypeForiOS
		cd $SRC
		cp objs/.libs/libfreetype.a libfreetype-i386.a
	
		./configure CFLAGS="-arch x86_64";
		make clean; make
		cp objs/.libs/libfreetype.a libfreetype-x86_64.a

		./configure --prefix=/usr/local/iphone --host=arm-apple-darwin --enable-static=yes --enable-shared=no CC=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/gcc-4.2  CFLAGS="-arch armv7 -pipe -mdynamic-no-pic -std=c99 -Wno-trigraphs -fpascal-strings -O2 -Wreturn-type -Wunused-variable -fmessage-length=0 -fvisibility=hidden -miphoneos-version-min=3.2 -I/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS3.2.sdk/usr/include/libxml2 -isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS3.2.sdk" CPP=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/cpp AR=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/ar LDFLAGS="-arch armv7 -isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS3.2.sdk -miphoneos-version-min=3.2"
		make clean; make
		cp objs/.libs/libfreetype.a libfreetype-arm7.a

		./configure --prefix=/usr/local/iphone --host=arm-apple-darwin --enable-static=yes --enable-shared=no CC=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/gcc-4.2  CFLAGS="-arch armv7s -pipe -mdynamic-no-pic -std=c99 -Wno-trigraphs -fpascal-strings -O2 -Wreturn-type -Wunused-variable -fmessage-length=0 -fvisibility=hidden -miphoneos-version-min=3.2 -I/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS3.2.sdk/usr/include/libxml2 -isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS3.2.sdk" CPP=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/cpp AR=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/ar LDFLAGS="-arch armv7s -isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS3.2.sdk -miphoneos-version-min=3.2"
		make clean; make
		cp objs/.libs/libfreetype.a libfreetype-arm7s.a

		lipo -create -output freetype.a libfreetype-i386.a libfreetype-x86_64 libfreetype-arm7.a
		cd -

		mkdir -p $DEST/lib/ios
		cp -v $SRC/freetype.a $DEST/lib/ios
	fi
fi

# cleanup
 rm -rf $SRC
