#! /bin/bash
#
# tess2
# Game and tools oriented refactored version of GLU tesselator
# https://code.google.com/p/libtess2/
#
# has no build system, only an old Xcode project
# we follow the Homebrew approach which is to use CMake via a custom CMakeLists.txt
# on ios, use some build scripts adapted from the Assimp project

# define the version
FORMULA_TYPES=( "osx" "vs" "emscripten" "ios" "tvos" "android" "linux" "linux64" )

# define the version
VER=1.1

# tools for git use
GIT_URL=https://github.com/memononen/libtess2
GIT_TAG=master

CSTANDARD=c11 # c89 | c99 | c11 | gnu11
CPPSTANDARD=c++11 # c89 | c99 | c11 | gnu11
COMPILER_CTYPE=clang # clang, gcc
COMPILER_CPPTYPE=clang++ # clang, gcc
STDLIB=libc++

GIT_REV=24e4bdd4158909e9720422208ab0a0aca788e700

# download the source code and unpack it into LIB_NAME
function download() {
	curl --insecure -L $GIT_URL/archive/$GIT_REV.tar.gz -o libtess2-$GIT_REV.tar.gz
	tar -xf libtess2-$GIT_REV.tar.gz
	mv libtess2-$GIT_REV tess2
	rm libtess2*.tar.gz
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	
	# check if the patch was applied, if not then patch
	patch -p1 -u -N  < $FORMULA_DIR/tess2.patch
	# copy in build script and CMake toolchains adapted from Assimp
	if [ "$OS" == "osx" ] ; then
		mkdir -p build
	fi
}

