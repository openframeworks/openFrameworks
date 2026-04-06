# Android Toolchain CMake Configuration
cmake_minimum_required(VERSION 3.14)

# Add additional flags or settings if needed
set(CMAKE_ANDROID_STL_TYPE "c++_shared") # Adjust STL type if needed

if(NOT DEFINED C_STANDARD)
    set(C_STANDARD 17) # Default to C17
endif()
if(NOT DEFINED CPP_STANDARD)
    set(CPP_STANDARD 17) # Default to C++17
endif()
set(CMAKE_C_STANDARD ${C_STANDARD})
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD ${CPP_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(NOT DEFINED ANDROID_ABI)
    if(DEFINED ENV{ANDROID_ABI})
        set(ANDROID_ABI $ENV{ANDROID_ABI})
    else()
        message(FATAL_ERROR "ANDROID_ABI must be specified (e.g., armeabi-v7a, arm64-v8a, x86, x86_64)")
    endif()
endif()

if(NOT DEFINED ANDROID_NDK_ROOT)
    if(DEFINED ENV{ANDROID_NDK_ROOT})
        set(ANDROID_NDK_ROOT $ENV{ANDROID_NDK_ROOT})
    else()
        message(FATAL_ERROR "ANDROID_NDK_ROOT must be specified as the path to the Android NDK")
    endif()
endif()

if(NOT DEFINED ANDROID_API)
    if(DEFINED ENV{ANDROID_API})
        set(ANDROID_API $ENV{ANDROID_API})
    else()
        message(FATAL_ERROR "ANDROID_API must be specified as the path to the ANDROID_API")
    endif()
endif()
set(ANDROID_NATIVE_API_LEVEL ${ANDROID_API})

# Detect Host Platform
if(NOT DEFINED HOST_PLATFORM)
    if(CMAKE_HOST_SYSTEM_NAME MATCHES "Darwin")
        if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "arm64")
            set(HOST_PLATFORM "darwin-arm64")
        else()
            set(HOST_PLATFORM "darwin-x86_64")
        endif()
    elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
        set(HOST_PLATFORM "windows-x86_64") # Windows ARM64 is rare, adjust if needed
    elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
        if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "aarch64")
            set(HOST_PLATFORM "linux-arm64")
        else()
            set(HOST_PLATFORM "linux-x86_64")
        endif()
    else()
        message(FATAL_ERROR "Unsupported host platform: ${CMAKE_HOST_SYSTEM_NAME} (${CMAKE_HOST_SYSTEM_PROCESSOR})")
    endif()
endif()

message(STATUS "Android ABI: ${ANDROID_ABI}")
message(STATUS "Detected Host Platform: ${HOST_PLATFORM}")

# NDK Configuration
set(TOOLCHAIN_TYPE "llvm")

set(TOOLCHAIN "${ANDROID_NDK_ROOT}/toolchains/${TOOLCHAIN_TYPE}/prebuilt/${HOST_PLATFORM}")
set(SYSROOT "${TOOLCHAIN}/sysroot")

# ABI-specific configuration
if(ANDROID_ABI STREQUAL "armeabi-v7a")
    set(MACHINE "armv7")
    set(ANDROID_PREFIX "armv7a-linux-androideabi")
    set(ANDROID_TARGET "armv7a-none-linux-androideabi${ANDROID_NATIVE_API_LEVEL}")
    set(CMAKE_ANDROID_ARM_MODE ON)
    set(CMAKE_ANDROID_ARM_NEON ON)
    set(CMAKE_SYSTEM_PROCESSOR "armv7-a")
    set(CMAKE_C_FLAGS "-mfpu=neon -mfloat-abi=softfp -O3 -ffast-math -funroll-loops -funsafe-math-optimizations --target=${ANDROID_TARGET} -D__ANDROID_API__=${ANDROID_API}")
    set(CMAKE_CXX_FLAGS "-mfpu=neon -mfloat-abi=softfp -O3 -ffast-math -funroll-loops -funsafe-math-optimizations -fexceptions -frtti --target=${ANDROID_TARGET} -D__ANDROID_API__=${ANDROID_API}")
elseif(ANDROID_ABI STREQUAL "arm64-v8a")
    set(MACHINE "arm64")
    set(ANDROID_PREFIX "aarch64-linux-android")
    set(ANDROID_TARGET "aarch64-none-linux-android${ANDROID_NATIVE_API_LEVEL}")
    set(CMAKE_SYSTEM_PROCESSOR "aarch64")
    set(CMAKE_C_FLAGS "-O3 -ffast-math -funroll-loops -funsafe-math-optimizations --target=${ANDROID_TARGET} -D__ANDROID_API__=${ANDROID_API}")
    set(CMAKE_CXX_FLAGS "-O3 -fexceptions -frtti -ffast-math -funroll-loops -funsafe-math-optimizations --target=${ANDROID_TARGET} -D__ANDROID_API__=${ANDROID_API}")
