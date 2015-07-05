#!/bin/bash
set -e
cd $OF_ROOT
cd examples/android/androidEmptyExample
echo "ABIS_TO_COMPILE_DEBUG = armv7 x86" >> config.make
make Debug PLATFORM_OS=Android
