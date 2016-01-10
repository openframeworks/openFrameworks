#! /bin/bash
#
# cpp-uri
# Uri parsing library that will become part of c++ std
# 
# uses cmake build system

FORMULA_TYPES=( "osx" "msys2" "ios" "android" "linux" "linux64" "linuxarmv6l" "linuxarmv7l" "emscripten" "vs" )
FORMULA_DEPENDS=( "boost" )
VER="master"

# define the version
COMMIT=55ec3cd78918c42dfb874e01c9745b4daf51091b
GIT_URL=https://github.com/cpp-netlib/uri.git

# download the source code and unpack it into LIB_NAME
function download() {
	git clone ${GIT_URL} uri
	cd uri
	git checkout ${COMMIT}
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	: # manually prepare dependencies

}

# executed inside the lib src dir
function build() {
	git checkout .
	rm -rf _build
	mkdir -p _build
	pwd
	if [ "$TYPE" == "wincb" ] ; then
	    git apply $FORMULA_DIR/uri-remove-tests.patch
	    cd _build
		#noop by now
		
	elif [ "$TYPE" == "vs" ] ; then
	    git apply $FORMULA_DIR/uri-remove-tests.patch
	    mkdir -p build_vs$ARCH
		cd build_vs$ARCH
		export BOOST_INCLUDEDIR=${BUILD_DIR}/boost/
		export BOOST_LIBRARYDIR=${BUILD_DIR}/boost/stage_$ARCH/lib
		if [ $ARCH == 32 ] ; then
			cmake .. -G "Visual Studio $VS_VER"  
			vs-build "ALL_BUILD.vcxproj"
			vs-build "ALL_BUILD.vcxproj" Build "Debug"
		elif [ $ARCH == 64 ] ; then
			cmake .. -G "Visual Studio $VS_VER Win64"  
			vs-build "ALL_BUILD.vcxproj" Build "Release|x64"
			vs-build "ALL_BUILD.vcxproj" Build "Debug|x64"
		fi
	
	elif [ "$TYPE" == "osx" ]; then
	    git apply $FORMULA_DIR/uri-remove-tests.patch
	    git apply $FORMULA_DIR/uri_xcode_fix.patch
	    cd _build
		export BOOST_LIBRARYDIR=${BUILD_DIR}/boost/stage/lib
		export BOOST_INCLUDEDIR=${BUILD_DIR}/boost/
		cmake -DCMAKE_BUILD_TYPE=Release \
			  -DCMAKE_C_FLAGS="-arch i386 -arch x86_64" \
			  -DCMAKE_CXX_FLAGS="-arch i386 -arch x86_64 -std=c++11 -stdlib=libc++" \
			  ..
		make -j${PARALLEL_MAKE}
	
	elif [ "$TYPE" == "ios" ]; then
        git apply $FORMULA_DIR/uri-remove-tests.patch
        git apply $FORMULA_DIR/uri_xcode_fix.patch
        export TOOLCHAIN=$XCODE_DEV_ROOT/Toolchains/XcodeDefault.xctoolchain
        export TARGET_IOS
        
        local IOS_ARCHS="i386 x86_64 armv7 arm64" #armv7s
        local STDLIB="libc++"
        local CURRENTPATH=`pwd`

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

       
        mkdir -p "$CURRENTPATH/libs/$TYPE"
        rm -rf _build/


        # loop through architectures! yay for loops!
        for IOS_ARCH in ${IOS_ARCHS}
        do
            mkdir -p "$CURRENTPATH/_build"
            unset ARCH IOS_DEVROOT IOS_SDKROOT IOS_CC TARGET_NAME HEADER
            unset CC CPP CXX CXXCPP CFLAGS CXXFLAGS LDFLAGS LD AR AS NM RANLIB LIBTOOL 
            unset EXTRA_PLATFORM_CFLAGS EXTRA_PLATFORM_LDFLAGS IOS_PLATFORM

            export ARCH=$IOS_ARCH
            
            local EXTRA_PLATFORM_CFLAGS=""
            export EXTRA_PLATFORM_LDFLAGS=""
            if [[ "${IOS_ARCH}" == "i386" || "${IOS_ARCH}" == "x86_64" ]];
            then
                PLATFORM="iPhoneSimulator"
            
            else
                PLATFORM="iPhoneOS"
            fi

            export CROSS_TOP="${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer"
            export CROSS_SDK="${PLATFORM}${SDKVERSION}.sdk"
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

            export BOOST_LIBRARYDIR=${BUILD_DIR}/boost/lib/ios
            export BOOST_INCLUDEDIR=${BUILD_DIR}/boost
            export Boost_DIR=${BUILD_DIR}/boost
            export Boost_FILESYSTEM_LIBRARY_DEBUG=${BOOST_LIBRARYDIR}/boost_filesystem.a
            export Boost_FILESYSTEM_LIBRARY_RELEASE=${Boost_FILESYSTEM_LIBRARY_DEBUG}
            export Boost_LIBRARY_DIR=${BOOST_LIBRARYDIR}
            export Boost_SYSTEM_LIBRARY_DEBUG=${BOOST_LIBRARYDIR}/boost_system.a
            export Boost_SYSTEM_LIBRARY_RELEASE=${BOOST_LIBRARYDIR}/boost_system.a


            export EXTRA_PLATFORM_CFLAGS="$EXTRA_PLATFORM_CFLAGS"
            export EXTRA_PLATFORM_LDFLAGS="$EXTRA_PLATFORM_LDFLAGS -isysroot ${CROSS_TOP}/SDKs/${CROSS_SDK} -Wl,-dead_strip -I${CROSS_TOP}/SDKs/${CROSS_SDK}/usr/include/ $MIN_TYPE$MIN_IOS_VERSION "

            EXTRA_LINK_FLAGS="-arch $IOS_ARCH -Os -DNDEBUG -fPIC"
            EXTRA_FLAGS="$EXTRA_LINK_FLAGS $MIN_TYPE$MIN_IOS_VERSION -isysroot ${CROSS_TOP}/SDKs/${CROSS_SDK} -I${CROSS_TOP}/SDKs/${CROSS_SDK}/usr/include/"

            export CC="$CC $EXTRA_FLAGS"
            export CFLAGS="-arch $IOS_ARCH $EXTRA_FLAGS"
            export CXXFLAGS="$EXTRA_FLAGS -std=c++11 -stdlib=libc++"
            export LDFLAGS="-arch $IOS_ARCH $EXTRA_PLATFORM_LDFLAGS $EXTRA_LINK_FLAGS $MIN_TYPE$MIN_IOS_VERSION -std=c++11 -stdlib=libc++"
            export LDFLAGS_PHONE=$LDFLAGS

            mkdir -p "$CURRENTPATH/libs/$TYPE/"
        
            echo "-----------------"
            echo "Building URI-${VER} for ${PLATFORM} ${SDKVERSION} ${IOS_ARCH} : iOS Minimum=$MIN_IOS_VERSION"
            set +e

            echo "Running make for ${IOS_ARCH}"
            echo "Please stand by..."

            cd "_build"
            cmake -DCMAKE_BUILD_TYPE=MINSIZEREL \
                  -DCMAKE_C_FLAGS="$CFLAGS" \
                  -DCMAKE_CXX_FLAGS="-I${BOOST_INCLUDEDIR} $CXXFLAGS" \
                  -DBOOST_LIBRARYDIR="${BOOST_LIBRARYDIR}" \
                  -DBoost_INCLUDE_DIR="${BOOST_INCLUDEDIR}" \
                  -DBoost_DIR="${Boost_DIR}" \
                  -DBoost_FILESYSTEM_LIBRARY_DEBUG="${Boost_FILESYSTEM_LIBRARY_DEBUG}" \
                  -DBoost_FILESYSTEM_LIBRARY_RELEASE="${Boost_FILESYSTEM_LIBRARY_RELEASE}" \
                  -DBoost_LIBRARY_DIR="${Boost_LIBRARY_DIR}" \
                  -DBoost_SYSTEM_LIBRARY_DEBUG="${Boost_SYSTEM_LIBRARY_DEBUG}" \
                  -DBoost_SYSTEM_LIBRARY_RELEASE="${Boost_SYSTEM_LIBRARY_RELEASE}" \
                  ..
            make -j${PARALLEL_MAKE}
            # Copy compiled type lib to common store so we can fat lib at the end.
            cp src/libnetwork-uri.a "$CURRENTPATH/libs/$TYPE/libnetwork-uri-$IOS_ARCH.a"
            echo "Completed Build for $IOS_ARCH of URI"
            cd ../ # back from _build
            # now remove the last build and repeat
            rm -rf _build/
        done
        echo "Completed Build for $TYPE"
        echo "---------------------------"
        cd "libs/$TYPE/"
        echo "Running lipo to create fat lib"
        echo "Please stand by..."
        lipo -create libnetwork-uri-armv7.a \
                    libnetwork-uri-arm64.a \
                    libnetwork-uri-i386.a \
                    libnetwork-uri-x86_64.a \
                    -output network-uri.a

        echo "lipo successful"
        lipo -info network-uri.a
        echo "--------------------"
        echo "Stripping any lingering symbols with strip -x"
        echo "Please stand by..."
        # validate all stripped debug:
        strip -x network-uri.a
        echo "stripping successful"
        echo "--------------------"
        cd ../../
        echo "URI Completely Built for $TYPE"
    
    elif [ "$TYPE" == "emscripten" ]; then
		export BOOST_LIBRARYDIR=${BUILD_DIR}/boost/stage/lib
		export BOOST_INCLUDEDIR=${BUILD_DIR}/boost/
	    git apply $FORMULA_DIR/uri-emscripten.patch
	    cd _build
    	pwd
		emcmake cmake .. -DCMAKE_CXX_FLAGS=-I${BOOST_INCLUDEDIR} -DCMAKE_BUILD_TYPE=Release -DBOOST_LIBRARYDIR=${BUILD_DIR}/boost/stage/lib -DBoost_INCLUDE_DIR=${BUILD_DIR}/boost
		emmake make -j${PARALLEL_MAKE}
	
	elif [ "$TYPE" == "android" ]; then
	    git apply $FORMULA_DIR/uri-remove-tests.patch
		export BOOST_INCLUDEDIR=${BUILD_DIR}/boost/
	    
	    ABI=armeabi-v7a
	    source ../../android_configure.sh $ABI
		export BOOST_LIBRARYDIR=${BUILD_DIR}/boost/stage_arm/lib
		mkdir -p build_arm
		cd build_arm
		cmake .. -DCMAKE_CXX_FLAGS="-I${BOOST_INCLUDEDIR} $CFLAGS" -DCMAKE_BUILD_TYPE=Release -DBOOST_LIBRARYDIR=${BUILD_DIR}/boost/stage_arm/lib -DBoost_INCLUDE_DIR=${BUILD_DIR}/boost
		make -j${PARALLEL_MAKE}
		cd ..
		
	    ABI=x86
	    source ../../android_configure.sh $ABI
		export BOOST_LIBRARYDIR=${BUILD_DIR}/boost/stage_x86/lib
		mkdir -p build_x86
		cd build_x86
		cmake .. -DCMAKE_CXX_FLAGS="-I${BOOST_INCLUDEDIR} $CFLAGS" -DCMAKE_BUILD_TYPE=Release -DBOOST_LIBRARYDIR=${BUILD_DIR}/boost/stage_x86/lib -DBoost_INCLUDE_DIR=${BUILD_DIR}/boost
		make -j${PARALLEL_MAKE}
		cd ..
	
	elif [ "$TYPE" == "linux" ] || [ "$TYPE" == "linux64" ] || [ "$TYPE" == "linuxarmv6l" ] || [ "$TYPE" == "linuxarmv7l" ]; then
	    git apply $FORMULA_DIR/uri-remove-tests.patch
	    cd _build
		cmake -DCMAKE_BUILD_TYPE=Release ..
		make -j${PARALLEL_MAKE}
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	# prepare headers directory if needed
	mkdir -p $1/include/network

	# prepare libs directory if needed
	mkdir -p $1/lib/$TYPE
	mkdir -p install_dir

	if [ "$TYPE" == "wincb" ] ; then
		: #noop by now
		
	elif [ "$TYPE" == "vs" ] ; then
		if [ $ARCH == 32 ] ; then
			mkdir -p $1/lib/$TYPE/Win32
			cp -v build_vs32/src/Release/network-uri.lib $1/lib/$TYPE/Win32/
			cp -v build_vs32/src/Debug/network-uri.lib $1/lib/$TYPE/Win32/network-uri_debug.lib
		elif [ $ARCH == 64 ] ; then
			mkdir -p $1/lib/$TYPE/x64
			cp -v build_vs64/src/Release/network-uri.lib $1/lib/$TYPE/x64/
			cp -v build_vs64/src/Debug/network-uri.lib $1/lib/$TYPE/x64/network-uri_debug.lib
		fi
		
	elif [ "$TYPE" == "osx" ]; then
		cp _build/src/*.a $1/lib/$TYPE/
		cp -r src/network $1/include/
		../boost/dist/bin/bcp --scan --boost=../boost $(find src/network/ -name "*.hpp") install_dir
		rsync -ar install_dir/boost/* $1/../boost/include/boost/
    elif [ "$TYPE" == "ios" ]; then
        cp -v libs/$TYPE/network-uri.a $1/lib/$TYPE/
        cp -r src/network $1/include/
        #../boost/dist/bin/bcp --scan --boost=../boost $(find src/network/ -name "*.hpp") install_dir
        #rsync -ar install_dir/boost/* $1/../boost/include/boost/
	elif [ "$TYPE" == "emscripten" ]; then
        cp _build/src/*.a $1/lib/$TYPE/
        cp -r src/network $1/include/
    elif [ "$TYPE" == "android" ]; then
        rm -r $1/lib/$TYPE/armeabi-v7a
        rm -r $1/lib/$TYPE/x86
        mkdir -p $1/lib/$TYPE/armeabi-v7a
        mkdir -p $1/lib/$TYPE/x86
        cp build_arm/src/*.a $1/lib/$TYPE/armeabi-v7a/
        cp build_x86/src/*.a $1/lib/$TYPE/x86/
        cp -r src/network $1/include/
	fi

	# copy license file
	rm -rf $1/license # remove any older files if exists
	mkdir -p $1/license
	cp -v LICENSE_1_0.txt $1/license/
}

# executed inside the lib src dir
function clean() {
	if [ "$TYPE" == "wincb" ] ; then
		rm -f *.lib
	else
		./b2 --clean
	fi
}