elseif(ANDROID_ABI STREQUAL "x86")
    set(MACHINE "i686")
    set(ANDROID_PREFIX "i686-linux-android")
    set(ANDROID_TARGET "i686-none-linux-android${ANDROID_NATIVE_API_LEVEL}")
    set(CMAKE_SYSTEM_PROCESSOR "i686")
    set(CMAKE_C_FLAGS "-O3 --target=${ANDROID_TARGET} -D__ANDROID_API__=${ANDROID_API}")
    set(CMAKE_CXX_FLAGS "-O3 -fexceptions -frtti --target=${ANDROID_TARGET} -D__ANDROID_API__=${ANDROID_API}")
elseif(ANDROID_ABI STREQUAL "x86_64")
    set(MACHINE "x86_64")
    set(ANDROID_PREFIX "x86_64-linux-android")
    set(ANDROID_TARGET "x86_64-none-linux-android${ANDROID_NATIVE_API_LEVEL}")
    set(CMAKE_SYSTEM_PROCESSOR "x86_64")
    set(CMAKE_C_FLAGS "-O3 --target=${ANDROID_TARGET} -D__ANDROID_API__=${ANDROID_API}")
    set(CMAKE_CXX_FLAGS "-O3 -fexceptions -frtti --target=${ANDROID_TARGET} -D__ANDROID_API__=${ANDROID_API}")
else()
    message(FATAL_ERROR "Unsupported ANDROID_ABI: ${ANDROID_ABI}")
endif()

message(STATUS "Configuring for ABI: ${ANDROID_ABI}")
message(STATUS "Machine: ${MACHINE}")
message(STATUS "Android Prefix: ${ANDROID_PREFIX}")

message(STATUS "CMAKE_C_COMPILER: ${TOOLCHAIN}/bin/${ANDROID_PREFIX}${CMAKE_ANDROID_API}${ANDROID_API}-clang")
message(STATUS "CMAKE_CXX_COMPILER: ${TOOLCHAIN}/bin/${ANDROID_PREFIX}${CMAKE_ANDROID_API}${ANDROID_API}-clang++")

# Set compilers
set(CMAKE_C_COMPILER "${TOOLCHAIN}/bin/${ANDROID_PREFIX}${CMAKE_ANDROID_API}${ANDROID_API}-clang")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN}/bin/${ANDROID_PREFIX}${CMAKE_ANDROID_API}${ANDROID_API}-clang++")
set(CMAKE_LINKER "${TOOLCHAIN}/bin/ld.lld")

# Paths
set(CMAKE_SYSROOT ${SYSROOT})
set(CMAKE_FIND_ROOT_PATH ${SYSROOT})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Include and library paths
set(CMAKE_INCLUDE_PATH "${SYSROOT}/usr/include")
set(CMAKE_LIBRARY_PATH "${SYSROOT}/usr/lib/${ANDROID_PREFIX}/${CMAKE_ANDROID_API}")

# Compiler Binary
set(BIN_PREFIX "${TOOLCHAIN_ROOT}/bin/")

#find_program(CMAKE_C_COMPILER clang PATHS "${TOOLCHAIN_ROOT}/bin/")
##find_program(CMAKE_CXX_COMPILER clang++ PATHS "${TOOLCHAIN_ROOT}/bin/")
#find_program(CMAKE_LINKER ld.lld PATHS "${TOOLCHAIN_ROOT}/bin/")
find_program(CMAKE_AR llvm-ar PATHS "${TOOLCHAIN_ROOT}/bin/")
find_program(CMAKE_NM llvm-nm PATHS "${TOOLCHAIN_ROOT}/bin/")
find_program(CMAKE_RANLIB llvm-ranlib PATHS "${TOOLCHAIN_ROOT}/bin/")
find_program(CMAKE_STRIP llvm-strip PATHS "${TOOLCHAIN_ROOT}/bin/")
find_program(CMAKE_OBJCOPY llvm-objcopy PATHS "${TOOLCHAIN_ROOT}/bin/")
find_program(CMAKE_OBJDUMP llvm-objdump PATHS "${TOOLCHAIN_ROOT}/bin/")

# Toolchain Debug Output
message(STATUS "---Android TOOLCHAIN CONFIGURATION")
message(STATUS "Host Platform: ${HOST_PLATFORM}")
message(STATUS "ANDROID_NDK_ROOT : ${ANDROID_NDK_ROOT}")
message(STATUS "Sysroot: ${SYSROOT}")
message(STATUS "Toolchain: ${TOOLCHAIN}")
message(STATUS "ABI: ${ANDROID_ABI}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Linker: ${CMAKE_LINKER}")

# Finalize toolchain settings
set(CMAKE_SYSTEM_NAME "Android")
set(CMAKE_SYSTEM_VERSION ${CMAKE_ANDROID_API})
set(CMAKE_ANDROID_NDK ${ANDROID_NDK_ROOT})

if(NOT EXISTS ${CMAKE_C_COMPILER})
    message(FATAL_ERROR "C Compiler not found: ${CMAKE_C_COMPILER}")
endif()
if(NOT EXISTS ${CMAKE_CXX_COMPILER})
    message(FATAL_ERROR "C++ Compiler not found: ${CMAKE_CXX_COMPILER}")
endif()
