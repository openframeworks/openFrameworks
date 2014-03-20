export LIBSPATH=android/armeabi-v7a
export ABI=armeabi-v7a
export NDK_PLATFORM=android-8
export NDK_ROOT=/home/arturo/Downloads/android-ndk-r8
export HOST_PLATFORM=linux-x86
export TOOLCHAIN=arm-linux-androideabi-4.4.3
export TOOLCHAIN_PATH=${NDK_ROOT}/toolchains/${TOOLCHAIN}/prebuilt/${HOST_PLATFORM}/bin/
export ANDROID_PREFIX=arm-linux-androideabi-
export CC=${NDK_ROOT}/toolchains/${TOOLCHAIN}/prebuilt/${HOST_PLATFORM}/bin/${ANDROID_PREFIX}gcc
export CXX=${NDK_ROOT}/toolchains/${TOOLCHAIN}/prebuilt/${HOST_PLATFORM}/bin/${ANDROID_PREFIX}g++
export AR=${NDK_ROOT}/toolchains/${TOOLCHAIN}/prebuilt/${HOST_PLATFORM}/bin/${ANDROID_PREFIX}ar
export SYSROOT=${NDK_ROOT}/platforms/${NDK_PLATFORM}/arch-arm/
export CFLAGS="-nostdlib --sysroot=${SYSROOT} -fno-short-enums"
export CFLAGS="$CFLAGS -I${NDK_ROOT}/platforms/${NDK_PLATFORM}/arch-arm/usr/include/ -I${NDK_ROOT}/sources/cxx-stl/gnu-libstdc++/include -I${NDK_ROOT}/sources/cxx-stl/gnu-libstdc++/libs/${ABI}/include -I${NDK_ROOT}/sources/crystax/include/"
#export CFLAGS=$CFLAGS "-fomit-frame-pointer -fstrict-aliasing -funswitch-loops -finline-limit=300 -Wa,--noexecstack -fpic -mthumb-interwork -ffunction-sections  -fstack-protector -Wno-psabi -march=armv7-a -mfloat-abi=softfp -mfpu=vfp -nostdlib -fno-short-enums -DANDROID -D__ARM_ARCH_5__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__ -I/home/arturo/Descargas/android-ndk-r4-crystax/build/platforms/android-8/arch-arm/usr/include/"
#export CXXFLAGS=$CFLAGS "-ffast-math -fomit-frame-pointer -fstrict-aliasing -funswitch-loops -finline-limit=300 -Wa,--noexecstack -fpic -mthumb-interwork -ffunction-sections  -fstack-protector -Wno-psabi -march=armv7-a -mfloat-abi=softfp -mfpu=vfp -nostdlib -fno-short-enums -DANDROID -D__ARM_ARCH_5__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__ -I/home/arturo/Descargas/android-ndk-r4-crystax/build/platforms/android-8/arch-arm/usr/include/"
export LDFLAGS="--sysroot=${SYSROOT} -nostdlib -L${NDK_ROOT}/sources/cxx-stl/gnu-libstdc++/libs/${ABI}"

#arm7
export CFLAGS="$CFLAGS -march=armv7-a -mfloat-abi=softfp -mthumb"
export LDFLAGS="$LDFLAGS -Wl,--fix-cortex-a8 -Wl,--no-undefined"

#arm
#export LDFLAGS=$LDFLAGS -Wl,--fix-cortex-a8 -shared -Wl,--no-undefined
