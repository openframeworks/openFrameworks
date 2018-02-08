#!/bin/bash
set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
source $ROOT/scripts/ci/ccache.sh
echo "**** Building oF + emptyExample - OSX Template Project ****"
xcodebuild -configuration Release -target emptyExample -project "$ROOT/scripts/templates/osx/emptyExample.xcodeproj"
echo "**** Done building emptyExample ****"
