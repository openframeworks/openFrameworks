#!/bin/bash
set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
cd $ROOT
cp scripts/linux/template/linux/Makefile examples/empty/emptyExample/
cp scripts/linux/template/linux/config.make examples/empty/emptyExample/
cd examples/empty/emptyExample
# Add compiler flag to reduce memory usage to enable builds to complete
# see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56746#c7
# the "proper" way does not work currently:
#export CXXFLAGS="$(CXXFLAGS) --param ftrack-macro-expansion=0"
echo "PROJECT_CFLAGS = -ftrack-macro-expansion=0" >> config.make
make Debug
