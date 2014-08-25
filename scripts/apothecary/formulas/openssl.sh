#! /bin/bash
#
# openssl

# define the version
VER=1.0.1i


# download the source code and unpack it into LIB_NAME
function download() {
	local FILENAME=openssl-$VER

	if ! [ -f $FILENAME ]; then
		curl -O https://www.openssl.org/source/$FILENAME.tar.gz
	fi

	if ! [ -f $FILENAME.sha1 ]; then
		curl -O https://www.openssl.org/source/$FILENAME.tar.gz.sha1
	fi
	
	if [ "$(shasum $FILENAME.tar.gz | awk '{print $1}')" == "$(cat $FILENAME.tar.gz.sha1)" ] ;  then  
		tar -xvf $FILENAME.tar.gz
		mv $FILENAME openssl
		rm $FILENAME.tar.gz
		rm $FILENAME.tar.gz.sha1
	else 
		echoError "Invalid shasum for $FILENAME."
	fi

}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	if [ "$TYPE" == "android" ] ; then
		installAndroidToolchain
	elif [ "$TYPE" == "ios" ] ; then
		# create output directories
		mkdir -p lib/$TYPE
		mkdir -p lib/include

		mkdir -p build/$TYPE/i386
		mkdir -p build/$TYPE/x86_64
		mkdir -p build/$TYPE/armv7
		mkdir -p build/$TYPE/armv7s
		mkdir -p build/$TYPE/arm64

		mkdir -p lib/$TYPE/i386
		mkdir -p lib/$TYPE/x86_64
		mkdir -p lib/$TYPE/armv7
		mkdir -p lib/$TYPE/armv7s
		mkdir -p lib/$TYPE/arm64

		# make copies of the source files before modifying
		cp Makefile Makefile.orig
		cp "crypto/ui/ui_openssl.c" "crypto/ui/ui_openssl.c.orig"
 	fi



}

