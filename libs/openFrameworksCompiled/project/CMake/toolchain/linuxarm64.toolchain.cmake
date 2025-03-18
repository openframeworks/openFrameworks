# Linux Ubuntu ARM64 Toolchain File

# Specify the system
set(CMAKE_SYSTEM_NAME Linux)        # Cross-compilation target system
set(CMAKE_SYSTEM_PROCESSOR aarch64) # Architecture (ARM64)
set(CMAKE_VERBOSE_MAKEFILE ON)

# GCC Version (Set this variable when invoking CMake)
if(NOT DEFINED GCC_VERSION)
    if(DEFINED ENV{GCC_VERSION})
        set(GCC_VERSION $ENV{GCC_VERSION})
    else()
        set(GCC_VERSION 14) # Default value
        message(STATUS "GCC_VERSION not specified. Defaulting to GCC_VERSION=${GCC_VERSION}")
    endif()
endif()

# Set CROSS_CPU to a default value if not provided
if(NOT DEFINED CROSS_CPU)
    if(DEFINED ENV{CROSS_CPU})
        set(CROSS_CPU $ENV{CROSS_CPU})
    else()
        set(CROSS_CPU "generic-armv8-a")
        message(STATUS "CROSS_CPU not specified. Defaulting to CROSS_CPU=generic-armv8-a")
    endif()
endif()

# Set SYSROOT
if(NOT DEFINED SYSROOT)
    if(DEFINED ENV{SYSROOT})
        set(SYSROOT $ENV{SYSROOT})
    else()
        if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
            set(SYSROOT "/")  # Use native root
            message(STATUS "Host is AArch64. Using native SYSROOT=${SYSROOT}")
        else()
            set(SYSROOT "/")  # Use cross-compilation root
            message(STATUS "SYSROOT not specified. Defaulting to SYSROOT=${SYSROOT} for cross-compilation")
        endif()
    endif()
endif()

# Set TOOLCHAIN_ROOT
if(NOT DEFINED TOOLCHAIN_ROOT)
    if(DEFINED ENV{TOOLCHAIN_ROOT})
        set(TOOLCHAIN_ROOT $ENV{TOOLCHAIN_ROOT})
    else()
        if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
            set(TOOLCHAIN_ROOT "/usr")  # Native AArch64 toolchain
            message(STATUS "Host is AArch64. Using native TOOLCHAIN_ROOT=${TOOLCHAIN_ROOT}")
        else()
            set(TOOLCHAIN_ROOT "/usr/aarch64-linux-gnu/")  # Cross-compilation toolchain
            message(STATUS "TOOLCHAIN_ROOT not specified. Defaulting to TOOLCHAIN_ROOT=${TOOLCHAIN_ROOT} for cross-compilation")
        endif()
    endif()
endif()

# Default GCC path
if(NOT DEFINED GCC_PATH)
    if(DEFINED ENV{GCC_PATH})
        set(GCC_PATH $ENV{GCC_PATH}) # Use GCC_PATH from env
    else()
        if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
            set(GCC_PATH "/usr/bin")  # Native AArch64 GCC
        else()
            set(GCC_PATH "/usr/bin/aarch64-linux-gnu")  # Cross-compilation GCC
            set(ENV{PKG_CONFIG_PATH} "/usr/aarch64-linux-gnu/lib/pkgconfig")
            set(ENV{PKG_CONFIG_LIBDIR} "/usr/aarch64-linux-gnu/lib/pkgconfig")
            set(PKG_CONFIG_PATH "/usr/aarch64-linux-gnu/lib/pkgconfig")
            set(PKG_CONFIG_LIBDIR "/usr/aarch64-linux-gnu/lib/pkgconfig")
            set(PKG_CONFIG_EXECUTABLE "/usr/aarch64-linux-gnu/bin/pkg-config")
        endif()
        message(STATUS "GCC_PATH not specified. Defaulting to GCC_PATH=${GCC_PATH}")
    endif()
endif()

# Compiler settings
set(CMAKE_C_COMPILER "${GCC_PATH}/aarch64-linux-gnu-gcc")
set(CMAKE_CXX_COMPILER "${GCC_PATH}/aarch64-linux-gnu-g++")
set(CMAKE_SYSROOT "${SYSROOT}")
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})

set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)

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
set(CMAKE_C_COMPILER "${GCC_PATH}/aarch64-linux-gnu-gcc")
set(CMAKE_CXX_COMPILER "${GCC_PATH}/aarch64-linux-gnu-g++")


find_program(CMAKE_C_COMPILER aarch64-linux-gnu-gcc PATHS "${TOOLCHAIN_ROOT}/bin" "/usr/bin" "/usr/aarch64-linux-gnu/bin" "/opt/aarch64-linux-gnu/bin" NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++ PATHS "${TOOLCHAIN_ROOT}/bin" "/usr/bin" "/usr/aarch64-linux-gnu/bin" "/opt/aarch64-linux-gnu/bin" NO_DEFAULT_PATH)

