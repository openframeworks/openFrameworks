#!/usr/bin/env bash
set -e

ORIGINAL_DIR="$(pwd)"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd $SCRIPT_DIR
APOTHECARY_LEVEL="$(cd "$SCRIPT_DIR/../.." && pwd)"

############################################
# Configuration
############################################

TOOLCHAIN_FILE="$(pwd)/../CMake/toolchain/android.toolchain.cmake"
CMAKELISTS_DIR="$(pwd)/openframeworksAndroid"

OUTPUT_DIR="$(pwd)/../../lib/android"

export MAKE_TARGET="${MAKE_TARGET:-cmake}"
export NDK_VERSION_MAJOR="${NDK_VERSION_MAJOR:-27}"
export ANDROID_API="${ANDROID_API:-34}" #minimum Android API supported. 21 default

# Usually must be >= 24 for modern libraries and NDKs.
ANDROID_PLATFORM="android-34"
C_STANDARD=17
CPP_STANDARD=23

# Default architectures
DEFAULT_ARCHS="armeabi-v7a arm64-v8a x86_64"

############################################
# Parse Argument
############################################

ARCH="$1"

# If user didn’t specify an arch or said "all", build for all
if [ -z "$ARCH" ] || [ "$ARCH" == "all" ]; then
  ARCH="$DEFAULT_ARCHS"
fi

if command -v sysctl &> /dev/null; then
    PARALLEL_MAKE=$(sysctl -n hw.ncpu)
elif command -v nproc &> /dev/null; then
    PARALLEL_MAKE=$(nproc)
else
    PARALLEL_MAKE=1
fi

echo "Will build for architecture(s): $ARCHES"

############################################
# Build
############################################

for ARCHE in $ARCH; do
  echo "-------------------------------------"
  echo "Building for $ARCHE..."
  echo "-------------------------------------"

  # Create a dedicated build folder for each architecture
  BUILD_DIR="build-$ARCHE"
  mkdir -p "$BUILD_DIR"
  cd "$BUILD_DIR"

  cmake ../openframeworksAndroid \
    -G "Ninja" \
    -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
    -DANDROID_ABI="$ARCH" \
    -DANDROID_PLATFORM="$ANDROID_PLATFORM" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_MAKE_PROGRAM=$(which ninja) \
    -DCONFIGURATION_BUILD_DIR="$OUTPUT_DIR" \
    "$CMAKELISTS_DIR"

  # Build using all available cores.
  ninja -j "$NUM_CORES"

   # cmake ../openframeworksAndroid \
   #          -DCMAKE_INSTALL_PREFIX=Release \
   #          -DCMAKE_BUILD_TYPE=Release \
   #          -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE \
   #          -DPLATFORM=Android \
   #          -DANDROID_PLATFORM=${ANDROID_PLATFORM} \
   #          -DANDROID_ABI=${ARCHE} \
   #          -DANDROID_API=${ANDROID_API} \
   #          -DANDROID_TOOLCHAIN=clang \
   #          -DANDROID_NDK_ROOT=$ANDROID_NDK_ROOT \
   #          -DBUILD_SHARED_LIBS=OFF \
   #          -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE \
   #          -DCMAKE_MINIMUM_REQUIRED_VERSION=3.22 \
   #          -DCMAKE_CXX_FLAGS="-DUSE_PTHREADS=1 -fvisibility-inlines-hidden -std=c++${CPP_STANDARD} -frtti" \
   #          -DCMAKE_C_FLAGS="-DUSE_PTHREADS=1 -fvisibility-inlines-hidden -std=c${C_STANDARD} -Wno-implicit-function-declaration -frtti" \
   #          -DENABLE_VISIBILITY=OFF \
   #          -DCMAKE_VERBOSE_MAKEFILE=ON \
   #          -DCMAKE_CXX_EXTENSIONS=OFF
   #      cmake --build . --config Release -j${PARALLEL_MAKE} --target install

  # # Configure
  # cmake \
  #   -G "Ninja" \
  #   -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
  #   -DANDROID_ABI="$ARCH" \
  #   -DANDROID_PLATFORM="$ANDROID_PLATFORM" \
  #   -DCMAKE_BUILD_TYPE=Release \
  #   "$CMAKELISTS_DIR"


  # # Build
  # ninja -j $NUM_CORES

  cd ..
done

echo -e "=========================================\n"

cd "$ORIGINAL_DIR"