# executed inside the lib src dir
function build() {
	echoWarning "TODO: build $TYPE lib"

	# if [ "$TYPE" == "osx" ] ; then
	# 	# local BUILD_OPTS="--no-tests --no-samples --static --omit=CppUnit,CppUnit/WinTestRunner,Data/MySQL,Data/ODBC,PageCompiler,PageCompiler/File2Page,CppParser,PocoDoc,ProGen"
		
	# 	# # 32 bit
	# 	# # For OS 10.9+ we must explicitly set libstdc++ for the 32-bit OSX build.
	# 	# ./configure $BUILD_OPTS --cflags=-stdlib=libstdc++ --config=Darwin32
	# 	# make

	# 	# # 64 bit
	# 	# ./configure $BUILD_OPTS --config=Darwin64-clang-libc++
	# 	# make

	# 	# cd lib/Darwin

	# 	# # delete debug builds
	# 	# rm i386/*d.a
	# 	# rm x86_64/*d.a

	# 	# # link into universal lib, strip "lib" from filename
	# 	# local lib
	# 	# for lib in $( ls -1 i386) ; do
	# 	# 	local renamedLib=$(echo $lib | sed 's|lib||')
	# 	# 	if [ ! -e $renamedLib ] ; then
	# 	# 		lipo -c i386/$lib x86_64/$lib -o $renamedLib
	# 	# 	fi
	# 	# done

	# elif [ "$TYPE" == "vs" ] ; then
	# 	# cmd //c buildwin.cmd ${VS_VER}0 build static_md both Win32 nosamples notests

	# elif [ "$TYPE" == "win_cb" ] ; then
	# 	# local BUILD_OPTS="--no-tests --no-samples --static --omit=CppUnit,CppUnit/WinTestRunner,Data/MySQL,Data/ODBC,PageCompiler,PageCompiler/File2Page,CppParser,PocoDoc,ProGen"

	# 	# # Locate the path of the openssl libs distributed with openFrameworks.
	# 	# local OF_LIBS_OPENSSL="../../../../libs/openssl/"

	# 	# # get the absolute path to the included openssl libs
	# 	# local OF_LIBS_OPENSSL_ABS_PATH=$(cd $(dirname $OF_LIBS_OPENSSL); pwd)/$(basename $OF_LIBS_OPENSSL)

	# 	# local OPENSSL_INCLUDE=$OF_LIBS_OPENSSL_ABS_PATH/include
	# 	# local OPENSSL_LIBS=$OF_LIBS_OPENSSL_ABS_PATH/lib/win_cb

	# 	# ./configure $BUILD_OPTS \
	# 	# 			--include-path=$OPENSSL_INCLUDE \
	# 	# 			--library-path=$OPENSSL_LIBS \
	# 	# 			--config=MinGW

	# 	# make

	# 	# # Delete debug libs.
	# 	# lib/MinGW/i686/*d.a

	if [ "$TYPE" == "ios" ] ; then

		# This was quite helpful as a reference: https://github.com/x2on/OpenSSL-for-iPhone
		# Refer to the other script if anything drastic changes for future versions

		export TOOLCHAIN=$XCODE_DEV_ROOT/Toolchains/XcodeDefault.xctoolchain 
		export DEVELOPER=$XCODE_DEV_ROOT

		local IOS_ARCHS="i386 x86_64 armv7 armv7s arm64"
		local STDLIB="libc++"
		CURRENTPATH=`pwd`	

		# Validate environment
		case $XCODE_DEV_ROOT in  
		     *\ * )
		           echo "Your Xcode path contains whitespaces, which is not supported."
		           exit 1
		          ;;
		esac
		case $CURRENTPATH in  
		     *\ * )
		           echo "Your path contains whitespaces, which is not supported by 'make install'."
		           exit 1
		          ;;
		esac 

		# loop through architectures! yay for loops!
		for IOS_ARCH in ${IOS_ARCHS}
		do
			unset RANLIB CC CFLAG CFLAGS 
			unset IOS_PLATFORM CROSS_TOP CROSS_SDK BUILD_TOOLS
			unset IOS_DEVROOT IOS_SDKROOT
	
			export RANLIB=$TOOLCHAIN/usr/bin/ranlib

			
			if [[ "${IOS_ARCH}" == "i386" || "${IOS_ARCH}" == "x86_64" ]];
			then
				IOS_PLATFORM="iPhoneSimulator"
			else
				cp "crypto/ui/ui_openssl.c" "crypto/ui/ui_openssl.c.orig"
				# need to change this file for iOS only
				sed -ie "s!static volatile sig_atomic_t intr_signal;!static volatile intr_signal;!" "crypto/ui/ui_openssl.c"
				IOS_PLATFORM="iPhoneOS"
			fi

			echo "Building OpenSSL $VER for $IOS_PLATFORM $IOS_SDK_VER $IOS_ARCH"

			local IOS_DEVROOT=$XCODE_DEV_ROOT/Platforms/$IOS_PLATFORM.platform/Developer
			local IOS_SDKROOT=$IOS_DEVROOT/SDKs/$IOS_PLATFORM$IOS_SDK_VER.sdk

			export CROSS_TOP=$IOS_DEVROOT
			export CROSS_SDK="$IOS_PLATFORM$IOS_SDK_VER.sdk"
			export BUILD_TOOLS="$XCODE_DEV_ROOT"

			export CC="$TOOLCHAIN/usr/bin/cc -arch $IOS_ARCH"

			if [[ "$VER" =~ 1.0.0. ]]; then
				echo "Building for OpenSSL Version before 1.0.0"
	    		./Configure BSD-generic32 --openssldir="$CURRENTPATH/build/$TYPE/$IOS_ARCH"
			elif [ "$IOS_ARCH" == "x86_64" ]; then
			    ./Configure darwin64-x86_64-cc --openssldir="$CURRENTPATH/build/$TYPE/$IOS_ARCH"
		    else
			    ./Configure iphoneos-cross --openssldir="$CURRENTPATH/build/$TYPE/$IOS_ARCH"
		    fi

		    local MIN_IOS_VERSION=$IOS_MIN_SDK_VER
		    # min iOS version for arm64 is iOS 7

		    if [ "$IOS_ARCH" == "arm64" ] ; then
		    	MIN_IOS_VERSION="7.0"
		    fi

		    sed -ie "s!^CFLAG=!CFLAG=-isysroot ${CROSS_TOP}/SDKs/${CROSS_SDK} -stdlib=$STDLIB -miphoneos-version-min=$MIN_IOS_VERSION !" "Makefile"


			# Must run at -j 1 (single thread only else will fail)
			make
			make install

			# copy libraries to lib folder
			cp "build/$TYPE/$IOS_ARCH/lib/libssl.a" "lib/$TYPE/$IOS_ARCH/libssl.a"
			cp "build/$TYPE/$IOS_ARCH/lib/libcrypto.a" "lib/$TYPE/$IOS_ARCH/libcrypto.a"

			# must clean between builds
			make clean

			# reset source file back.
			cp "crypto/ui/ui_openssl.c.orig" "crypto/ui/ui_openssl.c"

		done

		unset RANLIB CC CFLAG CFLAGS 
		unset IOS_PLATFORM CROSS_TOP CROSS_SDK BUILD_TOOLS
		unset IOS_DEVROOT IOS_SDKROOT 

		cd lib/$TYPE/
		# stripping the lib prefix to bypass any issues with existing sdk libraries
		echo "Creating Fat Lib for crypto"
		lipo -create armv7/libcrypto.a \
					armv7s/libcrypto.a \
					arm64/libcrypto.a \
					i386/libcrypto.a \
					x86_64/libcrypto.a \
					-output crypto.a
		echo "Creating Fat Lib for ssl"
		lipo -create armv7/libssl.a \
					armv7s/libssl.a \
					arm64/libssl.a \
					i386/libssl.a \
					x86_64/libssl.a \
					-output ssl.a
		cd ../../

		# copy includes
		echo "Copying includes"

		cp -R "build/$TYPE/x86_64/include/" "lib/include/"

		unset TOOLCHAIN DEVELOPER

	fi # end iOS Build

	# elif [ "$TYPE" == "android" ] ; then
	# 	# local BUILD_OPTS="--no-tests --no-samples --static --omit=CppUnit,CppUnit/WinTestRunner,Data/MySQL,Data/ODBC,PageCompiler,PageCompiler/File2Page,CppParser,PocoDoc,ProGen"
		
	# 	# local OLD_PATH=$PATH

	# 	# export PATH=$PATH:$BUILD_DIR/Toolchains/Android/androideabi/bin:$BUILD_DIR/Toolchains/Android/x86/bin

	# 	# local OF_LIBS_OPENSSL="../../../../libs/openssl/"

	# 	# # get the absolute path to the included openssl libs
	# 	# local OF_LIBS_OPENSSL_ABS_PATH=$(cd $(dirname $OF_LIBS_OPENSSL); pwd)/$(basename $OF_LIBS_OPENSSL)

	# 	# local OPENSSL_INCLUDE=$OF_LIBS_OPENSSL_ABS_PATH/include
	# 	# local OPENSSL_LIBS=$OF_LIBS_OPENSSL_ABS_PATH/lib/

	# 	# local BUILD_OPTS="--no-tests --no-samples --static --omit=CppUnit,CppUnit/WinTestRunner,Data/MySQL,Data/ODBC,PageCompiler,PageCompiler/File2Page,CppParser,PocoDoc,ProGen"

	# 	# ./configure $BUILD_OPTS \
	# 	# 			--include-path=$OPENSSL_INCLUDE \
	# 	# 			--library-path=$OPENSSL_LIBS/armeabi \
	# 	# 			--config=Android

	# 	# make ANDROID_ABI=armeabi

	# 	# ./configure $BUILD_OPTS \
	# 	# 			--include-path=$OPENSSL_INCLUDE \
	# 	# 			--library-path=$OPENSSL_LIBS/armeabi-v7a \
	# 	# 			--config=Android

	# 	# make ANDROID_ABI=armeabi-v7a

	# 	# ./configure $BUILD_OPTS \
	# 	# 			--include-path=$OPENSSL_INCLUDE \
	# 	# 			--library-path=$OPENSSL_LIBS/x86 \
	# 	# 			--config=Android

	# 	# make ANDROID_ABI=x86

	# 	# echo `pwd`

	# 	# rm -v lib/Android/armeabi/*d.a
	# 	# rm -v lib/Android/armeabi-v7a/*d.a
	# 	# rm -v lib/Android/x86/*d.a

	# 	# export PATH=$OLD_PATH

	# elif [ "$TYPE" == "linux" ] ; then
	# 	# local BUILD_OPTS="--no-tests --no-samples --static --omit=CppUnit,CppUnit/WinTestRunner,Data/MySQL,Data/ODBC,PageCompiler,PageCompiler/File2Page,CppParser,PocoDoc,ProGen"
	# 	# ./configure $BUILD_OPTS
	# 	# make
	# 	# # delete debug builds
	# 	# rm lib/Linux/$(uname -m)/*d.a
	# elif [ "$TYPE" == "linux64" ] ; then
	# 	# local BUILD_OPTS="--no-tests --no-samples --static --omit=CppUnit,CppUnit/WinTestRunner,Data/MySQL,Data/ODBC,PageCompiler,PageCompiler/File2Page,CppParser,PocoDoc,ProGen"
	# 	# ./configure $BUILD_OPTS
	# 	# make
	# 	# # delete debug builds
	# 	# rm lib/Linux/x86_64/*d.a
	# elif [ "$TYPE" == "linuxarmv6l" ] ; then
	# 	# local BUILD_OPTS="--no-tests --no-samples --static --omit=CppUnit,CppUnit/WinTestRunner,Data/MySQL,Data/ODBC,PageCompiler,PageCompiler/File2Page,CppParser,PocoDoc,ProGen"
	# 	# ./configure $BUILD_OPTS
	# 	# make
	# 	# # delete debug builds
	# 	# rm lib/Linux/armv6l/*d.a
	# elif [ "$TYPE" == "linuxarmv7l" ] ; then
	# 	# local BUILD_OPTS="--no-tests --no-samples --static --omit=CppUnit,CppUnit/WinTestRunner,Data/MySQL,Data/ODBC,PageCompiler,PageCompiler/File2Page,CppParser,PocoDoc,ProGen"
	# 	# ./configure $BUILD_OPTS
	# 	# make
	# 	# # delete debug builds
	# 	# rm lib/Linux/armv7l/*d.a
	# else 
	# 	echoWarning "TODO: build $TYPE lib"
	# fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	#echoWarning "TODO: copy $TYPE lib"

	# # headers
	mkdir -pv $1/include/
	# storing a copy of the include in lib/include/
	# set via: cp -R "build/$TYPE/x86_64/include/" "lib/include/"
	cp -Rv lib/include/ $1/include/

	# libs
	 if [ "$TYPE" == "osx" ] ; then	
	 	echoWarning "TODO: copy $TYPE lib"
	# 	mkdir -p $1/lib/$TYPE
	# 	cp -v lib/Darwin/*.a $1/lib/$TYPE
	 elif [ "$TYPE" == "ios" ] ; then
	 	mkdir -p $1/lib/$TYPE
	 	cp -v lib/$TYPE/*.a $1/lib/$TYPE
	# elif [ "$TYPE" == "vs" ] ; then
	# 	mkdir -p $1/lib/$TYPE
	# 	cp -v lib/*.lib $1/lib/$TYPE
	# elif [ "$TYPE" == "win_cb" ] ; then
	# 	mkdir -p $1/lib/$TYPE
	# 	cp -v lib/MinGW/i686/*.a $1/lib/$TYPE
	# elif [ "$TYPE" == "linux" ] ; then
	# 	mkdir -p $1/lib/$TYPE
	# 	cp -v lib/Linux/$(uname -m)/*.a $1/lib/$TYPE
	# elif [ "$TYPE" == "linux64" ] ; then
	# 	mkdir -p $1/lib/$TYPE
	# 	cp -v lib/Linux/x86_64/*.a $1/lib/$TYPE
	# elif [ "$TYPE" == "linuxarmv6l" ] ; then
	# 	mkdir -p $1/lib/$TYPE
	# 	cp -v lib/Linux/armv6l/*.a $1/lib/$TYPE
	# elif [ "$TYPE" == "linuxarmv7l" ] ; then
	# 	mkdir -p $1/lib/$TYPE
	# 	cp -v lib/Linux/armv7l/*.a $1/lib/$TYPE
	# elif [ "$TYPE" == "android" ] ; then
	# 	mkdir -p $1/lib/$TYPE/armeabi
	# 	cp -v lib/Android/armeabi/*.a $1/lib/$TYPE/armeabi

	# 	mkdir -p $1/lib/$TYPE/armeabi-v7a
	# 	cp -v lib/Android/armeabi-v7a/*.a $1/lib/$TYPE/armeabi-v7a

	# 	mkdir -p $1/lib/$TYPE/x86
	# 	cp -v lib/Android/x86/*.a $1/lib/$TYPE/x86
	else
	 	echoWarning "TODO: copy $TYPE lib"
	fi
}

# executed inside the lib src dir
function clean() {
	echoWarning "TODO: clean $TYPE lib"
	if [ "$TYPE" == "ios" ] ; then
		make clean
		# clean up old build folder
		rm -rf /build
		# clean up compiled libraries
		rm -rf /lib
	# if [ "$TYPE" == "vs" ] ; then
	# 	cmd //c buildwin.cmd ${VS_VER}0 clean static_md both Win32 nosamples notests
	# elif [ "$TYPE" == "android" ] ; then
	# 	export PATH=$PATH:$ANDROID_TOOLCHAIN_ANDROIDEABI/bin:$ANDROID_TOOLCHAIN_X86/bin
	# 	make clean ANDROID_ABI=armeabi
	# 	make clean ANDROID_ABI=armeabi-v7a
	# 	make clean ANDROID_ABI=x86
	# 	unset PATH
	else
	 	make clean
	fi
}

