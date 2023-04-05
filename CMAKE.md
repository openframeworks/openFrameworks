
# CMake Support

## Introduction

CMake is a Meta- build system. You no longer have to deal with strange compiler flags or huge unreadable Makefiles, instead you describe the project layout logically. Then, when executing, it sets everything up so that the compilation will work without errors.

The best part of this is that is is 100% cross platform: The same CMake script can generate project files for more than 40 different build systems on any operating system, including Visual Studio, GCC Makefiles, Ninja Makefiles, Code::Blocks, and many more.

## Building with CMake

To build the library itself for testing the examples:

```bash
git clone https://github.com/openframeworks/openFrameworks
cd openframeworks
mkdir build
cd build
cmake ..
cmake --build .
```

This is it! OpenFrameworks and all examples have been built!

```
    set(CMAKE_SYSTEM_NAME "Android")
    set(CMAKE_SYSTEM_VERSION 1)

    cmake .. -DCMAKE_C_COMPILER=x86_64-linux-android33-clang -DCMAKE_CXX_COMPILER=x86_64-linux-android33-clang++

    cmake .. \
        -DCMAKE_C_COMPILER=armv7a-linux-androideabi33-clang \
        -DCMAKE_CXX_COMPILER=armv7a-linux-androideabi33-clang++ \
        -DCMAKE_SYSTEM_NAME=Android \
        -DCMAKE_SYSTEM_VERSION=25

    cmake .. \
        -DCMAKE_SYSTEM_NAME=Android \
        -DCMAKE_SYSTEM_VERSION=25 \
        -DCMAKE_ANDROID_STANDALONE_TOOLCHAIN=/home/${USER}/android-ndk-r25c/toolchains/llvm/prebuilt/linux-x86_64



    https://developer.android.com/ndk/guides/cmake#command-line

    cmake .. -DCMAKE_TOOLCHAIN_FILE=/home/$USER/android-ndk-r25c/build/cmake/android.toolchain.cmake -DANDROID_ABI=x86_64

    AT LEAST CMake 3.19 !!!

    deb http://deb.debian.org/debian bullseye-backports main
    sudo apt install cmake -t bullseye-backports
```
