#!/bin/bash
set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
echo "**** Building oF + emptyExample - OSX Template Project ****"
xcodebuild -configuration Release -target emptyExample -project "$ROOT/scripts/templates/osx/emptyExample.xcodeproj"

echo "**** Building allAddonsExample ****"
cd $ROOT
cp scripts/templates/osx/Makefile examples/addons/allAddonsExample/
cp scripts/templates/osx/config.make examples/addons/allAddonsExample/
cd examples/addons/allAddonsExample/
make Debug
