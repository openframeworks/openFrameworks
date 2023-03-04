cmake_minimum_required(VERSION 3.25)
project(openframeworks)

# Here are all the options you can set for customizing your build
#set(OF_WINDOW_PROVIDER "GLFW" CACHE STRING "The preferred window manager to use: GLFW, GLUT, EGL, NONE")
option(BUILD_SHARED_LIBS "Build OF as a shared library instead of a static one" OFF)

# This defines the actual library
if (BUILD_SHARED_LIBS)
    add_library(openframeworks-core SHARED)
else()
    add_library(openframeworks-core STATIC)
endif()
add_library(openframeworks::core ALIAS openframeworks-core)

# C++ standard version
target_compile_features(openframeworks-core PUBLIC cxx_std_17)

# Preprocessor defines
target_compile_definitions(openframeworks-core PUBLIC 
    OF_USING_STD_FS         # Use the new C++17 filesystem library instead of boost
    UNICODE                 # WinAPI setup
    _UNICODE
)

# if(OF_WINDOW_PROVIDER STREQUAL "GLFW")
#     target_compile_definitions(openframeworks-core PUBLIC TARGET_GLFW_WINDOW)
# elseif(OF_WINDOW_PROVIDER STREQUAL "GLUT")

if(MSVC)
    target_compile_options(openframeworks-core PUBLIC "/Zc:__cplusplus")    # Force MSVC to set macro correctly
    target_compile_options(openframeworks-core PUBLIC "/MP")                # Enable multi-core compilation
endif()

# This is another CMake script that adds the source files
add_subdirectory(libs/openFrameworks)

# This downloads and configures the dependencies for us
include(${CMAKE_CURRENT_LIST_DIR}/cmake/download_deps.cmake)

# And link to all the dependencies we depend upon
target_link_libraries(openframeworks-core 
    openframeworks::glew 
    openframeworks::tess2 
    openframeworks::cairo 
    openframeworks::glm 
    openframeworks::utf8
    openframeworks::glfw
    openframeworks::glut
    openframeworks::fmod
    openframeworks::FreeImage
    openframeworks::rtAudio
    openframeworks::videoInput
    openframeworks::uriparser
    openframeworks::curl
    openframeworks::freetype
    openframeworks::pugixml
)