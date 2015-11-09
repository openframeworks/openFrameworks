#! /bin/bash
#
# Cairo
# 2D graphics library with support for multiple output devices
# http://www.cairographics.org/
#
# has an autotools build system and requires pkg-config, libpng, & pixman,
# dependencies have their own formulas in cairo/depends
#
# following http://www.cairographics.org/end_to_end_build_for_mac_os_x,
# we build and install dependencies into a subfolder of cairo by setting the 
# prefix (install location) and use a custom copy of pkg-config which returns
# the dependent lib cflags/ldflags for that prefix (cairo/apothecary-build)

FORMULA_TYPES=( "osx" "vs" "msys2" )

FORMULA_DEPENDS=( "pkg-config" "zlib" "libpng" "pixman" "freetype" )

# tell apothecary we want to manually call the dependency commands
# as we set some env vars for osx the depends need to know about
FORMULA_DEPENDS_MANUAL=1

# define the version
VER=1.12.14

# tools for git use
GIT_URL=http://anongit.freedesktop.org/git/cairo
GIT_TAG=$VER

# download the source code and unpack it into LIB_NAME
function download() {
	if [ "$TYPE" == "vs" ] ; then
		# Download the xz extractor.
		curl -LO http://tukaani.org/xz/xz-5.2.1-windows.zip

		# Unzip xz and save it to the local directory.
		unzip -j xz-5.2.1-windows.zip bin_x86-64/xz.exe -d .
	fi

	curl -LO http://cairographics.org/releases/cairo-$VER.tar.xz
	tar -xf cairo-$VER.tar.xz
	mv cairo-$VER cairo
	rm cairo-$VER.tar.xz

	# manually download dependencies
	apothecaryDependencies download

	if [ "$TYPE" == "vs" ] ; then
		# Get the custom cairo visual studio solution.
		git clone https://github.com/DomAmato/Cairo-VS

		# Remove the xz extractor.
		rm xz.exe
	fi
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	if [ "$TYPE" == "vs" ] ; then
	
		apothecaryDependencies prepare
		
		apothecaryDepend build freetype
		
		cd ../Cairo-VS
		
		cp -Rv ../cairo/* cairo
		cp -Rv ../freetype/* freetype
		cp -Rv ../libpng/* libpng
		cp -Rv ../pixman/* pixman
		cp -Rv ../zlib/* zlib
		
		if [ $ARCH == 32 ] ; then
			cp -v ../freetype/objs/vc2010/Win32/*.lib libs/freetype.lib
		elif [ $ARCH == 64 ] ; then
			cp -v ../freetype/objs/vc2010/x64/*.lib libs/freetype.lib
		fi
		
	else
		# generate the configure script if it's not there
		if [ ! -f configure ] ; then
			./autogen.sh
		fi

		# manually prepare dependencies
		apothecaryDependencies prepare

		# Build and copy all dependencies in preparation
		apothecaryDepend build pkg-config
		apothecaryDepend copy pkg-config
		apothecaryDepend build libpng
		apothecaryDepend copy libpng
		apothecaryDepend build pixman
		apothecaryDepend copy pixman
		apothecaryDepend build freetype
		apothecaryDepend copy freetype
	fi
}

# executed inside the lib src dir
function build() {

	if [ "$TYPE" == "vs" ] ; then
		cd ../Cairo-VS/projects
		if [ $ARCH == 32 ] ; then
			vs-build "cairo.sln"
		elif [ $ARCH == 64 ] ; then
			vs-build "cairo.sln" Build "Release|x64"
		fi
		 
	elif [ "$TYPE" == "osx" ] ; then
		./configure PKG_CONFIG="$BUILD_ROOT_DIR/bin/pkg-config" \
					PKG_CONFIG_PATH="$BUILD_ROOT_DIR/lib/pkgconfig" \
					LDFLAGS="-arch i386 -stdlib=libstdc++ -arch x86_64 -Xarch_x86_64 -stdlib=libc++" \
					CFLAGS="-Os -arch i386 -stdlib=libstdc++ -arch x86_64 -Xarch_x86_64 -stdlib=libc++" \
					--prefix=$BUILD_ROOT_DIR \
					--disable-gtk-doc \
					--disable-gtk-doc-html \
					--disable-gtk-doc-pdf \
					--disable-full-testing \
					--disable-dependency-tracking \
					--disable-xlib \
					--disable-qt 
		make -j${PARALLEL_MAKE} 
		make install
	else 
		./configure PKG_CONFIG="$BUILD_ROOT_DIR/bin/pkg-config" \
					PKG_CONFIG_PATH="$BUILD_ROOT_DIR/lib/pkgconfig" \
					LDFLAGS="-arch i386 -arch x86_64" \
					CFLAGS="-Os -arch i386 -arch x86_64" \
					--prefix=$BUILD_ROOT_DIR \
					--disable-gtk-doc \
					--disable-gtk-doc-html \
					--disable-gtk-doc-pdf \
					--disable-full-testing \
					--disable-dependency-tracking \
					--disable-xlib \
					--disable-qt 
		make -j${PARALLEL_MAKE} 
		make install
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	if [ "$TYPE" == "vs" ] ; then
		cd ..
		#this copies all header files but we dont need all of them it seems
		#maybe alter the VS-Cairo build to separate necessary headers
		# make the path in the libs dir
		mkdir -p $1/include/cairo

		# copy the cairo headers
		cp -Rv cairo/src/*.h $1/include/cairo		
		
		if [ $ARCH == 32 ] ; then
			# make the libs path 
			mkdir -p $1/lib/$TYPE/Win32
			cp -v Cairo-VS/projects/Release/cairo.lib $1/lib/$TYPE/Win32/cairo-static.lib
			cp -v Cairo-VS/projects/Release/pixman.lib $1/lib/$TYPE/Win32/pixman-1.lib
			cp -v Cairo-VS/libs/libpng.lib $1/lib/$TYPE/Win32
		elif [ $ARCH == 64 ] ; then
			# make the libs path 
			mkdir -p $1/lib/$TYPE/x64
			cp -v Cairo-VS/projects/x64/Release/cairo.lib $1/lib/$TYPE/x64/cairo-static.lib
			cp -v Cairo-VS/projects/x64/Release/pixman.lib $1/lib/$TYPE/x64/pixman-1.lib
			cp -v Cairo-VS/libs/libpng.lib $1/lib/$TYPE/x64
		fi
		cd cairo

	elif [ "$TYPE" == "osx" -o "$TYPE" == "msys2" ] ; then
		# make the path in the libs dir
		mkdir -p $1/include/cairo

		# copy the cairo headers
		cp -Rv $BUILD_ROOT_DIR/include/cairo/* $1/include/cairo

		# make the libs path 
		mkdir -p $1/lib/$TYPE
	
		if [ "$TYPE" == "osx" ] ; then
			cp -v $BUILD_ROOT_DIR/lib/libcairo-script-interpreter.a $1/lib/$TYPE/cairo-script-interpreter.a
		fi
		cp -v $BUILD_ROOT_DIR/lib/libcairo.a $1/lib/$TYPE/cairo.a
		cp -v $BUILD_ROOT_DIR/lib/libpixman-1.a $1/lib/$TYPE/pixman-1.a
		cp -v $BUILD_ROOT_DIR/lib/libpng.a $1/lib/$TYPE/png.a
	fi

	# copy license files
	rm -rf $1/license # remove any older files if exists
	mkdir -p $1/license
	cp -v COPYING $1/license/
	cp -v COPYING-LGPL-2.1 $1/license/
	cp -v COPYING-MPL-1.1 $1/license/
}

# executed inside the lib src dir
function clean() {
	
	# manually clean dependencies
	apothecaryDependencies clean

	# cairo
	make clean
}
