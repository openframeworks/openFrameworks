#!/bin/bash
#DISTRIB_CODENAME=`cat /etc/lsb-release | grep DISTRIB_CODENAME | sed 's/DISTRIB_CODENAME\=\(\.*\)/\1/g'`
WX_DEB="deb http://apt.wxwidgets.org/ etch-wx main"
CB_DEB="deb http://apt.jenslody.de/stable stable main"

echo $WX_DEB > /etc/apt/sources.list.d/wx.list
echo $CB_DEB > /etc/apt/sources.list.d/cb-nightly.list

wget -q http://apt.wxwidgets.org/key.asc -O- | apt-key add -

apt-get update
apt-get install jens-lody-debian-keyring
apt-get install libcodeblocks0 codeblocks libwxsmithlib0 codeblocks-contrib libwxgtk2.8-0


