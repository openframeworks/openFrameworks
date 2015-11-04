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
  exit "${code}"
}
trap 'error ${LINENO}' ERR


version=$1
cd $(cat ~/.ofprojectgenerator/config)/scripts/dev

./create_package.sh linux $version master
./create_package.sh linux64 $version master
./create_package.sh linuxarmv6l $version master
./create_package.sh linuxarmv7l $version master
./create_package.sh msys2 $version master
./create_package.sh vs $version master
./create_package.sh ios $version master
./create_package.sh osx $version master
./create_package.sh android $version master
mkdir /var/www/versions/v${version}
mv *.tar.gz /var/www/versions/v${version}
mv *.zip /var/www/versions/v${version}

wget http://openframeworks.cc/release_hook.php?version=${version} -O /dev/null
