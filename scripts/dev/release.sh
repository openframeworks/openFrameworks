#!/bin/bash

. $HOME/.profile

set -o pipefail  # trace ERR through pipes
set -o errtrace  # trace ERR through 'time command' and other functions
set -o nounset   # set -u : exit the script if you try to use an uninitialized variable
set -o errexit   # set -e : exit the script if any statement returns a non-true return value

SCRIPT_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$SCRIPT_DIR" ]]; then SCRIPT_DIR="$PWD"; fi
. "$SCRIPT_DIR/downloader.sh"

error() {
  local parent_lineno="$1"
  if [[ "$#" = "3" ]] ; then
    local message="$2"
    local code="${3:-1}"
    echo "Error on or near line ${parent_lineno}: ${message}; exiting with status ${code}"
  else
    local code="${2:-1}"
    echo "Error on or near line ${parent_lineno}; exiting with status ${code}"
  fi
  rm -f *.tar.gz
  rm -f *.zip

  cd $(cat ~/.ofprojectgenerator/config)
  git checkout master

  exit "${code}"
}
trap 'error ${LINENO}' ERR


version=$1
if [ $# -eq 2 ]; then
branch=$2
else
branch=master
fi
cd $(cat ~/.ofprojectgenerator/config)
git fetch upstreamhttps
git reset --hard upstreamhttps/$branch
scripts/dev/download_libs.sh


cd $(cat ~/.ofprojectgenerator/config)/apps/projectGenerator
git pull origin master

cd $(cat ~/.ofprojectgenerator/config)/scripts/apothecary
git pull origin master

cd $(cat ~/.ofprojectgenerator/config)
git commit scripts/apothecary apps/projectGenerator -m"update submodules to latest commit"

cd $(cat ~/.ofprojectgenerator/config)/scripts/dev

#./create_package.sh linux64 $version $branch gcc4
#./create_package.sh linux64 $version $branch gcc5
./create_package.sh linux64 $version $branch gcc6
./create_package.sh linuxarmv6l $version $branch
./create_package.sh linuxarmv7l $version $branch
#./create_package.sh msys2 $version $branch mingw32
./create_package.sh msys2 $version $branch mingw64
./create_package.sh vs $version $branch
./create_package.sh ios $version $branch
./create_package.sh osx $version $branch
./create_package.sh android $version $branch
mkdir -p /var/www/versions/v${version}
mv *.tar.gz /var/www/versions/v${version}
mv *.zip /var/www/versions/v${version}

downloader http://openframeworks.cc/release_hook.php?version=${version} 2> /dev/null

cd $(cat ~/.ofprojectgenerator/config)
git checkout master

ip=$(ping -c 1 ci.openframeworks.cc | gawk -F'[()]' '/PING/{print $2}')
ip addr | grep $ip
ret=$?
if [ $ret -eq 0 ]; then
  # We are running on ci server, create snapshots of binary libraries and PG
  mkdir -p $OF_LIBS_ROOT/openFrameworks_libs/$version
  cp -rf $OF_LIBS_ROOT/openFrameworks_libs/android $OF_LIBS_ROOT/openFrameworks_libs/$version/
  cp -rf $OF_LIBS_ROOT/openFrameworks_libs/emscripten $OF_LIBS_ROOT/openFrameworks_libs/$version/
  cp -rf $OF_LIBS_ROOT/openFrameworks_libs/ios $OF_LIBS_ROOT/openFrameworks_libs/$version/
  cp -rf $OF_LIBS_ROOT/openFrameworks_libs/linux64 $OF_LIBS_ROOT/openFrameworks_libs/$version/
  cp -rf $OF_LIBS_ROOT/openFrameworks_libs/linuxarmv6l $OF_LIBS_ROOT/openFrameworks_libs/$version/
  cp -rf $OF_LIBS_ROOT/openFrameworks_libs/linuxarmv7l $OF_LIBS_ROOT/openFrameworks_libs/$version/
  cp -rf $OF_LIBS_ROOT/openFrameworks_libs/msys2 $OF_LIBS_ROOT/openFrameworks_libs/$version/
  cp -rf $OF_LIBS_ROOT/openFrameworks_libs/osx $OF_LIBS_ROOT/openFrameworks_libs/$version/
  cp -rf $OF_LIBS_ROOT/openFrameworks_libs/tvos $OF_LIBS_ROOT/openFrameworks_libs/$version/
  cp -rf $OF_LIBS_ROOT/openFrameworks_libs/vs $OF_LIBS_ROOT/openFrameworks_libs/$version/

  mkdir -p $OF_LIBS_ROOT/libs/$version
  cp -f $OF_LIBS_ROOT/libs/*.tar.bz2 $OF_LIBS_ROOT/libs/$version/
  cp -f $OF_LIBS_ROOT/libs/*.zip $OF_LIBS_ROOT/libs/$version/

  mkdir -p $OF_LIBS_ROOT/projectGenerator_builds/$version
  cp -f $OF_LIBS_ROOT/projectGenerator_builds/projectGenerator-osx.zip $OF_LIBS_ROOT/projectGenerator_builds/$version/
  cp -f $OF_LIBS_ROOT/projectGenerator_builds/projectGenerator-vs.zip $OF_LIBS_ROOT/projectGenerator_builds/$version/
  cp -f $OF_LIBS_ROOT/projectGenerator_builds/projectGenerator_linux $OF_LIBS_ROOT/projectGenerator_builds/$version/
fi

git tag $version
git push -u origin $version
git push origin master
