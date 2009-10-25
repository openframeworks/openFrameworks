#!/bin/bash
DISTRIB_CODENAME=`cat /etc/lsb-release | grep DISTRIB_CODENAME | sed 's/DISTRIB_CODENAME\=\(\.*\)/\1/g'`
WX_DEB="deb http://apt.wxwidgets.org/ hardy-wx main"
CB_DEB="deb http://lgp203.free.fr/ubuntu/ $DISTRIB_CODENAME universe"

echo $WX_DEB > /etc/apt/sources.list.d/wx.list
echo $CB_DEB > /etc/apt/sources.list.d/cb-nightly.list

wget -q http://apt.wxwidgets.org/key.asc -O- | apt-key add -
wget -q http://lgp203.free.fr/public.key -O- | apt-key add -

apt-get update

apt-get install libcodeblocks0 codeblocks libwxsmithlib0 codeblocks-contrib libwxgtk2.8-0


