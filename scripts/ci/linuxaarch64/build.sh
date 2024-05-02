#!/bin/bash
set -ev
OF_ROOT=$( cd "$(dirname "$0")/../../.." ; pwd -P )

export RPI_ROOT=${OF_ROOT}/scripts/ci/linuxaarch64/raspbian
export GCC_PREFIX=aarch64-linux-gnu
export GST_VERSION=1.0
export PLATFORM_OS=Linux
export PLATFORM_ARCH=aarch64
export PKG_CONFIG_LIBDIR=${RPI_ROOT}/usr/include:${RPI_ROOT}/usr/lib/pkgconfig:${RPI_ROOT}/usr/lib/${GCC_PREFIX}/pkgconfig:${RPI_ROOT}/usr/share/pkgconfig
export PKG_CONFIG_PATH=${RPI_ROOT}/usr/include:${RPI_ROOT}/usr/lib/pkgconfig:${RPI_ROOT}/usr/lib/${GCC_PREFIX}/pkgconfig:${RPI_ROOT}/usr/share/pkgconfig
export TOOLCHAIN_ROOT=/usr/bin
export CXX="${TOOLCHAIN_ROOT}/${GCC_PREFIX}-g++"
export CC="${TOOLCHAIN_ROOT}/${GCC_PREFIX}-gcc"
export AR=${TOOLCHAIN_ROOT}/${GCC_PREFIX}-ar
export LD=${TOOLCHAIN_ROOT}/${GCC_PREFIX}-ld

# export PATH=${RPI_ROOT}/bin/:$PATH
# export LD_LIBRARY_PATH=${RPI_ROOT}/lib:$LD_LIBRARY_PATH

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
