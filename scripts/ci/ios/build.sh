#!/bin/bash
set -ev
echo "Building openFrameworks - iOS Template Project"
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
xcodebuild -project "$ROOT/scripts/templates/ios/emptyExample.xcodeproj"  -target emptyExample -sdk iphonesimulator ONLY_ACTIVE_ARCH=NO

