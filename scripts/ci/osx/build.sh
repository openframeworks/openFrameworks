#!/bin/bash
set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
echo "**** Building oF + emptyExample - OSX Template Project ****"
mkdir -p $ROOT/apps/myApps
cp -r "$ROOT/scripts/osx/template/standard" "$ROOT/apps/myApps/emptyExample"
xcodebuild -configuration Release -target emptyExample -project "$ROOT/apps/myApps/emptyExample/emptyExample.xcodeproj"

echo "**** Building allAddonsExample ****"
cd $ROOT
cp scripts/osx/template/standard/Makefile examples/addons/allAddonsExample/
cp scripts/osx/template/standard/config.make examples/addons/allAddonsExample/
cd examples/addons/allAddonsExample/
make Debug
