#!/bin/bash
set -ev
ROOT=${GITHUB_WORKSPACE}

#sudo dpkg --add-architecture i386
#sudo apt update
#
#sudo apt-add-repository -r 'deb https://dl.winehq.org/wine-builds/ubuntu/ focal main'
#
#wget -q https://download.opensuse.org/repositories/Emulators:/Wine:/Debian/xUbuntu_20.04/Release.key -O Release.key -O- | sudo apt-key add -
#sudo apt-add-repository 'deb https://download.opensuse.org/repositories/Emulators:/Wine:/Debian/xUbuntu_20.04/ ./'
#
#sudo apt-get update
#
#sudo apt-get install wine-stable-i386
#
#sudo apt-get -y install --install-recommends winehq-stable

#from https://askubuntu.com/questions/1226845/cannot-install-wine-on-ubuntu-19-10-20-04
sudo add-apt-repository -r 'deb https://dl.winehq.org/wine-builds/ubuntu/ bionic main'
sudo add-apt-repository -r 'deb https://dl.winehq.org/wine-builds/ubuntu/ cosmic main'
sudo add-apt-repository -r 'deb https://dl.winehq.org/wine-builds/ubuntu/ disco main'
sudo add-apt-repository -r 'deb https://dl.winehq.org/wine-builds/ubuntu/ eoan main'
sudo add-apt-repository -r 'deb https://dl.winehq.org/wine-builds/ubuntu/ focal main'

sudo apt-get update


# remove obsolete PPA
#sudo add-apt-repository -r ppa:gnome3-team/gnome3

# check real reason of the problem
#sudo apt-get install aptitude
#sudo aptitude install wine32:i386
#sudo aptitude install libfaudio0=20.04-2 libodbc1=2.3.6-0.1build1 libpcre3=2:8.39-12build1


sudo dpkg --add-architecture i386 && sudo add-apt-repository 'deb https://dl.winehq.org/wine-builds/ubuntu/ focal main'
wget -nc https://dl.winehq.org/wine-builds/winehq.key && sudo apt-key add winehq.key

sudo apt-get update
sudo apt-get dist-upgrade # to get newest dependencies

sudo apt-get install --install-recommends winehq-stable
wine --version

#sudo apt-get -y install --install-recommends wine


pwd
cd $ROOT
pwd
ls -la

OUTPUT_FOLDER=$ROOT/out

mkdir $OUTPUT_FOLDER

lastversion=$(date +%Y%m%d)

git submodule update --init --recursive
git submodule update --recursive --remote
cd apps/projectGenerator
git pull origin master

cd $OUTPUT_FOLDER

$ROOT/scripts/dev/create_package.sh msys2 $lastversion master mingw64
#$ROOT/scripts/dev/create_package.sh vs $lastversion master
#$ROOT/scripts/dev/create_package.sh ios $lastversion master
#$ROOT/scripts/dev/create_package.sh osx $lastversion master
#$ROOT/scripts/dev/create_package.sh linuxarmv6l $lastversion master
#$ROOT/scripts/dev/create_package.sh linux64 $lastversion master gcc6

ls -la
cd $ROOT

#export a list of files for the next step - matching .zip or .tar anything and including the relative paths
echo "FILES_OUT=$( (ls -t out/*.zip 2> /dev/null || true) && (ls -t out/*.tar* 2> /dev/null || true) )" >> $GITHUB_OUTPUT
