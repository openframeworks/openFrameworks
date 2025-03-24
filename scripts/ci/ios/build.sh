#!/bin/bash
set -e
echo "Building openFrameworks - iOS Template Project"
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
if [[ "$GITHUB_ACTIONS" = true ]]; then
    ROOT=$GITHUB_WORKSPACE
fi
SKIP_DEVICE=${SKIP_DEVICE:-1}  ## SKIP until figure out Identity
HOST_ARCH=$(uname -m)
if [[ "$HOST_ARCH" == "arm64" ]]; then
    SIM_ARCH="arm64"
else
    SIM_ARCH="x86_64 arm64"
fi
if [ -z "${IDENTITY+x}" ]; then
   IDENTITY="-"
fi
echo "Building emptyExample for iphonesimulator"
xcodebuild -configuration Release \
    -project "$ROOT/scripts/templates/ios/emptyExample.xcodeproj" \
    -target emptyExample \
    -sdk iphonesimulator \
    -arch "$SIM_ARCH" \
    CODE_SIGNING_ALLOWED=NO
if [ $? -ne 0 ]; then
    echo "Error: Failed to build emptyExample for iphonesimulator"
    exit 1
fi
echo "emptyExample simulotor built successfully"

if [[ "$SKIP_DEVICE" = 0 ]]; then
    echo "Building emptyExample for iphoneos"
    xcodebuild -configuration Release \
        -project "$ROOT/scripts/templates/ios/emptyExample.xcodeproj" \
        -target emptyExample \
        -sdk iphoneos \
        -arch "arm64" \
        CODE_SIGN_STYLE=Automatic \
        DEVELOPMENT_TEAM="$IDENTITY" \
        CODE_SIGN_IDENTITY="Apple Development"
    if [ $? -ne 0 ]; then
        echo "Error: Failed to build emptyExample for device"
        exit 1
    fi
    echo "emptyExample device built successfully"
fi

echo "ios tests complete"
