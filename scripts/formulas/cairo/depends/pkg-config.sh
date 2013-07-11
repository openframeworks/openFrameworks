#! /bin/bash
#
# a helper tool used when compiling applications and libraries
# http://www.freedesktop.org/wiki/Software/pkg-config/

VER=0.28

# download the source code and unpack it into LIB_NAME
function download() {
	curl -LO http://pkgconfig.freedesktop.org/releases/pkg-config-$VER.tar.gz
	tar -xf pkg-config-$VER.tar.gz
	mv pkg-config-$VER pkg-config
	rm pkg-config-$VER.tar.gz
}

# executed inside the lib src dir
function build() {
	# setting empty flags so it ignores an existing pkg-config install
	./configure --prefix=$BUILD_DIR --with-internal-glib GLIB_CFLAGS="" GLIB_LIBS=""
	make
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	make install
}

# executed inside the lib src dir
function clean() {
	make uninstall
	make clean
}
