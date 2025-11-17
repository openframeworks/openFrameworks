#!/usr/bin/env bash
set -e

ORIGINAL_DIR="$(pwd)"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd $SCRIPT_DIR
APOTHECARY_LEVEL="$(cd "$SCRIPT_DIR/../.." && pwd)"

############################################
# CMake Build Core Android openFrameworks
############################################

export MAKE_TARGET="${MAKE_TARGET:-cmake}"
export CPP_STANDARD="${CPP_STANDARD:-23}"
export C_STANDARD="${C_STANDARD:-17}"
export NDK_VERSION_MAJOR="${NDK_VERSION_MAJOR:-27}"
export ANDROID_API="${ANDROID_API:-34}" #minimum Android API supported. 34 default
ANDROID_PLATFORM="android-34" # Usually must be >= 24 for modern libraries and NDKs.
DEFAULT_ARCHS="armeabi-v7a arm64-v8a x86_64"
TOOLCHAIN_FILE="$(pwd)/../CMake/toolchain/android.toolchain.cmake"
CMAKELISTS_DIR="$(pwd)/openframeworksAndroid"
OUTPUT_DIR="$(pwd)/../../lib/android"


OS_TYPE="unknown"
if [[ "$(uname)" == "Darwin" ]]; then
    OS_TYPE="macOS"
    PARALLEL_MAKE=$(sysctl -n hw.ncpu)
elif [[ "$(uname -o 2>/dev/null)" == "Msys" || "$(uname -o 2>/dev/null)" == "Cygwin" || "$(uname -o 2>/dev/null)" == "Windows_NT" ]]; then
    OS_TYPE="Windows"
    PARALLEL_MAKE=$(nproc 2>/dev/null || echo 1)
elif [[ "$(uname)" == "Linux" ]]; then
    OS_TYPE="Linux"
    PARALLEL_MAKE=$(nproc --all)
else
    PARALLEL_MAKE=1
fi
if [[ "$OS_TYPE" == "macOS" ]]; then
    echo "Running on macOS"
    export ANDROID_SDK_PATH="$HOME/Library/Android/sdk"
elif [[ "$OS_TYPE" == "Windows" ]]; then
    echo "Running on Windows (Git Bash)"
    WIN_USER=$(whoami)
    if [ -z "$WIN_USER" ]; then
        echo "Error: Could not determine Windows username."
        exit 1
    fi
    echo "Windows User: $WIN_USER"
    APPDATA_WIN="C:\\Users\\$WIN_USER\\AppData\\Local"
    echo "LOCALAPPDATA (Windows): $APPDATA_WIN"
    ANDROID_SDK_PATH="$(cygpath -u "${APPDATA_WIN}")/Android/Sdk"
elif [[ "$OS_TYPE" == "Linux" ]]; then
    echo "Running on Linux"
    export ANDROID_SDK_PATH="${ANDROID_HOME:-/usr/local/lib/android/sdk}"
else
    echo "Unsupported OS!"
    exit 1
fi
if [ -d "$ANDROID_SDK_PATH" ]; then
    echo "Android SDK found at: $ANDROID_SDK_PATH"
else
    echo "Android SDK not found at expected location."
    exit 1
fi

# Find latest NDK inside the SDK directory
ANDROID_NDK_PATH=$(ls -d "$ANDROID_SDK_PATH/ndk/"* 2>/dev/null | sort -V | tail -n 1)
export ANDROID_NDK_ROOT=$ANDROID_NDK_PATH
if [ -n "$ANDROID_NDK_PATH" ]; then
    echo "Latest Android NDK found at: $ANDROID_NDK_PATH"
else
    echo "No NDK found inside SDK directory."
    exit 1
fi
ANDROID_CMAKE_PATH=$(ls -d "$ANDROID_SDK_PATH/cmake/"* 2>/dev/null | sort -V | tail -n 1)
NINJA_PATH=$(find "$ANDROID_CMAKE_PATH" -name "ninja" -type f 2>/dev/null | head -n 1)
if [[ "$OS_TYPE" == "Windows" ]]; then
    NINJA_PATH=$(find "$ANDROID_CMAKE_PATH/bin" -name "ninja.exe" -type f 2>/dev/null | head -n 1)
fi
if [ -n "$NINJA_PATH" ]; then
    echo "Latest Ninja found at: [$NINJA_PATH]"
else
    echo "Ninja not found in Android SDK! CMakePath: $ANDROID_CMAKE_PATH"
    exit 1
fi

if [ -z "$ARCH" ] || [ "$ARCH" == "all" ]; then
  ARCH="$DEFAULT_ARCHS"
fi

echo "build for architecture(s): $ARCHES"
for ARCHE in $ARCH; do
  echo "-------------------------------------"
  echo "Building for $ARCHE..."
  echo "-------------------------------------"
  BUILD_DIR="../../lib/android/build/build-$ARCHE"
  mkdir -p "$BUILD_DIR"
  cd "$SCRIPT_DIR"
  cd "$BUILD_DIR"
  export ANDROID_ABI=${ARCHE}

  cmake ../../../../project/android/openframeworksAndroid \
    -G "Ninja" \
    -DCMAKE_TOOLCHAIN_FILE=${TOOLCHAIN_FILE} \
    -DANDROID_PLATFORM=${ANDROID_PLATFORM} \
    -DCMAKE_BUILD_TYPE=Release \
    -DANDROID_TOOLCHAIN=clang \
    -DANDROID_NDK_ROOT=$ANDROID_NDK_ROOT \
    -DANDROID_ABI=${ARCHE} \
    -DANDROID=TRUE \
    -DANDROID_API=${ANDROID_API} \
    -DCMAKE_MAKE_PROGRAM="${NINJA_PATH}" \
    -DCMAKE_CXX_FLAGS="-DUSE_PTHREADS=1 -fvisibility-inlines-hidden -std=c++${CPP_STANDARD} -frtti" \
    -DCMAKE_C_FLAGS="-DUSE_PTHREADS=1 -fvisibility-inlines-hidden -std=c${C_STANDARD} -Wno-implicit-function-declaration -frtti" \
    -DCMAKE_MINIMUM_REQUIRED_VERSION=3.22 \
    -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE \
    -DCONFIGURATION_BUILD_DIR="$OUTPUT_DIR" \
    "$CMAKELISTS_DIR"

  ninja -j "$NUM_CORES"

  cd "$SCRIPT_DIR"
done

echo -e "=========================================\n"

cd "$ORIGINAL_DIR"
