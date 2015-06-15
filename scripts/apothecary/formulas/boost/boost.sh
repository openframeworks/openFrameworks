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
	if [ "$TYPE" == "osx" ] || [ "$TYPE" == "ios" ] || [ "$TYPE" == "emscripten" ]; then
		./bootstrap.sh --with-toolset=clang --with-libraries=filesystem
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
		
		
	elif [ "$TYPE" == "osx" ] || [ "$TYPE" == "ios" ]; then
		./b2 -j${PARALLEL_MAKE} toolset=clang cxxflags="-std=c++11 -stdlib=libc++ -arch i386 -arch x86_64" linkflags="-stdlib=libc++" threading=multi variant=release --build-dir=build --stage-dir=stage link=static stage
		cd tools/bcp  
		../../b2
		
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
	elif [ "$TYPE" == "osx" ] || [ "$TYPE" == "ios" ]; then
		dist/bin/bcp filesystem install_dir
		rsync -ar install_dir/boost/* $1/include/boost/
		cp stage/lib/libboost_filesystem.a $1/lib/$TYPE/boost_filesystem.a
		cp stage/lib/libboost_system.a $1/lib/$TYPE/boost_system.a
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
