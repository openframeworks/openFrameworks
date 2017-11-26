#!/bin/bash

yum localinstall -y --nogpgcheck \
	https://download1.rpmfusion.org/free/el/rpmfusion-free-release-6.noarch.rpm \
	https://download1.rpmfusion.org/nonfree/el/rpmfusion-nonfree-release-6.noarch.rpm


yum install -y ffmpeg-devel libmpg123-devel mpg123 gstreamer-plugins-ugly gstreamer-ffmpeg
