#! /bin/bash
#
# Poco
# C++ with Batteries Included
# http://pocoproject.org/
#
# uses an autotools build system,
# specify specfic build configs in poco/config using ./configure --config=NAME

# define the version
VER=apothecary-1.4

# tools for git use
GIT_URL=https://github.com/bakercp/poco
GIT_TAG=poco-$VER

# For Poco Builds, we omit both Data/MySQL and Data/ODBC because they require
# 3rd Party libraries.  See https://github.com/pocoproject/poco/blob/develop/README
# for more information.

SHA=

# download the source code and unpack it into LIB_NAME
function download() {
	if [ "$SHA" == "" ] ; then
		curl -Lk $GIT_URL/archive/$GIT_TAG.tar.gz -o poco-$GIT_TAG.tar.gz
		tar -xf poco-$GIT_TAG.tar.gz
		mv poco-$GIT_TAG poco
		rm poco*.tar.gz
	else
		git clone $GIT_URL -b poco-$VER
	fi
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	if [ "$SHA" != "" ] ; then
		git reset --hard $SHA
	fi

	# make backups of the ios config files since we need to edit them
	if [ "$TYPE" == "ios" ] ; then
		mkdir -p lib/$TYPE
		cd build/config
		cp iPhoneSimulator-clang-libc++ iPhoneSimulator-clang-libc++.orig
		# fix using sed i636 reference and allow overloading variable
		sed -i .tmp "s|POCO_TARGET_OSARCH.* = .*|POCO_TARGET_OSARCH ?= i386|" iPhoneSimulator-clang-libc++
	elif [ "$TYPE" == "vs" ] ; then
		# Patch the components to exclude those that we aren't using.  
		if patch -p0 -u -N --dry-run --silent < $FORMULA_DIR/components.patch 2>/dev/null ; then
			patch -p0 -u < $FORMULA_DIR/components.patch
		fi

		# Locate the path of the openssl libs distributed with openFrameworks.
		local OF_LIBS_OPENSSL="../../../../libs/openssl/"

		# get the absolute path to the included openssl libs
		local OF_LIBS_OPENSSL_ABS_PATH=$(cd $(dirname $OF_LIBS_OPENSSL); pwd)/$(basename $OF_LIBS_OPENSSL)

		# convert the absolute path from unix to windows
		local OPENSSL_DIR=$(echo $OF_LIBS_OPENSSL_ABS_PATH | sed 's/^\///' | sed 's/\//\\/g' | sed 's/^./\0:/')

		# escape windows slashes and a few common escape sequences before passing to sed
		local OPENSSL_DIR=$(echo $OPENSSL_DIR | sed 's/\\/\\\\\\/g' | sed 's/\\\U/\\\\U/g' | sed 's/\\\l/\\\\l/g')

		# replace OPENSSL_DIR=C:\OpenSSL with our OPENSSL_DIR
		sed -i.tmp "s|C:\\\OpenSSL|$OPENSSL_DIR|g" buildwin.cmd

		# replace OPENSSL_LIB=%OPENSSL_DIR%\lib;%OPENSSL_DIR%\lib\VC with OPENSSL_LIB=%OPENSSL_DIR%\lib\vs
		sed -i.tmp "s|%OPENSSL_DIR%\\\lib;.*|%OPENSSL_DIR%\\\lib\\\vs|g" buildwin.cmd
	elif [ "$TYPE" == "android" ] ; then
		installAndroidToolchain
	fi

}

