apt-get update
apt-get install libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libavcodec-dev libavformat-dev libavutil-dev libswscale-dev freeglut3-dev libasound2-dev libxmu-dev libxxf86vm-dev g++ libgl1-mesa-dev libglu1-mesa-dev libraw1394-dev libudev-dev libdrm-dev gstreamer0.10-ffmpeg
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
        echo "there has been a problem compiling Debug OF library"
        echo "please report this problem in the forums"
fi
chown -R $ID:$ID obj ../../lib/${LIBSPATH}/*
# libpoco-dev 
