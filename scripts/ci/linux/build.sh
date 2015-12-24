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
echo "PLATFORM_CFLAGS += $CUSTOMFLAGS" >> libs/openFrameworksCompiled/project/linux64/config.linux64.default.mk
sed -i "s/PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = .*/PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = -g0/" libs/openFrameworksCompiled/project/makefileCommon/config.linux.common.mk
cd libs/openFrameworksCompiled/project
make Debug

echo "**** Building emptyExample ****"
cd $ROOT/scripts/templates/linux64
make Debug

echo "**** Building allAddonsExample ****"
cd $ROOT
cp scripts/templates/linux64/Makefile examples/addons/allAddonsExample/
cp scripts/templates/linux64/config.make examples/addons/allAddonsExample/
cd examples/addons/allAddonsExample/
make Debug
