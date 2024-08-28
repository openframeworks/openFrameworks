#!/bin/bash
set -ev
echo "Building openFrameworks - iOS Template Project"
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
if [[ "$GITHUB_ACTIONS" = true ]]; then
    ROOT=$GITHUB_WORKSPACE
fi

xcodebuild -configuration Release -project "$ROOT/scripts/templates/ios/emptyExample.xcodeproj"  -target emptyExample -sdk 'iphonesimulator' ARCHS='x86_64'
