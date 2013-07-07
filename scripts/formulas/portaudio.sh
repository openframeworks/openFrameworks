#! /bin/bash
#
# Portable Cross-platform Audio I/O
# http://www.portaudio.com/

VER=stable_v19_20111121

# download the source code and unpack it into LIB_NAME
function download() {
	curl -O http://www.portaudio.com/archives/pa_$VER.tgz
	tar -xf pa_$VER.tgz
	rm pa_$VER.tgz
}

# executed inside the build dir
function build() {
	echo "build not needed for $TYPE"
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	
	# headers
	mkdir -p $1/include
	cp -Rv include/* $1/include
}
