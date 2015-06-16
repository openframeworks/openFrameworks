#! /bin/bash
#
# a low-level software library for pixel manipulation
# http://pixman.org/

# define the version
VER=0.32.4

# tools for git use
GIT_URL=http://anongit.freedesktop.org/git/pixman.git
GIT_TAG=pixman-$VER

# download the source code and unpack it into LIB_NAME
function download() {
	curl -LO http://cairographics.org/releases/pixman-$VER.tar.gz
	tar -xf pixman-$VER.tar.gz
	mv pixman-$VER pixman
	rm pixman-$VER.tar.gz
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	if [ "$TYPE" == "vs" ] ; then
		echo "handled in the cairo script"
	# generate the configure script if it's not there
	else 
		if [ ! -f configure ] ; then
			./autogen.sh
		fi
	fi
}

# executed inside the lib src dir
function build() {

	if [ "$TYPE" == "osx" ] ; then

		# these flags are used to create a fat 32/64 binary with i386->libstdc++, x86_64->libc++
		# see https://gist.github.com/tgfrerer/8e2d973ed0cfdd514de6
		local FAT_LDFLAGS="-arch i386 -arch x86_64 -stdlib=libstdc++ -Xarch_x86_64 -stdlib=libc++"

		./configure LDFLAGS="${FAT_LDFLAGS} " \
				CFLAGS="-O3 ${FAT_LDFLAGS}" \
				--prefix=$BUILD_ROOT_DIR \
				--disable-dependency-tracking \
				--disable-gtk \
				--disable-shared
		# only build & install lib, ignore demos/tests
		cd pixman
		make clean
		make
	elif [ "$TYPE" == "vs" ] ; then
		echo "build is handled in the cairo script"
#		sed s/-MD/-MT/ Makefile.win32 > Makefile.fixed
#		mv Makefile.fixed Makefile.win32
		
#		CURRENTPATH=`pwd`
#		WINPATH=$(echo "$CURRENTPATH" | sed -e 's/^\///' -e 's/\//\\/g' -e 's/^./\0:/')
#		cmd.exe /c 'call "%VS'${VS_VER}'0COMNTOOLS%vsvars32.bat" && nmake -f Makefile.win32 "CFG=release"'
		
		#cmd.exe /c 'nmake -f Makefile.win32 "CFG=release"'
	else
	./configure LDFLAGS="-arch i386 -arch x86_64" \
				CFLAGS="-O3 -arch i386 -arch x86_64" \
				--prefix=$BUILD_ROOT_DIR \
				--disable-dependency-tracking \
				--disable-gtk \
				--disable-shared
		# only build & install lib, ignore demos/tests
		cd pixman
		make clean
		make
	fi

	
	
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	if [ "$TYPE" == "vs" ] ; then
		echo "copy vs"
	else
		# lib
		cd pixman
		make install
		
		# pkg-config info
		cd ../
		make install-pkgconfigDATA
	fi
}

# executed inside the lib src dir
function clean() {
	make uninstall
	make clean
}