# executed inside the lib src dir
function build() {
	
	# use CMake for the build using CMakeLists.txt from HomeBrew since the original source doesn't have one
	# see : https://github.com/mxcl/homebrew/pull/19634/files
	cp -v $FORMULA_DIR/CMakeLists.txt .
	
	if [ "$TYPE" == "osx" ] ; then

		OSX_ARCHS="i386 x86_64" 

		for OSX_ARCH in ${OSX_ARCHS}
		do

			unset CFLAGS CPPFLAGS LINKFLAGS CXXFLAGS LDFLAGS
			rm -f CMakeCache.txt
			set +e

			STD_LIB_FLAGS="-stdlib=libc++"
		
			
			OPTIM_FLAGS="-O3"				 # 	choose "fastest" optimisation

			export CFLAGS="-arch $OSX_ARCH $OPTIM_FLAGS -DNDEBUG -fPIC -mmacosx-version-min=${OSX_MIN_SDK_VER}"
			export CPPFLAGS=$CFLAGS
			export LINKFLAGS="$CFLAGS $STD_LIB_FLAGS"
			export LDFLAGS="$LINKFLAGS"
			export CXXFLAGS=$CPPFLAGS

			LOG="build-tess2-${VER}-${OSX_ARCH}-cmake.log"

			echo "Building library slice for ${OSX_ARCH}..."

			cmake -G 'Unix Makefiles' \
 				.
 			make clean >> "${LOG}" 2>&1
			make -j${PARALLEL_MAKE} >> "${LOG}" 2>&1


			# now we need to create a directory were we can keep our current build result.

			mkdir -p $TYPE
			mv libtess2.a $TYPE/libtess2-$OSX_ARCH.a

		done

		# combine into fat lib using lipo
		echo "Running lipo to create fat lib"
		echo "Please stand by..."

		LIPO_SLICES=	#initialise empty

		for OSX_ARCH in ${OSX_ARCHS}; do
			LIPO_SLICES+="${TYPE}/libtess2-${OSX_ARCH}.a "
		done

		LOG="build-tess2-${VER}-lipo.log"
		lipo -create $LIPO_SLICES \
			 -output build/libtess2.a \
			 > "${LOG}" 2>&1

	elif [ "$TYPE" == "vs" ] ; then
		if [ $ARCH == 32 ] ; then
			mkdir -p build_vs_32
			cd build_vs_32
			cmake .. -G "Visual Studio $VS_VER" -DCMAKE_CXX_FLAGS=-DNDEBUG -DCMAKE_C_FLAGS=-DNDEBUG
			vs-build "tess2.sln"
		elif [ $ARCH == 64 ] ; then
			mkdir -p build_vs_64
			cd build_vs_64
			cmake .. -G "Visual Studio $VS_VER Win64" -DCMAKE_CXX_FLAGS=-DNDEBUG -DCMAKE_C_FLAGS=-DNDEBUG
			vs-build "tess2.sln" Build "Release|x64"
		fi
		

	elif [[ "$TYPE" == "ios" || "${TYPE}" == "tvos" ]] ; then
	
		local IOS_ARCHS
        if [ "${TYPE}" == "tvos" ]; then 
            IOS_ARCHS="x86_64 arm64"
        elif [ "$TYPE" == "ios" ]; then
            IOS_ARCHS="i386 x86_64 armv7 arm64" #armv7s
        fi

		SDKVERSION=`xcrun -sdk iphoneos --show-sdk-version`	
		set -e
		CURRENTPATH=`pwd`
		
		DEVELOPER=$XCODE_DEV_ROOT
		TOOLCHAIN=${DEVELOPER}/Toolchains/XcodeDefault.xctoolchain

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
		
		export CC=$TOOLCHAIN/usr/bin/$COMPILER_CTYPE
		export CPP=$TOOLCHAIN/usr/bin/$COMPILER_CPPTYPE
		export CXX=$TOOLCHAIN/usr/bin/$COMPILER_CTYPE
		export CXXCPP=$TOOLCHAIN/usr/bin/$COMPILER_CPPTYPE

		export LD=$TOOLCHAIN/usr/bin/ld
		export AR=$TOOLCHAIN/usr/bin/ar
		export AS=$TOOLCHAIN/usr/bin/as
		export NM=$$TOOLCHAIN/usr/bin/nm
		export RANLIB=$TOOLCHAIN/usr/bin/ranlib

		SDKVERSION=""
        if [ "${TYPE}" == "tvos" ]; then 
            SDKVERSION=`xcrun -sdk appletvos --show-sdk-version`
        elif [ "$TYPE" == "ios" ]; then
            SDKVERSION=`xcrun -sdk iphoneos --show-sdk-version`
        fi
	
		EXTRA_LINK_FLAGS="-stdlib=libc++ -Os -fPIC"
		EXTRA_FLAGS="$EXTRA_LINK_FLAGS -fvisibility-inlines-hidden"
		
		# loop through architectures! yay for loops!
		for IOS_ARCH in ${IOS_ARCHS}
		do

			unset CFLAGS CPPFLAGS LINKFLAGS CXXFLAGS LDFLAGS
			rm -f CMakeCache.txt
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

			MIN_IOS_VERSION=$IOS_MIN_SDK_VER
		    if [[ "${IOS_ARCH}" == "arm64" || "${IOS_ARCH}" == "x86_64" ]]; then
		    	MIN_IOS_VERSION=7.0 # 7.0 as this is the minimum for these architectures
		    elif [ "${IOS_ARCH}" == "i386" ]; then
		    	MIN_IOS_VERSION=7.0 
		    fi

            if [ "${TYPE}" == "tvos" ]; then 
    		    MIN_TYPE=-mtvos-version-min=
    		    if [[ "${IOS_ARCH}" == "i386" || "${IOS_ARCH}" == "x86_64" ]]; then
    		    	MIN_TYPE=-mtvos-simulator-version-min=
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


			export CFLAGS="-arch $IOS_ARCH $BITCODE -DNDEBUG -pipe -no-cpp-precomp -isysroot ${CROSS_TOP}/SDKs/${CROSS_SDK} $MIN_TYPE$MIN_IOS_VERSION -I${CROSS_TOP}/SDKs/${CROSS_SDK}/usr/include/" 
	
			export CPPFLAGS=$CFLAGS
			export LINKFLAGS="$CFLAGS $EXTRA_LINK_FLAGS "
			export LDFLAGS="-L${CROSS_TOP}/SDKs/${CROSS_SDK}/usr/lib/ $LINKFLAGS -std=c++11 -stdlib=libc++"
			export CXXFLAGS="$CFLAGS $EXTRA_FLAGS"

			mkdir -p "$CURRENTPATH/builddir/$TYPE/$IOS_ARCH"
			LOG="$CURRENTPATH/builddir/$TYPE/$IOS_ARCH/build-tess2-${VER}-$IOS_ARCH.log"
			echo "-----------------"
			echo "Building tess2-${VER} for ${PLATFORM} ${SDKVERSION} ${IOS_ARCH} : iOS Minimum=$MIN_IOS_VERSION"
			set +e

			echo "Running make for ${IOS_ARCH}"
			echo "Please stand by..."

			cmake -G 'Unix Makefiles' 
			make clean >> "${LOG}" 2>&1
			make -j${PARALLEL_MAKE} >> "${LOG}" 2>&1

			if [ $? != 0 ];
		    then 
		    	tail -n 100 "${LOG}"
		    	echo "Problem while make - Please check ${LOG}"
		    	exit 1
		    else
		    	echo "Make Successful for ${IOS_ARCH}"
		    fi

			mv libtess2.a builddir/$TYPE/libtess2-$IOS_ARCH.a

		done

		echo "-----------------"
		echo `pwd`
		echo "Finished for all architectures."
		mkdir -p "$CURRENTPATH/builddir/$TYPE/"
		LOG="$CURRENTPATH/builddir/$TYPE/build-tess2-${VER}-lipo.log"

		mkdir -p "lib/$TYPE"

		# link into universal lib
		echo "Running lipo to create fat lib"
		echo "Please stand by..."

		if [[ "${TYPE}" == "tvos" ]]; then
			lipo -create builddir/$TYPE/libtess2-arm64.a \
			 	builddir/$TYPE/libtess2-x86_64.a \
			 	-output builddir/$TYPE/libtess2.a \
			 	>> "${LOG}" 2>&1
		 elif [[ "$TYPE" == "ios" ]]; then
		# builddir/$TYPE/libtess2-armv7s.a \
		lipo -create builddir/$TYPE/libtess2-armv7.a \
			 	builddir/$TYPE/libtess2-arm64.a \
			 	builddir/$TYPE/libtess2-i386.a \
			 	builddir/$TYPE/libtess2-x86_64.a \
			 	-output builddir/$TYPE/libtess2.a \
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

		mv builddir/$TYPE/libtess2.a lib/$TYPE/libtess2.a
		lipo -info lib/$TYPE/libtess2.a

		if [[ "$TYPE" == "ios" ]]; then
			echo "--------------------"
			echo "Stripping any lingering symbols"

			SLOG="$CURRENTPATH/lib/$TYPE/tess2-stripping.log"

			strip -x lib/$TYPE/libtess2.a >> "${SLOG}" 2>&1
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
		echo "Build Successful for Tess2 $TYPE"
		unset SDKROOT CFLAGS CC LD CPP CXX AR AS NM CXXCPP RANLIB LDFLAGS CPPFLAGS CXXFLAGS LINKFLAGS
		unset CROSS_TOP CROSS_SDK BUILD_TOOLS

	elif [ "$TYPE" == "android" ] ; then
		echoWarning "TODO: android build"
		
	elif [ "$TYPE" == "emscripten" ] ; then
    	cp -v $FORMULA_DIR/CMakeLists.txt .
    	mkdir -p build
    	cd build
    	emcmake cmake .. -DCMAKE_CXX_FLAGS=-DNDEBUG -DCMAKE_C_FLAGS=-DNDEBUG
    	emmake make -j${PARALLEL_MAKE}
	elif [ "$TYPE" == "linux64" ]; then
	    premake4 gmake
	    cd Build
	    make config=release64 tess2
	elif [ "$TYPE" == "linux" ]; then
	    premake4 gmake
	    cd Build
	    make config=release32 tess2
	else
		mkdir -p build/$TYPE
		cd build/$TYPE
		cmake -G "Unix Makefiles" -DCMAKE_CXX_COMPILER=/mingw32/bin/g++.exe -DCMAKE_C_COMPILER=/mingw32/bin/gcc.exe -DCMAKE_CXX_FLAGS=-DNDEBUG -DCMAKE_C_FLAGS=-DNDEBUG ../../
		make
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	
	# headers
	rm -rf $1/include
	mkdir -p $1/include
	cp -Rv Include/* $1/include/

	# lib
	mkdir -p $1/lib/$TYPE
	if [ "$TYPE" == "vs" ] ; then 
		if [ $ARCH == 32 ] ; then
			mkdir -p $1/lib/$TYPE/Win32
			cp -v build_vs_32/Release/tess2.lib $1/lib/$TYPE/Win32/tess2.lib
		elif [ $ARCH == 64 ] ; then
			mkdir -p $1/lib/$TYPE/x64
			cp -v build_vs_64/Release/tess2.lib $1/lib/$TYPE/x64/tess2.lib
		fi
	elif [[ "$TYPE" == "ios" || "$TYPE" == "tvos" ]]; then 
		cp -v lib/$TYPE/libtess2.a $1/lib/$TYPE/tess2.a

	elif [ "$TYPE" == "osx" ]; then
		cp -v build/libtess2.a $1/lib/$TYPE/tess2.a

	elif [ "$TYPE" == "emscripten" ]; then
		cp -v build/libtess2.a $1/lib/$TYPE/libtess2.a

	elif [ "$TYPE" == "linux64" ]; then
		cp -v Build/libtess2.a $1/lib/$TYPE/libtess2.a

	elif [ "$TYPE" == "linux32" ]; then
		cp -v Build/libtess2.a $1/lib/$TYPE/libtess2.a
		
	else
		cp -v build/$TYPE/libtess2.a $1/lib/$TYPE/libtess2.a
	fi

	# copy license files
	rm -rf $1/license # remove any older files if exists
	mkdir -p $1/license
	cp -v LICENSE.txt $1/license/
}

# executed inside the lib src dir
function clean() {

	if [ "$TYPE" == "vs" ] ; then
		rm -f CMakeCache.txt *.lib
	
	elif [ "$TYPE" == "android" ] ; then
		echoWarning "TODO: clean android"
	elif [[ "$TYPE" == "ios" || "$TYPE" == "tvos" ]]; then
		make clean
		rm -f CMakeCache.txt *.a *.lib
		rm -f builddir/$TYPE
		rm -f builddir
		rm -f lib
	else
		make clean
		rm -f CMakeCache.txt *.a *.lib
	fi
}
