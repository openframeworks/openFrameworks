# openFrameworks app (CMake)
#
#   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
#   cmake --build build
#
# The app is built into bin/. The openFrameworks core is linked from
# libs/openFrameworksCompiled/lib, or built here first if it isn't there.

cmake_minimum_required(VERSION 3.22)

# project folder name is the app name by default
get_filename_component(OF_APP_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
project(${OF_APP_NAME} LANGUAGES C CXX)

# path to openFrameworks, relative to this project (apps/myApps/<app> by default)
if(NOT OF_ROOT)
	set(OF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")
endif()
include("${OF_ROOT}/libs/openFrameworksCompiled/project/CMake/openFrameworks.cmake")

# sources come from src/, addons from addons.make; or list them:
#   of_add_app(${OF_APP_NAME} ADDONS ofxGui ofxOsc)
of_add_app(${OF_APP_NAME})
