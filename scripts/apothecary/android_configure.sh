export ABI=$1
if [ "$(uname)" == "Darwin" ]; then
    export HOST_PLATFORM=darwin-x86_64
else
    export HOST_PLATFORM=linux-x86_64
fi
export LIBSPATH=android/$ABI
export NDK_PLATFORM=$ANDROID_PLATFORM
export TOOLCHAIN_VERSION=4.9
if [ $ABI = armeabi-v7a ] || [ $ABI = armeabi ]; then
    export SYSROOT=${NDK_ROOT}/platforms/${NDK_PLATFORM}/arch-arm
    export ANDROID_PREFIX=arm-linux-androideabi
    export TOOLCHAIN=$ANDROID_PREFIX-4.9
elif [ $ABI = x86 ]; then
    export SYSROOT=${NDK_ROOT}/platforms/${NDK_PLATFORM}/arch-x86
    export ANDROID_PREFIX=i686-linux-android
    export TOOLCHAIN=x86-4.9
fi
export TOOLCHAIN_PATH=${NDK_ROOT}/toolchains/${TOOLCHAIN}/prebuilt/${HOST_PLATFORM}/bin
export PATH=${PATH}:${TOOLCHAIN_PATH}
export CC=${TOOLCHAIN_PATH}/${ANDROID_PREFIX}-gcc
export CXX=${TOOLCHAIN_PATH}/${ANDROID_PREFIX}-g++
export AR=${TOOLCHAIN_PATH}/${ANDROID_PREFIX}-ar
export RANLIB=${TOOLCHAIN_PATH}/${ANDROID_PREFIX}-ranlib
export CFLAGS="-nostdlib --sysroot=${SYSROOT} -fno-short-enums"
export CFLAGS="$CFLAGS -I${SYSROOT}/usr/include/ -I${NDK_ROOT}/sources/cxx-stl/gnu-libstdc++/${TOOLCHAIN_VERSION}/include -I${NDK_ROOT}/sources/cxx-stl/gnu-libstdc++/${TOOLCHAIN_VERSION}/libs/${ABI}/include -I${NDK_ROOT}/sources/android/cpufeatures"
export LDFLAGS="--sysroot=${SYSROOT} -nostdlib -L${NDK_ROOT}/sources/cxx-stl/gnu-libstdc++/${TOOLCHAIN_VERSION}/libs/${ABI} -lz -lsupc++ -llog -ldl -lm -lc -lgnustl_static -lgcc"

if [ $ABI = armeabi-v7a ]; then
    export CFLAGS="$CFLAGS -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16"
    export LDFLAGS="$LDFLAGS -march=armv7-a -Wl,--fix-cortex-a8 -Wl,--no-undefined"
elif [ $ABI = armeabi ]; then
    export CFLAGS="$CFLAGS"
    export LDFLAGS="$LDFLAGS -Wl,--fix-cortex-a8 -shared -Wl,--no-undefined"
elif [ $ABI = x86 ]; then
    export CFLAGS="$CFLAGS  -march=i686 -msse3 -mstackrealign -mfpmath=sse -fno-stack-protector"
    export LDFLAGS="$LDFLAGS  -march=i686"
fi
