export ABI=$1
export TOOLCHAIN_PATH=$2

if [ $ABI = armeabi-v7a ] || [ $ABI = armeabi ]; then
    export TOOLCHAIN=androideabi
    export ANDROID_PREFIX=arm-linux-androideabi
elif [ $ABI = x86 ]; then
    export TOOLCHAIN=x86
    export ANDROID_PREFIX=arm-linux-androideabi
fi

export TOOL=${TOOLCHAIN_PATH}/${TOOLCHAIN}/bin/${ANDROID_PREFIX}
export SYSROOT=${TOOLCHAIN_PATH}/${TOOLCHAIN}/sysroot

export CC=${TOOL}-gcc
export CXX=${TOOL}-g++
export LINK=${CXX}
export STRIP=${TOOL}-strip
export LIB=${TOOL}-ar
export RANLIB=${TOOL}-ranlib

export CFLAGS="--sysroot=${SYSROOT} -fno-short-enums -I${SYSROOT}/usr/include/"
export LDFLAGS="--sysroot=${SYSROOT} -lz -lsupc++ -llog -ldl -lm -lc -lgnustl_static -lgcc"

if [ $ABI = armeabi-v7a ]; then
    export CFLAGS="$CFLAGS -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16"
    export LDFLAGS="$LDFLAGS -march=armv7-a -Wl,--fix-cortex-a8 -Wl,--no-undefined"
elif [ $ABI = armeabi ]; then
    export CFLAGS="$CFLAGS"
    export LDFLAGS="$LDFLAGS -Wl,--fix-cortex-a8 -shared -Wl,--no-undefined"
elif [ $ABI = x86 ]; then
    export CFLAGS="$CFLAGS  -march=i686 -msse3 -mstackrealign -mfpmath=sse -fno-stack-protector"
    export LDFLAGS="$LDFLAGS"
fi
