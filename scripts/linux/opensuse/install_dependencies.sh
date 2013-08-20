#!/bin/bash
DISTROVER=$(grep ^VERSION /etc/SuSE-release | tr -d ' ' | cut -d = -f 2)
zypper addrepo obs://games/openSUSE_$DISTROVER games # for freeimage
zypper refresh games
zypper install freeglut-devel alsa-devel libXmu-devel libXxf86vm-devel gcc-c++ libraw1394-devel gstreamer-devel gstreamer-plugins-base-devel libudev-devel libtheora-devel libvorbis-devel openal-soft-devel libsndfile-devel python-lxml glew-devel flac-devel freeimage-devel cairo-devel libjack-devel portaudio-devel libpulse-devel
