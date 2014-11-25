#! /bin/bash
#
# openssl

# define the version
VER=1.0.1j
CSTANDARD=gnu11 # c89 | c99 | c11 | gnu11
COMPILER_TYPE=clang # clang, gcc


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
	if [ "$TYPE" == "ios" ] ; then
		# create output directories
		mkdir -p "src"
		mkdir -p "bin"
		mkdir -p "lib"

		mkdir -p lib/$TYPE
		mkdir -p lib/include

		mkdir -p build/$TYPE/i386
		mkdir -p build/$TYPE/x86_64
		mkdir -p build/$TYPE/armv7
        #mkdir -p build/$TYPE/armv7s
		mkdir -p build/$TYPE/arm64

		mkdir -p lib/$TYPE/i386
		mkdir -p lib/$TYPE/x86_64
		mkdir -p lib/$TYPE/armv7
        #mkdir -p lib/$TYPE/armv7s
		mkdir -p lib/$TYPE/arm64

		

		# make copies of the source files before modifying
		cp Makefile Makefile.orig
		cp Configure Configure.orig
		cp "crypto/ui/ui_openssl.c" "crypto/ui/ui_openssl.c.orig"
 	elif  [ "$TYPE" == "osx" ] ; then
		mkdir -p lib/$TYPE
		mkdir -p lib/include
 	fi



}

