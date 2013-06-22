#! /bin/bash
#
# Free Image
# http://freeimage.sourceforge.net/

VER=3154 # 3.15.4

SRC=FreeImage
DEST=../FreeImage

###

# get first arg
if [ $# < 2 ] ; then
	echo "need os build type"
	exit
fi

# change to the dir of this script
cd $(dirname $0)

OS=$1 #`./ostype.sh`

# get source
mkdir -p $DEST
curl -LO http://downloads.sourceforge.net/freeimage/FreeImage"$VER".zip
unzip -qo FreeImage"$VER".zip

# copy headers
mkdir -p $DEST/include
cp -v $SRC/Dist/*.h $DEST/include

# build & copy libs
cd $SRC
if [ "$OS" == "osx" ] ; then
	mkdir -p $DEST/lib/osx
	make -f Makefile.gnu
	cd -
	cp -v $SRC/Dist/*.a $DEST/lib/osx

elif [ "$OS" == "vs2010" ] ; then
	mkdir -p $DEST/lib/vs2010
	MSBuild.exe FreeImage.2008.sln
	cd -
	cp -v $SRC/Dist/*.lib $DEST/lib/vs2010

elif [ "$OS" == "win_cb" ] ; then
	mkdir -p $DEST/lib/win_cb
	make -f Makefile.minigw
	cd -
	cp -v $SRC/Dist/*.lib $DEST/lib/win_cb

if [ "$OS" == "ios" ] ; then
	mkdir -p $DEST/lib/ios
	make -f Makefile.iphone
	cd -
	cp -v $SRC/Dist/*.a $DEST/lib/ios

if [ "$OS" == "android" ] ; then

fi

#
#fi
#cp $SRC/Dist/*.lib $DEST/lib

# # remove uneeded makefiles
# find libpd -name "GNUmakefile.am" -delete
# find libpd -name "Makefile.am" -delete
# find libpd -name "makefile" -delete
# rm libpd/pure-data/extra/makefile.subdir

# # we dont need the java or csharp wrappers
# rm libpd/libpd_wrapper/z_jni.c
# rm libpd/libpd_wrapper/z_jni.h
# rm libpd/libpd_wrapper/z_csharp_helper.c
# rm libpd/libpd_wrapper/z_csharp_helper.h

# # remove expr~ since it's GPL, leave that up to devs
# rm -rf libpd/pure-data/extra/expr~
# rm libpd/pure-data/extra/expr-help.pd

# # copy license
# cp -v libpd/LICENSE.txt ../src/pd

# # copy sources
# cp -Rv libpd/cpp ../src/pd
# cp -Rv libpd/pure-data ../src/pd
# cp -Rv libpd/libpd_wrapper ../src/pd

# # cleanup
 rm -rf FreeImage*

