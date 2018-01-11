#!/bin/bash
#set -e
#set -o pipefail
# trap any script errors and exit
trap "trapError" ERR

SUDO=

trapError() {
	echo
	echo " ^ Received error ^"
	exit 1
}

cp scripts/ci/emscripten/.emscripten ~/
sed -i "s|%HOME%|${HOME}|g" ~/.emscripten
cd ~/
git clone https://github.com/urho3d/emscripten-sdk
cd emscripten-sdk
# ./emsdk update-tags
# ./emsdk install latest
./emsdk activate latest
