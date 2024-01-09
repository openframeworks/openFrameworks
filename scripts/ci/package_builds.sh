#!/bin/bash
set -ev
ROOT=${GITHUB_WORKSPACE}

sudo apt-get -y install aptitude

#for ubuntu 22.04 we need to install wine32
#sudo dpkg --add-architecture i386
sudo apt-get update
sudo aptitude -y install wine64

cd $ROOT

OUTPUT_FOLDER=$ROOT/out
mkdir $OUTPUT_FOLDER

lastversion=$(date +%Y%m%d)
if [ -n "$1" ] && [ "$1" != "nightly" ]; then
    lastversion=$1
fi

git submodule update --init --recursive
git submodule update --recursive --remote
cd apps/projectGenerator
git pull origin master

cd $OUTPUT_FOLDER

$ROOT/scripts/dev/create_package.sh linux64 $lastversion master gcc6
$ROOT/scripts/dev/create_package.sh linuxarmv6l $lastversion master
$ROOT/scripts/dev/create_package.sh linuxaarch64 $lastversion master
$ROOT/scripts/dev/create_package.sh osx $lastversion master
$ROOT/scripts/dev/create_package.sh ios $lastversion master
$ROOT/scripts/dev/create_package.sh msys2 $lastversion master mingw64
$ROOT/scripts/dev/create_package.sh vs $lastversion master

ls -la
cd $ROOT

#export a list of files for the next step - matching .zip or .tar anything and including the relative paths
FILES_OUT=$( (ls -t out/*.zip 2> /dev/null || true) && (ls -t out/*.tar* 2> /dev/null || true) )
#remove new lines
FILES_OUT=$(echo $FILES_OUT | tr '\n' ' ')
echo "FILES_OUT=$FILES_OUT" >> $GITHUB_OUTPUT
