#! /bin/bash
#
# Open Asset Import Library
# cross platform 3D model loader
# https://github.com/assimp/assimp
#
# uses CMake

# define the version
VER=3.1.1

# tools for git use
GIT_URL=
# GIT_URL=https://github.com/assimp/assimp.git
GIT_TAG=

FORMULA_TYPES=( "osx" "osx-clang-libc++" "ios" )

# download the source code and unpack it into LIB_NAME
function download() {

	# stable release from source forge
	curl -LO "https://github.com/assimp/assimp/archive/v$VER.zip"
	unzip -oq "v$VER.zip"
	mv "assimp-$VER" assimp
	rm "v$VER.zip"

    # fix an issue with static libs being disabled - see issue https://github.com/assimp/assimp/issues/271
    # this could be fixed fairly soon - so see if its needed for future releases.

    if [ "$TYPE" == "ios" ] ; then

    	echo "iOS"

	else 

    	sed -i -e 's/SET ( ASSIMP_BUILD_STATIC_LIB OFF/SET ( ASSIMP_BUILD_STATIC_LIB ON/g' assimp/CMakeLists.txt
    	sed -i -e 's/option ( BUILD_SHARED_LIBS "Build a shared version of the library" ON )/option ( BUILD_SHARED_LIBS "Build a shared version of the library" OFF )/g' assimp/CMakeLists.txt
	fi
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	echo "Prepare"

	# if [ "$TYPE" == "ios" ] ; then

	# 	# # patch outdated Makefile.osx provided with FreeImage, check if patch was applied first
	# 	# if patch -p0 -u -N --dry-run --silent < $FORMULA_DIR/assimp.ios.patch 2>/dev/null ; then
	# 	# 	patch -p0 -u < $FORMULA_DIR/assimp.ios.patch
	# 	# fi

	# fi
}

