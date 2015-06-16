#! /bin/bash
#
# Boost
# Filesystem and system modules only until they are part of c++ std
# 
# uses a own build system

FORMULA_TYPES=( "osx" "win_cb" "ios" "android" "emscripten" "vs" )

# define the version
VERSION=1.58.0
VERSION_UNDERSCORES="$(echo "$VERSION" | sed 's/\./_/g')"
TARBALL="boost_${VERSION_UNDERSCORES}.tar.gz" 

BOOST_LIBS="filesystem system"
EXTRA_CPPFLAGS="-DBOOST_AC_USE_PTHREADS -DBOOST_SP_USE_PTHREADS -std=c++11 -stdlib=libc++ -fPIC"

# tools for git use
URL=http://sourceforge.net/projects/boost/files/boost/${VERSION}/${TARBALL}/download

# download the source code and unpack it into LIB_NAME
function download() {
	curl -Lk ${URL} > ${TARBALL}
	tar -xf ${TARBALL}
	mv boost_${VERSION_UNDERSCORES} boost
	rm ${TARBALL}
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	if [ "$TYPE" == "osx" ] || [ "$TYPE" == "emscripten" ]; then
		./bootstrap.sh --with-toolset=clang --with-libraries=filesystem
	elif [ "$TYPE" == "ios" ]; then
		mkdir -p lib/
		mkdir -p build/
		IPHONE_SDKVERSION=`xcrun -sdk iphoneos --show-sdk-version`
	    cp $FORMULA_DIR/project-config-ios.jam tools/build/example/project-config-ios.jam

		cp $XCODE_DEV_ROOT/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator${IPHONE_SDKVERSION}.sdk/usr/include/{crt_externs,bzlib}.h .
		BOOST_LIBS_COMMA=$(echo $BOOST_LIBS | sed -e "s/ /,/g")
	    echo "Bootstrapping (with libs $BOOST_LIBS_COMMA)"
	    ./bootstrap.sh --with-libraries=$BOOST_LIBS_COMMA

	elif [ "$TYPE" == "android" ]; then
		./bootstrap.sh --with-toolset=gcc --with-libraries=filesystemel
	elif [ "$TYPE" == "vs" ]; then
		cmd.exe /c "bootstrap"
	else
		./bootstrap.bat
	fi
}

