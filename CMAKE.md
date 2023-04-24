
# CMake Support

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

## Visual Studio

The following instructions are for building openFrameworks itself, in the root directory of the repository. Make sure to download a nightly build as soon as CMake support is available, as the main repository takes tremendously longer to download.

### Building in IDE

Open the generated `openFrameworks.sln` in your build folder with Visual Studio. Then, just build it like any other VS project. Right-click a project and click on `Set as Startup Project` to be able to run it with F5.

### Building in the terminal

You can also build Visual Studio projects without actually opening Visual Studio, by the use of MSBuild. CMake does all of that for you. 

Note that configurations in Visual Studio work a bit different compared to makefiles, as VS is a multi-configuration build tool, makefiles are not.

#### Build all examples
```bash
# in root directory of openFrameworks
mkdir build && cd build
cmake ..
cmake --build . --target=build_all_examples --config=Release   # Or 'Debug'
```

#### Build specific example

Replace `ofNodeExample` with the project name of the example.

```bash
# in root directory of openFrameworks
mkdir build && cd build
cmake ..
cmake --build . --target=ofNodeExample --config=Release   # Or 'Debug'
```

#### Run tests

```bash
# in root directory of openFrameworks
mkdir build && cd build
cmake ..
cmake --build . --config=Release   # Or 'Debug'
ctest -C Release                   # Or 'Debug'
```

In the VS IDE, building RUN_TESTS (in CMakePredefinedTargets folder) does the same thing.
