#! /bin/bash
#
# Poco
# C++ with Batteries Included
# http://pocoproject.org/
#
# uses an autotools build system,
# specify specfic build configs in poco/config using ./configure --config=NAME

# define the version
VER=1.5.3-release

# tools for git use
GIT_URL=https://github.com/pocoproject/poco
GIT_TAG=poco-$VER

# For Poco Builds, we omit both Data/MySQL and Data/ODBC because they require
# 3rd Party libraries.  See https://github.com/pocoproject/poco/blob/develop/README
# for more information.


# @tgfrerer: we need more fine-grained control over poco source code versions, 
# which is why we specify the specific poco source code commit we want to use -
# When updating poco/this recipe, make sure to specify the proper hash here,
# and check if the header patches still apply cleanly.

# @TODO: New commit ref?
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
		cd build/config
		cp iPhone iPhone.orig
		cp iPhoneSimulator iPhoneSimulator.orig
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
	
	elif [ "$TYPE" == "win_cb" ] ; then
		# Use the mingw equivalent of mc to compile the pocomsg.mc file into a pocomsg.h.
		cd Foundation/src
		windmc pocomsg.mc
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
					--cflags="-D_WIN32 -DMINGW32 -DWINVER=0x500 -DODBCVER=0x0300 -O3 -std=c++11 -DPOCO_ENABLE_CPP11 -DPOCO_NO_SOO -DPOCO_NO_AUTOMATIC_LIB_INIT" \
					--include-path=$OPENSSL_INCLUDE \
					--library-path=$OPENSSL_LIBS \
					--config=MinGW
		make

	elif [ "$TYPE" == "ios" ] ; then

		# maybe --poquito is a good idea?
		local BUILD_OPTS="--no-tests --no-samples --static --omit=CppUnit,CppUnit/WinTestRunner,Data/MySQL,Data/ODBC,PageCompiler,PageCompiler/File2Page,CppParser,PocoDoc,ProGen,NetSSL_OpenSSL,Crypto"

		cp build/config/iPhone.orig build/config/iPhone
		cp build/config/iPhoneSimulator.orig build/config/iPhoneSimulator

		# set SDK
		sed -i .tmp "s|#.*IPHONE_SDK_VERSION =.*|IPHONE_SDK_VERSION = $IOS_SDK_VER|" build/config/iPhone

		# fix any xcode path issues (currently fixed in newer poco, done here manually for now)
		sed -i .tmp 's|= /Developer|= $(shell xcode-select -print-path)|' build/config/iPhone
		sed -i .tmp 's|= /Developer|= $(shell xcode-select -print-path)|' build/config/iPhoneSimulator

		# armv7
		sed -i .tmp "s|POCO_TARGET_OSARCH.*?=.*|POCO_TARGET_OSARCH ?= armv7|" build/config/iPhone
		./configure $BUILD_OPTS --config=iPhone
		make

		# armv7s
		sed -i .tmp "s|POCO_TARGET_OSARCH.*?=.*|POCO_TARGET_OSARCH ?= armv7s|" build/config/iPhone
		./configure $BUILD_OPTS --config=iPhone
		make

		# simulator
		sed -i .tmp "s|POCO_TARGET_OSARCH.* = .*|POCO_TARGET_OSARCH = i386|" build/config/iPhoneSimulator
		./configure $BUILD_OPTS --config=iPhoneSimulator
		make

		cd lib/iPhoneOS

		# delete debug builds
		rm armv7/*d.a armv7s/*d.a ../iPhoneSimulator/i386/*d.a

		# link into universal lib, strip "lib" from filename
		local lib
		for lib in $( ls -1 ../iPhoneSimulator/i386) ; do
			local renamedLib=$(echo $lib | sed 's|lib||')
			if [ ! -e $renamedLib ] ; then
				lipo -c armv7/$lib armv7s/$lib ../iPhoneSimulator/i386/$lib -o $renamedLib
			fi
		done

	elif [ "$TYPE" == "android" ] ; then
		echoWarning "TODO: android build"

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
		cp -v lib/iPhoneOS/*.a $1/lib/$TYPE

	elif [ "$TYPE" == "vs" ] ; then
		mkdir -p $1/lib/$TYPE
		cp -v lib/Release/*.lib $1/lib/$TYPE
		cp -v lib/Debug/*.lib $1/lib/$TYPE
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
	else
		echoWarning "TODO: copy $TYPE lib"
	fi
}

# executed inside the lib src dir
function clean() {

	if [ "$TYPE" == "vs" ] ; then
		cmd //c buildwin.cmd ${VS_VER}0 clean static_md both Win32 nosamples notests
	elif [ "$TYPE" == "android" ] ; then
		echoWarning "TODO: clean android"
	else
		make clean
	fi
}
