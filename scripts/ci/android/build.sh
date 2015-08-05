#!/bin/bash
set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
cd $ROOT/examples/android/androidEmptyExample
echo "ABIS_TO_COMPILE_DEBUG = $1" >> config.make
make Debug PLATFORM_OS=Android
