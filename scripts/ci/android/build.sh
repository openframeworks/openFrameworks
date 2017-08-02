#!/bin/bash
set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
export ANDROID_NDK_HOME=$(cd ~/$NDK_DIR && pwd)

gradle wrapper --gradle-version 3.3

~/projectGenerator/projectGenerator_linux -p"android" -o"$ROOT" $ROOT/examples/android/androidEmptyExample

echo "ANDROID_NDK_HOME: $ANDROID_NDK_HOME"
./gradlew -b $ROOT/examples/android/androidEmptyExample/build.gradle compileArm7DebugSources
