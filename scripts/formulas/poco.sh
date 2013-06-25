#! /bin/bash
#
# C++ with Batteries Included
# http://pocoproject.org/

VER=1.4.6

# used when building for ios, the sdks you have installed are found in:
# /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator#.#.sdk
IPHONE_SDK_VER=6.1

# download the source code and unpack it into LIB_NAME
function download() {
	git clone https://github.com/pocoproject/poco -b poco-$VER
	
	# make backups of the config files since we need to edit them
	cd poco
	cp build/config/iPhone build/config/iPhone.orig
	cp build/config/iPhoneSimulator build/config/iPhoneSimulator.orig
}

# executed inside the build dir
function build() {

	if [ "$TYPE" == "osx" ] ; then
		local BUILD_OPTS="--no-tests --no-samples --static --omit=Data/MySQL,Data/SQLite,Data/ODBC"
		if [ $ARCH == 32 ] ; then
			./configure $BUILD_OPTS --config=Darwin32
		elif [ $ARCH == 64 ] ; then
			./configure $BUILD_OPTS --config=Darwin64
		fi
		make

	elif [ "$TYPE" == "vs2010" ] ; then
		echo "vs2010 build"
	
	elif [ "$TYPE" == "ios" ] ; then

		# maybe --poquito is a good idea?
		local BUILD_OPTS="--no-tests --no-samples --static --omit=Data/MySQL,Data/SQLite,Data/ODBC,NetSSL_OpenSSL,Crypto"

		# set SDK
		sed "s|#.*IPHONE_SDK_VERSION =.*|IPHONE_SDK_VERSION = $IPHONE_SDK_VER|" build/config/iPhone.orig > build/config/iPhone

		# fix any xcode path issues (currently fixed in newer poco, done here manually for now)
		sed 's|= /Developer|= $(shell xcode-select -print-path)|' build/config/iPhone > build/config/iPhone.tmp
		mv build/config/iPhone.tmp build/config/iPhone
		sed 's|= /Developer|= $(shell xcode-select -print-path)|' build/config/iPhoneSimulator > build/config/iPhoneSimulator.tmp
		mv build/config/iPhoneSimulator.tmp build/config/iPhoneSimulator

		# armv7
		sed "s|POCO_TARGET_OSARCH.*?=.*|POCO_TARGET_OSARCH ?= armv7|" build/config/iPhone > build/config/iPhone.tmp
		mv build/config/iPhone.tmp build/config/iPhone
		./configure $BUILD_OPTS --config=iPhone
		make

		# armv7s
		sed "s|POCO_TARGET_OSARCH.*?=.*|POCO_TARGET_OSARCH ?= armv7s|" build/config/iPhone > build/config/iPhone.tmp
		mv build/config/iPhone.tmp build/config/iPhone
		./configure $BUILD_OPTS --config=iPhone
		make

		# simulator
		sed "s|POCO_TARGET_OSARCH.* = .*|POCO_TARGET_OSARCH = i386|" build/config/iPhoneSimulator.orig > build/config/iPhoneSimulator
		./configure $BUILD_OPTS --config=iPhoneSimulator
		make

		# link into universal lib
		cd lib

		# delete debug builds
		rm iPhoneOS/armv7/*d.a iPhoneOS/armv7s/*d.a iPhoneSimulator/i386/*d.a

		# link into universal lib, strip "lib" from filename
		for lib in $( ls -1 iPhoneSimulator/i386) ; do
			local renamedLib=$(echo $lib | sed 's|lib||')
			lipo -c iPhoneOS/armv7/$lib iPhoneOS/armv7s/$lib iPhoneSimulator/i386/$lib -o $renamedLib
		done

	else
		./configure $BUILD_OPTS
		make
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {

	# headers
	mkdir -p $1/include/CppUnit
	cp -Rv CppUnit/include/CppUnit/* $1/include/CppUnit
	mkdir -p $1/include/Poco
	cp -Rv Foundation/include/Poco/* $1/include/Poco
	cp -Rv Crypto/include/Poco/Crypto $1/include/Poco
	cp -Rv Data/include/Poco/Data $1/include/Poco
	cp -Rv Net/include/Poco/Net $1/include/Poco
	cp -Rv NetSSL_OpenSSL/include/Poco/Net/* $1/include/Poco/Net
	cp -Rv Util/include/Poco/Util $1/include/Poco
	cp -Rv XML/include/Poco/* $1/include/Poco
	cp -Rv Zip/include/Poco/Zip $1/include/Poco

	# libs
	if [ "$TYPE" == "osx" ] ; then
		
		mkdir -p $1/lib/osx

		# choose source dir based on arch type
		local DIR=lib/Darwin/i386
		if [ $ARCH == 64 ] ; then
			DIR=lib/Darwin/x86_64
		fi

		# delete debug builds
		rm $DIR/*d.a

		# strip "lib" from filenames and copy
		for lib in $( ls -1 $DIR) ; do
			local renamedLib=$(echo $lib | sed 's|lib||')
			cp -v $DIR/$lib $1/lib/osx/$renamedLib
		done
	
	elif [ "$TYPE" == "ios" ] ; then
		mkdir -p $1/lib/ios
		cp -v lib/*.a $1/lib/ios
	fi
}
