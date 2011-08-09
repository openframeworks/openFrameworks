## thanks to Claudio for details on packages to install on fedora

yum install freeglut-devel alsa-lib-devel libXmu-devel libXxf86vm-devel gcc-c++ libraw1394-devel gstreamer-devel gstreamer-plugins-base-devel libudev-devel libtheora-devel libvorbis-devel openal-soft-devel libsndfile-devel python-lxml gstreamer-devel glew-devel flac-devel freeimage-devel cairo-devel jack-audio-connection-kit-devel

ARCH=$(uname -m)
if [ "$ARCH" = "x86_64" ]; then
        LIBSPATH=linux64
else
        LIBSPATH=linux
fi

WHO=`sudo who am i`;ID=`echo ${WHO%% *}`
GROUP_ID=`id --group -n ${ID}`
cd ../../../libs/openFrameworksCompiled/project/$LIBSPATH
make Debug
if [ $? != 0 ]; then
        echo "there has been a problem compiling Debug OF library"
        echo "please report this problem in the forums"
        exit
fi
chown -R $ID:$GROUP_ID obj ../../lib/${LIBSPATH}/*
make Release
if [ $? != 0 ]; then
        echo "there has been a problem compiling Release OF library"
        echo "please report this problem in the forums"
fi
chown -R $ID:$GROUP_ID obj ../../lib/${LIBSPATH}/*

