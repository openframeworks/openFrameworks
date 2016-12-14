#!/usr/bin/env bash

set -e

export OF_ROOT=~/openFrameworks
cd ${OF_ROOT}/addons/$(basename $TRAVIS_BUILD_DIR)
if [ "$TARGET" == "linuxarmv6l" ]; then
    export CXXFLAGS="${CXXFLAGS} -ftrack-macro-expansion=0";
    sed -i "s/PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = .*/PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = -g0/" ${OF_ROOT}/libs/openFrameworksCompiled/project/makefileCommon/config.linux.common.mk;
    export GCC_PREFIX=arm-linux-gnueabihf;
    export GST_VERSION=1.0;
    export RPI_ROOT=${OF_ROOT}/scripts/ci/${TARGET}/raspbian;
    export TOOLCHAIN_ROOT=${OF_ROOT}/scripts/ci/${TARGET}/rpi_toolchain;
    export PLATFORM_OS=Linux;
    export PLATFORM_ARCH=armv6l;
    export PKG_CONFIG_LIBDIR=${RPI_ROOT}/usr/lib/pkgconfig:${RPI_ROOT}/usr/lib/${GCC_PREFIX}/pkgconfig:${RPI_ROOT}/usr/share/pkgconfig;
elif [ "$TARGET" == "linuxarmv7l" ]; then
    sed -i "s/PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = .*/PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = -g0/" ${OF_ROOT}/libs/openFrameworksCompiled/project/makefileCommon/config.linux.common.mk;
    export GCC_PREFIX=armv7l-unknown-linux-gnueabihf;
    export TOOLCHAIN_PREFIX=arm-linux-gnueabihf;
    export GST_VERSION=1.0;
    export RPI_ROOT=$HOME/archlinux;
    export TOOLCHAIN_ROOT=$HOME/rpi2_toolchain;
    export PLATFORM_OS=Linux;
    export PLATFORM_ARCH=armv7l;
    export PKG_CONFIG_DIR=;
    export PKG_CONFIG_LIBDIR=${RPI_ROOT}/usr/lib/pkgconfig:${RPI_ROOT}/usr/share/pkgconfig;
    export PKG_CONFIG_SYSROOT_DIR=${RPI_ROOT};
    export CXX=${TOOLCHAIN_ROOT}/bin/${GCC_PREFIX}-g++;
    export CC=${TOOLCHAIN_ROOT}/bin/${GCC_PREFIX}-gcc;
    export AR=${TOOLCHAIN_ROOT}/bin/${GCC_PREFIX}-ar;
    export LD=${TOOLCHAIN_ROOT}/bin/${GCC_PREFIX}-ld;
    export CFLAGS="$CFLAGS --sysroot=${RPI_ROOT}";
    export CXXFLAGS="$CXXFLAGS --sysroot=${RPI_ROOT}";
    export CXXFLAGS="${CXXFLAGS} -ftrack-macro-expansion=0";
elif [ "$TARGET" == "emscripten" ]; then        
    sed -i "s/PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = .*/PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = -g0/" ${OF_ROOT}/libs/openFrameworksCompiled/project/makefileCommon/config.linux.common.mk;
    source ~/emscripten-sdk/emsdk_env.sh;
fi

for example in example*; do
    cp ${OF_ROOT}/scripts/templates/$TARGET/Makefile $example/
    cp ${OF_ROOT}/scripts/templates/$TARGET/config.make $example/
    cd $example
    if [ "$TARGET" == "android" ]; then
        echo "ABIS_TO_COMPILE_DEBUG = $OPT" >> config.make
        make DebugNoOF PLATFORM_OS=Android
    elif [ "$TARGET" == "emscripten" ]; then
        emmake make DebugNoOF
    elif [ "$TARGET" == "linuxarmv7l" ]; then
        make DebugNoOF PLATFORM_VARIANT=raspberry2
    else
        make DebugNoOF
    fi
    cd ..
done
