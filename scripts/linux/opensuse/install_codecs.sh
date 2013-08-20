#!/bin/bash
DISTROVER=$(grep ^VERSION /etc/SuSE-release | tr -d ' ' | cut -d = -f 2)
zypper addrepo http://packman.inode.at/suse/$DISTROVER/ packman
zypper refresh packman
zypper install libffmpeg-devel libmpg123-devel mpg123 gstreamer-plugins-ugly gstreamer-0_10-plugins-ffmpeg
