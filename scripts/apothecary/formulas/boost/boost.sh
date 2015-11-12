#! /bin/bash
#
# Boost
# Filesystem and system modules only until they are part of c++ std
# 
# uses a own build system

FORMULA_TYPES=( "osx" "msys2" "ios" "tvos" "android" "emscripten" "vs" )

# define the version
VERSION=1.58.0
VERSION_UNDERSCORES="$(echo "$VERSION" | sed 's/\./_/g')"
TARBALL="boost_${VERSION_UNDERSCORES}.tar.gz" 

BOOST_LIBS="filesystem system"
EXTRA_CPPFLAGS="-std=c++11 -stdlib=libc++ -fPIC -DBOOST_SP_USE_SPINLOCK"

# tools for git use
URL=http://sourceforge.net/projects/boost/files/boost/${VERSION}/${TARBALL}/download

# download the source code and unpack it into LIB_NAME
function download() {
	curl -Lk ${URL} > ${TARBALL}
	tar -xf ${TARBALL}
	mv boost_${VERSION_UNDERSCORES} boost
	rm ${TARBALL}

	if [ "$VERSION" == "1.58.0" ]; then
                cp -v boost/boost/config/compiler/visualc.hpp boost/boost/config/compiler/visualc.hpp.orig # back this up as we manually patch it
                cp -v boost/libs/filesystem/src/operations.cpp boost/libs/filesystem/src/operations.cpp.orig # back this up as we manually patch it
	fi

	if [[ "$TYPE" == "ios" || "${TYPE}" == "tvos" ]]; then
		cp -v boost/tools/build/example/user-config.jam boost/tools/build/example/user-config.jam.orig # back this up as we manually patch it
	fi
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	if [ "$VERSION" == "1.58.0" ]; then 
		if patch -p0 -u -N --dry-run --silent < $FORMULA_DIR/operations.cpp.patch_1.58 2>/dev/null ; then
               	    patch -p0 -u < $FORMULA_DIR/operations.cpp.patch_1.58
                fi
                
		if patch -p0 -u -N --dry-run --silent < $FORMULA_DIR/visualc.hpp.patch_1.58 2>/dev/null ; then
                    patch -p0 -u < $FORMULA_DIR/visualc.hpp.patch_1.58
                fi
	fi

	if [ "$TYPE" == "osx" ] || [ "$TYPE" == "emscripten" ]; then
		./bootstrap.sh --with-toolset=clang --with-libraries=filesystem
	elif [[ "${TYPE}" == "ios" || "${TYPE}" == "tvos" ]]; then
		mkdir -p lib/
		mkdir -p build/
		SDKVERSION=""
        
        SDKVERSION=`xcrun -sdk iphoneos --show-sdk-version`
 
		cp -v tools/build/example/user-config.jam.orig tools/build/example/user-config.jam
		cp $XCODE_DEV_ROOT/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator${SDKVERSION}.sdk/usr/include/{crt_externs,bzlib}.h .
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
		./b2 -j${PARALLEL_MAKE} toolset=clang cxxflags="-std=c++11 -stdlib=libc++ -arch i386 -arch x86_64 -mmacosx-version-min=${OSX_MIN_SDK_VER}" linkflags="-stdlib=libc++" threading=multi variant=release --build-dir=build --stage-dir=stage link=static stage
		cd tools/bcp  
		../../b2
	elif [[ "$TYPE" == "ios" || "${TYPE}" == "tvos" ]]; then
		# set some initial variables

		local IOS_ARCHS
        if [ "${TYPE}" == "tvos" ]; then 
            IOS_ARCHS="x86_64 arm64"
        elif [ "$TYPE" == "ios" ]; then
            IOS_ARCHS="i386 x86_64 armv7 arm64" #armv7s
        fi

		SDKVERSION=""
        if [ "${TYPE}" == "tvos" ]; then 
            SDKVERSION=`xcrun -sdk appletvos --show-sdk-version`
        elif [ "$TYPE" == "ios" ]; then
            SDKVERSION=`xcrun -sdk iphoneos --show-sdk-version`
        fi

		set -e
		CURRENTPATH=`pwd`
		ARM_DEV_CMD="xcrun --sdk iphoneos"
		SIM_DEV_CMD="xcrun --sdk iphonesimulator"
		OSX_DEV_CMD="xcrun --sdk macosx"
		DEVELOPER=$XCODE_DEV_ROOT
		TOOLCHAIN=${DEVELOPER}/Toolchains/XcodeDefault.xctoolchain
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
		# Set some locations and variables
		IPHONE_SDKVERSION="$SDKVERSION"
        SRCDIR=`pwd`/build/src
        IOSBUILDDIR=`pwd`/build/libs/boost/lib
        IOSINCLUDEDIR=`pwd`/build/libs/boost/include/boost
        PREFIXDIR=`pwd`/build/ios/prefix
        OUTPUT_DIR_LIB=`pwd`/lib/boost/ios
        OUTPUT_DIR_SRC=`pwd`/lib/boost/include/boost
        BOOST_SRC=$CURRENTPATH
        BITCODE=""
        MIN_IOS_VERSION=$IOS_MIN_SDK_VER
        if [ "${TYPE}" == "tvos" ]; then 
			local CROSS_TOP_IOS="${DEVELOPER}/Platforms/AppleTVOS.platform/Developer"
			local CROSS_SDK_IOS="AppleTVOS${SDKVERSION}.sdk"
			local CROSS_TOP_SIM="${DEVELOPER}/Platforms/AppleTVSimulator.platform/Developer"
			local CROSS_SDK_SIM="AppleTVSimulator${SDKVERSION}.sdk"
			local TARGET_OS="iphone"
			local ARCH="-arch arm64"
			local ARCHSIM="-arch x86_64"
			local TARGET_TYPE="iphone"
			local TARGET_TYPE_SIM="iphonesim"
			MIN_TYPE=-mtvos-version-min=
			BITCODE=-fembed-bitcode;
			MIN_IOS_VERSION=9.0
        elif [ "$TYPE" == "ios" ]; then
	        local CROSS_TOP_IOS="${DEVELOPER}/Platforms/iPhoneOS.platform/Developer"
			local CROSS_SDK_IOS="iPhoneOS${SDKVERSION}.sdk"
			local CROSS_TOP_SIM="${DEVELOPER}/Platforms/iPhoneSimulator.platform/Developer"
			local CROSS_SDK_SIM="iPhoneSimulator${SDKVERSION}.sdk"
			local TARGET_OS="iphone"
			local ARCH="-arch armv7 -arch arm64"
			local ARCHSIM="-arch i386 -arch x86_64"
			local TARGET_TYPE="iphone"
			local TARGET_TYPE_SIM="iphonesim"
			MIN_TYPE=-miphoneos-version-min=
		fi

		local BUILD_TOOLS="${DEVELOPER}"
		# Patch the user-config file -- Add some dynamic flags
	    cat >> tools/build/example/user-config.jam <<EOF
using darwin : ${IPHONE_SDKVERSION}~$TARGET_TYPE
: $XCODE_DEV_ROOT/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++ $ARCH $EXTRA_CPPFLAGS $BITCODE "-isysroot ${CROSS_TOP_IOS}/SDKs/${CROSS_SDK_IOS}" -I${CROSS_TOP_IOS}/SDKs/${CROSS_SDK_IOS}/usr/include/
: <striper> <root>$CROSS_TOP_IOS
: <architecture>arm <target-os>$TARGET_OS
;
using darwin : ${IPHONE_SDKVERSION}~$TARGET_TYPE_SIM
: $XCODE_DEV_ROOT/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++ $ARCHSIM $EXTRA_CPPFLAGS $BITCODE "-isysroot ${CROSS_TOP_SIM}/SDKs/${CROSS_SDK_SIM}" -I${CROSS_TOP_SIM}/SDKs/${CROSS_SDK_SIM}/usr/include/
: <striper> <root>$CROSS_TOP_SIM
: <architecture>x86 <target-os>$TARGET_OS
;
EOF
		# Build the Library with ./b2 /bjam
		echo "Boost iOS Device Staging"
		./b2 -j${PARALLEL_MAKE} --toolset=darwin-${IPHONE_SDKVERSION}~$TARGET_TYPE cxxflags="-stdlib=libc++ $MIN_TYPE$MIN_IOS_VERSION $BITCODE" linkflags="-stdlib=libc++" --build-dir=iphone-build  variant=release  -sBOOST_BUILD_USER_CONFIG=$BOOST_SRC/tools/build/example/user-config.jam --stagedir=iphone-build/stage --prefix=$PREFIXDIR architecture=arm target-os=iphone define=_LITTLE_ENDIAN link=static stage
    	echo "Boost iOS Device Install"
    	./b2 -j${PARALLEL_MAKE} --toolset=darwin-${IPHONE_SDKVERSION}~$TARGET_TYPE cxxflags="-stdlib=libc++ $MIN_TYPE$MIN_IOS_VERSION $BITCODE" linkflags="-stdlib=libc++" --build-dir=iphone-build  variant=release  -sBOOST_BUILD_USER_CONFIG=$BOOST_SRC/tools/build/example/user-config.jam --stagedir=iphone-build/stage --prefix=$PREFIXDIR architecture=arm target-os=iphone define=_LITTLE_ENDIAN link=static install
    	echo "Boost iOS Simulator Install"
    	./b2 -j${PARALLEL_MAKE} --toolset=darwin-${IPHONE_SDKVERSION}~$TARGET_TYPE_SIM cxxflags="-stdlib=libc++ $MIN_TYPE$MIN_IOS_VERSION $BITCODE" linkflags="-stdlib=libc++" --build-dir=iphonesim-build variant=release -sBOOST_BUILD_USER_CONFIG=$BOOST_SRC/tools/build/example/user-config.jam --stagedir=iphonesim-build/stage architecture=x86 target-os=iphone link=static stage
		mkdir -p $OUTPUT_DIR_LIB
		mkdir -p $OUTPUT_DIR_SRC
		mkdir -p $IOSBUILDDIR/armv7/ $IOSBUILDDIR/arm64/ $IOSBUILDDIR/i386/ $IOSBUILDDIR/x86_64/
		ALL_LIBS=""
		echo Splitting all existing fat binaries...
	    for NAME in $BOOST_LIBS; do
	        ALL_LIBS="$ALL_LIBS $NAME"
	        echo "Splitting '$NAME' to $IOSBUILDDIR/*/$NAME.a"
	        if [[ "$TYPE" == "ios" ]]; then
	        	$ARM_DEV_CMD lipo "iphone-build/stage/lib/libboost_$NAME.a" -thin armv7 -o $IOSBUILDDIR/armv7/$NAME.a
	        	$ARM_DEV_CMD lipo "iphonesim-build/stage/lib/libboost_$NAME.a" -thin i386 -o $IOSBUILDDIR/i386/$NAME.a
	        fi
	        $ARM_DEV_CMD lipo "iphone-build/stage/lib/libboost_$NAME.a" -thin arm64 -o $IOSBUILDDIR/arm64/$NAME.a
			$ARM_DEV_CMD lipo "iphonesim-build/stage/lib/libboost_$NAME.a" -thin x86_64 -o $IOSBUILDDIR/x86_64/$NAME.a
	    done
	    echo "done"
		echo "---------------"
	    echo "Decomposing each architecture's .a files"
	    for NAME in $ALL_LIBS; do
	    	mkdir -p $IOSBUILDDIR/armv7/$NAME-obj
			mkdir -p $IOSBUILDDIR/arm64/$NAME-obj
	    	mkdir -p $IOSBUILDDIR/i386/$NAME-obj
			mkdir -p $IOSBUILDDIR/x86_64/$NAME-obj
	        echo Decomposing $NAME ...
	        if [[ "$TYPE" == "ios" ]]; then
	        	(cd $IOSBUILDDIR/armv7/$NAME-obj;  ar -x ../$NAME.a; );
	        	(cd $IOSBUILDDIR/i386/$NAME-obj;   ar -x ../$NAME.a; );
	        fi
			(cd $IOSBUILDDIR/arm64/$NAME-obj;  ar -x ../$NAME.a; );
			(cd $IOSBUILDDIR/x86_64/$NAME-obj; ar -x ../$NAME.a; );
	    done
	    echo "done"
		echo "---------------"
		# remove broken symbol file (empty symbol)
		if [[  "$TYPE" == "ios" ]]; then
			rm $IOSBUILDDIR/armv7/filesystem-obj/windows_file_codecvt.o;
			rm $IOSBUILDDIR/i386/filesystem-obj/windows_file_codecvt.o;
		fi
		rm $IOSBUILDDIR/arm64/filesystem-obj/windows_file_codecvt.o;
		rm $IOSBUILDDIR/x86_64/filesystem-obj/windows_file_codecvt.o;
		echo "Re-forging architecture's .a files"
	    for NAME in $ALL_LIBS; do
	    	echo ar crus $NAME ...
	    	if [[ "$TYPE" == "ios" ]]; then
	    		(cd $IOSBUILDDIR/i386;    $SIM_DEV_CMD ar crus re-$NAME.a $NAME-obj/*.o;  )
	    		(cd $IOSBUILDDIR/armv7;   $ARM_DEV_CMD ar crus re-$NAME.a $NAME-obj/*.o; )
	    	fi
		    (cd $IOSBUILDDIR/arm64;   $ARM_DEV_CMD ar crus re-$NAME.a $NAME-obj/*.o;  )
			(cd $IOSBUILDDIR/x86_64;  $SIM_DEV_CMD ar crus re-$NAME.a $NAME-obj/*.o;  )
		done
		echo "done"
		echo "---------------"
	    echo "Decomposing each architecture's .a files"
	    for NAME in $ALL_LIBS; do

	    	if [[ "$TYPE" == "tvos" ]]; then
	    		echo "Lipo -c for $NAME for all tvOS Architectures (arm64, x86_64)"
	    		lipo -c $IOSBUILDDIR/arm64/re-$NAME.a \
		            $IOSBUILDDIR/x86_64/re-$NAME.a \
		            -output $OUTPUT_DIR_LIB/boost_$NAME.a
	    	elif [[ "$TYPE" == "ios" ]]; then
	    		echo "Lipo -c for $NAME for all iOS Architectures (arm64, armv7, i386, x86_64)"
		    	lipo -c $IOSBUILDDIR/armv7/re-$NAME.a \
		            $IOSBUILDDIR/arm64/re-$NAME.a \
		            $IOSBUILDDIR/i386/re-$NAME.a \
		            $IOSBUILDDIR/x86_64/re-$NAME.a \
		            -output $OUTPUT_DIR_LIB/boost_$NAME.a
	        fi

	        echo "---------------"
	        if [[ "$TYPE" == "ios" ]]; then
		        echo "Now strip the binary"
		        strip -x $OUTPUT_DIR_LIB/boost_$NAME.a
		        echo "---------------"
	   		fi
	    done
	    echo "done"
		echo "---------------"
	    mkdir -p $IOSINCLUDEDIR
	    echo "------------------"
	    echo "Copying Includes to Final Dir $OUTPUT_DIR_SRC"
	    set +e
	    cp -r $PREFIXDIR/include/boost/*  $OUTPUT_DIR_SRC/ 
	    echo "------------------"
	    # clean up the build area as it is quite large.
	    rm -rf build/lib iphone-build iphonesim-build
	    echo "Finished Build for $TYPE"
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
	elif  [[ "$TYPE" == "ios" || "$TYPE" == "tvos" ]]; then
		OUTPUT_DIR_LIB=`pwd`/lib/boost/ios/
        OUTPUT_DIR_SRC=`pwd`/lib/boost/include/boost
        #rsync -ar $OUTPUT_DIR_SRC/* $1/include/boost/
        lipo -info $OUTPUT_DIR_LIB/boost_filesystem.a 
        lipo -info $OUTPUT_DIR_LIB/boost_system.a
        cp -v $OUTPUT_DIR_LIB/boost_filesystem.a $1/lib/$TYPE/
		cp -v $OUTPUT_DIR_LIB/boost_system.a $1/lib/$TYPE/
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
	elif [[ "$TYPE" == "ios" || "$TYPE" == "tvos" ]]; then
		rm -rf build iphone-build iphonesim-build lib
		./b2 --clean
	else
		./b2 --clean
	fi
}
