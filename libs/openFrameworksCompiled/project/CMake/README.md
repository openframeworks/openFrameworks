# openFrameworks CMake

CMake build for the openFrameworks core and apps on macOS, iOS, tvOS, Emscripten,
Linux (linux/64, linux/aarch64, linux/armv7l, linux/armv6l) and Windows (Visual Studio,
MSYS2). Android has its own CMake in `project/android` for now.

It mirrors the makefiles and IDE projects: the same sources, defines, flags and
third-party libraries per platform, apps built into the project's `bin/`, and
the core in `libs/openFrameworksCompiled/lib/<platform>`.

## Requirements

- CMake 3.22 or newer (3.28 for iOS / tvOS, which link the libraries' xcframeworks directly)
- the libraries for the platform (`scripts/<platform>/download_libs.sh`), as for the other builds
- any generator: Ninja or Makefiles, Xcode (macOS / iOS / tvOS), Visual Studio 2022 / 2026 (x64, ARM64, ARM64EC)
- Emscripten: `emcmake` from the Emscripten SDK (openFrameworks CI uses 6.0.13)

## App

Copy `scripts/templates/cmake/CMakeLists.txt` into a project (`apps/myApps/myApp`):

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Sources come from `src/`, addons from `addons.make`. The core is linked from
`libs/openFrameworksCompiled/lib/<platform>` if it's there for the configuration
being built, otherwise it's built in this build first (and written there for the
next app). `-DOF_BUILD_CORE=ON` always builds it here (e.g. after changing the
core sources).

System frameworks and libraries come from the project's `config.make`, like the
makefiles: `PROJECT_FRAMEWORKS = Security` (`-framework`, Apple only) and
`PROJECT_SYSTEM_LIBS = sqlite3 z` (`-l`).

| Platform | Configure | Output |
|---|---|---|
| macOS | `cmake -S . -B build -G Ninja` (or `-G Xcode`) | `bin/<app>.app` |
| Linux | `cmake -S . -B build -G Ninja` | `bin/<app>` |
| Visual Studio | `cmake -S . -B build -G "Visual Studio 17 2022" -A x64` (`ARM64`, `ARM64EC`) | `bin/<app>.exe` |
| MSYS2 | `cmake -S . -B build -G Ninja` (from the MSYS2 shell) | `bin/<app>.exe` |
| Emscripten | `emcmake cmake -S . -B build -G Ninja`, then `emrun bin/em/<app>` | `bin/em/<app>/index.html` |
| iOS | `cmake -S . -B build -G Xcode -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphonesimulator` | `bin/<app>.app` |
| tvOS | `cmake -S . -B build -G Xcode -DCMAKE_SYSTEM_NAME=tvOS -DCMAKE_OSX_SYSROOT=appletvsimulator` | `bin/<app>.app` |

Debug builds are named `<app>_debug`. For an iOS / tvOS device use
`-DCMAKE_OSX_SYSROOT=iphoneos` (`appletvos`) and `-DOF_APPLE_DEVELOPMENT_TEAM=<team id>`.

In `CMakeLists.txt`:

```cmake
of_add_app(myApp)                                  # src/ + addons.make
of_add_app(myApp ADDONS ofxGui ofxOsc)             # explicit addons
of_add_app(myApp SOURCES src/main.cpp src/ofApp.cpp NO_ADDONS_MAKE)
target_compile_definitions(myApp PRIVATE MY_DEFINE)   # it's a normal CMake target
```

## Core only

```sh
cmake -S libs/openFrameworksCompiled/project/CMake -B build/core -DCMAKE_BUILD_TYPE=Release
cmake --build build/core
```

