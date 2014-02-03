echo "inserting gstreamer 1.0 repository"
rm /etc/apt/sources.list.d/gstreamer.list
touch /etc/apt/sources.list.d/gstreamer.list
echo "deb http://vontaene.de/raspbian-updates/ . main" > /etc/apt/sources.list.d/gstreamer.list
echo "updating apt database"
apt-get update

GSTREAMER_VERSION=1.0

echo "installing OF dependencies"
apt-get install alsa-base alsa-tools alsa-utils libupnp-dev automake cvs libavcodec-dev libavformat-dev libavutil-dev libswscale-dev libasound2-dev libxmu-dev libxxf86vm-dev g++ libgl1-mesa-dev libglu1-mesa-dev libraw1394-dev libudev-dev libdrm-dev libglew-dev libopenal-dev libsndfile-dev libfreeimage-dev libcairo2-dev libgtk2.0-dev python-lxml python-argparse libfreetype6-dev libassimp-dev portaudio19-dev libssl-dev libpulse-dev libgtk2.0-dev
exit_code=$?
if [ $exit_code != 0 ]; then
    echo "error installing dependencies, there could be an error with your internet connection"
    echo "if the error persists, please report an issue in github: http://github.com/openframeworks/openFrameworks/issues"
	exit $exit_code
fi


echo "installing gstreamer"
apt-get install libgstreamer${GSTREAMER_VERSION}-dev libgstreamer-plugins-base${GSTREAMER_VERSION}-dev gstreamer${GSTREAMER_VERSION}-pulseaudio gstreamer${GSTREAMER_VERSION}-x gstreamer${GSTREAMER_VERSION}-plugins-bad gstreamer${GSTREAMER_VERSION}-alsa gstreamer${GSTREAMER_VERSION}-plugins-base gstreamer${GSTREAMER_VERSION}-plugins-good ${GSTREAMER_VERSION}-omx 
exit_code=$?
if [ $exit_code != 0 ]; then
	echo "error installing gstreamer, there could be an error with your internet connection"
    echo "if the error persists, please report an issue in github: http://github.com/openframeworks/openFrameworks/issues"
	exit $exit_code
fi
