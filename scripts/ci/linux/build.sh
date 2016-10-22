#!/bin/bash
set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
# Add compiler flag to reduce memory usage to enable builds to complete
# see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56746#c7
# the "proper" way does not work currently:
#export CXXFLAGS="$(CXXFLAGS) --param ftrack-macro-expansion=0"
CUSTOMFLAGS="-ftrack-macro-expansion=0"

echo "**** Building OF core ****"
cd $ROOT
# this carries over to subsequent compilations of examples
echo "PLATFORM_CFLAGS += $CUSTOMFLAGS" >> build/projects/linux64/config.linux64.default.mk
sed -i "s/PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = .*/PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = -g0/" build/projects/makefileCommon/config.linux.common.mk
cd build/projects
make Debug

echo "**** Building emptyExample ****"
cd $ROOT/scripts/templates/linux64
make Debug

echo "**** Building allAddonsExample ****"
cd $ROOT
cp scripts/templates/linux64/Makefile examples/templates/allAddonsExample/
cp scripts/templates/linux64/config.make examples/templates/allAddonsExample/
cd examples/templates/allAddonsExample/
make Debug
