#! /bin/bash
#
# Cairo 2D Graphics
# http://cairographics.org/

VER=1.12.14

SRC=cairo
DEST=../cairo

###

# get first arg
if [ $# -gt 2 ] ; then
	OS=$1
else
	OS=`./ostype.sh`
fi

# change to the dir of this script
cd $(dirname $0)

# get source
curl -LO http://cairographics.org/releases/$SRC.tar.xz
tar -xf $SRC.tar.xz

# copy headers
mkdir -p $DEST/include
cp -Rv $SRC/include/assimp/* $DEST/include

# build & copy libs
if [ "$OS" == "vs2010" ] ; then

else

	cd $SRC
	./configure
	//cmake -G 'Unix Makefiles' -DASSIMP_BUILD_STATIC_LIB=1
	make assimp
	cd -

	mkdir -p $DEST/lib/osx
	cp -v $SRC/lib/libassimp.a $DEST/lib/osx/assimp.a
fi

# cleanup
 rm -rf $SRC
