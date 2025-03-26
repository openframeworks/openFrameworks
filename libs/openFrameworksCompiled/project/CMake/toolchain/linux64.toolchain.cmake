# Linux Ubuntu 64-bit Toolchain File

# Specify the system
set(CMAKE_SYSTEM_NAME Linux)        # Cross-compilation target system
set(CMAKE_SYSTEM_PROCESSOR x86_64) # Architecture (64-bit)
set(CMAKE_VERBOSE_MAKEFILE ON)

# GCC Version (Set this variable when invoking CMake)
if(NOT DEFINED GCC_VERSION)
    if(DEFINED ENV{GCC_VERSION})
        set(GCC_VERSION $ENV{GCC_VERSION})
    else()
        set(GCC_VERSION 14) # Default value
        message(WARNING "GCC_VERSION not specified. Defaulting to GCC_VERSION=${GCC_VERSION}")
    endif()
endif()

if(NOT DEFINED SYSROOT)
    if(DEFINED ENV{SYSROOT})
        set(SYSROOT $ENV{SYSROOT})
    else()
        set(SYSROOT /) # Default value
    endif()
endif()

# Path to GCC 
if(NOT DEFINED GCC_PATH)
    if(DEFINED ENV{GCC_PATH})
        set(GCC_PATH $ENV{GCC_PATH}) # Use GCC_PATH from the env
    else()
        set(GCC_PATH "/usr/bin") # Default path
    endif()
endif()

if(NOT DEFINED C_STANDARD)
    set(C_STANDARD 17 CACHE STRING "" FORCE) # Default to C17
endif()
if(NOT DEFINED CPP_STANDARD)
    set(CPP_STANDARD 17 CACHE STRING "" FORCE) # Default to C++17
endif()

set(CMAKE_C_STANDARD ${C_STANDARD} CACHE STRING "" FORCE)
set(CMAKE_C_STANDARD_REQUIRED ON )
set(CMAKE_CXX_STANDARD ${CPP_STANDARD} CACHE STRING "" FORCE)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler Binary Paths
set(CMAKE_C_COMPILER "${GCC_PATH}/gcc-${GCC_VERSION}")
set(CMAKE_CXX_COMPILER "${GCC_PATH}/g++-${GCC_VERSION}")
set(TOOLCHAIN_ROOT "${GCC_PATH}")

# Paths to system libraries and includes
set(CMAKE_SYSROOT "/")
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)

find_program(CMAKE_C_COMPILER ${CMAKE_SYSTEM_PROCESSOR}-linux-gnu-gcc PATHS "${TOOLCHAIN_ROOT}/bin/")
find_program(CMAKE_CXX_COMPILER ${CMAKE_SYSTEM_PROCESSOR}-linux-gnu-g++ PATHS "${TOOLCHAIN_ROOT}/bin/")
find_program(CMAKE_LINKER ${CMAKE_SYSTEM_PROCESSOR}-linux-gnu-ld PATHS "${TOOLCHAIN_ROOT}/bin/")
find_program(CMAKE_AR ${CMAKE_SYSTEM_PROCESSOR}-linux-gnu-ar PATHS "${TOOLCHAIN_ROOT}/bin/")
find_program(CMAKE_NM ${CMAKE_SYSTEM_PROCESSOR}-linux-gnu-nm PATHS "${TOOLCHAIN_ROOT}/bin/")
find_program(CMAKE_RANLIB ${CMAKE_SYSTEM_PROCESSOR}-linux-gnu-ranlib PATHS "${TOOLCHAIN_ROOT}/bin/")
find_program(CMAKE_STRIP ${CMAKE_SYSTEM_PROCESSOR}-linux-gnu-strip PATHS "${TOOLCHAIN_ROOT}/bin/")
find_program(CMAKE_OBJCOPY ${CMAKE_SYSTEM_PROCESSOR}-linux-gnu-objcopy PATHS "${TOOLCHAIN_ROOT}/bin/")
find_program(CMAKE_OBJDUMP ${CMAKE_SYSTEM_PROCESSOR}-linux-gnu-objdump PATHS "${TOOLCHAIN_ROOT}/bin/")

message(STATUS "Using GCC Version: ${GCC_VERSION}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")

# Check for the existence of the specified GCC version
if(NOT EXISTS ${CMAKE_C_COMPILER})
    message(WARNING "C Compiler not found: ${CMAKE_C_COMPILER}")
endif()
if(NOT EXISTS ${CMAKE_CXX_COMPILER})
    message(WARNING "C++ Compiler not found: ${CMAKE_CXX_COMPILER}")
endif()

set(EXTRA_LINKS "-Wl,-rpath-link,${CMAKE_SYSROOT}/lib/ \
    -L${CMAKE_SYSROOT}/lib/ \
    -Wl,-rpath-link,${CMAKE_SYSROOT}/lib64/ \
    -L${CMAKE_SYSROOT}/lib64/ \
    -L${CMAKE_SYSROOT}/lib/x86_64-linux-gnu \
    -Wl,-rpath-link,${CMAKE_SYSROOT}/lib/x86_64-linux-gnu")

# Compiler and linker flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --sysroot=${CMAKE_SYSROOT} -fPIC -O3 -Wall -Wextra -march=x86-64 -mtune=generic ${EXTRA_LINKS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --sysroot=${CMAKE_SYSROOT} -fPIC -O3 -Wall -Wextra -std=c++${CPP_STANDARD} -march=x86-64 -mtune=generic ${EXTRA_LINKS}")
set(CMAKE_EXE_LINKER_FLAGS "-fPIE -pie ${EXTRA_LINKS}")
set(CMAKE_SHARED_LINKER_FLAGS "-shared -fPIC")


message(STATUS "Using GCC Version: ${GCC_VERSION}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "System Root: ${CMAKE_SYSROOT}")

