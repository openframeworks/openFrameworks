#! /bin/bash
#
# openssl

# define the version
FORMULA_TYPES=( "osx" "vs" "msys2" "ios" "tvos" "android" )

VER=1.0.2d
VERDIR=1.0.2
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
	if [ "$TYPE" == "vs" ] ; then
		#hasSha=$(cmd.exe /c 'call 'CertUtil' '-hashfile' '$FILENAME.tar.gz' 'SHA1'')
		echo "TO DO: check against the SHA for windows"
		tar -xf $FILENAME.tar.gz
		mv $FILENAME openssl
		rm $FILENAME.tar.gz
		rm $FILENAME.tar.gz.sha1
	else
		if [ "$(shasum $FILENAME.tar.gz | awk '{print $1}')" == "$(cat $FILENAME.tar.gz.sha1)" ] ;  then  
			tar -xvf $FILENAME.tar.gz
			mv $FILENAME openssl
			rm $FILENAME.tar.gz
			rm $FILENAME.tar.gz.sha1
		else 
			echoError "Invalid shasum for $FILENAME."
		fi
	fi
}

# prepare the build environment, executed inside the lib src dir
function prepare() {

	if [[ "$TYPE" == "ios" || "${TYPE}" == "tvos" ]] ; then
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

        cp Makefile Makefile.orig
        cp Configure Configure1.orig

        #cp $FORMULA_DIR/Configure Configure
 	elif  [ "$TYPE" == "vs" ] ; then
		if patch -p1 -u -N --dry-run --silent < $FORMULA_DIR/winOpenSSL.patch 2>/dev/null ; then
			patch -p1 -u < $FORMULA_DIR/winOpenSSL.patch
		fi
	fi
}