# executed inside the lib src dir
function build() {

    rm -f CMakeCache.txt || true

    # we don't use the build script for iOS now as it is less reliable than doing it our self
	if [ "$TYPE" == "ios" ] ; then
		# ref: http://stackoverflow.com/questions/6691927/how-to-build-assimp-library-for-ios-device-and-simulator-with-boost-library

        export TOOLCHAIN=$XCODE_DEV_ROOT/Toolchains/XcodeDefault.xctoolchain
		export TARGET_IOS
        
        local IOS_ARCHS="armv7 arm64 i386 x86_64" #armv7s
        local STDLIB="libc++"
        local CURRENTPATH=`pwd`

        echo $CURRENTPATH

		SDKVERSION=`xcrun -sdk iphoneos --show-sdk-version`	
        DEVELOPER=$XCODE_DEV_ROOT
		TOOLCHAIN=${DEVELOPER}/Toolchains/XcodeDefault.xctoolchain
		VERSION=$VER

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

        mkdir -p "builddir/$TYPE"

        libsToLink=""

        echo $DEVELOPER

        # loop through architectures! yay for loops!
        for IOS_ARCH in ${IOS_ARCHS}
        do
        	unset IOS_DEVROOT IOS_SDKROOT
            unset CC CPP CXX CXXCPP CFLAGS CXXFLAGS LDFLAGS LD AR AS NM RANLIB LIBTOOL 
            unset EXTRA_PLATFORM_CFLAGS EXTRA_PLATFORM_LDFLAGS
            unset CROSS_TOP CROSS_SDK BUILD_TOOLS PLATFORM 

            export CC=$TOOLCHAIN/usr/bin/clang
			export CPP=$TOOLCHAIN/usr/bin/clang++
			export CXX=$TOOLCHAIN/usr/bin/clang++
			export CXXCPP=$TOOLCHAIN/usr/bin/clang++
	
			export LD=$TOOLCHAIN/usr/bin/ld
			export AR=$TOOLCHAIN/usr/bin/ar
			export AS=$TOOLCHAIN/usr/bin/as
			export NM=$TOOLCHAIN/usr/bin/nm
			export RANLIB=$TOOLCHAIN/usr/bin/ranlib
			export LIBTOOL=$TOOLCHAIN/usr/bin/libtool

            echo "Building $IOS_ARCH "

			export PLATFORM=""
			if [[ "${IOS_ARCH}" == "i386" || "${IOS_ARCH}" == "x86_64" ]];
			then
				PLATFORM="iPhoneSimulator"
			
			else
				PLATFORM="iPhoneOS"
			fi

			export CROSS_TOP="${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer"
			export CROSS_SDK="${PLATFORM}.sdk"
			export BUILD_TOOLS="${DEVELOPER}"

           
            MIN_IOS_VERSION=$IOS_MIN_SDK_VER
		    # min iOS version for arm64 is iOS 7
		
		    if [[ "${IOS_ARCH}" == "arm64" || "${IOS_ARCH}" == "x86_64" ]]; then
		    	MIN_IOS_VERSION=7.0 # 7.0 as this is the minimum for these architectures
		    elif [ "${IOS_ARCH}" == "i386" ]; then
		    	MIN_IOS_VERSION=5.1 # 6.0 to prevent start linking errors
		    fi

		    MIN_TYPE=-miphoneos-version-min=
		    if [[ "${IOS_ARCH}" == "i386" || "${IOS_ARCH}" == "x86_64" ]]; then
		    	MIN_TYPE=-mios-simulator-version-min=
		    fi

			export EXTRA_PLATFORM_CFLAGS="$"
		    export EXTRA_PLATFORM_LDFLAGS="$ -L${CROSS_TOP}/SDKs/$CROSS_SDK/usr/lib/"

		    echo $EXTRA_PLATFORM_LDFLAGS

		    EXTRA_LINK_FLAGS="-arch $IOS_ARCH -stdlib=libc++ -Os -DHAVE_UNISTD_H=1 -DNDEBUG -fPIC "
			EXTRA_FLAGS="$EXTRA_LINK_FLAGS -pipe -no-cpp-precomp -funroll-loops $MIN_TYPE$MIN_IOS_VERSION -isysroot ${CROSS_TOP}/SDKs/${CROSS_SDK} -I${CROSS_TOP}/SDKs/${CROSS_SDK}/usr/include/"

			unset CFLAGS LDFLAGS CPPFLAGS CXXFLAGS DEVROOT SDKROOT
          
            export LDFLAGS="$EXTRA_LINK_FLAGS $EXTRA_PLATFORM_LDFLAGS -std=c++11"
            export DEVROOT="$CROSS_TOP"
            export SDKROOT="$CROSS_SDK"
            export CFLAGS="$EXTRA_FLAGS -std=c11"
            export CPPFLAGS="$EXTRA_FLAGS -std=c++11"
            export CXXFLAGS="$EXTRA_FLAGS -std=c++11"

            #echo " out c_flags are $OUR_CFLAGS "
            rm -f CMakeCache.txt
            cmake -G 'Unix Makefiles' -DCMAKE_TOOLCHAIN_FILE=./port/iOS/IPHONEOS_$(echo $IOS_ARCH | tr '[:lower:]' '[:upper:]')_TOOLCHAIN.cmake -DASSIMP_ENABLE_BOOST_WORKAROUND=1 -DASSIMP_BUILD_STATIC_LIB=1 -DBUILD_SHARED_LIBS=0 -DCMAKE_C_FLAGS="$EXTRA_FLAGS" -DCMAKE_CXX_FLAGS="$EXTRA_FLAGS" -DCMAKE_CXX_FLAGS="$EXTRA_FLAGS".

            $XCODE_DEV_ROOT/usr/bin/make clean

			echo "--------------------"
		    echo "Running make for ${IOS_ARCH}"
			echo "Please stand by..."

            $XCODE_DEV_ROOT/usr/bin/make assimp -j 8 -l

            fileToRenameTo="./builddir/$TYPE/libassimp-$IOS_ARCH.a"

            mv ./lib/libassimp.a $fileToRenameTo

            libsToLink="$libsToLink $fileToRenameTo"

            $XCODE_DEV_ROOT/usr/bin/make clean

            echo "--------------------"

        done

        mkdir -p "lib/$TYPE"
        cd "./builddir/$TYPE/"
		# link into universal lib
		echo "Running lipo to create fat lib"
        echo "Please stand by..."

        SDKVERSION=`xcrun -sdk iphoneos --show-sdk-version` 
        DEVELOPER=$XCODE_DEV_ROOT
        TOOLCHAIN=${DEVELOPER}/Toolchains/XcodeDefault.xctoolchain
        #           libassimp-armv7s.a \
        $TOOLCHAIN/usr/bin/lipo -create libassimp-armv7.a \
                    libassimp-arm64.a \
                    libassimp-i386.a \
                    libassimp-x86_64.a \
                    -output "../../lib/$TYPE/assimp.a" 

        cd ../../


        if [ $? != 0 ];
        then 
            echo "Problem while creating fat lib with lipo"
            exit 1
        else
            echo "Lipo Successful."
        fi

		echo "--------------------"
		echo "Stripping any lingering symbols"

		set -e
		CURRENTPATH=`pwd`

		cd lib/$TYPE
		SLOG="$CURRENTPATH/lib/$TYPE-stripping.log"
		local TOBESTRIPPED
		for TOBESTRIPPED in $( ls -1) ; do
			$TOOLCHAIN/usr/bin/strip -x $TOBESTRIPPED >> "${SLOG}" 2>&1
			if [ $? != 0 ];
		    then
                tail -n 100 "${LOG}"
		    	echo "Problem while stripping lib - Please check ${SLOG}"
		    	exit 1
		    else
		    	echo "Strip Successful for ${SLOG}"
		    fi
		done

		cd ../../
		echo "--------------------"

		echo "Completed Assimp for $TYPE"
	fi

	if [ "$TYPE" == "osx" ] ; then

		# warning, assimp on github uses the ASSIMP_ prefix for CMake options ...
		# these may need to be updated for a new release
		local buildOpts="--build build/$TYPE -DASSIMP_BUILD_STATIC_LIB=1 -DASSIMP_BUILD_SHARED_LIB=0 -DASSIMP_ENABLE_BOOST_WORKAROUND=1"

		# 32 bit
		cmake -G 'Unix Makefiles' $buildOpts -DCMAKE_C_FLAGS="-arch i386 -O3 -DNDEBUG -funroll-loops" -DCMAKE_CXX_FLAGS="-arch i386 -stdlib=libstdc++ -O3 -DNDEBUG -funroll-loops" .
		make assimp
		mv lib/libassimp.a lib/libassimp-osx-i386.a
		make clean

		# 64 bit
		cmake -G 'Unix Makefiles' $buildOpts -DCMAKE_C_FLAGS="-arch x86_64 -O3 -DNDEBUG -funroll-loops" -DCMAKE_CXX_FLAGS="-arch x86_64 -stdlib=libc++ -O3 -DNDEBUG -funroll-loops" .
		make assimp 
		mv lib/libassimp.a lib/libassimp-osx-x86_64.a
		make clean

		# link into universal lib
		lipo -c lib/libassimp-osx-i386.a lib/libassimp-osx-x86_64.a -o lib/libassimp-osx.a

	elif [ "$TYPE" == "osx-clang-libc++" ] ; then
		echoWarning "WARNING: this needs to be updated - do we even need it anymore?"

		# warning, assimp on github uses the ASSIMP_ prefix for CMake options ...
		# these may need to be updated for a new release
		local buildOpts="--build build/$TYPE"

		export CPP=`xcrun -find clang++`
		export CXX=`xcrun -find clang++`
		export CXXCPP=`xcrun -find clang++`
		export CC=`xcrun -find clang`
		
		# 32 bit
		cmake -G 'Unix Makefiles' $buildOpts -DCMAKE_C_FLAGS="-arch i386 $assimp_flags" -DCMAKE_CXX_FLAGS="-arch i386 -std=c++11 -stdlib=libc++ $assimp_flags" .
		make assimp -j 
		mv lib/libassimp.a lib/libassimp-i386.a
		make clean

		# rename lib
		libtool -c lib/libassimp-i386.a -o lib/libassimp-osx.a

	elif [ "$TYPE" == "linux" ] ; then
		echoWarning "TODO: linux build"

	elif [ "$TYPE" == "linux64" ] ; then
		echoWarning "TODO: linux64 build"

	elif [ "$TYPE" == "vs" ] ; then
		echoWarning "TODO: vs build"

	elif [ "$TYPE" == "win_cb" ] ; then
		echoWarning "TODO: win_cb build"

	elif [ "$TYPE" == "android" ] ; then
		echoWarning "TODO: android build"
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {

	# headers
	mkdir -p $1/include
    rm -r $1/include/assimp || true
    rm -r $1/include/* || true
	cp -Rv include/* $1/include

	# libs
	mkdir -p $1/lib/$TYPE
	if [ "$TYPE" == "vs" ] ; then
		cp -Rv lib/libassimp.lib $1/lib/$TYPE/assimp.lib
	elif [ "$TYPE" == "osx" ] ; then
		cp -Rv lib/libassimp-osx.a $1/lib/$TYPE/assimp.a
	elif [ "$TYPE" == "ios" ] ; then
		cp -Rv lib/$TYPE/assimp.a $1/lib/$TYPE/assimp.a
	else
		cp -Rv lib/libassimp.a $1/lib/$TYPE/assimp.a
	fi
}

# executed inside the lib src dir
function clean() {

	if [ "$TYPE" == "vs" ] ; then
		echoWarning "TODO: clean vs"

	elif [ "$TYPE" == "vs" ] ; then
		echoWarning "TODO: clean android"

	else
		make clean
		make rebuild_cache
		rm -f CMakeCache.txt
	fi
}
