#! /bin/bash
#
# openssl

# define the version
FORMULA_TYPES=( "osx" "vs" "win_cb" "ios" "android" )

VER=1.0.2c
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
 	elif  [ "$TYPE" == "vs" ] ; then
		if patch -p1 -u -N --dry-run --silent < $FORMULA_DIR/winOpenSSL.patch 2>/dev/null ; then
			patch -p1 -u < $FORMULA_DIR/winOpenSSL.patch
		fi
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

            # patch the Configure file to make sure the correct compiler is invoked.
			sed -ie "s!\"darwin-i386-cc\",\"cc:-arch i386 -g3!\"darwin-i386-cc\",\"${THECOMPILER}:-arch i386 -g3!" Configure
			sed -ie "s!\"darwin64-x86_64-cc\",\"cc:-arch x86_64 -O3!\"darwin64-x86_64-cc\",\"$THECOMPILER:-arch x86_64 -O3!" Configure

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

			echo "Running make for ${OSX_ARCH}"
			echo "Please stand by..."

			# Must run at -j 1 (single thread only else will fail)
			# this is super annoying, but true for OS X, as well as iOS.
			make -j 1 >> "${LOG}" 2>&1
			
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

        cd lib/$TYPE
        SLOG="$CURRENTPATH/lib/$TYPE-stripping.log"
        local TOBESTRIPPED
        for TOBESTRIPPED in $( ls -1) ; do
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

				sed -ie "s!\"debug-darwin-i386-cc\",\"cc:-arch i386 -g3!\"debug-darwin-i386-cc\",\"$THECOMPILER:-arch i386 -g3!" Configure
				sed -ie "s!\"darwin64-x86_64-cc\",\"cc:-arch x86_64 -O3!\"darwin64-x86_64-cc\",\"$THECOMPILER:-arch x86_64 -O3!" Configure

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
				sed -ie "s!static volatile sig_atomic_t intr_signal;!static volatile intr_signal;!" "crypto/ui/ui_openssl.c"
				PLATFORM="iPhoneOS"

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

				sed -ie "s!\"iphoneos-cross\",\"llvm-gcc:-O3!\"iphoneos-cross\",\"$THECOMPILER:-Os!" Configure

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
                tail -n 100 "${LOG}"
		    	echo "Problem while configure - Please check ${LOG}"
		    	exit 1
		    fi

		    MIN_TYPE=-miphoneos-version-min=
		    if [[ "${IOS_ARCH}" == "i386" || "${IOS_ARCH}" == "x86_64" ]]; then
		    	MIN_TYPE=-mios-simulator-version-min=
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

			sed -ie "s!^CFLAG=!CFLAG=-isysroot ${CROSS_TOP}/SDKs/${CROSS_SDK} -arch $IOS_ARCH -Os -fPIC -stdlib=libc++ $MIN_TYPE$MIN_IOS_VERSION !" Makefile

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

			echo "Running make for ${IOS_ARCH}"
			echo "Please stand by..."
			# Must run at -j 1 (single thread only else will fail)
			make >> "${LOG}" 2>&1
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
		perl -pi -e 's/install: all install_docs install_sw/install: install_docs install_sw/g' Makefile.org
		
        # armv7
        ABI=armeabi-v7a
        local BUILD_TO_DIR=$BUILD_DIR/openssl/build/$TYPE/$ABI
        source ../../android_configure.sh $ABI
        export CC="$CC $CFLAGS"
        ./Configure --prefix=$BUILD_TO_DIR no-ssl2 no-ssl3 no-comp no-hw no-engine no-shared android-armv7
        make clean
        make deps -j${PARALLEL_MAKE}
        make build_libs -j${PARALLEL_MAKE}
        make install
		cp libssl.a $BUILD_DIR/openssl/build/$TYPE/$ABI/lib/
        cp libcrypto.a $BUILD_DIR/openssl/build/$TYPE/$ABI/lib/
        
        # x86
        ABI=x86
        local BUILD_TO_DIR=$BUILD_DIR/openssl/build/$TYPE/$ABI
        source ../../android_configure.sh $ABI
        export CC="$CC $CFLAGS $LDFLAGS"
        export LDCMD="-lz -lsupc++ -llog -ldl -lm -lc -lgnustl_static -lgcc"
        ./Configure --prefix=$BUILD_TO_DIR no-ssl2 no-ssl3 no-comp no-hw no-engine no-shared android-x86
        make clean
        make deps -j${PARALLEL_MAKE}
        make build_libs -j${PARALLEL_MAKE}
        mkdir -p $BUILD_DIR/openssl/build/$TYPE/$ABI/lib
        cp libssl.a $BUILD_DIR/openssl/build/$TYPE/$ABI/lib/
        cp libcrypto.a $BUILD_DIR/openssl/build/$TYPE/$ABI/lib/

	else 

		echoWarning "TODO: build $TYPE lib"

	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	#echoWarning "TODO: copy $TYPE lib"

	# # headers
	#deleting these is problematic when we do separate builds
	if [ -f $1/include/openssl/opensslconf_osx.h ]; then
        cp $1/include/openssl/opensslconf_osx.h /tmp/
    fi
	if [ -f lib/include/openssl/opensslconf_ios.h ]; then
        cp lib/include/openssl/opensslconf_ios.h /tmp/
    fi
	if [ -f $1/include/openssl/opensslconf_win32.h ]; then
        cp $1/include/openssl/opensslconf_win32.h /tmp/
    fi
	if [ -f $1/include/openssl/opensslconf_android.h ]; then
        cp $1/include/openssl/opensslconf_android.h /tmp/
    fi
	if [ -f $1/include/openssl/opensslconf_vs.h ]; then
        cp $1/include/openssl/opensslconf_vs.h /tmp/
    fi
	if [ -d $1/include/ ]; then
	    rm -r $1/include/
	fi
	
	mkdir -pv $1/include/openssl/
	if [ -f /tmp/opensslconf_osx.h ]; then
        mv /tmp/opensslconf_osx.h $1/include/openssl/
    fi
	if [ -f /tmp/opensslconf_ios.h ]; then
        mv /tmp/opensslconf_ios.h $1/include/openssl/
    fi
	if [ -f /tmp/opensslconf_win32.h ]; then
        mv /tmp/opensslconf_win32.h $1/include/openssl/
    fi
	if [ -f /tmp/opensslconf_android.h ]; then
        mv /tmp/opensslconf_android.h $1/include/openssl/
    fi
	if [ -f /tmp/opensslconf_vs.h ]; then
        mv /tmp/opensslconf_vs.h $1/include/openssl/
    fi
	
	# storing a copy of the include in lib/include/
	# set via: cp -R "build/$TYPE/x86_64/include/" "lib/include/"

	# suppress file not found errors
	#same here doesn't seem to be a solid reason to delete the files
	#rm -rf $1/lib/$TYPE/* 2> /dev/null

	# libs
	if [ "$TYPE" == "osx" ] ; then
		mv lib/include/openssl/opensslconf.h lib/include/openssl/opensslconf_osx.h
	    cp -Rv lib/include/* $1/include/
		mkdir -p $1/lib/$TYPE
		cp -v lib/$TYPE/*.a $1/lib/$TYPE
	elif [ "$TYPE" == "ios" ] ; then
	    mv lib/include/openssl/opensslconf.h lib/include/openssl/opensslconf_ios.h
	    cp -Rv lib/include/* $1/include/
	 	mkdir -p $1/lib/$TYPE
	 	cp -v lib/$TYPE/*.a $1/lib/$TYPE
	elif [ "$TYPE" == "vs" ] ; then	 
		if [ $ARCH == 32 ] ; then
			cp -Rv ms/Win32/include/openssl $1/include/
			rm -rf $1/lib/$TYPE/Win32
			mkdir -p $1/lib/$TYPE/Win32
			cp -v ms/Win32/lib/*.lib $1/lib/$TYPE/Win32/
			for f in $1/lib/$TYPE/Win32/*; do
				base=`basename $f .lib`
				mv -v $f $1/lib/$TYPE/Win32/${base}md.lib
			done
		elif [ $ARCH == 64 ] ; then
			cp -Rv ms/X64/include/openssl $1/include/
			rm -rf $1/lib/$TYPE/x64
			mkdir -p $1/lib/$TYPE/x64
			cp -v ms/x64/lib/*.lib $1/lib/$TYPE/x64/
			for f in $1/lib/$TYPE/x64/*; do
				base=`basename $f .lib`
				mv -v $f $1/lib/$TYPE/x64/${base}md.lib
			done
		fi
	 	
	# elif [ "$TYPE" == "win_cb" ] ; then
	# 	mkdir -p $1/lib/$TYPE
	# 	cp -v lib/MinGW/i686/*.a $1/lib/$TYPE
	
	elif [ "$TYPE" == "android" ] ; then
	    mv build/include/openssl/opensslconf.h build/include/openssl/opensslconf_android.h
	    cp -Rv build/android/armeabi-v7a/include/* $1/include/
	    if [ -d $1/lib/$TYPE/ ]; then
	        rm -r $1/lib/$TYPE/
	    fi
	    mkdir -p $1/lib/$TYPE/armeabi-v7a
		cp -rv build/android/armeabi-v7a/lib/*.a $1/lib/$TYPE/armeabi-v7a/
	    mkdir -p $1/lib/$TYPE/x86
		cp -rv build/android/x86/lib/*.a $1/lib/$TYPE/x86/

		git checkout ../../libs/openssl/include/openssl/comp.h
        git checkout ../../libs/openssl/include/openssl/engine.h

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
	
	
    # opensslconf.h might be different per platform. Copy custom file
    cp -v $FORMULA_DIR/opensslconf.h $1/include/openssl/opensslconf.h
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

