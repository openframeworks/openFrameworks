#!/bin/bash
set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
# Add compiler flag to reduce memory usage to enable builds to complete
# see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56746#c7
# the "proper" way does not work currently:
#export CXXFLAGS="$(CXXFLAGS) --param ftrack-macro-expansion=0"
CUSTOMFLAGS="-ftrack-macro-expansion=0"

echo "Building OF core"
cd $ROOT
echo "PLATFORM_CFLAGS += $CUSTOMFLAGS" >> libs/openFrameworksCompiled/project/linux64/config.linux64.default.mk
cd libs/openFrameworksCompiled/project
make Debug

echo "Building emptyExample"
cd $ROOT
cp scripts/linux/template/linux64/Makefile examples/empty/emptyExample/
cp scripts/linux/template/linux64/config.make examples/empty/emptyExample/
cd examples/empty/emptyExample
# this is not even necessary if we include that in the default.mk above
# echo "PROJECT_CFLAGS = $CUSTOMFLAGS" >> config.make
make Debug
