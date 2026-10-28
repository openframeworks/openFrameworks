# openFrameworks CMake
#
# Shared entry point for the core library project (CMakeLists.txt in this
# folder) and for apps. An app's CMakeLists.txt does:
#
#   cmake_minimum_required(VERSION 3.22)
#   project(myApp LANGUAGES C CXX)
#   set(OF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")
#   include("${OF_ROOT}/libs/openFrameworksCompiled/project/CMake/openFrameworks.cmake")
#   of_add_app(myApp ADDONS ofxGui)
#
# Platforms (OF_PLATFORM, detected from the toolchain, or set it):
#   macos ios tvos emscripten linux64 linuxaarch64 linuxarmv7l linuxarmv6l vs msys2
#
# Options:
#   OF_BUILD_CORE=ON   always build the core in this build, even if a prebuilt
#                      libs/openFrameworksCompiled/lib/<platform> library exists
#   OF_USE_CCACHE=OFF  do not prefix the compilers with ccache

include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/ccache.cmake")

get_filename_component(OF_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../.." ABSOLUTE)
set(OF_ROOT "${OF_ROOT}" CACHE PATH "openFrameworks root" FORCE)
set(OF_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(OF_LIBS_DIR "${OF_ROOT}/libs")
set(OF_CORE_SRC_DIR "${OF_LIBS_DIR}/openFrameworks")
set(OF_ADDONS_DIR "${OF_ROOT}/addons")

option(OF_BUILD_CORE "Build the openFrameworks core in this build instead of linking a prebuilt one" OFF)
option(OF_PRECOMPILED_HEADER "Precompile ofMain.h for each app's own C++ sources" OFF)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

include("${OF_CMAKE_DIR}/OFPlatform.cmake")
include("${OF_CMAKE_DIR}/OFCore.cmake")
include("${OF_CMAKE_DIR}/platform/${OF_PLATFORM_FAMILY}.cmake")
include("${OF_CMAKE_DIR}/OFAddons.cmake")
include("${OF_CMAKE_DIR}/OFApp.cmake")

# libs/openFrameworksCompiled/lib/<platform>[/<arch>]
set(OF_CORE_LIB_DIR "${OF_LIBS_DIR}/openFrameworksCompiled/lib/${OF_PLATFORM}")
if(OF_ARCH)
	string(APPEND OF_CORE_LIB_DIR "/${OF_ARCH}")
endif()
