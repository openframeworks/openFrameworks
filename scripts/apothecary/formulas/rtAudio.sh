#! /bin/bash
#
# RtAudio
# RealTime Audio input/output across Linux, Macintosh OS-X and Windows
# http://www.music.mcgill.ca/~gary/rtaudio/
#
# uses an autotools build system

FORMULA_TYPES=( "osx" "linux" "linux64" "vs" "win_cb" )

VER=4.0.12

# download the source code and unpack it into LIB_NAME
function download() {
	curl -O http://www.music.mcgill.ca/~gary/rtaudio/release/rtaudio-$VER.tar.gz
	tar -xf rtaudio-$VER.tar.gz
	mv rtaudio-$VER rtAudio
	rm rtaudio-$VER.tar.gz
}

# executed inside the lib src dir
function build() {

	# choose audio api for configure
	if [ "$TYPE" == "osx" ] ; then
		# with help from http://stackoverflow.com/questions/3350107/how-to-compile-universal-libraries-on-mac-os-x
		local API="--with-core"

		# 32 bit
		./configure $API CFLAGS="-arch i386" CPPFLAGS="-arch i386" LDFLAGS="-arch i386" 
		make clean; make
		mv librtaudio.a librtaudio-i386.a

		# 64 bit
		./configure $API CFLAGS="-arch x86_64" CPPFLAGS="-arch x86_64" LDFLAGS="-arch x86_64" 
		make clean; make
		mv librtaudio.a librtaudio-x86_64.a

		# link into universal lib
		lipo -c librtaudio-i386.a librtaudio-x86_64.a -o librtaudio.a

	else

		if [ "$TYPE" == "linux" -o "$TYPE" == "linux64" ] ; then
			local API="--with-alsa" # jack or pulse as well?
			echoWarning "TODO: build linux"
			
		elif [ "$TYPE" == "vs" -o "$TYPE" == "win_cb" ] ; then
			local API="--with-ds" # asio as well?
			echoWarning "TODO: build $TYPE"
		fi
	fi
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
}

# executed inside the lib src dir
function clean() {
	if [ "$TYPE" == "vs" ] ; then
		echoWarning "TODO: clean vs"
	else
		make clean
	fi
}
