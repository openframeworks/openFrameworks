#! /bin/bash

WD=$(dirname $0)

DEST=../libs/libfreenect
SRC=libfreenect

###

cd $WD

# get latest source
#git clone git://github.com/OpenKinect/libfreenect.git
git clone git://github.com/wizgrav/libfreenect.git

# remove uneeded makefiles
rm $SRC/src/CMakeLists.txt
rm $SRC/src/libfreenect.pc.in

# we dont need freenect audio
rm $SRC/include/libfreenect-audio.h
rm $SRC/src/audio.c
rm $SRC/src/loader.c

# no python either
rm $SRC/src/fwfetcher.py

# make folders
mkdir -p $DEST/include
mkdir -p $DEST/src
mkdir -p $DEST/platform

# copy licenses, etc
cp -v $SRC/APACHE20 $DEST
cp -v $SRC/CONTRIB $DEST
cp -v $SRC/GPL2 $DEST

# copy headers
cp -Rv $SRC/include/* $DEST/include

# copy sources
cp -Rv $SRC/src/* $DEST/src

# copy platform specfic stuff
cp -Rv $SRC/platform/linux $DEST/platform
cp -Rv $SRC/platform/windows $DEST/platform

# cleanup
rm -rf $SRC
