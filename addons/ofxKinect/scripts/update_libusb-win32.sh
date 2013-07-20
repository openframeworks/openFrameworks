#! /bin/bash

WD=$(dirname $0)

VER=1.2.6.0

DEST=../libs/libusb-win32
SRC=libusb-win32-bin-$VER

###

cd $WD

# make folders
mkdir -p $DEST/include
mkdir -p $DEST/lib/vs2010
mkdir -p $DEST/lib/win_cb

### Debug

# get source
curl -LO http://downloads.sourceforge.net/project/libusb-win32/libusb-win32-releases/$VER/libusb-win32-bin-debug-$VER.zip
unzip -oq libusb-win32-bin-debug-$VER.zip

# copy libs
cp -v $SRC/lib/msvc/libusb.lib $DEST/lib/vs2010/libusbd.lib
cp -v $SRC/lib/msvc/libusb.lib $DEST/lib/win_cb/libusbd.lib

# cleanup
rm -rf libusb-win32-bin-debug-$VER
rm libusb-win32-bin-debug-$VER.zip

### Release

# get source
curl -LO http://downloads.sourceforge.net/project/libusb-win32/libusb-win32-releases/$VER/$SRC.zip
unzip -oq $SRC.zip

# copy licenses, etc
cp -v $SRC/AUTHORS.txt $DEST
cp -v $SRC/COPYING_LGPL.txt $DEST
cp -v $SRC/README.txt $DEST

# copy headers
cp -Rv $SRC/include/* $DEST/include

# copy libs
cp -v $SRC/lib/msvc/libusb.lib $DEST/lib/vs2010
cp -v $SRC/lib/msvc/libusb.lib $DEST/lib/win_cb

# cleanup
rm -rf $SRC
rm $SRC.zip