#!/bin/bash
set -ev
echo "Building openFrameworks - iOS Template Project"
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
# source $ROOT/scripts/ci/ccache.sh
export CXX="$(which ccache)"
export CXXFLAGS="$(xcodebuild -find clang++)"
export CC="$(which ccache)"
export CFLAGS="$(xcodebuild -find clang)"

xcodebuild -project "$ROOT/scripts/templates/ios/emptyExample.xcodeproj"  -target emptyExample -sdk iphonesimulator ONLY_ACTIVE_ARCH=NO