# executed inside the lib src dir
function build() {
	
	if [ "$TYPE" == "osx" ] ; then

        set -e
        CURRENTPATH=`pwd`

        DEVELOPER=$XCODE_DEV_ROOT
        TOOLCHAIN=${DEVELOPER}/Toolchains/XcodeDefault.xctoolchain
        SDKVERSION=""
        SDKVERSION=`xcrun -sdk macosx --show-sdk-version`

        if [ "${COMPILER_TYPE}" == "clang" ]; then
                export THECOMPILER=clang
            else
                export THECOMPILER=gcc
            fi

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
        
        export OSX_CC=$TOOLCHAIN/usr/bin/$THECOMPILER
        export OSX_CXX=$TOOLCHAIN/usr/bin/$THECOMPILER++
        export LD=$TOOLCHAIN/usr/bin/ld
        export AR=$TOOLCHAIN/usr/bin/ar
        export AS=$TOOLCHAIN/usr/bin/as
        export NM=$TOOLCHAIN/usr/bin/nm
        export RANLIB=$TOOLCHAIN/usr/bin/ranlib
		
		local BUILD_OPTS="-no-shared -no-asm -no-ec_nistp_64_gcc_128 -no-gmp -no-jpake -no-krb5 -no-md2 -no-rc5 -no-rfc3779 -no-sctp -no-shared -no-store -no-unit-test -no-zlib -no-zlib-dynamic"
		local OSX_ARCHS="i386 x86_64"
		
		VERSION=$VER
		CURRENTPATH=`pwd`
		
		# create build directories 

		for OSX_ARCH in ${OSX_ARCHS}
			do
			
            make -j 1 clean 
            rm -rf build/$TYPE/OSX_ARCH
            rm -rf *.a # remove temp lib from main directory
			# Back up configure & makefile

			cp "Configure" "Configure.orig" 
			cp "Makefile" "Makefile.orig"

			# create build directory for current arch
			mkdir -p "$CURRENTPATH/build/$TYPE/$OSX_ARCH"

			#create logfile
			LOG="$CURRENTPATH/build/$TYPE/$OSX_ARCH/build-openssl-${VER}.log"
			echo "Using Compiler: $THECOMPILER for $OSX_ARCH"

            # patch the Configure file to make sure the correct compiler is invoked.
			export LC_CTYPE=C
            export LANG=C
            sed -ie "s!\"darwin-i386-cc\",\"cc:-arch i386 -O3!\"darwin-i386-cc\",\"cc:-arch i386 -O3!" Configure
            
            export LC_CTYPE=C
            export LANG=C
            sed -ie "s!\"darwin64-x86_64-cc\",\"cc:-arch x86_64 -O3!\"darwin64-x86_64-cc\",\"cc:-arch x86_64 -O3!" Configure

   			OSX_C_FLAGS="" 		# Flags for stdlib, std and arch
   			CONFIG_TARGET=""	# Which one of the target presets to use

			if [[ "${OSX_ARCH}" == "i386" ]]; then
		    	# 386 -> libstdc++
		    	OSX_C_FLAGS="-arch ${OSX_ARCH} -std=${CSTANDARD} -fPIC -stdlib=libc++ -mmacosx-version-min=${OSX_MIN_SDK_VER}"
		    	CONFIG_TARGET=darwin-i386-cc
		    	export CC="${OSX_CC} ${OSX_C_FLAGS}"
		    elif [ "${OSX_ARCH}" == "x86_64" ]; then
		    	# 86_64 -> libc++
		    	OSX_C_FLAGS="-arch ${OSX_ARCH} -std=${CSTANDARD} -fPIC -stdlib=libc++ -mmacosx-version-min=${OSX_MIN_SDK_VER}"
				CONFIG_TARGET=darwin64-x86_64-cc
		    	export CC="${OSX_CC} ${OSX_C_FLAGS}"
		    fi

	    	echo "Configure for target: $CONFIG_TARGET"

		    ./Configure $CONFIG_TARGET $BUILD_OPTS --openssldir="$CURRENTPATH/build/$TYPE/$OSX_ARCH" > "${LOG}" 2>&1

			if [ $? != 0 ]; then 
                tail -n 100 "${LOG}"
		    	echo "Problem during configure - Please check ${LOG}"
		    	exit 1
		    fi

            # we need to unset LANG otherwise sed will get upsed.
   			# unset LANG if defined
			if test ${LANG+defined};
			then
                OLD_LANG=$LANG
				unset LANG
			fi

            # unset LC_CTYPE if defined
            if test ${LC_CTYPE+defined};
            then
                OLD_LC_CTYPE=$LC_CTYPE
                LC_CTYPE=C
            fi

            # patching Makefile to use the correct c flags.
			sed -ie "s!^CFLAG=!CFLAG=$OSX_C_FLAGS !" Makefile

            # reset LANG if it was defined
			if test ${OLD_LANG+defined};
			then
				export LANG=$OLD_LANG
            fi

            # reset LC_CTYPE if it was defined
            if test ${OLD_LC_CTYPE+defined};
            then
                export LC_CTYPE=$OLD_LC_CTYPE
            fi
            set -o pipefail  # trace ERR through pipes
            set -o errtrace  # trace ERR through 'time command' and other functions

            export BUILD_OUTPUT=$LOG
            export PING_SLEEP=30s
            export PING_LOOP_PID
            trap 'error_handler ${LINENO} ${?}' ERR
            bash -c "while true; do echo \$(date) - Building OpenSSL ...; sleep $PING_SLEEP; done" &
PING_LOOP_PID=$!
            echo "Running make for ${OSX_ARCH}"
            echo "Please stand by..."
            # Must run at -j 1 (single thread only else will fail)
            # this is super annoying, but true for OS X, as well as iOS.
            make -j 1 >> "${BUILD_OUTPUT}" 2>&1
            dump_output
            kill $PING_LOOP_PID
			trap - ERR
            
			if [ $? != 0 ];
		    then 
                tail -n 100 "${LOG}"
		    	echo "Problem while make - Please check ${LOG}"
		    	exit 1
		    else
		    	echo "Make Successful for ${OSX_ARCH}"
		    fi

			set -e
			make -j 1 install >> "${LOG}" 2>&1
			make -j 1 clean 

			# restore configure & makefile

			cp "Configure.orig" "Configure" 
			cp "Makefile.orig" "Makefile"

            rm -rf *.a # remove temp lib from main directory
			unset CC CFLAG CFLAGS EXTRAFLAGS
		done
        unset THECOMPILER
		# Stage includes
		echo "Staging includes"

		cp -R "build/$TYPE/x86_64/include/" "lib/include/"

		# Stage fat libs - this is where we omit the lib-prefix
		echo "Building & staging fat libs"
		lipo -c "build/$TYPE/i386/lib/libcrypto.a" "build/$TYPE/x86_64/lib/libcrypto.a" -o "lib/$TYPE/crypto.a"
		lipo -c "build/$TYPE/i386/lib/libssl.a" "build/$TYPE/x86_64/lib/libssl.a" -o "lib/$TYPE/ssl.a"

        cd lib/$TYPE
        SLOG="$CURRENTPATH/lib/$TYPE-stripping.log"
        local TOBESTRIPPED
        for TOBESTRIPPED in $( ls *.a) ; do
            strip -x $TOBESTRIPPED >> "${SLOG}" 2>&1
            if [ $? != 0 ];
            then
                tail -n 100 "${SLOG}"
                echo "Problem while stripping lib - Please check ${SLOG}"
                exit 1
            else
                echo "Strip Successful for ${SLOG}"
            fi
        done

        cd ../../

        echo "Build Finished!"
		
		# ------------ END OS X Recipe.

	 elif [ "$TYPE" == "vs" ] ; then
		CURRENTPATH=`pwd`
		cp -v $FORMULA_DIR/buildwin.cmd $CURRENTPATH
		WINPATH=$(echo "$CURRENTPATH" | sed -e 's/^\///' -e 's/\//\\/g' -e 's/^./\0:/')
		if [ $ARCH == 32 ] ; then
			if [ -d ms/Win32 ]; then
				rm -r ms/Win32
			fi
			mkdir ms/Win32
			cmd //c buildwin.cmd Win32 "${WINPATH}"
		elif [ $ARCH == 64 ] ; then
			if [ -d ms/x64 ]; then
				rm -r ms/x64
			fi
			mkdir ms/x64
			cmd //c buildwin.cmd x64 "${WINPATH}"
		fi
	# elif [ "$TYPE" == "msys2" ] ; then
	# 	# local BUILD_OPTS="--no-tests --no-samples --static --omit=CppUnit,CppUnit/WinTestRunner,Data/MySQL,Data/ODBC,PageCompiler,PageCompiler/File2Page,CppParser,PocoDoc,ProGen"

	# 	# # Locate the path of the openssl libs distributed with openFrameworks.
	# 	# local OF_LIBS_OPENSSL="../../../../libs/openssl/"

	# 	# # get the absolute path to the included openssl libs
	# 	# local OF_LIBS_OPENSSL_ABS_PATH=$(cd $(dirname $OF_LIBS_OPENSSL); pwd)/$(basename $OF_LIBS_OPENSSL)

	# 	# local OPENSSL_INCLUDE=$OF_LIBS_OPENSSL_ABS_PATH/include
	# 	# local OPENSSL_LIBS=$OF_LIBS_OPENSSL_ABS_PATH/lib/msys2

	# 	# ./configure $BUILD_OPTS \
	# 	# 			--include-path=$OPENSSL_INCLUDE \
	# 	# 			--library-path=$OPENSSL_LIBS \
	# 	# 			--config=MinGW

	# 	# make

	# 	# # Delete debug libs.
	# 	# lib/MinGW/i686/*d.a

	elif [[ "$TYPE" == "ios" || "${TYPE}" == "tvos" ]] ; then

		# This was quite helpful as a reference: https://github.com/x2on/OpenSSL-for-iPhone
		# Refer to the other script if anything drastic changes for future versions
		
		set -e
		CURRENTPATH=`pwd`
		
		DEVELOPER=$XCODE_DEV_ROOT
		TOOLCHAIN=${DEVELOPER}/Toolchains/XcodeDefault.xctoolchain
		VERSION=$VER

        local IOS_ARCHS
        if [ "${TYPE}" == "tvos" ]; then 
            IOS_ARCHS="x86_64 arm64"
        elif [ "$TYPE" == "ios" ]; then
            IOS_ARCHS="i386 x86_64 armv7 arm64" #armv7s
        fi
		local STDLIB="libc++"

        SDKVERSION=""
        if [ "${TYPE}" == "tvos" ]; then 
            SDKVERSION=`xcrun -sdk appletvos --show-sdk-version`
        elif [ "$TYPE" == "ios" ]; then
            SDKVERSION=`xcrun -sdk iphoneos --show-sdk-version`
        fi

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
            cp "apps/speed.c" "apps/speed.c.orig" 
			cp "Makefile" "Makefile.orig"

			if [ "${COMPILER_TYPE}" == "clang" ]; then
				export THECOMPILER=$TOOLCHAIN/usr/bin/clang
			else
				export THECOMPILER=${BUILD_TOOLS}/usr/bin/gcc
			fi
			echo "The compiler: $THECOMPILER"

            ## Fix for tvOS fork undef 9.0
            if [ "${TYPE}" == "tvos" ]; then

            # Patch apps/speed.c to not use fork() since it's not available on tvOS
                LC_ALL=C sed -i -- 's/define HAVE_FORK 1/define HAVE_FORK 0/' "apps/speed.c"
                # Patch Configure to build for tvOS, not iOS
                LC_ALL=C sed -i -- 's/D\_REENTRANT\:iOS/D\_REENTRANT\:tvOS/' "Configure"
                chmod u+x ./Configure 
            #     export LC_CTYPE=C 
            #     export LANG=C
            #     sed -ie "s!\"defined(OPENSSL_SYS_NETWARE)\"!\"defined(OPENSSL_SYS_NETWARE) || defined(TARGET_IOS)\"!" "./apps/speed.c"
                
            #     sed -ie "s!\"-D_REENTRANT:iOS\"!\"-D_REENTRANT:tvOS\"!" "./Configure"
            fi

			if [[ "${IOS_ARCH}" == "i386" || "${IOS_ARCH}" == "x86_64" ]];
			then
				if [ "${TYPE}" == "tvos" ]; then 
                    PLATFORM="AppleTVSimulator"
                elif [ "$TYPE" == "ios" ]; then
                    PLATFORM="iPhoneSimulator"
                fi

                # unset LANG if defined
				if test ${LANG+defined};
				then
					OLD_LANG=$LANG
					unset LANG
				fi

                # unset LC_CTYPE if defined
                if test ${LC_CTYPE+defined};
                then
                    OLD_LC_CTYPE=$LC_CTYPE
                    LC_CTYPE=C
                fi

				LC_ALL=C sed -ie "s!\"debug-darwin-i386-cc\",\"cc:-arch i386 -g3!\"debug-darwin-i386-cc\",\"$THECOMPILER:-arch i386 -g3!" Configure
				LC_ALL=C sed -ie "s!\"darwin64-x86_64-cc\",\"cc:-arch x86_64 -O3!\"darwin64-x86_64-cc\",\"$THECOMPILER:-arch x86_64 -O3!" Configure

                # reset LANG if it was defined
				if test ${OLD_LANG+defined};
				then
					export LANG=$OLD_LANG
                fi

                # reset LC_CTYPE if it was defined
                if test ${OLD_LC_CTYPE+defined};
                then
                    export LC_CTYPE=$OLD_LC_CTYPE
                fi

			else
				cp "crypto/ui/ui_openssl.c" "crypto/ui/ui_openssl.c.orig"
				LC_ALL=C sed -ie "s!static volatile sig_atomic_t intr_signal;!static volatile intr_signal;!" "crypto/ui/ui_openssl.c"
				if [ "${TYPE}" == "tvos" ]; then 
                    PLATFORM="AppleTVOS"
                elif [ "$TYPE" == "ios" ]; then
                    PLATFORM="iPhoneOS"
                fi

                # unset LANG if defined
				if test ${LANG+defined};
				then
					OLD_LANG=$LANG				
					unset LANG
				fi

                # unset LC_CTYPE if defined
                if test ${LC_CTYPE+defined};
                then
                    OLD_LC_CTYPE=$LC_CTYPE
                    LC_CTYPE=C
                fi

				LC_ALL=C sed -ie "s!\"iphoneos-cross\",\"llvm-gcc:-O3!\"iphoneos-cross\",\"$THECOMPILER:-Os!" Configure

                # reset LANG if it was defined
				if test ${OLD_LANG+defined};
				then
					export LANG=$OLD_LANG
				fi

                # reset LC_CTYPE if it was defined
                if test ${OLD_LC_CTYPE+defined};
                then
                    export LC_CTYPE=$OLD_LC_CTYPE
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
                MIN_IOS_VERSION=5.1 # 6.0 to prevent start linking errors
            fi

            BITCODE=""
            if [[ "$TYPE" == "tvos" ]]; then
                BITCODE=-fembed-bitcode;
                MIN_IOS_VERSION=9.0
            fi

			export CC="${THECOMPILER} -arch ${IOS_ARCH} -std=${CSTANDARD} $BITCODE"
			mkdir -p "$CURRENTPATH/build/$TYPE/$IOS_ARCH"
			LOG="$CURRENTPATH/build/$TYPE/$IOS_ARCH/build-openssl-${VER}.log"
			
			

			echo "Compiler: $CC"
			echo "Building openssl-${VER} for ${PLATFORM} ${SDKVERSION} ${IOS_ARCH} : iOS Minimum=$MIN_IOS_VERSION"

			set +e
			if [ "${IOS_ARCH}" == "i386" ]; then
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
                tail -n 100 "${LOG}"
		    	echo "Problem while configure - Please check ${LOG}"
		    	exit 1
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

            # unset LANG if defined
            if test ${LANG+defined};
			then
			  OLD_LANG=$LANG
				unset LANG
			fi

            # unset LC_CTYPE if defined
            if test ${LC_CTYPE+defined};
            then
                OLD_LC_CTYPE=$LC_CTYPE
                LC_CTYPE=C
            fi

			LC_ALL=C sed -ie "s!^CFLAG=!CFLAG=-isysroot ${CROSS_TOP}/SDKs/${CROSS_SDK} -arch $IOS_ARCH -Os -fPIC $BITCODE -stdlib=libc++ $MIN_TYPE$MIN_IOS_VERSION !" Makefile

            # reset LANG if it was defined
			if test ${OLD_LANG+defined};
			then
				export LANG=$OLD_LANG
			fi

            # reset LC_CTYPE if it was defined
            if test ${OLD_LC_CTYPE+defined};
            then
                export LC_CTYPE=$OLD_LC_CTYPE
            fi

            export BUILD_OUTPUT=$LOG
            export PING_SLEEP=30s
            export PING_LOOP_PID
            trap 'error_handler' ERR
            bash -c "while true; do echo \$(date) - Building OpenSSL ...; sleep $PING_SLEEP; done" &
PING_LOOP_PID=$!

			echo "Running make for ${IOS_ARCH}"
			echo "Please stand by..."
			# Must run at -j 1 (single thread only else will fail)
			make >> "${BUILD_OUTPUT}" 2>&1
            dump_output
            kill $PING_LOOP_PID
            trap - ERR

			if [ $? != 0 ];
		    then 
                tail -n 100 "${LOG}"
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
            cp "apps/speed.c.orig" "apps/speed.c"



			unset CC CFLAG CFLAGS EXTRAFLAGS THECOMPILER

		done

		unset CC CFLAG CFLAGS 
		unset PLATFORM CROSS_TOP CROSS_SDK BUILD_TOOLS
		unset IOS_DEVROOT IOS_SDKROOT 

		cd lib/$TYPE/

        # stripping the lib prefix to bypass any issues with existing sdk libraries
        echo "Creating Fat Lib for crypto"
        if [ "${TYPE}" == "tvos" ]; then 
            lipo -create arm64/crypto.a \
                        x86_64/crypto.a \
                        -output crypto.a
            echo "Creating Fat Lib for ssl"
            lipo -create arm64/ssl.a \
                        x86_64/ssl.a \
                        -output ssl.a
        elif [ "$TYPE" == "ios" ]; then
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
        fi
		cd ../../

		# copy includes
		echo "Copying includes"

		cp -R "build/$TYPE/x86_64/include/" "lib/include/"

		cp "crypto/ui/ui_openssl.c.orig" "crypto/ui/ui_openssl.c"

		unset TOOLCHAIN DEVELOPER

	elif [ "$TYPE" == "android" ]; then
		source $LIBS_DIR/openFrameworksCompiled/project/android/paths.make
		perl -pi -e 's/install: all install_docs install_sw/install: install_docs install_sw/g' Makefile.org
		export _ANDROID_NDK_ROOT=$NDK_ROOT
		export FIPS_SIG=
		curl -O http://wiki.openssl.org/images/7/70/Setenv-android.sh
		perl -pi -e 's/^_ANDROID_EABI=(.*)$/#_ANDROID_EABI=\1/g' Setenv-android.sh
		perl -pi -e 's/^_ANDROID_ARCH=(.*)$/#_ANDROID_ARCH=\1/g' Setenv-android.sh
		perl -pi -e 's/^_ANDROID_API=(.*)$/#_ANDROID_API=\1/g' Setenv-android.sh
		export _ANDROID_API=$ANDROID_PLATFORM
		
        # armv7
        echoInfo "Compiling armv7"
        export _ANDROID_EABI=arm-linux-androideabi-4.9
		export _ANDROID_ARCH=arch-arm
        local BUILD_TO_DIR=$BUILD_DIR/openssl/build/$TYPE/armeabi-v7a
        mkdir -p $BUILD_TO_DIR
        source Setenv-android.sh
        ./config --openssldir=$BUILD_TO_DIR no-ssl2 no-ssl3 no-comp no-hw no-engine no-shared
        make clean
        make depend -j${PARALLEL_MAKE}
        make build_libs -j${PARALLEL_MAKE}
        mkdir -p $BUILD_TO_DIR/lib
		cp libssl.a $BUILD_TO_DIR/lib/
        cp libcrypto.a $BUILD_TO_DIR/lib/
        
        # x86
        echoInfo "Compiling x86"
        export _ANDROID_EABI=x86-4.9
		export _ANDROID_ARCH=arch-x86
        local BUILD_TO_DIR=$BUILD_DIR/openssl/build/$TYPE/x86
        mkdir -p $BUILD_TO_DIR
        source Setenv-android.sh
        ./config --openssldir=$BUILD_TO_DIR no-ssl2 no-ssl3 no-comp no-hw no-engine no-shared
        make clean
        make depend -j${PARALLEL_MAKE}
        make build_libs -j${PARALLEL_MAKE}
        mkdir -p $BUILD_TO_DIR/lib
        cp libssl.a $BUILD_TO_DIR/lib/
        cp libcrypto.a $BUILD_TO_DIR/lib/

	else 

		echoWarning "TODO: build $TYPE lib"

	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	#echoWarning "TODO: copy $TYPE lib"

	# # headers
	if [ -d $1/include/ ]; then
	    rm -r $1/include/
	fi
	
	mkdir -pv $1/include/openssl/
	
	# opensslconf.h is different in every platform, we need to copy
	# it as opensslconf_$(TYPE).h and use a modified version of 
	# opensslconf.h that detects the platform and includes the 
	# correct one. Then every platform checkouts the rest of the config
	# files that were deleted here
     if [[ "$TYPE" == "osx" || "$TYPE" == "ios" || "$TYPE" == "tvos" ]] ; then
        if [ -f lib/include/openssl/opensslconf.h ]; then
            mv lib/include/openssl/opensslconf.h lib/include/openssl/opensslconf_${TYPE}.h
        fi
        cp -RHv lib/include/openssl/* $1/include/openssl/
        cp -v $FORMULA_DIR/opensslconf.h $1/include/openssl/opensslconf.h

    elif [ -f include/openssl/opensslconf.h ]; then
        mv include/openssl/opensslconf.h include/openssl/opensslconf_${TYPE}.h
        cp -RHv include/openssl/* $1/include/openssl/
        cp -v $FORMULA_DIR/opensslconf.h $1/include/openssl/opensslconf.h
    fi
	# suppress file not found errors
	#same here doesn't seem to be a solid reason to delete the files
	#rm -rf $1/lib/$TYPE/* 2> /dev/null

	# libs
	if [ "$TYPE" == "osx" ] ; then
		mkdir -p $1/lib/$TYPE
		cp -v lib/$TYPE/*.a $1/lib/$TYPE
        if [[ "$CHECKOUT" == "NO" ]]; then 
            echo "no git checkout"
        else 
    		git checkout $1/include/openssl/opensslconf_osx.h
            git checkout $1/include/openssl/opensslconf_ios.h
        	git checkout $1/include/openssl/opensslconf_android.h
        	git checkout $1/include/openssl/opensslconf_vs.h
        	git checkout $1/include/openssl/opensslconf_win32.h
        fi
	elif [[ "$TYPE" == "ios" || "${TYPE}" == "tvos" ]] ; then
	 	mkdir -p $1/lib/$TYPE
	 	cp -v lib/$TYPE/*.a $1/lib/$TYPE
        if [[ "$CHECKOUT" == "NO" ]]; then 
            echo "no git checkout"
        else 
    		git checkout $1/include/openssl/opensslconf_osx.h
            git checkout $1/include/openssl/opensslconf_ios.h
        	git checkout $1/include/openssl/opensslconf_android.h
        	git checkout $1/include/openssl/opensslconf_vs.h
        	git checkout $1/include/openssl/opensslconf_win32.h
        fi
	elif [ "$TYPE" == "vs" ] ; then	 
		if [ $ARCH == 32 ] ; then
			rm -rf $1/lib/$TYPE/Win32
			mkdir -p $1/lib/$TYPE/Win32
			cp -v ms/Win32/lib/*.lib $1/lib/$TYPE/Win32/
			for f in $1/lib/$TYPE/Win32/*; do
				base=`basename $f .lib`
				mv -v $f $1/lib/$TYPE/Win32/${base}md.lib
			done
		elif [ $ARCH == 64 ] ; then
			rm -rf $1/lib/$TYPE/x64
			mkdir -p $1/lib/$TYPE/x64
			cp -v ms/x64/lib/*.lib $1/lib/$TYPE/x64/
			for f in $1/lib/$TYPE/x64/*; do
				base=`basename $f .lib`
				mv -v $f $1/lib/$TYPE/x64/${base}md.lib
			done
		fi
	 	
		git checkout $1/include/openssl/opensslconf_ios.h
		git checkout $1/include/openssl/opensslconf_osx.h
    	git checkout $1/include/openssl/opensslconf_android.h
    	git checkout $1/include/openssl/opensslconf_win32.h
	# elif [ "$TYPE" == "msys2" ] ; then
	# 	mkdir -p $1/lib/$TYPE
	# 	cp -v lib/MinGW/i686/*.a $1/lib/$TYPE
	
	elif [ "$TYPE" == "android" ] ; then
	    if [ -d $1/lib/$TYPE/ ]; then
	        rm -r $1/lib/$TYPE/
	    fi
	    mkdir -p $1/lib/$TYPE/armeabi-v7a
		cp -rv build/android/armeabi-v7a/lib/*.a $1/lib/$TYPE/armeabi-v7a/
	    mkdir -p $1/lib/$TYPE/x86
		cp -rv build/android/x86/lib/*.a $1/lib/$TYPE/x86/
	    mv include/openssl/opensslconf_android.h include/openssl/opensslconf.h

		git checkout $1/include/openssl/comp.h
        git checkout $1/include/openssl/engine.h
		git checkout $1/include/openssl/opensslconf_osx.h
    	git checkout $1/include/openssl/opensslconf_ios.h
    	git checkout $1/include/openssl/opensslconf_vs.h
    	git checkout $1/include/openssl/opensslconf_win32.h

	# 	mkdir -p $1/lib/$TYPE/armeabi-v7a
	# 	cp -v lib/Android/armeabi-v7a/*.a $1/lib/$TYPE/armeabi-v7a

	# 	mkdir -p $1/lib/$TYPE/x86
	# 	cp -v lib/Android/x86/*.a $1/lib/$TYPE/x86
	else
	 	echoWarning "TODO: copy $TYPE lib"
	fi

    # copy license file
    rm -rf $1/license # remove any older files if exists
    mkdir -p $1/license
    cp -v LICENSE $1/license/
	
	
}

# executed inside the lib src dir
function clean() {
	echoWarning "TODO: clean $TYPE lib"
	if [[ "$TYPE" == "ios" || "${TYPE}" == "tvos" ]] ; then
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
    elif [[ "$TYPE" == "osx" ]] ; then
        make clean
        # clean up old build folder
        rm -rf /build
        # clean up compiled libraries
        rm -rf /lib
        rm -rf *.a
	else
	 	make clean
	fi
}