| Platform | Core library (same name and place as the existing builds) |
|---|---|
| macOS | `lib/macos/openFrameworks.a`, `openFrameworksDebug.a` (Xcode's) |
| iOS / tvOS | `lib/ios/<iphoneos\|iphonesimulator>/openFrameworksiOS.a`, `lib/tvos/<appletvos\|appletvsimulator>/openFrameworkstvOS.a` |
| Linux | `lib/<platform>/libopenFrameworks.a`, `libopenFrameworksDebug.a` (make's) |
| Visual Studio | `lib/vs/<arch>/openframeworksLib.lib`, `openframeworksLib_debug.lib` (the VS project's) |
| MSYS2 | `lib/msys2/libopenFrameworks.a` (make's) |
| Emscripten | `lib/emscripten/libopenFrameworks.a` (make builds `libopenFrameworks.o` next to it) |

## Tests

`-DOF_BUILD_TESTS=ON` on the core build adds every test in `tests/` (desktop
platforms) and registers it with CTest. Each test runs from its executable's
folder, like `make RunDebug`:

```sh
cmake -S libs/openFrameworksCompiled/project/CMake -B build/tests -DCMAKE_BUILD_TYPE=Debug -DOF_BUILD_TESTS=ON
cmake --build build/tests
ctest --test-dir build/tests --output-on-failure     # Linux without a display: xvfb-run -a ctest ...
```

Multi-config generators (Visual Studio, Xcode) take `--config Debug` on the build
and `-C Debug` on `ctest`. The tests need no `CMakeLists.txt`: they're added with
`of_add_app(... PROJECT_DIR <dir>)` (`OFProjects.cmake`). This follows the CTest
setup in [#7524](https://github.com/openframeworks/openFrameworks/pull/7524).

## All examples

`-DOF_BUILD_EXAMPLES=ON` on the core build adds every example for the platform
(not built by default) and a `build_all_examples` target. Targets are grouped in
IDE folders (`openFrameworks`, `examples/<group>`, `tests/<group>`), so one Visual
Studio solution, Xcode project or CLion project opens them all:

```sh
cmake -S libs/openFrameworksCompiled/project/CMake -B build/examples -DOF_BUILD_EXAMPLES=ON
cmake --build build/examples --target build_all_examples    # or one example: --target graphicsExample
```

Each example builds into its own `bin/`, as with its own `CMakeLists.txt`. An example
name used in two groups gets the group as a prefix. This follows the examples setup in
[#7524](https://github.com/openframeworks/openFrameworks/pull/7524).

## Options

The Emscripten `PROJECT_EMSCRIPTEN_*` options have the same names and values as the
makefiles' `config.make` options.

| Option | Default | |
|---|---|---|
| `OF_BUILD_CORE` | `OFF` | build the core in this build even if a prebuilt one exists |
| `OF_BUILD_TESTS` | `OFF` | core build: add `tests/` and register them with CTest |
| `OF_BUILD_EXAMPLES` | `OFF` | core build: add `examples/` and a `build_all_examples` target |
| `OF_PRECOMPILED_HEADER` | `OFF` | precompile `ofMain.h` for each app's own `src/` C++ files (faster rebuilds); addon sources compile without it |
| `OF_USE_CCACHE` | `ON` | prefix C and C++ with ccache when it is installed. A launcher you already set is left as it is |
| `OF_PLATFORM` | detected | `macos ios tvos emscripten linux/64 linux/aarch64 linux/armv7l linux/armv6l vs msys2`. `linux64` and the other flat names still select the same platform |
| `OF_USE_DAWN`, `OF_USE_ANGLE` | `OFF` | link `libs/dawn`, `libs/metalangle` |
| `PROJECT_EMSCRIPTEN_WASM` | `32` | `32`: libs from `lib/emscripten/WASM`, core in `lib/emscripten`. `64`: libs and core in `lib/emscripten/WASM64` |
| `PROJECT_EMSCRIPTEN_TOTAL_MEMORY` | `134217728` | initial memory in bytes |
| `PROJECT_EMSCRIPTEN_EMBIND_AOT` | `OFF` | ON needs an `EMSCRIPTEN_BINDINGS` block and breaks some addons |
| `PROJECT_EMSCRIPTEN_TEMPLATE` | `project/emscripten/template.html` | HTML shell |
| `OF_WEBGL_VERSION` | `2` | `1` for a WebGL 1 build |
| `APP_VERSION` | `project()` version, else `1.0` | app version (macOS / iOS / tvOS `CFBundleShortVersionString`), same as the makefiles' `config.make` |
| `APP_BUILD` | `1` | build number (`CFBundleVersion`) |
| `OF_APPLE_DEVELOPMENT_TEAM` | | signing team for iOS / tvOS devices |

## Addons

`of_target_addons(<target> <addon>...)` adds addons the way the makefiles do:
`src/` and `libs/*/src` sources, `src/`, `libs/` and `libs/*/include` include paths,
`libs/*/lib/<platform>` libraries, then `addon_config.mk`'s `common:` and
`<platform>:` sections (`=` replaces, `+=` appends, `%` is a wildcard), including
`ADDON_DEPENDENCIES`, `ADDON_*_EXCLUDE`, `ADDON_DEFINES`, `ADDON_CFLAGS`,
`ADDON_LDFLAGS`, `ADDON_FRAMEWORKS` and `ADDON_PKG_CONFIG_LIBRARIES`. An addon is a
name in `addons/`, or a path relative to the project for local addons.
`of_add_app()` calls it for `ADDONS` / `addons.make`.

## Project Generator

projectGenerator 0.110.0 and later (GUI 0.102.0, projectGenerator#664) has a `cmake`
template: it adds this template's `CMakeLists.txt` next to the project for the
platform (an existing `CMakeLists.txt` is kept) and writes the addons to `addons.make`:

```sh
projectGenerator -o"<OF root>" -p"linux64" -t"cmake" apps/myApps/myApp     # -p vs, osx, emscripten, ...
```

It also reads `APP_VERSION` / `APP_BUILD` and `PROJECT_EMSCRIPTEN_WASM` back from the
`CMakeLists.txt` (projectGenerator#664, #670). The projectGenerator in `apps/projectGenerator`
on master (0.115.0) has it.

A generated project only needs the template `CMakeLists.txt` with the addons
listed (or an `addons.make`). If PG resolves the addons itself, it can write them
out explicitly instead:

```cmake
of_add_app(myApp NO_ADDONS_MAKE)
of_target_addon_files(myApp NAME ofxOsc
	SOURCES    ${OF_ROOT}/addons/ofxOsc/src/ofxOscReceiver.cpp ...
	INCLUDES   ${OF_ROOT}/addons/ofxOsc/src ${OF_ROOT}/addons/ofxOsc/libs/oscpack/src ...
	LIBS       ...            # static libraries, absolute paths
	DEFINES    ...
	CFLAGS     ...
	LDFLAGS    ...
	FRAMEWORKS ...            # Apple frameworks
	PKG_CONFIG ...)           # pkg-config packages (Linux, MSYS2)
```

## Layout

| File | |
|---|---|
| `openFrameworks.cmake` | entry point, included by apps and `CMakeLists.txt` |
| `CMakeLists.txt` | the core library project |
| `OFPlatform.cmake` | platform / arch detection |
| `OFCore.cmake` | core sources, third-party libs, `openFrameworks` and `openFrameworks::deps` targets |
| `OFAddons.cmake` | addons and `addon_config.mk` |
| `OFApp.cmake` | `of_add_app()` |
| `platform/<family>.cmake` | per platform: excludes, defines, flags, libraries, app bundle / output |
