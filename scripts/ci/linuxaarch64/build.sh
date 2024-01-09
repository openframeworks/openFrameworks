#!/bin/bash
set -ev
OF_ROOT=$( cd "$(dirname "$0")/../../.." ; pwd -P )

export GCC_PREFIX=aarch64-linux-gnu
export GCC_VERSION=10.3.0
export GST_VERSION=1.0
export RPI_ROOT=${OF_ROOT}/scripts/ci/$TARGET/raspbian
export TOOLCHAIN_ROOT=${OF_ROOT}/scripts/ci/$TARGET/rpi_toolchain
export PLATFORM_OS=Linux
export PLATFORM_ARCH=aarch64
export PKG_CONFIG_LIBDIR=${RPI_ROOT}/usr/lib/pkgconfig:${RPI_ROOT}/usr/lib/${GCC_PREFIX}/pkgconfig:${RPI_ROOT}/usr/share/pkgconfig
export CXX="${TOOLCHAIN_ROOT}/bin/${GCC_PREFIX}-g++"
export CC="${TOOLCHAIN_ROOT}/bin/${GCC_PREFIX}-gcc"
export AR=${TOOLCHAIN_ROOT}/bin/${GCC_PREFIX}-ar
export LD=${TOOLCHAIN_ROOT}/bin/${GCC_PREFIX}-ld

export PATH=/rpi_toolchain/bin/:$PATH
export LD_LIBRARY_PATH=/rpi_toolchain/lib:$LD_LIBRARY_PATH

echo "**** Building emptyExample ****"
cd $OF_ROOT
cp scripts/templates/linuxaarch64/Makefile examples/templates/emptyExample/
cp scripts/templates/linuxaarch64/config.make examples/templates/emptyExample/
cd examples/templates/emptyExample/
make Debug -j

#TODO:
#echo "**** Building allAddonsExample ****"
#cd $OF_ROOT
#cp scripts/templates/linuxaarch64/Makefile examples/templates/allAddonsExample/
#cp scripts/templates/linuxaarch64/config.make examples/templates/allAddonsExample/
#cd examples/templates/allAddonsExample/
#make Debug -j
