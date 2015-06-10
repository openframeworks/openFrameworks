#! /bin/bash
#
# cpp-uri
# Uri parsing library that will become part of c++ std
# 
# uses cmake build system

FORMULA_TYPES=( "osx" "win_cb" "ios" "android" )

# define the version
COMMIT=55ec3cd78918c42dfb874e01c9745b4daf51091b
GIT_URL=https://github.com/cpp-netlib/uri.git

# download the source code and unpack it into LIB_NAME
function download() {
	git clone ${GIT_URL} uri
	git checkout ${COMMIT}
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	: #noop
}

# executed inside the lib src dir
function build() {
	if [ "$TYPE" == "wincb" ] ; then
		: #noop by now
	elif [ "$TYPE" == "osx" ]; then
		git checkout .
		git apply ../../formulas/uri/uri-remove-tests.patch
		export BOOST_LIBRARYDIR=../boost/stage/lib
		export BOOST_INCLUDEDIR=../boost/
		mkdir -p _build
		cd _build
		cmake -DCMAKE_BUILD_TYPE=Release \
			  -DCMAKE_C_FLAGS="-arch i386 -arch x86_64" \
			  -DCMAKE_CXX_FLAGS="-arch i386 -arch x86_64" \
			  ..
		make
	elif [ "$TYPE" == "linux" ] || [ "$TYPE" == "linux64" ] || [ "$TYPE" == "linuxarmv6l" ] || [ "$TYPE" == "linuxarmv7l" ]; then
		git checkout .
		git apply ../../formulas/uri/uri-remove-tests.patch
		mkdir -p _build
		cd _build
		cmake -DCMAKE_BUILD_TYPE=Release ..
		make
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
	else
		cp _build/src/*.a $1/lib/$TYPE/
		cp -r src/network $1/include/
		../boost/dist/bin/bcp --scan --boost=../boost $(find src/network/ -name "*.hpp") install_dir
		rsync -ar install_dir/boost/* $1/../boost/include/boost/
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
