#!/bin/bash
set -ev
OF_ROOT=$( cd "$(dirname "$0")/../../.." ; pwd -P )

export GCC_PREFIX=arm-linux-gnueabihf
export GST_VERSION=1.0
#NOTE: GCC_VERSION should match the version present in the downloaded toolchain
export GCC_VERSION=9.4.0
export RPI_ROOT=${OF_ROOT}/scripts/ci/$TARGET/raspbian
export TOOLCHAIN_ROOT=${OF_ROOT}/scripts/ci/$TARGET/rpi_toolchain
export PLATFORM_OS=Linux
export PLATFORM_ARCH=armv6l
export PKG_CONFIG_LIBDIR=${RPI_ROOT}/usr/lib/pkgconfig:${RPI_ROOT}/usr/lib/${GCC_PREFIX}/pkgconfig:${RPI_ROOT}/usr/share/pkgconfig
export CXX="${TOOLCHAIN_ROOT}/bin/${GCC_PREFIX}-g++"
export CC="${TOOLCHAIN_ROOT}/bin/${GCC_PREFIX}-gcc"
export AR=${TOOLCHAIN_ROOT}/bin/${GCC_PREFIX}-ar
export LD=${TOOLCHAIN_ROOT}/bin/${GCC_PREFIX}-ld
export CROSS_COMPILING=1

export PATH=/rpi_toolchain/bin/:$PATH
export LD_LIBRARY_PATH=/rpi_toolchain/lib:$LD_LIBRARY_PATH
 
echo "**** Building emptyExample ****"
cd $OF_ROOT
cp scripts/templates/linuxarmv6l/Makefile examples/templates/emptyExample/
cp scripts/templates/linuxarmv6l/config.make examples/templates/emptyExample/
cd examples/templates/emptyExample/
make Debug -j

echo "**** Building allAddonsExample ****"
cd $OF_ROOT
cp scripts/templates/linuxarmv6l/Makefile examples/templates/allAddonsExample/
cp scripts/templates/linuxarmv6l/config.make examples/templates/allAddonsExample/
cd examples/templates/allAddonsExample/
make Debug -j
