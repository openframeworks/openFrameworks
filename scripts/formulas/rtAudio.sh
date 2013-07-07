#! /bin/bash
#
# RealTime Audio input/output across Linux, Macintosh OS-X and Windows
# http://www.music.mcgill.ca/~gary/rtaudio/

VER=4.0.12

# download the source code and unpack it into LIB_NAME
function download() {
	curl -O http://www.music.mcgill.ca/~gary/rtaudio/release/rtaudio-$VER.tar.gz
	tar -xf rtaudio-$VER.tar.gz
	mv rtaudio-$VER rtaudio
}

# executed inside the build dir
function build() {
	
	if [ "$TYPE" == "ios" -o "$TYPE" == "android" ] ; then
		echo "build not needed for $TYPE"
		return
	fi

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
			echo "TODO: linux build here"
			
		elif [ "$TYPE" == "vs2010" -o "$TYPE" == "win_cb" ] ; then
			local API="--with-ds" # asio as well?
			echo "TODO: windows build here"
		fi
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	
	if [ "$TYPE" == "ios" -o "$TYPE" == "android" ] ; then
		return
	fi

	# headers
	mkdir -p $1/include
	cp -v RtAudio.h $1/include
	cp -v RtError.h $1/include

	# libs
	mkdir -p $1/lib/$TYPE
	if [ "$TYPE" == "vs2010" ] ; then
		echo "TODO: copy for $TYPE"
	else
		cp -v librtaudio.a $1/lib/$TYPE/rtaudio.a
	fi
}
