apt-get update
apt-get install libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libavcodec-dev libavformat-dev libavutil-dev libswscale-dev freeglut3-dev libasound2-dev libxmu-dev libxxf86vm-dev g++ libgl1-mesa-dev libglu1-mesa-dev libraw1394-dev libudev-dev libdrm-dev gstreamer0.10-ffmpeg libglew-dev libopenal-dev libsndfile-dev libfreeimage-dev libcairo2-dev libgtk2.0-dev libjack-dev python-lxml python-argparse
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
exit_code=$?
if [ $exit_code != 0 ]; then
  echo "there has been a problem compiling Debug OF library"
  echo "please report this problem in the forums"
  exit $exit_code
fi
chown -R $ID:$GROUP_ID obj ../../lib/${LIBSPATH}/*
make Release
exit_code=$?
if [ $exit_code != 0 ]; then
  echo "there has been a problem compiling Release OF library"
  echo "please report this problem in the forums"
  exit $exit_code
fi
chown -R $ID:$GROUP_ID obj ../../lib/${LIBSPATH}/*
