
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
