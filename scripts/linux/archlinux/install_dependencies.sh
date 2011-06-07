pacman -Syu
pacman -S make pkg-config gcc openal python-lxml glew
#pacman -S libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libavcodec-dev libavformat-dev libavutil-dev libswscale-dev freeglut3-dev libasound2-dev libxmu-dev libxxf86vm-dev g++ libgl1-mesa-dev libglu1-mesa-dev libraw1394-dev libudev-dev libdrm-dev gstreamer0.10-ffmpeg libglew1.5-dev libopenal-dev libsndfile-dev libfreeimage-dev
ARCH=$(uname -m)
if [ "$ARCH" = "x86_64" ]; then
	LIBSPATH=linux64
else
	LIBSPATH=linux
fi

WHO=`su -c 'who am i'`;ID=`echo ${WHO%% *}`
#since users in arch have no default named group
GROUP_ID=`id --group -n ${ID}`
cd ../../../libs/openFrameworksCompiled/project/$LIBSPATH
#make Debug
if [ $? != 0 ]; then
	echo "there has been a problem compiling Debug OF library"
	echo "please report this problem in the forums"
	exit
fi
chown -R $ID:$GROUP_ID obj ../../lib/${LIBSPATH}/*
#make Release
if [ $? != 0 ]; then
        echo "there has been a problem compiling Debug OF library"
        echo "please report this problem in the forums"
fi
chown -R $ID:$GROUP_ID obj ../../lib/${LIBSPATH}/*
# libpoco-dev 
