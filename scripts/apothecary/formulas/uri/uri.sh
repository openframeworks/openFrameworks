#! /bin/bash
#
# cpp-uri
# Uri parsing library that will become part of c++ std
# 
# uses cmake build system

FORMULA_TYPES=( "osx" "win_cb" "ios" "android" "linux" "linux64" "linuxarmv6l" "linuxarmv7l" "emscripten" "vs" )

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
	: #noop
}

# executed inside the lib src dir
function build() {
	git checkout .
	rm -rf _build
	mkdir -p _build
	pwd
	if [ "$TYPE" == "wincb" ] ; then
	    git apply ../../formulas/uri/uri-remove-tests.patch
	    cd _build
		#noop by now
		
	elif [ "$TYPE" == "vs" ] ; then
	    git apply ../../formulas/uri/uri-remove-tests.patch
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
	    git apply ../../formulas/uri/uri-remove-tests.patch
	    cd _build
		export BOOST_LIBRARYDIR=${BUILD_DIR}/boost/stage/lib
		export BOOST_INCLUDEDIR=${BUILD_DIR}/boost/
		cmake -DCMAKE_BUILD_TYPE=Release \
			  -DCMAKE_C_FLAGS="-arch i386 -arch x86_64" \
			  -DCMAKE_CXX_FLAGS="-arch i386 -arch x86_64" \
			  ..
		make -j${PARALLEL_MAKE}
	
	elif [ "$TYPE" == "emscripten" ]; then
		export BOOST_LIBRARYDIR=${BUILD_DIR}/boost/stage/lib
		export BOOST_INCLUDEDIR=${BUILD_DIR}/boost/
	    git apply ../../formulas/uri/uri-emscripten.patch
	    cd _build
    	pwd
		emcmake cmake .. -DCMAKE_CXX_FLAGS=-I${BOOST_INCLUDEDIR} -DCMAKE_BUILD_TYPE=Release -DBOOST_LIBRARYDIR=${BUILD_DIR}/boost/stage/lib -DBoost_INCLUDE_DIR=${BUILD_DIR}/boost
		emmake make -j${PARALLEL_MAKE}
	
	elif [ "$TYPE" == "android" ]; then
	    git apply ../../formulas/uri/uri-remove-tests.patch
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
		
	elif [ "$TYPE" == "osx" ] || [ "$TYPE" == "ios" ]; then
		cp _build/src/*.a $1/lib/$TYPE/
		cp -r src/network $1/include/
		../boost/dist/bin/bcp --scan --boost=../boost $(find src/network/ -name "*.hpp") install_dir
		rsync -ar install_dir/boost/* $1/../boost/include/boost/
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