# executed inside the lib src dir
function build() {

	if [ "$TYPE" == "osx" ] ; then
		local BUILD_OPTS="--no-tests --no-samples --static --omit=CppUnit,CppUnit/WinTestRunner,Data/MySQL,Data/ODBC,PageCompiler,PageCompiler/File2Page,CppParser,PocoDoc,ProGen"
		
		# 32 bit
		# For OS 10.9+ we must explicitly set libstdc++ for the 32-bit OSX build.
		./configure $BUILD_OPTS --cflags=-stdlib=libstdc++ --config=Darwin32
		make

		# 64 bit
		./configure $BUILD_OPTS --config=Darwin64-clang-libc++
		make

		cd lib/Darwin

		# delete debug builds
		rm i386/*d.a
		rm x86_64/*d.a

		# link into universal lib, strip "lib" from filename
		local lib
		for lib in $( ls -1 i386) ; do
			local renamedLib=$(echo $lib | sed 's|lib||')
			if [ ! -e $renamedLib ] ; then
				lipo -c i386/$lib x86_64/$lib -o $renamedLib
			fi
		done

	elif [ "$TYPE" == "vs" ] ; then
		cmd //c buildwin.cmd ${VS_VER}0 build static_md both Win32 nosamples notests

	elif [ "$TYPE" == "win_cb" ] ; then
		local BUILD_OPTS="--no-tests --no-samples --static --omit=CppUnit,CppUnit/WinTestRunner,Data/MySQL,Data/ODBC,PageCompiler,PageCompiler/File2Page,CppParser,PocoDoc,ProGen"

		local OPENSSL_INCLUDE="/c/Users/bakercp/openFrameworks/libs/openssl/include"
		local OPENSSL_LIBS="/c/Users/bakercp/openFrameworks/libs/openssl/lib/win_cb"

		./configure $BUILD_OPTS \
					--include-path=$OPENSSL_INCLUDE \
					--library-path=$OPENSSL_LIBS \
					--config=MinGW

		make

		# Delete debug libs.
		lib/MinGW/i686/*d.a

	elif [ "$TYPE" == "ios" ] ; then

		# maybe --poquito is a good idea?

		local BUILD_POCO_CONFIG_IPHONE=iPhone-clang-libc++
		local BUILD_POCO_CONFIG_SIMULATOR=iPhoneSimulator-clang-libc++

		# Locate the path of the openssl libs distributed with openFrameworks.
		local OF_LIBS_OPENSSL="../../../../libs/openssl/"

		# get the absolute path to the included openssl libs
		local OF_LIBS_OPENSSL_ABS_PATH=$(cd $(dirname $OF_LIBS_OPENSSL); pwd)/$(basename $OF_LIBS_OPENSSL)

		local OPENSSL_INCLUDE=$OF_LIBS_OPENSSL_ABS_PATH/include
		local OPENSSL_LIBS=$OF_LIBS_OPENSSL_ABS_PATH/lib/ios

		local BUILD_OPTS="--no-tests --no-samples --static --omit=CppUnit,CppUnit/WinTestRunner,Data/MySQL,Data/ODBC,PageCompiler,PageCompiler/File2Page,CppParser,PocoDoc,ProGen --include-path=$OPENSSL_INCLUDE --library-path=$OPENSSL_LIBS"
		local TOOLCHAIN=$XCODE_DEV_ROOT/Toolchains/XcodeDefault.xctoolchain 
		
		export IPHONE_SDK="iPhoneOS"
		local IOS_DEVROOT=$XCODE_DEV_ROOT/Platforms/$IPHONE_SDK.platform/Developer
		export IPHONE_SDK_ROOT=$IOS_DEVROOT/SDKs
		
		# armv7 
		export IPHONE_SDK_VERSION_MIN=$IOS_MIN_SDK_VER
		export POCO_TARGET_OSARCH="armv7"

		# configure and make
		./configure $BUILD_OPTS --config=$BUILD_POCO_CONFIG_IPHONE
		make
		# remove debug builds
		rm lib/$IPHONE_SDK/$POCO_TARGET_OSARCH/*d.a 

		unset POCO_TARGET_OSARCH IPHONE_SDK_VERSION_MIN

		# armv7s
		export IPHONE_SDK_VERSION_MIN=$IOS_MIN_SDK_VER
		export POCO_TARGET_OSARCH="armv7s"

		# configure and make
		./configure $BUILD_OPTS --config=$BUILD_POCO_CONFIG_IPHONE
		make
		# remove debug builds
		rm lib/$IPHONE_SDK/$POCO_TARGET_OSARCH/*d.a 

		unset POCO_TARGET_OSARCH IPHONE_SDK_VERSION_MIN

		# arm64
		local IPHONE_SDK_VERSION_64_MIN="7.0" # arm64 min is iOS 7.0
		export IPHONE_SDK_VERSION_MIN=$IPHONE_SDK_VERSION_64_MIN
		export POCO_TARGET_OSARCH="arm64"

		# configure and make
		./configure $BUILD_OPTS --config=$BUILD_POCO_CONFIG_IPHONE
		make
		# remove debug builds
		rm lib/$IPHONE_SDK/$POCO_TARGET_OSARCH/*d.a 

		unset POCO_TARGET_OSARCH IPHONE_SDK_VERSION_MIN
		
		unset IPHONE_SDK IPHONE_SDK_ROOT

		export IPHONE_SDK="iPhoneSimulator"
		local IOS_DEVROOT=$XCODE_DEV_ROOT/Platforms/$IPHONE_SDK.platform/Developer
		export IPHONE_SDK_ROOT=$IOS_DEVROOT/SDKs
		
		# i386 simulator
		export IPHONE_SDK_VERSION_MIN=$IOS_MIN_SDK_VER
		export POCO_TARGET_OSARCH="i386"

		# configure and make
		./configure $BUILD_OPTS --config=$BUILD_POCO_CONFIG_SIMULATOR
		make
		# remove debug builds
		rm lib/$IPHONE_SDK/$POCO_TARGET_OSARCH/*d.a 

		unset POCO_TARGET_OSARCH IPHONE_SDK_VERSION_MIN

		# x86_64 simulator
		export IPHONE_SDK_VERSION_MIN=$IOS_MIN_SDK_VER
		export POCO_TARGET_OSARCH="x86_64"

		# configure and make
		./configure $BUILD_OPTS --config=$BUILD_POCO_CONFIG_SIMULATOR
		make
		# remove debug builds
		rm lib/$IPHONE_SDK/$POCO_TARGET_OSARCH/*d.a 

		unset POCO_TARGET_OSARCH IPHONE_SDK_VERSION_MIN

		unset IPHONE_SDK IPHONE_SDK_ROOT

		cd lib/$TYPE
		# link into universal lib, strip "lib" from filename
		local lib
		for lib in $( ls -1 ../iPhoneSimulator/i386) ; do
			local renamedLib=$(echo $lib | sed 's|lib||')
			if [ ! -e $renamedLib ] ; then
				lipo -c ../iPhoneOS/armv7/$lib ../iPhoneOS/armv7s/$lib ../iPhoneOS/arm64/$lib ../iPhoneSimulator/i386/$lib ../iPhoneSimulator/x86_64/$lib -o $renamedLib
			fi
		done

	elif [ "$TYPE" == "android" ] ; then
		local BUILD_OPTS="--no-tests --no-samples --static --omit=CppUnit,CppUnit/WinTestRunner,Data/MySQL,Data/ODBC,PageCompiler,PageCompiler/File2Page,CppParser,PocoDoc,ProGen"
		
		local OLD_PATH=$PATH

		export PATH=$PATH:$BUILD_DIR/Toolchains/Android/androideabi/bin:$BUILD_DIR/Toolchains/Android/x86/bin

		local OF_LIBS_OPENSSL="../../../../libs/openssl/"

		# get the absolute path to the included openssl libs
		local OF_LIBS_OPENSSL_ABS_PATH=$(cd $(dirname $OF_LIBS_OPENSSL); pwd)/$(basename $OF_LIBS_OPENSSL)

		local OPENSSL_INCLUDE=$OF_LIBS_OPENSSL_ABS_PATH/include
		local OPENSSL_LIBS=$OF_LIBS_OPENSSL_ABS_PATH/lib/

		local BUILD_OPTS="--no-tests --no-samples --static --omit=CppUnit,CppUnit/WinTestRunner,Data/MySQL,Data/ODBC,PageCompiler,PageCompiler/File2Page,CppParser,PocoDoc,ProGen"

		./configure $BUILD_OPTS \
					--include-path=$OPENSSL_INCLUDE \
					--library-path=$OPENSSL_LIBS/armeabi \
					--config=Android

		make ANDROID_ABI=armeabi

		./configure $BUILD_OPTS \
					--include-path=$OPENSSL_INCLUDE \
					--library-path=$OPENSSL_LIBS/armeabi-v7a \
					--config=Android

		make ANDROID_ABI=armeabi-v7a

		./configure $BUILD_OPTS \
					--include-path=$OPENSSL_INCLUDE \
					--library-path=$OPENSSL_LIBS/x86 \
					--config=Android

		make ANDROID_ABI=x86

		echo `pwd`

		rm -v lib/Android/armeabi/*d.a
		rm -v lib/Android/armeabi-v7a/*d.a
		rm -v lib/Android/x86/*d.a

		export PATH=$OLD_PATH

	elif [ "$TYPE" == "linux" ] ; then
		local BUILD_OPTS="--no-tests --no-samples --static --omit=CppUnit,CppUnit/WinTestRunner,Data/MySQL,Data/ODBC,PageCompiler,PageCompiler/File2Page,CppParser,PocoDoc,ProGen"
		./configure $BUILD_OPTS
		make
		# delete debug builds
		rm lib/Linux/$(uname -m)/*d.a
	elif [ "$TYPE" == "linux64" ] ; then
		local BUILD_OPTS="--no-tests --no-samples --static --omit=CppUnit,CppUnit/WinTestRunner,Data/MySQL,Data/ODBC,PageCompiler,PageCompiler/File2Page,CppParser,PocoDoc,ProGen"
		./configure $BUILD_OPTS
		make
		# delete debug builds
		rm lib/Linux/x86_64/*d.a
	elif [ "$TYPE" == "linuxarmv6l" ] ; then
		local BUILD_OPTS="--no-tests --no-samples --static --omit=CppUnit,CppUnit/WinTestRunner,Data/MySQL,Data/ODBC,PageCompiler,PageCompiler/File2Page,CppParser,PocoDoc,ProGen"
		./configure $BUILD_OPTS
		make
		# delete debug builds
		rm lib/Linux/armv6l/*d.a
	elif [ "$TYPE" == "linuxarmv7l" ] ; then
		local BUILD_OPTS="--no-tests --no-samples --static --omit=CppUnit,CppUnit/WinTestRunner,Data/MySQL,Data/ODBC,PageCompiler,PageCompiler/File2Page,CppParser,PocoDoc,ProGen"
		./configure $BUILD_OPTS
		make
		# delete debug builds
		rm lib/Linux/armv7l/*d.a
	else 
		echoWarning "TODO: build $TYPE lib"
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {

	# headers
	mkdir -pv $1/include/Poco
	cp -Rv Crypto/include/Poco/Crypto $1/include/Poco
	cp -Rv Data/include/Poco/Data $1/include/Poco
	cp -Rv Data/SQLite/include/Poco/Data $1/include/Poco
	cp -Rv Foundation/include/Poco/* $1/include/Poco
	cp -Rv JSON/include/Poco/JSON $1/include/Poco
	cp -Rv MongoDB/include/Poco/MongoDB $1/include/Poco
	cp -Rv Net/include/Poco/Net $1/include/Poco
	cp -Rv NetSSL_OpenSSL/include/Poco/Net/* $1/include/Poco/Net
	cp -Rv PDF/include/Poco/PDF $1/include/Poco
	cp -Rv SevenZip/include/Poco/SevenZip $1/include/Poco
	cp -Rv Util/include/Poco/Util $1/include/Poco
	cp -Rv XML/include/Poco/* $1/include/Poco
	cp -Rv Zip/include/Poco/Zip $1/include/Poco

	# libs
	if [ "$TYPE" == "osx" ] ; then		
		mkdir -p $1/lib/$TYPE
		cp -v lib/Darwin/*.a $1/lib/$TYPE
	elif [ "$TYPE" == "ios" ] ; then
		mkdir -p $1/lib/$TYPE
		cp -v lib/$TYPE/*.a $1/lib/$TYPE
	elif [ "$TYPE" == "vs" ] ; then
		mkdir -p $1/lib/$TYPE
		cp -v lib/*.lib $1/lib/$TYPE
	elif [ "$TYPE" == "win_cb" ] ; then
		mkdir -p $1/lib/$TYPE
		cp -v lib/MinGW/i686/*.a $1/lib/$TYPE
	elif [ "$TYPE" == "linux" ] ; then
		mkdir -p $1/lib/$TYPE
		cp -v lib/Linux/$(uname -m)/*.a $1/lib/$TYPE
	elif [ "$TYPE" == "linux64" ] ; then
		mkdir -p $1/lib/$TYPE
		cp -v lib/Linux/x86_64/*.a $1/lib/$TYPE
	elif [ "$TYPE" == "linuxarmv6l" ] ; then
		mkdir -p $1/lib/$TYPE
		cp -v lib/Linux/armv6l/*.a $1/lib/$TYPE
	elif [ "$TYPE" == "linuxarmv7l" ] ; then
		mkdir -p $1/lib/$TYPE
		cp -v lib/Linux/armv7l/*.a $1/lib/$TYPE
	elif [ "$TYPE" == "android" ] ; then
		mkdir -p $1/lib/$TYPE/armeabi
		cp -v lib/Android/armeabi/*.a $1/lib/$TYPE/armeabi

		mkdir -p $1/lib/$TYPE/armeabi-v7a
		cp -v lib/Android/armeabi-v7a/*.a $1/lib/$TYPE/armeabi-v7a

		mkdir -p $1/lib/$TYPE/x86
		cp -v lib/Android/x86/*.a $1/lib/$TYPE/x86
	else
		echoWarning "TODO: copy $TYPE lib"
	fi
}

# executed inside the lib src dir
function clean() {

	if [ "$TYPE" == "vs" ] ; then
		cmd //c buildwin.cmd ${VS_VER}0 clean static_md both Win32 nosamples notests
	elif [ "$TYPE" == "android" ] ; then
		export PATH=$PATH:$ANDROID_TOOLCHAIN_ANDROIDEABI/bin:$ANDROID_TOOLCHAIN_X86/bin
		make clean ANDROID_ABI=armeabi
		make clean ANDROID_ABI=armeabi-v7a
		make clean ANDROID_ABI=x86
		unset PATH
	else
		make clean
	fi
}
