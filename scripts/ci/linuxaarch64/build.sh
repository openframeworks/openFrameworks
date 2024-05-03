#!/bin/bash
set -ev
OF_ROOT=$( cd "$(dirname "$0")/../../.." ; pwd -P )

export CROSS_COMPILING=1

export RPI_ROOT=${OF_ROOT}/scripts/ci/linux${ARCH}/raspbian
if [ ${ARCH} = "aarch64" ]; then
export GCC_PREFIX=aarch64-linux-gnu
else
export GCC_PREFIX=arm-linux-gnueabihf
fi
# export GCC_PREFIX=aarch64-linux-gnu
export GST_VERSION=1.0
export PLATFORM_OS=Linux

if [ -n "$ARCH" ]; then
export PLATFORM_ARCH=${ARCH}
else
export PLATFORM_ARCH=aarch64
fi

echo ${PLATFORM_ARCH}

export PKG_CONFIG_LIBDIR=${RPI_ROOT}/usr/include:${RPI_ROOT}/usr/lib/pkgconfig:${RPI_ROOT}/usr/lib/${GCC_PREFIX}/pkgconfig:${RPI_ROOT}/usr/share/pkgconfig
export PKG_CONFIG_PATH=${RPI_ROOT}/usr/include:${RPI_ROOT}/usr/lib/pkgconfig:${RPI_ROOT}/usr/lib/${GCC_PREFIX}/pkgconfig:${RPI_ROOT}/usr/share/pkgconfig
export TOOLCHAIN_ROOT=/usr
export CXX="${TOOLCHAIN_ROOT}/bin/${GCC_PREFIX}-g++"
export CC="${TOOLCHAIN_ROOT}/bin/${GCC_PREFIX}-gcc"
export AR=${TOOLCHAIN_ROOT}/bin/${GCC_PREFIX}-ar
export LD=${TOOLCHAIN_ROOT}/bin/${GCC_PREFIX}-ld

echo "**** Building emptyExample ****"
cd $OF_ROOT
cp scripts/templates/linux${PLATFORM_ARCH}/Makefile examples/templates/emptyExample/
cp scripts/templates/linux${PLATFORM_ARCH}/config.make examples/templates/emptyExample/
cd examples/templates/emptyExample/
make Debug -j

echo "**** Building allAddonsExample ****"
cd $OF_ROOT
cp scripts/templates/linux${PLATFORM_ARCH}/Makefile examples/templates/allAddonsExample/
cp scripts/templates/linux${PLATFORM_ARCH}/config.make examples/templates/allAddonsExample/
cd examples/templates/allAddonsExample/
make Debug -j
