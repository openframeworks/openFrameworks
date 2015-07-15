#!/bin/bash
set -ev
echo "Building openFrameworks - OSX Template Project"
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
xcodebuild -configuration Release -target emptyExample -project "$ROOT/scripts/osx/template/emptyExample.xcodeproj"