find_program(CMAKE_LINKER aarch64-linux-gnu-ld PATHS "${TOOLCHAIN_ROOT}/bin/")
find_program(CMAKE_AR aarch64-linux-gnu-ar PATHS "${TOOLCHAIN_ROOT}/bin/")
find_program(CMAKE_NM aarch64-linux-gnu-nm PATHS "${TOOLCHAIN_ROOT}/bin/")
find_program(CMAKE_RANLIB aarch64-linux-gnu-ranlib PATHS "${TOOLCHAIN_ROOT}/bin/")
find_program(CMAKE_STRIP aarch64-linux-gnu-strip PATHS "${TOOLCHAIN_ROOT}/bin/")
find_program(CMAKE_OBJCOPY aarch64-linux-gnu-objcopy PATHS "${TOOLCHAIN_ROOT}/bin/")
find_program(CMAKE_OBJDUMP aarch64-linux-gnu-objdump PATHS "${TOOLCHAIN_ROOT}/bin/")

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

set(EXTRA_LINKS "")
if (EXISTS "${CMAKE_SYSROOT}/lib/")
    list(APPEND EXTRA_LINKS "-Wl,-rpath-link,${CMAKE_SYSROOT}/lib/" "-L${CMAKE_SYSROOT}/lib/")
endif()
if (EXISTS "${CMAKE_SYSROOT}/lib64/")
    list(APPEND EXTRA_LINKS "-Wl,-rpath-link,${CMAKE_SYSROOT}/lib64/" "-L${CMAKE_SYSROOT}/lib64/")
endif()
if (EXISTS "${CMAKE_SYSROOT}/lib/aarch64-linux-gnu/")
    list(APPEND EXTRA_LINKS "-Wl,-rpath-link,${CMAKE_SYSROOT}/lib/aarch64-linux-gnu" "-L${CMAKE_SYSROOT}/lib/aarch64-linux-gnu")
endif()
if (EXISTS "/usr/lib/aarch64-linux-gnu/")
    list(APPEND EXTRA_LINKS "-Wl,-rpath-link,/usr/lib/aarch64-linux-gnu" "-L/usr/lib/aarch64-linux-gnu")
endif()

set(CFLAGS "--sysroot=${SYSROOT} -I${TOOLCHAIN_ROOT}/${GCC_PREFIX}/libc/usr/include -I${TOOLCHAIN_ROOT}/lib/gcc/${GCC_PREFIX}/${GCC_VERSION}/include -I/usr/include -DSTANDALONE -DPIC -D_REENTRANT -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST")

# Compiler and linker flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CFLAGS} -fPIC -O3 -Wall -Wextra -march=armv8-a")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CFLAGS} -fPIC -O3 -Wall -Wextra -std=c++${CPP_STANDARD} -march=armv8-a+simd+crypto")

# Linker flags
set(CMAKE_EXE_LINKER_FLAGS "-fPIE -pie ${EXTRA_LINKS}")
set(CMAKE_SHARED_LINKER_FLAGS "-shared -fPIC ${EXTRA_LINKS}")

set(CMAKE_LIBRARY_PATH /usr/aarch64-linux-gnu/lib)
set(CMAKE_INCLUDE_PATH /usr/aarch64-linux-gnu/include)

set(OPENGL_FOUND TRUE)
set(OPENGL_gl_LIBRARY "/usr/lib/aarch64-linux-gnu/libGL.so")
set(OPENGL_opengl_LIBRARY "/usr/lib/aarch64-linux-gnu/libGL.so")
set(OPENGL_glx_LIBRARY "/usr/lib/aarch64-linux-gnu/libGLX.so")
set(GLES_LIBRARY "/usr/lib/aarch64-linux-gnu/libGLESv2.so")
set(EGL_LIBRARY "/usr/lib/aarch64-linux-gnu/libEGL.so")

set(X11_INCLUDE_DIR "/usr/aarch64-linux-gnu/include")
set(X11_X11_LIB "/usr/lib/aarch64-linux-gnu/libX11.so")
set(X11_Xext_LIB "/usr/lib/aarch64-linux-gnu/libXext.so")
set(X11_Xrandr_LIB "/usr/lib/aarch64-linux-gnu/libXrandr.so")
set(X11_Xinerama_LIB "/usr/lib/aarch64-linux-gnu/libXinerama.so")
set(X11_Xcursor_LIB "/usr/lib/aarch64-linux-gnu/libXcursor.so")

set(WAYLAND_INCLUDE_DIR "/usr/aarch64-linux-gnu/include")
set(WAYLAND_CLIENT_LIB "/usr/lib/aarch64-linux-gnu/libwayland-client.so")
set(WAYLAND_CURSOR_LIB "/usr/lib/aarch64-linux-gnu/libwayland-cursor.so")
set(WAYLAND_EGL_LIB "/usr/lib/aarch64-linux-gnu/libwayland-egl.so")
set(XKBCOMMON_LIB "/usr/lib/aarch64-linux-gnu/libxkbcommon.so")

message(STATUS "Using GCC Version: ${GCC_VERSION}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "System Root: ${CMAKE_SYSROOT}")
