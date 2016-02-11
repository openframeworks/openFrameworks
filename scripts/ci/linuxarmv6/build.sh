#!/bin/bash
set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
# Add compiler flag to reduce memory usage to enable builds to complete
# see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56746#c7
# the "proper" way does not work currently:
export CXXFLAGS="$(CXXFLAGS) --param ftrack-macro-expansion=0"
CUSTOMFLAGS="-ftrack-macro-expansion=0"

echo "**** Building OF core ****"
cd $ROOT
# this carries over to subsequent compilations of examples
echo "PLATFORM_CFLAGS += $CUSTOMFLAGS" >> libs/openFrameworksCompiled/project/linuxarmv6l/config.linuxarmv6l.default.mk
sed -i "s/PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = .*/PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = -g0/" libs/openFrameworksCompiled/project/makefileCommon/config.linux.common.mk
cd libs/openFrameworksCompiled/project
export GST_VERSION=1.0
export RPI_ROOT=${TRAVIS_BUILD_DIR}/scripts/ci/linuxarmv6/raspbian
export TOOLCHAIN_ROOT=${TRAVIS_BUILD_DIR}/scripts/ci/linuxarmv6/tools-master
export PLATFORM_OS=Linux
export PLATFORM_ARCH=armv6l
export PKG_CONFIG_PATH=$RPI_ROOT/usr/lib/arm-linux-gnueabihf/pkgconfig:$RPI_ROOT/usr/share/pkgconfig:$RPI_ROOT/usr/lib/pkgconfig
make Debug

echo "**** Building emptyExample ****"
cd $ROOT/scripts/templates/linuxarmv6l
make Debug

echo "**** Building allAddonsExample ****"
cd $ROOT
cp scripts/templates/linuxarmv6l/Makefile examples/addons/allAddonsExample/
cp scripts/templates/linuxarmv6l/config.make examples/addons/allAddonsExample/
cd examples/addons/allAddonsExample/
make Debug
