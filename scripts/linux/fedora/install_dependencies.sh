## thanks to Claudio for details on packages to install on fedora

yum install freeglut-devel alsa-lib-devel libXmu-devel libXxf86vm-devel gcc-c++ libraw1394-devel ffmpeg-devel 

yum install gstreamer-devel gstreamer-plugins-base-devel libudev-devel


rpm -Uvh http://download1.rpmfusion.org/free/fedora/rpmfusion-free-release-stable.noarch.rpm http://download1.rpmfusion.org/nonfree/fedora/rpmfusion-nonfree-release-stable.noarch.rpm


yum install ffmpeg-devel libtheora-devel libvorbis-devel

ARCH=$(uname -m)
if [ "$ARCH" = "x86_64" ]; then
        LIBSPATH=linux64
else
        LIBSPATH=linux
fi

WHO=`sudo who am i`;ID=`echo ${WHO%% *}`
cd ../../../libs/openFrameworksCompiled/project/$LIBSPATH
make Debug
if [ $? != 0 ]; then
        echo "there has been a problem compiling Debug OF library"
        echo "please report this problem in the forums"
        exit
fi
chown -R $ID:$ID obj ../../lib/${LIBSPATH}/*
make Release
if [ $? != 0 ]; then
        echo "there has been a problem compiling Release OF library"
        echo "please report this problem in the forums"
fi
chown -R $ID:$ID obj ../../lib/${LIBSPATH}/*

