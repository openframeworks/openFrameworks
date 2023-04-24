
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

## Visual Studio (and multi-config build systems)

The following instructions are for building openFrameworks itself, in the root directory of the repository. Make sure to download a nightly build as soon as CMake support is available, as the main repository takes tremendously longer to download.

Visual Studio automatically uses all of the cores available.

In the following examples, we always use cmake like `cmake ..` in the build folder. However, you can also use following signature: 
`cmake -S source_dir -B build_dir`. This way the current working directory is irrelevant and the two directories can be anywhere (especially useful for CI toolchains).

### Building in IDE

Open the generated `openFrameworks.sln` in your build folder with Visual Studio. Then, just build it like any other VS project. Right-click a project and click on `Set as Startup Project` to be able to run it with F5.

### Building in the terminal

You can also build Visual Studio projects without actually opening Visual Studio, by the use of MSBuild. CMake does all of that for you. 

Note that configurations in Visual Studio work a bit different compared to makefiles, as VS is a multi-configuration build tool, makefiles are not.

#### Build all examples
```bash
# in root directory of openFrameworks
mkdir build
cd build
cmake ..
cmake --build . --target=build_all_examples --config=Release   # Or 'Debug'
```

#### Build specific example

Replace `ofNodeExample` with the project name of the example.

```bash
# in root directory of openFrameworks
mkdir build
cd build
cmake ..
cmake --build . --target=ofNodeExample --config=Release   # Or 'Debug'
```

#### Run tests

```bash
# in root directory of openFrameworks
mkdir build
cd build
cmake ..
cmake --build . --config=Release   # Or 'Debug'
ctest -C Release                   # Or 'Debug'
```

In the VS IDE, building RUN_TESTS (in CMakePredefinedTargets folder) does the same thing.

## Makefiles (and single-configuration build systems)

#### Building all examples

```bash
# in root directory of openFrameworks
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release             # Or 'Debug'
cmake --build . --target=build_all_examples -j<number-of-cores>
```

#### Building specific example

```bash
# in root directory of openFrameworks
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release             # Or 'Debug'
cmake --build . --target=ofNodeExample -j<number-of-cores>
```

#### Run tests

```bash
# in root directory of openFrameworks
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release            # Or 'Debug'
cmake --build . -j<number-of-cores>
ctest -C Release                               # Or 'Debug'
```

## Jetbrains CLion

When starting with CLion, you do not generate anything with CMake. It's the opposite: You open your root CMakeLists.txt in CLion,
which then calls CMake for you. Time to relax.

CLion is especially useful when it comes to unit tests due to its excellent support for CMake CTest.

### CLion on Windows

Currently, MinGW/MSYS2 is entirely unsupported. If you want to use CLion on Windows, you must use the Microsoft MSVC toolchain 
(Visual Studio), which is also faster and more reliable than MinGW or MSYS as it's native.
If [Visual Studio](https://visualstudio.microsoft.com/de/downloads/) is installed, it is automatically recognized in the CLion toolchain settings.
Thus, you can build using CLion's excellent IDE, while the Visual Studio compiler is used under the hood.
This results in the most robust development experience on Windows.

MinGW support (especially for CI) and more is yet to be finalized by you! :)

## Visual Studio Code

When working with Visual Studio Code, the default C++ extensions are recommended, as well as the CMake extension from Microsoft. 
See the CLion section above, as the same applies to VS Code. The CMake extension simply invokes CMake, which then uses MSVC under the hood.
The CMake extension's panel in the left bar allows you to build and run targets.

This means you can also build and run directly in VS Code, very similar to CLion.

# Yet to be done

 - [ ] Test everything on all Apple platforms
 - [ ] Add full support for Android & iOS
 - [ ] Add support for Emscripten
 - [ ] Add support for MinGW/MSYS2
 - [ ] Add CMake install targets, which would allow to:
    - 1: Install the library globally, so that new OF projects can simply link to it instead of building from source.
    - 2: Install projects derived from it into an installation directory, which in turn allows to:
    - 3: Use CMake's CPack to bundle all installed files and create native installers for each platform. (e.g. .exe, .msi, .deb, .rpm, .dmg, etc.)

Feel free to contribute to make this list shorter, Happy Coding!
