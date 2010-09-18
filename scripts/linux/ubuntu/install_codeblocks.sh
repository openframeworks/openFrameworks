#!/bin/bash
DISTRIB_CODENAME=`cat /etc/lsb-release | grep DISTRIB_CODENAME | sed 's/DISTRIB_CODENAME\=\(\.*\)/\1/g'`


CB_DEB="deb http://lgp203.free.fr/ubuntu/ $DISTRIB_CODENAME universe"
if [ $DISTRIB_CODENAME = "maverick" ]; then
    CB_DEB=""
elif [ $DISTRIB_CODENAME = "karmic" ]; then
    CB_DEB="deb http://ppa.launchpad.net/ubuntu-backports-testers/ppa/ubuntu lucid main"
elif [ $DISTRIB_CODENAME = "lucid" ] || [ $DISTRIB_CODENAME = "jaunty" ]; then
    CB_DEB="ppa:ubuntu-backports-testers/ppa"
fi

if [ ! $CB_DEB = "" ]; then
    add-apt-repository $CB_DEB
fi

#wget -q http://lgp203.free.fr/public.key -O- | apt-key add -

apt-get update

apt-get install libcodeblocks0 codeblocks libwxsmithlib0 codeblocks-contrib libwxgtk2.8-0

if [ ! $CB_DEB = "" ]; then
    add-apt-repository -r $CB_DEB
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