# executed inside the lib src dir
function build() {
	

	if [ "$TYPE" == "osx" ] ; then
		
		local BUILD_OPTS="-no-shared -no-asm -no-ec_nistp_64_gcc_128 -no-gmp -no-jpake -no-krb5 -no-md2 -no-rc5 -no-rfc3779 -no-sctp -no-shared -no-store -no-unit-test -no-zlib -no-zlib-dynamic"
		local OSX_ARCHS="x86_64 i386"
		
		VERSION=$VER
		CURRENTPATH=`pwd`
		
		# create build directories 

		for OSX_ARCH in ${OSX_ARCHS}
			do
			
			# Back up configure & makefile

			cp "Configure" "Configure.orig" 
			cp "Makefile" "Makefile.orig"

			# create build directory for current arch
			mkdir -p "$CURRENTPATH/build/$TYPE/$OSX_ARCH"

			#create logfile
			LOG="$CURRENTPATH/build/$TYPE/$OSX_ARCH/build-openssl-${VER}.log"

			
			if [ "${COMPILER_TYPE}" == "clang" ]; then
				export THECOMPILER=clang
			else
				export THECOMPILER=gcc
			fi

			echo "Using Compiler: $THECOMPILER"

			# patch the Configure file to make sure the correct compiler is invoked.

			OLD_LANG=$LANG
			unset LANG
				sed -ie "s!\"darwin-i386-cc\",\"cc:-arch i386 -g3!\"darwin-i386-cc\",\"${THECOMPILER}:-arch i386 -g3!" Configure
				sed -ie "s!\"darwin64-x86_64-cc\",\"cc:-arch x86_64 -O3!\"darwin64-x86_64-cc\",\"$THECOMPILER:-arch x86_64 -O3!" Configure
			export LANG=$OLD_LANG

   			OSX_C_FLAGS="" 		# Flags for stdlib, std and arch
   			CONFIG_TARGET=""	# Which one of the target presets to use

			if [[ "${OSX_ARCH}" == "i386" ]]; then
		    	# 386 -> libstdc++
		    	OSX_C_FLAGS="-arch ${OSX_ARCH} -std=${CSTANDARD} -stdlib=libstdc++"
		    	CONFIG_TARGET=darwin-i386-cc
		    	export CC="${THECOMPILER} ${OSX_C_FLAGS}"
		    elif [ "${OSX_ARCH}" == "x86_64" ]; then
		    	# 86_64 -> libc++
		    	OSX_C_FLAGS="-arch ${OSX_ARCH} -std=${CSTANDARD} -stdlib=libc++"
				CONFIG_TARGET=darwin64-x86_64-cc
		    	export CC="${THECOMPILER} ${OSX_C_FLAGS}"
		    fi

	    	echo "Configure for target: $CONFIG_TARGET"

		    ./Configure $CONFIG_TARGET $BUILD_OPTS --openssldir="$CURRENTPATH/build/$TYPE/$OSX_ARCH" > "${LOG}" 2>&1

			if [ $? != 0 ]; then 
		    	echo "Problem during configure - Please check ${LOG}"
		    	exit 1
		    fi

		    # patching Makefile to use the correct c flags.

		    OLD_LANG=$LANG
		    # we need to unset LANG otherwise sed will get upsed.
			unset LANG
			sed -ie "s!^CFLAG=!CFLAG=$OSX_C_FLAGS !" Makefile
			export LANG=$OLD_LANG


			echo "Running make for ${OSX_ARCH}"
			echo "Please stand by..."

			# Must run at -j 1 (single thread only else will fail)
			# this is super annoying, but true for OS X, as well as iOS.
			make -j 1 >> "${LOG}" 2>&1
			
			if [ $? != 0 ];
		    then 
		    	echo "Problem while make - Please check ${LOG}"
		    	exit 1
		    else
		    	echo "Make Successful for ${OSX_ARCH}"
		    fi

			set -e
			make -j 1 install >> "${LOG}" 2>&1
			make -j 1 clean >> "${LOG}" 2>&1

			# restore configure & makefile

			cp "Configure.orig" "Configure" 
			cp "Makefile.orig" "Makefile"

			unset CC CFLAG CFLAGS EXTRAFLAGS THECOMPILER
		done

		# Stage includes
		echo "Staging includes"

		cp -R "build/$TYPE/x86_64/include/" "lib/include/"

		# Stage fat libs - this is where we omit the lib-prefix
		echo "Building & staging fat libs"
		lipo -c "build/$TYPE/i386/lib/libcrypto.a" "build/$TYPE/x86_64/lib/libcrypto.a" -o "lib/$TYPE/crypto.a"
		lipo -c "build/$TYPE/i386/lib/libssl.a" "build/$TYPE/x86_64/lib/libssl.a" -o "lib/$TYPE/ssl.a"
		
		# ------------ END OS X Recipe.

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

	elif [ "$TYPE" == "ios" ] ; then

		# This was quite helpful as a reference: https://github.com/x2on/OpenSSL-for-iPhone
		# Refer to the other script if anything drastic changes for future versions
		SDKVERSION=`xcrun -sdk iphoneos --show-sdk-version`	
		set -e
		CURRENTPATH=`pwd`
		
		DEVELOPER=$XCODE_DEV_ROOT
		TOOLCHAIN=${DEVELOPER}/Toolchains/XcodeDefault.xctoolchain
		VERSION=$VER
		


        local IOS_ARCHS="i386 x86_64 armv7 arm64" #armv7s
		local STDLIB="libc++"


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
			# make sure backed up
			cp "Configure" "Configure.orig" 
			cp "Makefile" "Makefile.orig"

			if [ "${COMPILER_TYPE}" == "clang" ]; then
				export THECOMPILER=$TOOLCHAIN/usr/bin/clang
			else
				export THECOMPILER=${BUILD_TOOLS}/usr/bin/gcc
			fi
			echo "The compiler: $THECOMPILER"

			if [[ "${IOS_ARCH}" == "i386" || "${IOS_ARCH}" == "x86_64" ]];
			then
				PLATFORM="iPhoneSimulator"
				OLD_LANG=$LANG
				unset LANG
				sed -ie "s!\"debug-darwin-i386-cc\",\"cc:-arch i386 -g3!\"debug-darwin-i386-cc\",\"$THECOMPILER:-arch i386 -g3!" Configure
				sed -ie "s!\"darwin64-x86_64-cc\",\"cc:-arch x86_64 -O3!\"darwin64-x86_64-cc\",\"$THECOMPILER:-arch x86_64 -O3!" Configure
				export LANG=$OLD_LANG
			else
				cp "crypto/ui/ui_openssl.c" "crypto/ui/ui_openssl.c.orig"
				sed -ie "s!static volatile sig_atomic_t intr_signal;!static volatile intr_signal;!" "crypto/ui/ui_openssl.c"
				PLATFORM="iPhoneOS"
				OLD_LANG=$LANG
				unset LANG
				sed -ie "s!\"iphoneos-cross\",\"llvm-gcc:-O3!\"iphoneos-cross\",\"$THECOMPILER:-Os!" Configure
				export LANG=$OLD_LANG
			fi

		

			export CROSS_TOP="${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer"
			export CROSS_SDK="${PLATFORM}${SDKVERSION}.sdk"
			export BUILD_TOOLS="${DEVELOPER}"

			export CC="${THECOMPILER} -arch ${IOS_ARCH} -std=${CSTANDARD}"
			mkdir -p "$CURRENTPATH/build/$TYPE/$IOS_ARCH"
			LOG="$CURRENTPATH/build/$TYPE/$IOS_ARCH/build-openssl-${VER}.log"
			
			MIN_IOS_VERSION=$IOS_MIN_SDK_VER
		    # min iOS version for arm64 is iOS 7
		
		    if [[ "${IOS_ARCH}" == "arm64" || "${IOS_ARCH}" == "x86_64" ]]; then
		    	MIN_IOS_VERSION=7.0 # 7.0 as this is the minimum for these architectures
		    elif [ "${IOS_ARCH}" == "i386" ]; then
		    	MIN_IOS_VERSION=5.1 # 6.0 to prevent start linking errors
		    fi
			

			echo "Compiler: $CC"
			echo "Building openssl-${VER} for ${PLATFORM} ${SDKVERSION} ${IOS_ARCH} : iOS Minimum=$MIN_IOS_VERSION"

			set +e
			if [[ "$VERSION" =~ 1.0.0. ]]; then
				echo "Building for OpenSSL Version before 1.0.0"
	    		./Configure BSD-generic32 -no-asm --openssldir="$CURRENTPATH/build/$TYPE/$IOS_ARCH" > "${LOG}" 2>&1
			elif [ "${IOS_ARCH}" == "i386" ]; then
				echo "Configuring i386"
			    ./Configure darwin-i386-cc -no-asm --openssldir="$CURRENTPATH/build/$TYPE/$IOS_ARCH" > "${LOG}" 2>&1
		    elif [ "${IOS_ARCH}" == "x86_64" ]; then
		    	echo "Configuring x86_64"
			    ./Configure darwin64-x86_64-cc -no-asm --openssldir="$CURRENTPATH/build/$TYPE/$IOS_ARCH" > "${LOG}" 2>&1
		    else
		    	# armv7, armv7s, arm64
		    	echo "Configuring arm"
			    ./Configure iphoneos-cross -no-asm --openssldir="$CURRENTPATH/build/$TYPE/$IOS_ARCH" > "${LOG}" 2>&1
		    fi

		    if [ $? != 0 ]; then 
		    	echo "Problem while configure - Please check ${LOG}"
		    	exit 1
		    fi

		    MIN_TYPE=-miphoneos-version-min=
		    if [[ "${IOS_ARCH}" == "i386" || "${IOS_ARCH}" == "x86_64" ]]; then
		    	MIN_TYPE=-mios-simulator-version-min=
		    fi
		    
		    OLD_LANG=$LANG
			unset LANG
			sed -ie "s!^CFLAG=!CFLAG=-isysroot ${CROSS_TOP}/SDKs/${CROSS_SDK} -arch $IOS_ARCH -Os -fPIC -stdlib=libc++ $MIN_TYPE$MIN_IOS_VERSION !" Makefile
			export LANG=$OLD_LANG


			echo "Running make for ${IOS_ARCH}"
			echo "Please stand by..."
			# Must run at -j 1 (single thread only else will fail)
			make >> "${LOG}" 2>&1
			if [ $? != 0 ];
		    then 
		    	echo "Problem while make - Please check ${LOG}"
		    	exit 1
		    else
		    	echo "Make Successful for ${IOS_ARCH}"
		    fi

			set -e
			make install >> "${LOG}" 2>&1
			make clean >> "${LOG}" 2>&1

			# copy libraries to lib folder
			cp "build/$TYPE/$IOS_ARCH/lib/libssl.a" "lib/$TYPE/$IOS_ARCH/ssl.a"
			cp "build/$TYPE/$IOS_ARCH/lib/libcrypto.a" "lib/$TYPE/$IOS_ARCH/crypto.a"

			# must clean between builds

			# reset source file back.
			cp "crypto/ui/ui_openssl.c.orig" "crypto/ui/ui_openssl.c"
			cp "Makefile.orig" "Makefile"
			cp "Configure.orig" "Configure"

			unset CC CFLAG CFLAGS EXTRAFLAGS THECOMPILER

		done

		unset CC CFLAG CFLAGS 
		unset PLATFORM CROSS_TOP CROSS_SDK BUILD_TOOLS
		unset IOS_DEVROOT IOS_SDKROOT 

		cd lib/$TYPE/
		# stripping the lib prefix to bypass any issues with existing sdk libraries
		echo "Creating Fat Lib for crypto"
		lipo -create armv7/crypto.a \
					arm64/crypto.a \
					i386/crypto.a \
					x86_64/crypto.a \
					-output crypto.a
		echo "Creating Fat Lib for ssl"
		lipo -create armv7/ssl.a \
					arm64/ssl.a \
					i386/ssl.a \
					x86_64/ssl.a \
					-output ssl.a
		cd ../../

		# copy includes
		echo "Copying includes"

		cp -R "build/$TYPE/x86_64/include/" "lib/include/"

		cp "crypto/ui/ui_openssl.c.orig" "crypto/ui/ui_openssl.c"

		unset TOOLCHAIN DEVELOPER
	elif [ "$TYPE" == "android" ]; then
		source $LIBS_DIR/openFrameworksCompiled/project/android/paths.make
		
        # armv7
        ABI=armeabi-v7a
        local BUILD_TO_DIR=$BUILD_DIR/openssl/build/$TYPE/$ABI
        source ../../formulas/android_configure.sh $ABI
        export CC="$CC $CFLAGS $LDFLAGS"
        ./Configure --prefix=$BUILD_TO_DIR -no-ssl2 -no-ssl3 -no-comp -no-hw -no-engine android-armv7 #--host armv7a-linux-android --enable-static=yes --enable-shared=no
        make clean
        make
        make install
        
        # x86
        ABI=x86
        local BUILD_TO_DIR=$BUILD_DIR/openssl/build/$TYPE/$ABI
        source ../../formulas/android_configure.sh $ABI
        export CC="$CC $CFLAGS $LDFLAGS"
        ./Configure --prefix=$BUILD_TO_DIR -no-ssl2 -no-ssl3 -no-comp -no-hw -no-engine android-x86 #--host x86-linux-android --enable-static=yes --enable-shared=no
        make clean
        make
        make install

	else 

		echoWarning "TODO: build $TYPE lib"

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
	if [ -d $1/include/ ]; then
	    rm -r $1/include/
	fi
	
	mkdir -pv $1/include/
	# storing a copy of the include in lib/include/
	# set via: cp -R "build/$TYPE/x86_64/include/" "lib/include/"

	# suppress file not found errors
	rm -rf $1/lib/$TYPE/* 2> /dev/null

	# libs
	 if [ "$TYPE" == "osx" ] ; then
	    cp -Rv lib/include/ $1/include/
		mkdir -p $1/lib/$TYPE
		cp -v lib/$TYPE/*.a $1/lib/$TYPE
	 elif [ "$TYPE" == "ios" ] ; then
	    cp -Rv lib/include/ $1/include/
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
	elif [ "$TYPE" == "android" ] ; then
	    cp -Rv build/android/armeabi-v7a/include/ $1/
	    if [ -d $1/lib/$TYPE/ ]; then
	        rm -r $1/lib/$TYPE/
	    fi
	    mkdir -p $1/lib/$TYPE/armeabi-v7a
		cp -rv build/android/armeabi-v7a/lib/*.a $1/lib/$TYPE/armeabi-v7a/
	    mkdir -p $1/lib/$TYPE/x86
		cp -rv build/android/x86/lib/*.a $1/lib/$TYPE/x86/

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

		# reset files back to original if 
		cp "crypto/ui/ui_openssl.c.orig" "crypto/ui/ui_openssl.c"
		cp "Makefile.orig" "Makefile"
		cp "Configure.orig" "Configure"
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

