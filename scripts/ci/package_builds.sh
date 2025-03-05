#!/bin/bash
set -e

ROOT=${GITHUB_WORKSPACE}

P_SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# Check if ROOT is empty
if [[ -z "$ROOT" ]]; then
  # Set ROOT to ../../../ if it's empty
  ROOT="../../"
fi

ROOT=$(realpath "$ROOT")

echo "##[group]apt update"
if [[ "$(uname -s)" == "Linux" ]]; then
    sudo apt-get -y install aptitude
    #for ubuntu 22.04 we need to install wine32
    #sudo dpkg --add-architecture i386
    sudo apt-get update
    sudo aptitude -y install wine64
fi
echo "##[endgroup]"

echo "##[group]ls"
echo "Where is ROOT: $ROOT"
cd $ROOT
ls


OUTPUT_FOLDER=$ROOT/out
mkdir -p $OUTPUT_FOLDER

RELEASE="${RELEASE:-latest}"

lastversion=$(date +%Y%m%d)
# if [ -n "$1" ] && [ "$1" != "nightly" ]; then
	# lastversion=$1
# fi
echo "##[endgroup]"

echo "##[group]submodule update and pull"
git submodule update --init --recursive
git submodule update --recursive --remote
cd apps/projectGenerator
git pull origin master
echo "##[endgroup]"

echo "##[group]create package"
cd $OUTPUT_FOLDER
pwd
if [[ "$(uname -s)" == "Linux" ]]; then
    $ROOT/scripts/dev/create_package.sh linux64 $lastversion master 10 || echo "Failed: linux64"
    $ROOT/scripts/dev/create_package.sh linuxarm64 $lastversion master 14 || echo "Failed: linuxarm64"
    $ROOT/scripts/dev/create_package.sh linuxarmv6l $lastversion master || echo "Failed: linuxarmv6l"
    $ROOT/scripts/dev/create_package.sh linuxaarch64 $lastversion master || echo "Failed: linuxaarch64"
    $ROOT/scripts/dev/create_package.sh msys2 $lastversion master mingw64 || echo "Failed: msys2-mingw64"
    $ROOT/scripts/dev/create_package.sh msys2 $lastversion master clang64 || echo "Failed: msys2-clang64"
    $ROOT/scripts/dev/create_package.sh msys2 $lastversion master ucrt64 || echo "Failed: msys2-ucrt64"
    $ROOT/scripts/dev/create_package.sh vs $lastversion master || echo "Failed: vs"
    $ROOT/scripts/dev/create_package.sh vs $lastversion master 64 || echo "Failed: vs 64"
    $ROOT/scripts/dev/create_package.sh vs2019 $lastversion master 64 || echo "Failed: vs2019 64"
fi

$ROOT/scripts/dev/create_package.sh osx $lastversion master || echo "Failed: osx"
$ROOT/scripts/dev/create_package.sh ios $lastversion master || echo "Failed: ios"
echo "##[endgroup]"

ls -la
cd $ROOT

#export a list of files for the next step - matching .zip or .tar anything and including the relative paths
FILES_OUT=$( (ls -t out/*.zip 2> /dev/null || true) && (ls -t out/*.tar* 2> /dev/null || true) )
#remove new lines
FILES_OUT=$(echo $FILES_OUT | tr '\n' ' ')
if [ -n "$GITHUB_OUTPUT" ]; then
	echo "FILES_OUT=$FILES_OUT" >> $GITHUB_OUTPUT
fi
