# openFrameworks core library
#
# Builds libs/openFrameworksCompiled/lib/<platform>[/<arch>]/<core library>.
# Apps include openFrameworks.cmake and link it with of_add_app(); they build
# this project themselves if the library isn't there yet.
#
#   cmake -S libs/openFrameworksCompiled/project/CMake -B build/core -DCMAKE_BUILD_TYPE=Release
#   cmake --build build/core

cmake_minimum_required(VERSION 3.22)
project(openFrameworks LANGUAGES C CXX)

include("${CMAKE_CURRENT_LIST_DIR}/openFrameworks.cmake")

of_add_core_library()