# executed inside the lib src dir
function build() {
	if [ "$TYPE" == "wincb" ] ; then
		: #noop by now
		
	elif [ "$TYPE" == "vs" ]; then
		./b2 -j${PARALLEL_MAKE} threading=multi variant=release --build-dir=build --with-filesystem link=static address-model=$ARCH stage
		./b2 -j${PARALLEL_MAKE} threading=multi variant=debug --build-dir=build --with-filesystem link=static address-model=$ARCH stage
		mv stage stage_$ARCH
		
		cd tools/bcp  
		../../b2
		
		
	elif [ "$TYPE" == "osx" ]; then
		./b2 -j${PARALLEL_MAKE} toolset=clang cxxflags="-std=c++11 -stdlib=libc++ -arch i386 -arch x86_64" linkflags="-stdlib=libc++" threading=multi variant=release --build-dir=build --stage-dir=stage link=static stage
		cd tools/bcp  
		../../b2
	elif [ "$TYPE" == "ios" ]; then
		SDKVERSION=`xcrun -sdk iphoneos --show-sdk-version`
		set -e
		CURRENTPATH=`pwd`

		DEVELOPER=$XCODE_DEV_ROOT
		TOOLCHAIN=${DEVELOPER}/Toolchains/XcodeDefault.xctoolchain

		local IOS_ARCHS="i386 x86_64 armv7 arm64"
		echo "--------------------"
		echo $CURRENTPATH

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

		IPHONE_SDKVERSION=`xcrun -sdk iphoneos --show-sdk-version`
        SRCDIR=`pwd`/build/src
        IOSBUILDDIR=`pwd`/build/libs/boost/lib
        IOSINCLUDEDIR=`pwd`/build/libs/boost/include/boost
        PREFIXDIR=`pwd`/build/ios/prefix
        OUTPUT_DIR_LIB=`pwd`/lib/boost/ios/
        OUTPUT_DIR_SRC=`pwd`/lib/boost/include/boost

		echo "Boost iOS Device Staging"
		./bjam -j${PARALLEL_MAKE} --build-dir=iphone-build -sBOOST_BUILD_USER_CONFIG=$CURRENTPATH/tools/build/example/project-config-ios.jam --stagedir=iphone-build/stage --prefix=$PREFIXDIR toolset=darwin architecture=arm target-os=iphone macosx-version=iphone-${IPHONE_SDKVERSION} define=_LITTLE_ENDIAN link=static stage
    	echo "Boost iOS Device Install"
    	./bjam -j${PARALLEL_MAKE} --build-dir=iphone-build -sBOOST_BUILD_USER_CONFIG=$CURRENTPATH/tools/build/example/project-config-ios.jam --stagedir=iphone-build/stage --prefix=$PREFIXDIR toolset=darwin architecture=arm target-os=iphone macosx-version=iphone-${IPHONE_SDKVERSION} define=_LITTLE_ENDIAN link=static install
    	echo "Boost iOS Simulator Install"
    	./bjam -j${PARALLEL_MAKE} --build-dir=iphonesim-build -sBOOST_BUILD_USER_CONFIG=$CURRENTPATH/tools/build/example/project-config-ios.jam --stagedir=iphonesim-build/stage --toolset=darwin-${IPHONE_SDKVERSION}~iphonesim architecture=x86 target-os=iphone macosx-version=iphonesim-${IPHONE_SDKVERSION} link=static stage
   
   		ARM_DEV_CMD="xcrun --sdk iphoneos"
		mkdir -p $OUTPUT_DIR_LIB
		mkdir -p $OUTPUT_DIR_SRC
		mkdir -p $IOSBUILDDIR/armv7/obj
		mkdir -p $IOSBUILDDIR/arm64/obj
	    mkdir -p $IOSBUILDDIR/i386/obj
		mkdir -p $IOSBUILDDIR/x86_64/obj
	    ALL_LIBS=""
	    echo Splitting all existing fat binaries...
	    for NAME in $BOOST_LIBS; do
	        ALL_LIBS="$ALL_LIBS libboost_$NAME.a"
	        $ARM_DEV_CMD lipo "iphone-build/stage/lib/libboost_$NAME.a" -thin armv7 -o $IOSBUILDDIR/armv7/libboost_$NAME.a
	        $ARM_DEV_CMD lipo "iphone-build/stage/lib/libboost_$NAME.a" -thin arm64 -o $IOSBUILDDIR/arm64/libboost_$NAME.a
			$ARM_DEV_CMD lipo "iphonesim-build/stage/lib/libboost_$NAME.a" -thin i386 -o $IOSBUILDDIR/i386/libboost_$NAME.a
			$ARM_DEV_CMD lipo "iphonesim-build/stage/lib/libboost_$NAME.a" -thin x86_64 -o $IOSBUILDDIR/x86_64/libboost_$NAME.a
	  
	    done
	    echo "Decomposing each architecture's .a files"
	    for NAME in $ALL_LIBS; do
	    	echo "Lipo -c for $NAME for all iOS Architectures (arm64, armv7, i386, x86_64)"
	    	lipo -c $IOSBUILDDIR/armv7/$NAME \
	            $IOSBUILDDIR/arm64/$NAME \
	            $IOSBUILDDIR/i386/$NAME \
	            $IOSBUILDDIR/x86_64/$NAME \
	            -output $OUTPUT_DIR_LIB$NAME
	        strip -x $OUTPUT_DIR_LIB$NAME
	    done
	    mkdir -p $IOSINCLUDEDIR
	    echo "------------------"
	    echo "Copying Includes to Final Dir $OUTPUT_DIR_SRC"
	    set +e
	    cp -r $PREFIXDIR/include/boost/*  $OUTPUT_DIR_SRC/ 
	    echo "------------------"

	elif [ "$TYPE" == "emscripten" ]; then
	    cp $FORMULA_DIR/project-config-emscripten.jam project-config.jam
		./b2 -j${PARALLEL_MAKE} toolset=clang cxxflags="-std=c++11" threading=single variant=release --build-dir=build --stage-dir=stage link=static stage
		
	elif [ "$TYPE" == "android" ]; then
	    rm -rf stage
	    
	    ABI=armeabi-v7a
	    source ../../android_configure.sh $ABI
	    cp $FORMULA_DIR/project-config-android_arm.jam project-config.jam
		./b2 -j${PARALLEL_MAKE} toolset=gcc cxxflags="-std=c++11 $CFLAGS" threading=multi threadapi=pthread target-os=android variant=release --build-dir=build_arm link=static stage
		mv stage stage_arm
		
	    ABI=x86
	    source ../../android_configure.sh $ABI
	    cp $FORMULA_DIR/project-config-android_x86.jam project-config.jam
		./b2 -j${PARALLEL_MAKE} toolset=gcc cxxflags="-std=c++11 $CFLAGS" threading=multi threadapi=pthread target-os=android variant=release --build-dir=build_x86 link=static stage
		mv stage stage_x86
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	# prepare headers directory if needed
	mkdir -p $1/include

	# prepare libs directory if needed
	mkdir -p $1/lib/$TYPE
	mkdir -p install_dir
	
	if [ "$TYPE" == "wincb" ] ; then
		: #noop by now
	elif [ "$TYPE" == "vs" ] ; then
		if [ "$ARCH" == "32" ]; then
			mkdir -p $1/lib/$TYPE/Win32
			cp stage_$ARCH/lib/libboost_filesystem-vc140-mt-1_58.lib $1/lib/$TYPE/Win32/
			cp stage_$ARCH/lib/libboost_system-vc140-mt-1_58.lib $1/lib/$TYPE/Win32/
			cp stage_$ARCH/lib/libboost_filesystem-vc140-mt-gd-1_58.lib $1/lib/$TYPE/Win32/
			cp stage_$ARCH/lib/libboost_system-vc140-mt-gd-1_58.lib $1/lib/$TYPE/Win32/
		elif [ "$ARCH" == "64" ]; then
			mkdir -p $1/lib/$TYPE/x64
			cp stage_$ARCH/lib/libboost_filesystem-vc140-mt-1_58.lib $1/lib/$TYPE/x64/
			cp stage_$ARCH/lib/libboost_system-vc140-mt-1_58.lib $1/lib/$TYPE/x64/
			cp stage_$ARCH/lib/libboost_filesystem-vc140-mt-gd-1_58.lib $1/lib/$TYPE/x64/
			cp stage_$ARCH/lib/libboost_system-vc140-mt-gd-1_58.lib $1/lib/$TYPE/x64/
		fi
	elif [ "$TYPE" == "osx" ]; then
		dist/bin/bcp filesystem install_dir
		rsync -ar install_dir/boost/* $1/include/boost/
		cp stage/lib/libboost_filesystem.a $1/lib/$TYPE/boost_filesystem.a
		cp stage/lib/libboost_system.a $1/lib/$TYPE/boost_system.a
	elif  [ "$TYPE" == "ios" ]; then
		OUTPUT_DIR_LIB=`pwd`/lib/boost/ios/
        OUTPUT_DIR_SRC=`pwd`/lib/boost/include/boost
        #rsync -ar $OUTPUT_DIR_SRC/* $1/include/boost/
        lipo -info $OUTPUT_DIR_LIB/libboost_filesystem.a 
        lipo -info $OUTPUT_DIR_LIB/libboost_system.a
        cp -v $OUTPUT_DIR_LIB/libboost_filesystem.a $1/lib/$TYPE/boost_filesystem.a
		cp -v $OUTPUT_DIR_LIB/libboost_system.a $1/lib/$TYPE/boost_system.a
	elif [ "$TYPE" == "emscripten" ]; then
		cp stage/lib/*.a $1/lib/$TYPE/
	elif [ "$TYPE" == "android" ]; then
	    mkdir -p $1/lib/$TYPE/armeabi-v7a
	    mkdir -p $1/lib/$TYPE/x86
		cp stage_arm/lib/*.a $1/lib/$TYPE/armeabi-v7a/
		cp stage_x86/lib/*.a $1/lib/$TYPE/x86/
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
