#!/bin/bash
set -ev
echo "Building openFrameworks - iOS Template Project"
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
source $ROOT/scripts/ci/ccache.sh

xcodebuild -configuration Release -project "$ROOT/scripts/templates/ios/emptyExample.xcodeproj"  -target emptyExample -sdk 'iphonesimulator' ARCHS='x86_64'
