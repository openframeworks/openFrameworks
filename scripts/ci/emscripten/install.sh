#!/bin/bash
set -ev

cp scripts/ci/emscripten/.emscripten ~/
sed -i "s|%HOME%|${HOME}|g" ~/.emscripten
cd ~/
git clone https://github.com/urho3d/emscripten-sdk
cd emscripten-sdk
./emsdk activate latest
