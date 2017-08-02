#!/bin/bash

. $HOME/.profile

set -o pipefail  # trace ERR through pipes
set -o errtrace  # trace ERR through 'time command' and other functions
set -o nounset   # set -u : exit the script if you try to use an uninitialized variable
set -o errexit   # set -e : exit the script if any statement returns a non-true return value

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

cd $(cat ~/.ofprojectgenerator/config)/scripts/dev

./create_package.sh linux64 $version $branch
./create_package.sh linux64 $version $branch gcc5
./create_package.sh linux64 $version $branch gcc6
./create_package.sh linuxarmv6l $version $branch
./create_package.sh linuxarmv7l $version $branch
./create_package.sh msys2 $version $branch
./create_package.sh vs $version $branch
./create_package.sh ios $version $branch
./create_package.sh osx $version $branch
./create_package.sh android $version $branch
mkdir -p /var/www/versions/v${version}
mv *.tar.gz /var/www/versions/v${version}
mv *.zip /var/www/versions/v${version}

wget http://openframeworks.cc/release_hook.php?version=${version} -O /dev/null

cd $(cat ~/.ofprojectgenerator/config)
git checkout master

