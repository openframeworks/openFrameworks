export ABI=$1
if [ "$(uname)" == "Darwin" ]; then
    export HOST_PLATFORM=darwin-x86_64
else
    export HOST_PLATFORM=linux-x86_64
fi
export LIBSPATH=android/$ABI
export NDK_PLATFORM=$ANDROID_PLATFORM
export TOOLCHAIN_VERSION=4.9
export CLANG_VERSION=3.6
if [ $ABI = armeabi-v7a ] || [ $ABI = armeabi ]; then
    export SYSROOT=${NDK_ROOT}/platforms/${NDK_PLATFORM}/arch-arm
    export ANDROID_PREFIX=arm-linux-androideabi
    export GCC_TOOLCHAIN=$ANDROID_PREFIX-${TOOLCHAIN_VERSION}
elif [ $ABI = x86 ]; then
    export SYSROOT=${NDK_ROOT}/platforms/${NDK_PLATFORM}/arch-x86
    export ANDROID_PREFIX=i686-linux-android
    export GCC_TOOLCHAIN=x86-${TOOLCHAIN_VERSION}
fi
export TOOLCHAIN=llvm-${CLANG_VERSION}
export TOOLCHAIN_PATH=${NDK_ROOT}/toolchains/${TOOLCHAIN}/prebuilt/${HOST_PLATFORM}/bin
export GCC_TOOLCHAIN_PATH=${NDK_ROOT}/toolchains/${GCC_TOOLCHAIN}/prebuilt/${HOST_PLATFORM}/bin
export PATH=${PATH}:${TOOLCHAIN_PATH}
export CC=${TOOLCHAIN_PATH}/clang
export CXX=${TOOLCHAIN_PATH}/clang++
export AR=${GCC_TOOLCHAIN_PATH}/${ANDROID_PREFIX}-ar
export RANLIB=${GCC_TOOLCHAIN_PATH}/${ANDROID_PREFIX}-ranlib
export CFLAGS="-nostdlib --sysroot=${SYSROOT} -fno-short-enums"
export CFLAGS="$CFLAGS -I${SYSROOT}/usr/include/ -I${NDK_ROOT}/sources/android/support/include -I${NDK_ROOT}/sources/cxx-stl/llvm-libc++/libcxx/include -I${NDK_ROOT}/sources/android/cpufeatures"
export LDFLAGS="--sysroot=${SYSROOT} -nostdlib -L${NDK_ROOT}/sources/cxx-stl/llvm-libc++/libs/${ABI} -lz -llog -lc++_static"

if [ $ABI = armeabi-v7a ]; then
    export CFLAGS="$CFLAGS -target armv7-none-linux-androideabi -gcc-toolchain ${NDK_ROOT}/toolchains/${GCC_TOOLCHAIN}/prebuilt/${HOST_PLATFORM} -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16"
    export LDFLAGS="$LDFLAGS -target armv7-none-linux-androideabi -gcc-toolchain ${NDK_ROOT}/toolchains/${GCC_TOOLCHAIN}/prebuilt/${HOST_PLATFORM} -march=armv7-a -Wl,--fix-cortex-a8 -Wl,--no-undefined"
elif [ $ABI = armeabi ]; then
    export CFLAGS="$CFLAGS"
    export LDFLAGS="$LDFLAGS -Wl,--fix-cortex-a8 -shared -Wl,--no-undefined"
elif [ $ABI = x86 ]; then
    export CFLAGS="$CFLAGS -target i686-none-linux-android -gcc-toolchain ${NDK_ROOT}/toolchains/${GCC_TOOLCHAIN}/prebuilt/${HOST_PLATFORM}  -march=i686 -msse3 -mstackrealign -mfpmath=sse -fno-stack-protector"
    export LDFLAGS="$LDFLAGS -target i686-none-linux-android -gcc-toolchain ${NDK_ROOT}/toolchains/${GCC_TOOLCHAIN}/prebuilt/${HOST_PLATFORM}  -march=i686"
fi
