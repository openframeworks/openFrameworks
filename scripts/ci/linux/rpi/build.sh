#!/bin/bash
set -ev
OF_ROOT=$( cd "$(dirname "$0")/../../.." ; pwd -P )

export CROSS_COMPILING=1

# Allows test locally without github actions env variables.
if [ -n "$ARCH" ]; then
export PLATFORM_ARCH=${ARCH}
else
# github actions will be defining this one, next line will set if we are working locally
export PLATFORM_ARCH=armv6l
# export PLATFORM_ARCH=aarch64
fi

export RPI_ROOT=${OF_ROOT}/scripts/ci/linuxrpi/raspbian
echo "${RPI_ROOT}"

if [ ${PLATFORM_ARCH} = "aarch64" ]; then
export GCC_PREFIX=aarch64-linux-gnu
else
export GCC_PREFIX=arm-linux-gnueabihf
fi

export GST_VERSION=1.0
export PLATFORM_OS=Linux

# export PKG_CONFIG_LIBDIR=
export PKG_CONFIG_PATH=${BCM_FOLDER}:${RPI_ROOT}/usr/include:${RPI_ROOT}/usr/lib/pkgconfig:${RPI_ROOT}/usr/lib/${GCC_PREFIX}/pkgconfig:${RPI_ROOT}/usr/share/pkgconfig
export TOOLCHAIN_ROOT=/usr
export CXX="${TOOLCHAIN_ROOT}/bin/${GCC_PREFIX}-g++"
export CC="${TOOLCHAIN_ROOT}/bin/${GCC_PREFIX}-gcc"
export AR=${TOOLCHAIN_ROOT}/bin/${GCC_PREFIX}-ar
export LD=${TOOLCHAIN_ROOT}/bin/${GCC_PREFIX}-ld

echo "##[group]**** Building emptyExample ****"
cd $OF_ROOT
cp scripts/templates/linux${PLATFORM_ARCH}/Makefile examples/templates/emptyExample/
cp scripts/templates/linux${PLATFORM_ARCH}/config.make examples/templates/emptyExample/
cd examples/templates/emptyExample/
make Debug -j2
echo "##[endgroup]"

# if [[ "$ALLADDONSEXAMPLE" == 1 ]]; then
echo "##[group]**** Building allAddonsExample ****"
cd $OF_ROOT
cp scripts/templates/linux${PLATFORM_ARCH}/Makefile examples/templates/allAddonsExample/
cp scripts/templates/linux${PLATFORM_ARCH}/config.make examples/templates/allAddonsExample/
cd examples/templates/allAddonsExample/
make Debug -j2
echo "##[endgroup]"
# fi