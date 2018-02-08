#!/bin/bash
set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
export ANDROID_NDK_HOME=$(cd ~/$NDK_DIR && pwd)
source $ROOT/scripts/ci/ccache.sh

gradle wrapper --gradle-version 3.3

~/projectGenerator/projectGenerator_linux -p"android" -o"$ROOT" $ROOT/examples/android/androidEmptyExample
#~/projectGenerator/projectGenerator_linux -p"android" -o"$ROOT" $ROOT/examples/android/androidOpenCVExample
#~/projectGenerator/projectGenerator_linux -p"android" -o"$ROOT" $ROOT/examples/android/androidImageExample

echo "ANDROID_NDK_HOME: $ANDROID_NDK_HOME"
echo "org.gradle.caching=true" > $ROOT/examples/android/androidEmptyExample/gradle.properties
# echo "ccache.path=/usr/local/bin/ccache" >> $ROOT/examples/android/androidEmptyExample/local.properties
# echo "ndk.dir=~/${NDK_DIR}" >> $ROOT/examples/android/androidEmptyExample/local.properties
./gradlew -b $ROOT/examples/android/androidEmptyExample/build.gradle compileArm7DebugSources
#./gradlew -b $ROOT/examples/android/androidOpenCVExample/build.gradle compileArm7DebugSources
#./gradlew -b $ROOT/examples/android/androidImageExample/build.gradle compileArm7DebugSources
