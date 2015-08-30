#!/bin/bash
set -ev
echo "Building openFrameworks - iOS Template Project"
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
mkdir -p $ROOT/apps/myApps
cp -r "$ROOT/scripts/ios/template/standard" "$ROOT/apps/myApps/emptyExample"
xcodebuild -project "$ROOT/apps/myApps/emptyExample/emptyExample.xcodeproj"  -target emptyExample -sdk iphonesimulator ONLY_ACTIVE_ARCH=NO

