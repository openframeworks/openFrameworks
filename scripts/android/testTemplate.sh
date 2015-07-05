#!/bin/bash
set -e
cd $OF_ROOT
cd examples/android/androidEmptyExample
make Debug PLATFORM_OS=Android
