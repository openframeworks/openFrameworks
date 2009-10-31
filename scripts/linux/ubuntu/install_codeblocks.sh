#!/bin/bash
DISTRIB_CODENAME=`cat /etc/lsb-release | grep DISTRIB_CODENAME | sed 's/DISTRIB_CODENAME\=\(\.*\)/\1/g'`

WX_DEB="deb http://apt.wxwidgets.org/ jaunty-wx main"

# wx-widgets repositories seem to be updated realy slow so ping them
# and compare with the available ubuntu distributions to know what's
# the newest available

wget http://apt.wxwidgets.org/dists/${DISTRIB_CODENAME} -q -O /tmp/oFtemp
if [ $? -eq 0 ]; then
	WX_DEB="deb http://apt.wxwidgets.org/ $DISTRIB_CODENAME-wx main"
else
    wget http://archive.ubuntu.com/ubuntu/dists -q -O /tmp/oFtemp
    UBUNTU_DISTRIBUTIONS=$(cat /tmp/oFtemp | grep "<tr><td valign=\"top\"><img src=\"/icons/folder.gif\" alt=\"\[DIR\]\"></td><td><a href=\".*\">.*/</a></td><td align=\"right\">.*</td><td align=\"right\">" | sed -e s/"<tr><td valign=\"top\"><img src=\"\/icons\/folder.gif\" alt=\"\[DIR\]\"><\/td><td><a href=\"\(.*\)\/\">.*\/<\/a><\/td><td align=\"right\">.*<\/td><td align=\"right\">  - <\/td><td>\&nbsp\;<\/td><\/tr>"/\\1/g | grep [a-z] | grep -v - | sed s/"\(.*\)"/\\1-wx/g)
    #echo $UBUNTU_DISTRIBUTIONS
    
    wget http://apt.wxwidgets.org/dists -q -O /tmp/oFtemp
	WX_DISTRIBUTIONS=$(cat /tmp/oFtemp | grep "<img src=\"/icons/folder.gif\"" | sed -e s/"<img src=\"\/icons\/folder.gif\" alt\=\"\[DIR\]\"> <a href=\"\(.*\)\/\">.*<\/a>.*"/\\1/g | grep [a-z] )
	#echo $WX_DISTRIBUTIONS
	for dist in $WX_DISTRIBUTIONS;
    do
        if [ "$(echo $UBUNTU_DISTRIBUTIONS | grep ${dist})" != "" ]; then
            WX_DEB="deb http://apt.wxwidgets.org/ $dist main"
        fi
    done	
fi
#echo $WX_DEB
CB_DEB="deb http://lgp203.free.fr/ubuntu/ $DISTRIB_CODENAME universe"

echo $WX_DEB > /etc/apt/sources.list.d/wx.list
echo $CB_DEB > /etc/apt/sources.list.d/cb-nightly.list

wget -q http://apt.wxwidgets.org/key.asc -O- | apt-key add -
wget -q http://lgp203.free.fr/public.key -O- | apt-key add -

apt-get update

apt-get install libcodeblocks0 codeblocks libwxsmithlib0 codeblocks-contrib libwxgtk2.8-0


