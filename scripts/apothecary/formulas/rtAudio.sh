#! /bin/bash
#
# RtAudio
# RealTime Audio input/output across Linux, Macintosh OS-X and Windows
# http://www.music.mcgill.ca/~gary/rtaudio/
#
# uses an autotools build system

FORMULA_TYPES=( "osx" "linux" "linux64" "vs" "win_cb" )

#FORMULA_DEPENDS=( "pkg-config" )

# tell apothecary we want to manually call the dependency commands
# as we set some env vars for osx the depends need to know about
#FORMULA_DEPENDS_MANUAL=1

# define the version
VER=4.0.12

# tools for git use
GIT_URL=
GIT_TAG=

# download the source code and unpack it into LIB_NAME
function download() {
	curl -O http://www.music.mcgill.ca/~gary/rtaudio/release/rtaudio-$VER.tar.gz
	tar -xf rtaudio-$VER.tar.gz
	mv rtaudio-$VER rtAudio
	rm rtaudio-$VER.tar.gz
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	: # noop
}

# executed inside the lib src dir
function build() {
	# The ./configure / MAKEFILE sequence is broken for OSX, making it 
	# impossible to create universal libs in one pass.  As a result, we compile
	# the project manually according to the author's page:
	# https://www.music.mcgill.ca/~gary/rtaudio/compiling.html

	if [ "$TYPE" == "osx" ] ; then

		# Compile the program
		/usr/bin/g++ -O2 \
					 -Wall \
					 -fPIC \
					 -arch i386 \
					 -arch x86_64 \
					 -Iinclude \
					 -DHAVE_GETTIMEOFDAY \
					 -D__MACOSX_CORE__ \
					 -c RtAudio.cpp \
					 -o RtAudio.o

		/usr/bin/ar ruv librtaudio.a RtAudio.o
		/usr/bin/ranlib librtaudio.a

	else

		if [ "$TYPE" == "linux" -o "$TYPE" == "linux64" ] ; then
			local API="--with-alsa" # jack or pulse as well?
			./configure --with-alsa
			make 
		elif [ "$TYPE" == "vs" -o "$TYPE" == "win_cb" ] ; then
			local API="--with-ds" # asio as well?
			echoWarning "TODO: build $TYPE"
		fi
	fi

	# clean up env vars
	# unset PKG_CONFIG PKG_CONFIG_PATH

}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {

	# headers
	mkdir -p $1/include
	cp -v RtAudio.h $1/include
	cp -v RtError.h $1/include

	# libs
	mkdir -p $1/lib/$TYPE
	if [ "$TYPE" == "vs" ] ; then
		echoWarning "TODO: copy vs lib"

	elif [ "$TYPE" == "win_cb" ] ; then
		echoWarning "TODO: copy win_cb lib"
	
	else
		cp -v librtaudio.a $1/lib/$TYPE/rtaudio.a
	fi

	# Copy License File
    cp -v readme $1/readme

}

# executed inside the lib src dir
function clean() {
	if [ "$TYPE" == "vs" ] ; then
		echoWarning "TODO: clean vs"
	else
		make clean
	fi

	# manually clean dependencies
	#apothecaryDependencies clean


}
