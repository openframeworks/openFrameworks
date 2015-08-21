#!/bin/bash
set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
echo "**** Building oF + emptyExample - OSX Template Project ****"
xcodebuild -configuration Release -target emptyExample -project "$ROOT/scripts/osx/template/emptyExample.xcodeproj"

echo "**** Building allAddonsExample ****"
cd $ROOT
cp scripts/osx/template/Makefile examples/addons/allAddonsExample/
cp scripts/osx/template/config.make examples/addons/allAddonsExample/
cd examples/addons/allAddonsExample/
make Debug
