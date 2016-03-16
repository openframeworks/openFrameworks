#! /bin/bash
#
# Free Image
# cross platform image io
# http://freeimage.sourceforge.net
#
# Makefile build system, 
# some Makefiles are out of date so patching/modification may be required

FORMULA_TYPES=( "osx" "vs" "msys2" "ios" "tvos" "android" "emscripten")

# define the version
VER=3170 # 3.16.0

# tools for git use
GIT_URL=https://github.com/danoli3/FreeImage
GIT_TAG=3.17.0-header-changes

# download the source code and unpack it into LIB_NAME
function download() {

	if [ "$TYPE" == "vs" -o "$TYPE" == "msys2" ] ; then
		# For win32, we simply download the pre-compiled binaries.
		curl -LO http://downloads.sourceforge.net/freeimage/FreeImage"$VER"Win32Win64.zip
		unzip -qo FreeImage"$VER"Win32Win64.zip
		rm FreeImage"$VER"Win32Win64.zip
	elif [[ "${TYPE}" == "osx" || "${TYPE}" == "ios" || "${TYPE}" == "tvos" ]]; then
        # Fixed issues for OSX / iOS for FreeImage compiling in git repo.
        echo "Downloading from $GIT_URL $GIT_TAG for OSX/iOS"
		echo $GIT_URL
		curl -Lk $GIT_URL/archive/$GIT_TAG.tar.gz -o FreeImage-$GIT_TAG.tar.gz
		tar -xf FreeImage-$GIT_TAG.tar.gz
		mv FreeImage-$GIT_TAG FreeImage
		rm FreeImage-$GIT_TAG.tar.gz
	else
		curl -LO http://downloads.sourceforge.net/freeimage/FreeImage"$VER".zip
		unzip -qo FreeImage"$VER".zip
		rm FreeImage"$VER".zip
	fi
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	
	if [ "$TYPE" == "osx" ] ; then

		cp -rf $FORMULA_DIR/Makefile.osx Makefile.osx

		# set SDK using apothecary settings
		sed -i tmp "s|MACOSX_SDK =.*|MACOSX_SDK = $OSX_SDK_VER|" Makefile.osx
		sed -i tmp "s|MACOSX_MIN_SDK =.*|MACOSX_MIN_SDK = $OSX_MIN_SDK_VER|" Makefile.osx

	elif [[ "$TYPE" == "ios" || "${TYPE}" == "tvos" ]] ; then

		mkdir -p Dist/$TYPE
		mkdir -p builddir/$TYPE

		# copy across new Makefile for iOS.
		cp -v $FORMULA_DIR/Makefile.ios Makefile.ios
	elif [ "$TYPE" == "android" ]; then
	local BUILD_TO_DIR=$BUILD_DIR/FreeImage_patched
	cp -r $BUILD_DIR/FreeImage $BUILD_DIR/FreeImage_patched
	cd $BUILD_DIR/FreeImage_patched
	    sed -i "s/#define HAVE_SEARCH_H/\/\/#define HAVE_SEARCH_H/g" Source/LibTIFF4/tif_config.h
	    cat > Source/LibRawLite/src/swab.h << ENDDELIM
	    #include <stdint.h>
        #include <asm/byteorder.h>
        inline void swab(const void *from, void*to, ssize_t n)
        {
            ssize_t i;
            if (n < 0)
                return;
            for (i = 0; i < (n/2)*2; i += 2)
                *((uint16_t*)to+i) = __arch__swab16(*((uint16_t*)from+i));
        }
ENDDELIM
        
        sed -i "s/#include \"swab.h\"//g" Source/LibRawLite/internal/dcraw_common.cpp
        echo "#include \"swab.h\"" > Source/LibRawLite/internal/dcraw_common_patched.cpp;
        cat Source/LibRawLite/internal/dcraw_common.cpp >> Source/LibRawLite/internal/dcraw_common_patched.cpp
        cat Source/LibRawLite/internal/dcraw_common_patched.cpp > Source/LibRawLite/internal/dcraw_common.cpp
        rm Source/LibRawLite/internal/dcraw_common_patched.cpp
        
        sed -i "s/#include \"swab.h\"//g" Source/LibRawLite/src/libraw_cxx.cpp
        echo "#include \"swab.h\"" > Source/LibRawLite/src/libraw_cxx_patched.cpp
        cat Source/LibRawLite/src/libraw_cxx.cpp >> Source/LibRawLite/src/libraw_cxx_patched.cpp
        cat Source/LibRawLite/src/libraw_cxx_patched.cpp > Source/LibRawLite/src/libraw_cxx.cpp
        rm Source/LibRawLite/src/libraw_cxx_patched.cpp
        
        #rm Source/LibWebP/src/dsp/dec_neon.c
        
        sed -i "s/#define WEBP_ANDROID_NEON/\/\/#define WEBP_ANDROID_NEON/g" Source/LibWebP/./src/dsp/dsp.h
	fi
}

