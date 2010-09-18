#!/bin/bash
DISTRIB_CODENAME=`cat /etc/lsb-release | grep DISTRIB_CODENAME | sed 's/DISTRIB_CODENAME\=\(\.*\)/\1/g'`

WX_DEB=""
ARCH=$(uname -m)

wget "http://openframeworks.cc/wx-dist.php?dist=${DISTRIB_CODENAME}&arch=${ARCH}" -q -O /tmp/oFtemp
ret=$?

if [ $ret -eq 0 ]; then
WX_DEB=$(cat /tmp/oFtemp)
fi

CB_DEB=""

wget "http://openframeworks.cc/cb-dist.php?dist=${DISTRIB_CODENAME}&arch=${ARCH}" -q -O /tmp/oFtemp
ret=$?

if [ $ret -eq 0 ]; then
CB_DEB=$(cat /tmp/oFtemp)
fi

echo "installing codeblocks from"
echo $CB_DEB
echo with wxwidgets from
echo $WX_DEB
if [ -f /etc/apt/sources.list.d/wxwidgets.list ]; then
    rm /etc/apt/sources.list.d/wxwidgets.list
fi

if [ -f /etc/apt/sources.list.d/cb-nightly.list ]; then
    rm /etc/apt/sources.list.d/cb-nightly.list
fi
if [ ! "$CB_DEB" = "" ]; then
    echo $CB_DEB > /etc/apt/sources.list.d/cb-nightly.list
fi

if [ ! "$WX_DEB" = "" ]; then
    echo $WX_DEB > /etc/apt/sources.list.d/wxwidgets.list
fi


apt-get update

apt-get install libcodeblocks0 codeblocks libwxsmithlib0 codeblocks-contrib libwxgtk2.8-0

if [ -f /etc/apt/sources.list.d/wxwidgets.list ]; then
    rm /etc/apt/sources.list.d/wxwidgets.list
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
