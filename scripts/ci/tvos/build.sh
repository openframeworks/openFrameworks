#!/bin/bash
set -ev
echo "Building openFrameworks - tvOS Template Project"
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
if [[ "$GITHUB_ACTIONS" = true ]]; then
    ROOT=$GITHUB_WORKSPACE
fi

xcodebuild -project "$ROOT/scripts/templates/tvos/emptyExample.xcodeproj"  -target emptyExample -sdk appletvsimulator ONLY_ACTIVE_ARCH=YES