# executed inside the lib src dir
function build() {
	
	if [ "$TYPE" == "osx" ] ; then
		make -j${PARALLEL_MAKE} -f Makefile.osx

		strip -x Dist/libfreeimage.a

	elif [[ "$TYPE" == "ios" || "${TYPE}" == "tvos" ]] ; then

		# Notes: 
        # --- for 3.1+ Must use "-DNO_LCMS -D__ANSI__ -DDISABLE_PERF_MEASUREMENT" to compile LibJXR
		export TOOLCHAIN=$XCODE_DEV_ROOT/Toolchains/XcodeDefault.xctoolchain
		export TARGET_IOS
        
        local IOS_ARCHS
        if [ "${TYPE}" == "tvos" ]; then 
            IOS_ARCHS="x86_64 arm64"
        elif [ "$TYPE" == "ios" ]; then
            IOS_ARCHS="i386 x86_64 armv7 arm64" #armv7s
        fi

        local STDLIB="libc++"
        local CURRENTPATH=`pwd`

        SDKVERSION=""
        if [ "${TYPE}" == "tvos" ]; then 
            SDKVERSION=`xcrun -sdk appletvos --show-sdk-version`
        elif [ "$TYPE" == "ios" ]; then
            SDKVERSION=`xcrun -sdk iphoneos --show-sdk-version`
        fi

			
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

        # loop through architectures! yay for loops!
        for IOS_ARCH in ${IOS_ARCHS}
        do

        	unset ARCH IOS_DEVROOT IOS_SDKROOT IOS_CC TARGET_NAME HEADER
            unset CC CPP CXX CXXCPP CFLAGS CXXFLAGS LDFLAGS LD AR AS NM RANLIB LIBTOOL 
            unset EXTRA_PLATFORM_CFLAGS EXTRA_PLATFORM_LDFLAGS IOS_PLATFORM NO_LCMS

            export ARCH=$IOS_ARCH
            
            local EXTRA_PLATFORM_CFLAGS=""
			export EXTRA_PLATFORM_LDFLAGS=""
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
		    # min iOS version for arm64 is iOS 7
		
		    if [[ "${IOS_ARCH}" == "arm64" || "${IOS_ARCH}" == "x86_64" ]]; then
		    	MIN_IOS_VERSION=7.0 # 7.0 as this is the minimum for these architectures
		    elif [ "${IOS_ARCH}" == "i386" ]; then
		    	MIN_IOS_VERSION=7.0 # 6.0 to prevent start linking errors
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
            
			export TARGET_NAME="$CURRENTPATH/libfreeimage-$IOS_ARCH.a"
			export HEADER="Source/FreeImage.h"

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

		  	export EXTRA_PLATFORM_CFLAGS="$EXTRA_PLATFORM_CFLAGS"
			export EXTRA_PLATFORM_LDFLAGS="$EXTRA_PLATFORM_LDFLAGS -isysroot ${CROSS_TOP}/SDKs/${CROSS_SDK} -Wl,-dead_strip -I${CROSS_TOP}/SDKs/${CROSS_SDK}/usr/include/ $MIN_TYPE$MIN_IOS_VERSION "

		   	EXTRA_LINK_FLAGS="-arch $IOS_ARCH $BITCODE -fmessage-length=0 -fdiagnostics-show-note-include-stack -fmacro-backtrace-limit=0 -Wno-trigraphs -fpascal-strings -Os -Wno-missing-field-initializers -Wno-missing-prototypes -Wno-return-type -Wno-non-virtual-dtor -Wno-overloaded-virtual -Wno-exit-time-destructors -Wno-missing-braces -Wparentheses -Wswitch -Wno-unused-function -Wno-unused-label -Wno-unused-parameter -Wno-unused-variable -Wunused-value -Wno-empty-body -Wno-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wno-constant-conversion -Wno-int-conversion -Wno-bool-conversion -Wno-enum-conversion -Wno-shorten-64-to-32 -Wno-newline-eof -Wno-c++11-extensions -DHAVE_UNISTD_H=1 -DOPJ_STATIC -DNO_LCMS -D__ANSI__ -DDISABLE_PERF_MEASUREMENT -DLIBRAW_NODLL -DLIBRAW_LIBRARY_BUILD -DFREEIMAGE_LIB -fexceptions -fasm-blocks -fstrict-aliasing -Wdeprecated-declarations -Winvalid-offsetof -Wno-sign-conversion -Wmost -Wno-four-char-constants -Wno-unknown-pragmas -DNDEBUG -fPIC -fexceptions -fvisibility=hidden"
			EXTRA_FLAGS="$EXTRA_LINK_FLAGS $BITCODE -DNDEBUG -ffast-math -DPNG_ARM_NEON_OPT=0 -DDISABLE_PERF_MEASUREMENT $MIN_TYPE$MIN_IOS_VERSION -isysroot ${CROSS_TOP}/SDKs/${CROSS_SDK} -I${CROSS_TOP}/SDKs/${CROSS_SDK}/usr/include/"

		    export CC="$CC $EXTRA_FLAGS"
			export CFLAGS="-arch $IOS_ARCH $EXTRA_FLAGS"
			export CXXFLAGS="$EXTRA_FLAGS -std=c++11 -stdlib=libc++"
			export LDFLAGS="-arch $IOS_ARCH $EXTRA_PLATFORM_LDFLAGS $EXTRA_LINK_FLAGS $MIN_TYPE$MIN_IOS_VERSION -std=c++11 -stdlib=libc++"
			export LDFLAGS_PHONE=$LDFLAGS

			mkdir -p "$CURRENTPATH/builddir/$TYPE/$IOS_ARCH"
			LOG="$CURRENTPATH/builddir/$TYPE/$IOS_ARCH/build-freeimage-${VER}-$IOS_ARCH.log"
			echo "-----------------"
			echo "Building FreeImage-${VER} for ${PLATFORM} ${SDKVERSION} ${IOS_ARCH} : iOS Minimum=$MIN_IOS_VERSION"
			set +e

			echo "Running make for ${IOS_ARCH}"
			echo "Please stand by..."

			# run makefile
			make -j${PARALLEL_MAKE} -f Makefile.ios >> "${LOG}" 2>&1
			if [ $? != 0 ];
		    then 
                tail -n 100 "${LOG}"
		    	echo "Problem while make - Please check ${LOG}"
		    	exit 1
		    else
		    	echo "Make Successful for ${IOS_ARCH}"
		    fi
		    
     		echo "Completed Build for $IOS_ARCH of FreeImage"

     		mv -v libfreeimage-$IOS_ARCH.a Dist/$TYPE/libfreeimage-$IOS_ARCH.a

     		cp Source/FreeImage.h Dist

            unset ARCH IOS_DEVROOT IOS_SDKROOT IOS_CC TARGET_NAME HEADER
            unset CC CPP CXX CXXCPP CFLAGS CXXFLAGS LDFLAGS LD AR AS NM RANLIB LIBTOOL 
            unset EXTRA_PLATFORM_CFLAGS EXTRA_PLATFORM_LDFLAGS IOS_PLATFORM NO_LCMS

		done

		echo "Completed Build for $TYPE"

        echo "-----------------"
		echo `pwd`
		echo "Finished for all architectures."
		mkdir -p "$CURRENTPATH/builddir/$TYPE/$IOS_ARCH"
		LOG="$CURRENTPATH/builddir/$TYPE/build-freeimage-${VER}-lipo.log"

		cd Dist/$TYPE/
		# link into universal lib
		echo "Running lipo to create fat lib"
		echo "Please stand by..."
        if [[ "${TYPE}" == "tvos" ]] ; then 
            lipo -create libfreeimage-arm64.a \
                    libfreeimage-x86_64.a \
                    -output freeimage.a >> "${LOG}" 2>&1
        elif [[ "$TYPE" == "ios" ]]; then
		    #			libfreeimage-armv7s.a \
		    lipo -create libfreeimage-armv7.a \
					libfreeimage-arm64.a \
					libfreeimage-i386.a \
					libfreeimage-x86_64.a \
					-output freeimage.a >> "${LOG}" 2>&1
        fi

		if [ $? != 0 ];
		then 
            tail -n 100 "${LOG}"
		    echo "Problem while creating fat lib with lipo - Please check ${LOG}"
		    exit 1
		else
		   	echo "Lipo Successful."
		fi

		lipo -info freeimage.a

        if [[ "$TYPE" == "ios" ]]; then
    		echo "--------------------"
    		echo "Stripping any lingering symbols"
    		echo "Please stand by..."
    		# validate all stripped debug:
    		strip -x freeimage.a  >> "${LOG}" 2>&1
    		if [ $? != 0 ];
    		then 
                tail -n 10 "${LOG}"
    		    echo "Problem while stripping lib - Please check ${LOG}"
    		    exit 1
    		else
    		    echo "Strip Successful for ${LOG}"
    		fi
        fi
		cd ../../

		echo "--------------------"
		echo "Build Successful for FreeImage $TYPE $VER"

		# include copied in the makefile to libs/$TYPE/include
		unset TARGET_IOS
		unset TOOLCHAIN

	elif [ "$TYPE" == "android" ] ; then
        source $LIBS_DIR/openFrameworksCompiled/project/android/paths.make
        local BUILD_TO_DIR=$BUILD_DIR/FreeImage/build/$TYPE
        rm -rf $BUILD_DIR/FreeImagePatched
        cp -r $BUILD_DIR/FreeImage $BUILD_DIR/FreeImagePatched
        cd $BUILD_DIR/FreeImagePatched
        
        # armv7
        ABI=armeabi-v7a
        local BUILD_TO_DIR=$BUILD_DIR/FreeImagePatched/build/$TYPE/$ABI
        source ../../android_configure.sh $ABI
        export CC="$CC $CFLAGS $LDFLAGS"
        export CXX="$CXX $CFLAGS $LDFLAGS"
        make clean -f Makefile.gnu
        make -j${PARALLEL_MAKE} -f Makefile.gnu libfreeimage.a
        mkdir -p $BUILD_DIR/FreeImage/Dist/$ABI
        mv libfreeimage.a $BUILD_DIR/FreeImage/Dist/$ABI
        
        # x86
        ABI=x86
        local BUILD_TO_DIR=$BUILD_DIR/FreeImagePatched/build/$TYPE/$ABI
        source ../../android_configure.sh $ABI
        export CC="$CC $CFLAGS $LDFLAGS"
        export CXX="$CXX $CFLAGS $LDFLAGS"
        make clean -f Makefile.gnu
        make -j${PARALLEL_MAKE} -f Makefile.gnu libfreeimage.a
        mkdir -p $BUILD_DIR/FreeImage/Dist/$ABI
        mv libfreeimage.a $BUILD_DIR/FreeImage/Dist/$ABI
        cd $BUILD_DIR/FreeImage
        #rm -r $BUILD_DIR/FreeImage_patched
    elif [ "$TYPE" == "emscripten" ]; then
        local BUILD_TO_DIR=$BUILD_DIR/FreeImage/build/$TYPE
        rm -rf $BUILD_DIR/FreeImagePatched
        cp -r $BUILD_DIR/FreeImage $BUILD_DIR/FreeImagePatched
        echo "#include <unistd.h>" > $BUILD_DIR/FreeImagePatched/Source/ZLib/gzlib.c
        cat $BUILD_DIR/FreeImage/Source/ZLib/gzlib.c >> $BUILD_DIR/FreeImagePatched/Source/ZLib/gzlib.c
        echo "#include <unistd.h>" > $BUILD_DIR/FreeImagePatched/Source/ZLib/gzread.c
        cat $BUILD_DIR/FreeImage/Source/ZLib/gzread.c >> $BUILD_DIR/FreeImagePatched/Source/ZLib/gzread.c
        echo "#include <unistd.h>" > $BUILD_DIR/FreeImagePatched/Source/ZLib/gzwrite.c
        cat $BUILD_DIR/FreeImage/Source/ZLib/gzread.c >> $BUILD_DIR/FreeImagePatched/Source/ZLib/gzwrite.c
        echo "" > $BUILD_DIR/FreeImagePatched/Source/LibRawLite/src/swab.h
        echo "#include <byteswap.h>" > $BUILD_DIR/FreeImagePatched/Source/LibJXR/image/decode/segdec.c
        echo "#define _byteswap_ulong __bswap_32" >> $BUILD_DIR/FreeImagePatched/Source/LibJXR/image/decode/segdec.c
        cat $BUILD_DIR/FreeImage/Source/LibJXR/image/decode/segdec.c >> $BUILD_DIR/FreeImagePatched/Source/LibJXR/image/decode/segdec.c
        echo "#include <wchar.h>" > $BUILD_DIR/FreeImagePatched/Source/LibJXR/jxrgluelib/JXRGlueJxr.c
        cat $BUILD_DIR/FreeImage/Source/LibJXR/jxrgluelib/JXRGlueJxr.c >> $BUILD_DIR/FreeImagePatched/Source/LibJXR/jxrgluelib/JXRGlueJxr.c
        cd $BUILD_DIR/FreeImagePatched
        emmake make clean -f Makefile.gnu
        emmake make -j${PARALLEL_MAKE} -f Makefile.gnu libfreeimage.a
        mkdir -p $BUILD_DIR/FreeImage/Dist/
        mv libfreeimage.a $BUILD_DIR/FreeImage/Dist/
        cd $BUILD_DIR/FreeImage
        #rm -rf $BUILD_DIR/FreeImagePatched
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	
	# headers
	if [ -d $1/include ]; then
	    rm -rf $1/include
	fi
	mkdir -p $1/include

	# lib
	if [ "$TYPE" == "osx" ] ; then
	    cp -v Dist/*.h $1/include
		mkdir -p $1/lib/$TYPE
		cp -v Dist/libfreeimage.a $1/lib/$TYPE/freeimage.a
	elif [ "$TYPE" == "vs" -o "$TYPE" == "msys2" ] ; then
		mkdir -p $1/include #/Win32
		#mkdir -p $1/include/x64
	    cp -v Dist/x32/*.h $1/include #/Win32/
		#cp -v Dist/x64/*.h $1/include/x64/
		mkdir -p $1/lib/$TYPE/Win32
		mkdir -p $1/lib/$TYPE/x64
		cp -v Dist/x32/FreeImage.lib $1/lib/$TYPE/Win32/FreeImage.lib
		cp -v Dist/x32/FreeImage.dll $1/../../export/$TYPE/FreeImage32.dll
		cp -v Dist/x64/FreeImage.lib $1/lib/$TYPE/x64/FreeImage.lib
		cp -v Dist/x64/FreeImage.dll $1/../../export/$TYPE/FreeImage64.dll
	elif [[ "$TYPE" == "ios" || "$TYPE" == "tvos" ]] ; then
        cp -v Dist/*.h $1/include
        if [ -d $1/lib/$TYPE/ ]; then
            rm -r $1/lib/$TYPE/
        fi
       	mkdir -p $1/lib/$TYPE
		cp -v Dist/$TYPE/freeimage.a $1/lib/$TYPE/freeimage.a

	elif [ "$TYPE" == "android" ] ; then
        cp Source/FreeImage.h $1/include
        if [ -d $1/lib/$TYPE/ ]; then
            rm -r $1/lib/$TYPE/
        fi
        mkdir -p $1/lib/$TYPE/armeabi-v7a
        cp -rv Dist/armeabi-v7a/*.a $1/lib/$TYPE/armeabi-v7a/
        mkdir -p $1/lib/$TYPE/x86
        cp -rv Dist/x86/*.a $1/lib/$TYPE/x86/
    elif [ "$TYPE" == "emscripten" ]; then
        cp Source/FreeImage.h $1/include
        if [ -d $1/lib/$TYPE/ ]; then
            rm -r $1/lib/$TYPE/
        fi
        mkdir -p $1/lib/$TYPE
        cp -rv Dist/libfreeimage.a $1/lib/$TYPE/
	fi	

    # copy license files
    rm -rf $1/license # remove any older files if exists
    mkdir -p $1/license
    cp -v license-fi.txt $1/license/
    cp -v license-gplv2.txt $1/license/
    cp -v license-gplv3.txt $1/license/
}

# executed inside the lib src dir
function clean() {
	
	if [ "$TYPE" == "android" ] ; then
		make clean 
		rm -rf Dist
		rm -f *.a
		rm -f builddir/$TYPE
		rm -f builddir
		rm -f lib		
	elif [ "$TYPE" == "emscripten" ] ; then
	    make clean
	    rm -rf Dist
		rm -f *.a
		rm -f builddir/$TYPE
		rm -f builddir
		rm -f lib		
	elif [[ "$TYPE" == "ios" || "$TYPE" == "tvos" ]] ; then
		# clean up compiled libraries
		make clean
		rm -rf Dist
		rm -f *.a *.lib
		rm -f builddir/$TYPE
		rm -f builddir
		rm -f lib		
	else
		make clean
		# run dedicated clean script
		clean.sh
	fi
}
