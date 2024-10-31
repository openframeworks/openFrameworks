#!/bin/bash
set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}

echo "**** Building oF + emptyExample - macOS Template Project ****"
# xcodebuild -arch x86_64 -configuration Release -target emptyExample -project "$ROOT/scripts/templates/macos/emptyExample.xcodeproj"
xcodebuild -configuration Release -target emptyExample -project "$ROOT/scripts/templates/macos/emptyExample.xcodeproj"
echo "**** Done building emptyExample ****"
