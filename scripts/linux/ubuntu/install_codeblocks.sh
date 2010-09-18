#!/bin/bash
DISTRIB_CODENAME=`cat /etc/lsb-release | grep DISTRIB_CODENAME | sed 's/DISTRIB_CODENAME\=\(\.*\)/\1/g'`

WX_DEB=""
ARCH=$(uname -m)

wget "http://openframeworks.cc/wx-dist.php?dist=${DISTRIB_CODENAME}&arch=${ARCH}" -q -O /tmp/oFtemp
ret=$?

if [ $ret -eq 0 ]; then
WX_DEB=$(cat /tmp/oFtemp)
fi
CB_DEB="deb http://lgp203.free.fr/ubuntu/ $DISTRIB_CODENAME universe"
if [ "$DISTRIB_CODENAME" = "maverick" ]; then
    CB_DEB=""
elif [ "$DISTRIB_CODENAME" = "lucid" ]; then
    CB_DEB="deb http://ppa.launchpad.net/ubuntu-backports-testers/ppa/ubuntu lucid main"
fi

if [ ! "$CB_DEB" = "" ]; then
    echo $CB_DEB /etc/apt/sources.list.d/cb-nightly.list
fi

#wget -q http://lgp203.free.fr/public.key -O- | apt-key add -

apt-get update

apt-get install libcodeblocks0 codeblocks libwxsmithlib0 codeblocks-contrib libwxgtk2.8-0


if [ -f /etc/apt/sources.list.d/wx.list ]; then
    rm /etc/apt/sources.list.d/wx.list
fi

if [ -f /etc/apt/sources.list.d/cb-nightly.list ]; then
    rm /etc/apt/sources.list.d/cb-nightly.list
fi

## fix pkg-config by installing missing dependencies

pkg-config --list-all 2>/dev/null 1>/dev/null
ret=$?
while [ $ret -ne 0 ]; do
    pkg=$(pkg-config --list-all 2>&1 1>/dev/null | grep "Package '.*'," | sed "s/Package '\(\.*\)/\1/g" | sed "s/\(\.*\)'.*/\1/g")
    sudo apt-get install ${pkg}-dev
    pkg-config --list-all 2>/dev/null 1>/dev/null
    ret=$?
done
