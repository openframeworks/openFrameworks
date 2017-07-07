#!/bin/bash
set -e
set -o pipefail
# trap any script errors and exit
trap "trapError" ERR
age () { stat=$(stat --printf="%Y %F\n" "$1"); echo $((($(date +%s) - ${stat%% *})/86400)); }


SUDO=
export ROOT=$( cd "$(dirname "$0")" ; pwd -P )

trapError() {
	echo
	echo " ^ Received error ^"
	exit 1
}

createArchImg(){
    #sudo apt-get install -y gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf libasound2-dev

    #sudo apt-get -y update
    #sudo apt-get -f -y --force-yes dist-upgrade
    #sudo apt-get install -y libgssapi-krb5-2 libkrb5-3 libidn11
    #sudo ./arch-bootstrap.sh archlinux

    if [ ! "$(ls -A ~/archlinux)" ] || [ $(age ~/archlinux/timestamp) -gt 7 ]; then
        #$ROOT/arch-bootstrap_downloadonly.sh -a armv7h -r "http://eu.mirror.archlinuxarm.org/" ~/archlinux
		junest -u << EOF
			cd ~
			wget http://archlinuxarm.org/os/ArchLinuxARM-rpi-2-latest.tar.gz
			mkdir archlinux
		    tar xzf ArchLinuxARM-rpi-2-latest.tar.gz -C archlinux/ 2> /dev/null
			sed -i s_/etc/pacman_$HOME/archlinux/etc/pacman_g archlinux/etc/pacman.conf
			pacman --noconfirm -r archlinux/ --config archlinux/etc/pacman.conf --arch=armv7h -Syu
			pacman --noconfirm -r archlinux/ --config archlinux/etc/pacman.conf --arch=armv7h -S make pkg-config gcc raspberrypi-firmware openal glew freeglut freeimage freetype2 cairo poco gstreamer gst-plugins-base gst-plugins-good assimp boost libxcursor opencv assimp glfw-x11 uriparser curl pugixml
        	touch ~/archlinux/timestamp
EOF
    else
        echo "Using cached archlinux image"
    fi
}

downloadToolchain(){
    if [ "$(ls -A ~/x-tools7h)" ]; then
        echo "Using cached RPI2 toolchain"
    else
		#xz -dc x-tools7h.tar.xz | tar x -C ~/;
		junest -u << EOF
		cd /tmp
		if [ -f x-tools7h.tar.xz ]; then
			rm x-tools7h.tar.xz
		fi
		wget http://archlinuxarm.org/builder/xtools/x-tools7h.tar.xz
	    tar -x --delay-directory-restore --no-same-owner -f x-tools7h.tar.xz -C ~/
	    rm x-tools7h.tar.xz
EOF
        #wget http://ci.openframeworks.cc/rpi2_toolchain.tar.bz2
        #tar xjf rpi2_toolchain.tar.bz2 -C ~/
        #rm rpi2_toolchain.tar.bz2
    fi
}

downloadFirmware(){
    if [ "$(ls -A ~/firmware-master)" ]; then
        echo "Using cached RPI2 firmware-master"
    else
        cd ~
        wget https://github.com/raspberrypi/firmware/archive/master.zip -O firmware.zip
        unzip firmware.zip
    fi
    ${SUDO} cp -r ~/firmware-master/opt archlinux/
}


relativeSoftLinks(){
    rel_link=$1
    escaped_rel_link=$2
    for link in $(ls -la | grep "\-> /" | sed "s/.* \([^ ]*\) \-> \/\(.*\)/\1->\/\2/g"); do
        lib=$(echo $link | sed "s/\(.*\)\->\(.*\)/\1/g");
        link=$(echo $link | sed "s/\(.*\)\->\(.*\)/\2/g");
        ${SUDO} rm $lib
        ${SUDO} ln -s ${rel_link}/${link} $lib
    done

    for f in *; do
        error_lib=$(grep " \/lib/" $f > /dev/null 2>&1; echo $?)
        error_usr=$(grep " \/usr/" $f > /dev/null 2>&1; echo $?)
        if [ $error_lib -eq 0 ] || [ $error_usr -eq 0 ]; then
            ${SUDO} sed -i "s/ \/lib/ $escaped_rel_link\/lib/g" $f
            ${SUDO} sed -i "s/ \/usr/ $escaped_rel_link\/usr/g" $f
        fi
    done
}

installRtAudio(){
    #cd $ROOT
    #wget http://www.music.mcgill.ca/~gary/rtaudio/release/rtaudio-4.1.1.tar.gz
    #tar xzf rtaudio-4.1.1.tar.gz
    #cd rtaudio-4.1.1
    #./configure --host=${GCC_PREFIX}
    #sed -i "s|CFLAGS[ ]*=\(.*\)|CFLAGS = ${CFLAGS} \1|g" Makefile
    #perl -p -i -e 's|\$\(CC\) (?!\$\(CFLAGS\))|\$(CC) \$(CFLAGS) |g' Makefile

    #make
    #cp RtAudio.h ${RPI_ROOT}/usr/local/include/
    #cp *.a ${RPI_ROOT}/usr/local/lib/
    #cd $ROOT
    #rm rtaudio-4.1.1.tar.gz
    #rm -r rtaudio-4.1.1
    cd ~/archlinux
    wget http://ci.openframeworks.cc/rtaudio-armv7hf.tar.bz2
    tar xjf rtaudio-armv7hf.tar.bz2
    rm rtaudio-armv7hf.tar.bz2
}

installJunest(){
	git clone git://github.com/fsquillace/junest ~/.local/share/junest
	export PATH=~/.local/share/junest/bin:$PATH
	junest -u << EOF
		pacman -Syy --noconfirm
		pacman -S --noconfirm git flex grep gcc pkg-config make wget
EOF
}

echo $ROOT
installJunest
createArchImg
downloadToolchain
downloadFirmware
installRtAudio

cd ~/archlinux/usr/lib
relativeSoftLinks "../.." "..\/.."
#cd $ROOT/archlinux/usr/lib/arm-unknown-linux-gnueabihf
#relativeSoftLinks  "../../.." "..\/..\/.."
#cd $ROOT/raspbian/usr/lib/gcc/arm-unknown-linux-gnueabihf/5.3
#relativeSoftLinks  "../../../.." "..\/..\/..\/.."
