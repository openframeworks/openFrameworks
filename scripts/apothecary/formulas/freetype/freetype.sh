#! /bin/bash
#
# Free Type
# cross platform ttf/optf font loder
# http://freetype.org/
#
# an autotools project

FORMULA_TYPES=( "osx" "vs" "msys2" "ios" "tvos" "android" "emscripten" )

# define the version
VER=2.5.5
FVER=255

# tools for git use
GIT_URL=http://git.savannah.gnu.org/r/freetype/freetype2.git
GIT_TAG=VER-2-5-5

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
	mkdir -p lib/$TYPE
}

# executed inside the lib src dir
function build() {
	
	if [ "$TYPE" == "osx" ] ; then
		local BUILD_TO_DIR=$BUILD_DIR/freetype/build/$TYPE/

		# these flags are used to create a fat 32/64 binary with i386->libstdc++, x86_64->libc++
		# see https://gist.github.com/tgfrerer/8e2d973ed0cfdd514de6
		local FAT_CFLAGS="-arch i386 -arch x86_64 -stdlib=libc++ -mmacosx-version-min=${OSX_MIN_SDK_VER}"

		set -e
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

		local TOOLCHAIN=$XCODE_DEV_ROOT/Toolchains/XcodeDefault.xctoolchain 

		./configure --prefix=$BUILD_TO_DIR --without-bzip2 --with-harfbuzz=no --enable-static=yes --enable-shared=no \
			CFLAGS="$FAT_CFLAGS -fPIC -pipe -Wno-trigraphs -fpascal-strings -O2 -Wreturn-type -Wunused-variable -fmessage-length=0 -fvisibility=hidden"
		make clean 
		make -j${PARALLEL_MAKE}
		make install
		cp $BUILD_TO_DIR/lib/libfreetype.a lib/$TYPE/libfreetype.a

		cd lib/$TYPE/
					
		mkdir -p "$BUILD_ROOT_DIR/lib/pkgconfig/"

		# copy pkgconfig file to _buildroot/lib/pkgconfig
		cp -v $BUILD_TO_DIR/lib/pkgconfig/freetype2.pc $BUILD_ROOT_DIR/lib/pkgconfig/

		# # copy fat lib to where pkgconfig points to, so that subsequent build stages will find it.
		# pkgconfig points to the $BUILD_TO_DIR
		cp -vf libfreetype.a $BUILD_TO_DIR/lib/libfreetype.a

		cd ../../

		unset OSX_ARCH STDLIB TOOLCHAIN CC CPP CXX

		echo "-----------"
		echo "$BUILD_DIR"
	
	elif [ "$TYPE" == "vs" ] ; then
		
		cd builds/windows/vc2010 #this upgrades without issue to vs2015
		vs-upgrade "freetype.sln"
		vs-build "freetype.vcxproj" Build "Release|Win32"
		vs-build "freetype.vcxproj" Build "Release|x64"
		cd ../../../
	
	elif [ "$TYPE" == "msys2" ] ; then
		# configure with arch
		if [ $ARCH ==  32 ] ; then
			./configure CFLAGS="-arch i386"
		elif [ $ARCH == 64 ] ; then
			./configure CFLAGS="-arch x86_64"
		fi
		
		make clean; 
		make -j${PARALLEL_MAKE}

	elif [[ "$TYPE" == "ios" || "${TYPE}" == "tvos" ]] ; then

		CSTANDARD=c11 # c89 | c99 | c11 | gnu11
		CPPSTANDARD=c++11 # c89 | c99 | c11 | gnu11
		COMPILER_CTYPE=clang # clang, gcc
		COMPILER_CPPTYPE=clang++ # clang, gcc
		STDLIB=libc++

		local IOS_ARCHS
        if [ "${TYPE}" == "tvos" ]; then 
            IOS_ARCHS="arm64 x86_64"
        elif [ "$TYPE" == "ios" ]; then
            IOS_ARCHS="i386 x86_64 armv7 arm64" #armv7s
        fi

		SDKVERSION=`xcrun -sdk iphoneos --show-sdk-version`	
		set -e
		CURRENTPATH=`pwd`
		
		DEVELOPER=$XCODE_DEV_ROOT
		TOOLCHAIN=${DEVELOPER}/Toolchains/XcodeDefault.xctoolchain
		VERSION=$VER

		SDKVERSION=""
        if [ "${TYPE}" == "tvos" ]; then 
            SDKVERSION=`xcrun -sdk appletvos --show-sdk-version`
        elif [ "$TYPE" == "ios" ]; then
            SDKVERSION=`xcrun -sdk iphoneos --show-sdk-version`
        fi

		mkdir -p "builddir/$TYPE"
	
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


		local TOOLCHAIN=$XCODE_DEV_ROOT/Toolchains/XcodeDefault.xctoolchain 
		MIN_IOS_VERSION=$IOS_MIN_SDK_VER
	    # min iOS version for arm64 is iOS 7
	
	    
		local IOS_CC=$TOOLCHAIN/usr/bin/cc
		local IOS_HOST="arm-apple-darwin"
		local IOS_PREFIX="/usr/local/iphone"

		export CC=$TOOLCHAIN/usr/bin/$COMPILER_CTYPE
		export CXX=$TOOLCHAIN/usr/bin/$COMPILER_CTYPE

		export LD=$TOOLCHAIN/usr/bin/ld
		export AR=$TOOLCHAIN/usr/bin/ar
		export AS=$TOOLCHAIN/usr/bin/as
		export NM=$TOOLCHAIN/usr/bin/nm
		export RANLIB=$TOOLCHAIN/usr/bin/ranlib


		
		# loop through architectures! yay for loops!
		for IOS_ARCH in ${IOS_ARCHS}
		do
			set +e

			if [[ "${IOS_ARCH}" == "i386" || "${IOS_ARCH}" == "x86_64" ]];
			then
	            if [ "${TYPE}" == "tvos" ]; then 
	                PLATFORM="AppleTVSimulator"
	            elif [ "$TYPE" == "ios" ]; then
	                PLATFORM="iPhoneSimulator"
	            fi
			else
	            if [ "${TYPE}" == "tvos" ]; then 
	                PLATFORM="AppleTVOS"
	            elif [ "$TYPE" == "ios" ]; then
	                PLATFORM="iPhoneOS"
	            fi
			fi

			export CROSS_TOP="${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer"
			export CROSS_SDK="${PLATFORM}${SDKVERSION}.sdk"
			export BUILD_TOOLS="${DEVELOPER}"

			if [[ "${IOS_ARCH}" == "arm64" || "${IOS_ARCH}" == "x86_64" ]]; then
	    		MIN_IOS_VERSION=7.0 # 7.0 as this is the minimum for these architectures
		    elif [ "${IOS_ARCH}" == "i386" ]; then
		    	MIN_IOS_VERSION=7.0 # 6.0 to prevent start linking errors
		    fi

	        if [ "${TYPE}" == "tvos" ]; then 
			    MIN_TYPE=-mtvos-version-min=
			    if [[ "${IOS_ARCH}" == "i386" || "${IOS_ARCH}" == "x86_64" ]]; then
			    	MIN_TYPE=-mtvos-version-min=
			    fi
	        elif [ "$TYPE" == "ios" ]; then
	            MIN_TYPE=-miphoneos-version-min=
	            if [[ "${IOS_ARCH}" == "i386" || "${IOS_ARCH}" == "x86_64" ]]; then
	                MIN_TYPE=-mios-simulator-version-min=
	            fi
	        fi

	        BITCODE=""
	        if [[ "$TYPE" == "tvos" ]]; then
	            BITCODE=-fembed-bitcode;
	            MIN_IOS_VERSION=9.0
	        fi
		
			export EXTRA_LINK_FLAGS="-std=$CSTANDARD $BITCODE -DNDEBUG -stdlib=$STDLIB $MIN_TYPE$MIN_IOS_VERSION -Os -fPIC -Wno-trigraphs -fpascal-strings -Wreturn-type -Wunused-variable -fmessage-length=0 -fvisibility=hidden"
			export EXTRA_FLAGS="-arch $IOS_ARCH $EXTRA_LINK_FLAGS $BITCODE -DNDEBUG -fvisibility-inlines-hidden $MIN_TYPE$MIN_IOS_VERSION"

			export PLATFORM_INCLUDE="-I${CROSS_TOP}/SDKs/${CROSS_SDK}/usr/include/ -I${CROSS_TOP}/SDKs/${CROSS_SDK}/usr/include/libxml2"
		    export CFLAGS="-arch $IOS_ARCH $EXTRA_FLAGS -pipe -no-cpp-precomp -isysroot ${CROSS_TOP}/SDKs/${CROSS_SDK} $PLATFORM_INCLUDE" 
			export LINKFLAGS="$CFLAGS $EXTRA_LINK_FLAGS"
			export LDFLAGS="-L${CROSS_TOP}/SDKs/${CROSS_SDK}/usr/lib/ $LINKFLAGS"
			export CXXFLAGS="$CFLAGS $EXTRA_FLAGS"

			mkdir -p "$CURRENTPATH/builddir/$TYPE/$IOS_ARCH"
			LOG="$CURRENTPATH/builddir/$TYPE/$IOS_ARCH/build-freetype-${VER}-$IOS_ARCH.log"
			echo "-----------------"
			echo "Building FreeType-${VER} for ${PLATFORM} ${SDKVERSION} ${IOS_ARCH} : iOS Minimum=$MIN_IOS_VERSION"
			set +e

			echo "Running make for ${IOS_ARCH}"
			echo "Please stand by..."

			echo "Configuring..."
			./configure --without-bzip2 --prefix=$IOS_PREFIX --host=$IOS_HOST --with-harfbuzz=no --enable-static=yes --enable-shared=no \
			CC="$CC" \
			CFLAGS="$CFLAGS" \
			CXXFLAGS="$CXXFLAGS" \
			CXX="$CXX" \
 			AS=$AS \
 			AR=$AR \
	        NM=$NM \
			LDFLAGS="$LDFLAGS" >> "${LOG}" 2>&1
			echo "Making..."
			make -j${PARALLEL_MAKE} >> "${LOG}" 2>&1
			if [ $? != 0 ];
		    then 
		    	tail -n 100 "${LOG}"
		    	echo "Problem while make - Please check ${LOG}"
		    	exit 1
		    else
		    	echo "Make Successful for ${IOS_ARCH}"
		    fi
		    echo "Cleaning up from Make"
		    make clean >> "${LOG}" 2>&1
		    if [ $? != 0 ];
		    then 
		    	tail -n 100 "${LOG}"
		    	echo "Problem while cleaning make - Please check ${LOG}"
		    	exit 1
		    else
		    	echo "Finished cleaning."
		    fi

		    cp objs/.libs/libfreetype.a lib/$TYPE/libfreetype-$IOS_ARCH.a

		    echo "-----------------"
		    echo "Build Successful for $IOS_ARCH"

		    unset CFLAGS LINKFLAGS LDFLAGS CXXFLAGS PLATFORM_INCLUDE EXTRA_FLAGS EXTRA_LINK_FLAGS
		done

		echo "-----------------"
		echo `pwd`
		echo "Finished for all architectures."
		mkdir -p "$CURRENTPATH/build/$TYPE/$IOS_ARCH"
		LOG="$CURRENTPATH/builddir/$TYPE/build-freetype-${VER}-lipo.log"
		# link into universal lib
		echo "Running lipo to create fat lib"
		echo "Please stand by..."
		# link into universal lib
		cd lib/$TYPE/

		if [ "${TYPE}" == "tvos" ]; then 
			lipo -create libfreetype-arm64.a \
						libfreetype-x86_64.a \
						-output libfreetype.a \
				 	>> "${LOG}" 2>&1
		elif [ "$TYPE" == "ios" ]; then
			# libfreetype-armv7s.a  \
			lipo -create libfreetype-armv7.a \
						libfreetype-arm64.a \
						libfreetype-i386.a \
						libfreetype-x86_64.a \
						-output libfreetype.a \
				 	>> "${LOG}" 2>&1
		fi

		if [ $? != 0 ];
		then 
			tail -n 10 "${LOG}"
		    echo "Problem while creating fat lib with lipo - Please check ${LOG}"
		    exit 1
		else
		   	echo "Lipo Successful."
		fi
		cd ../../
		lipo -info lib/$TYPE/libfreetype.a

		if [[ "$TYPE" == "ios" ]]; then
			echo "--------------------"
			echo "Stripping any lingering symbols"

			SLOG="$CURRENTPATH/lib/$TYPE/freetype-stripping.log"

			strip -x lib/$TYPE/libfreetype.a >> "${SLOG}" 2>&1
			if [ $? != 0 ];
			then 
				tail -n 100 "${SLOG}"
			    echo "Problem while stripping lib - Please check ${SLOG}"
			    exit 1
			else
			    echo "Strip Successful for ${SLOG}"
			fi
		fi

		echo "--------------------"
		echo "Build Successful for FreeType $TYPE $VER"
 
		unset IOS_AR IOS_HOST IOS_PREFIX  CPP CXX CXXCPP CXXCPP CC LD AS AR NM RANLIB LDFLAGS STDLIB

	elif [ "$TYPE" == "android" ] ; then
		
        source $LIBS_DIR/openFrameworksCompiled/project/android/paths.make
		
		# armv7
		ABI=armeabi-v7a
		local BUILD_TO_DIR=$BUILD_DIR/freetype/build/$TYPE/$ABI
		source ../../android_configure.sh $ABI

		./configure --prefix=$BUILD_TO_DIR --host armv7a-linux-android --with-harfbuzz=no --enable-static=yes --enable-shared=no 
		make clean 
		make -j${PARALLEL_MAKE}
		make install
		
		# x86
		ABI=x86
		local BUILD_TO_DIR=$BUILD_DIR/freetype/build/$TYPE/$ABI
		source ../../android_configure.sh $ABI

		./configure --prefix=$BUILD_TO_DIR --host x86-linux-android --with-harfbuzz=no --enable-static=yes --enable-shared=no 
		make clean 
		make -j${PARALLEL_MAKE}
		make install

		echo "-----------"
		echo "$BUILD_DIR"
	elif [ "$TYPE" == "emscripten" ]; then
	    if [ "$EMSCRIPTEN" == "" ]; then
	        echo "error emscripten is not installed or environment not set"
	        exit 1
	    fi
	    patch -p0 -u < $FORMULA_DIR/emscripten.patch
	    local BUILD_TO_DIR=$BUILD_DIR/freetype/build/$TYPE
	    ./configure --prefix=$BUILD_TO_DIR --with-harfbuzz=no --enable-static=yes --enable-shared=no --with-zlib=no --with-png=no
	    make clean
	    make -j${PARALLEL_MAKE}
	    cp $BUILD_DIR/freetype/objs/apinames .
	    emconfigure ./configure --prefix=$BUILD_TO_DIR --with-harfbuzz=no --enable-static=yes --enable-shared=no --with-zlib=no --with-png=no
	    emmake make clean
	    cp apinames $BUILD_DIR/freetype/objs/
	    emmake make -j${PARALLEL_MAKE}
	    emmake make install
	    emcc objs/*.o -o build/$TYPE/lib/libfreetype.bc
		echo "-----------"
		echo "$BUILD_DIR"
	    
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
    #remove old include files if they exist
    rm -rf $1/include

	# copy headers 
	mkdir -p $1/include/freetype2/

	# copy files from the build root
	cp -Rv include/* $1/include/freetype2/

	# older versions before 2.5.x
	#mkdir -p $1/include/freetype2/freetype/config
	#cp -Rv include/* $1/include/freetype2/freetype
	#cp -Rv include/ft2build.h $1/include/

	# copy lib
	mkdir -p $1/lib/$TYPE

	if [ "$TYPE" == "osx" ] ; then
		cp -v lib/$TYPE/libfreetype.a $1/lib/$TYPE/freetype.a
	elif [[ "$TYPE" == "ios" || "$TYPE" == "tvos" ]]; then
		cp -v lib/$TYPE/libfreetype.a $1/lib/$TYPE/freetype.a
	elif [ "$TYPE" == "vs" ] ; then
		mkdir -p $1/lib/$TYPE/Win32
		mkdir -p $1/lib/$TYPE/x64		
		cp -v objs/vc2010/Win32/freetype$FVER.lib $1/lib/$TYPE/Win32/libfreetype.lib
		cp -v objs/vc2010/x64/freetype$FVER.lib $1/lib/$TYPE/x64/libfreetype.lib
	elif [ "$TYPE" == "msys2" ] ; then
		# cp -v lib/$TYPE/libfreetype.a $1/lib/$TYPE/libfreetype.a
		echoWarning "TODO: copy msys2 lib"
	elif [ "$TYPE" == "android" ] ; then
		cp -v build/$TYPE/armeabi-v7a/lib/libfreetype.a $1/lib/$TYPE/armeabi-v7a/libfreetype.a
		cp -v build/$TYPE/x86/lib/libfreetype.a $1/lib/$TYPE/x86/libfreetype.a
	elif [ "$TYPE" == "emscripten" ] ; then
		cp -v build/$TYPE/lib/libfreetype.bc $1/lib/$TYPE/libfreetype.bc
	fi

	# copy license files
	rm -rf $1/license # remove any older files if exists
	mkdir -p $1/license
	cp -v docs/LICENSE.TXT $1/license/
	cp -v docs/FTL.TXT $1/license/
	cp -v docs/GPLv2.TXT $1/license/
}

# executed inside the lib src dir
function clean() {

	if [ "$TYPE" == "vs" ] ; then
		vs-clean "freetype.sln"
	elif [ "$TYPE" == "android" ] ; then
		make clean
		rm -f build/$TYPE
	elif [[ "$TYPE" == "ios" || "$TYPE" == "tvos" ]]; then
		make clean
		rm -f *.a *.lib
		rm -f builddir/$TYPE
		rm -f builddir
		rm -f lib
	else
		make clean
		rm -f *.a *.lib
	fi
}
