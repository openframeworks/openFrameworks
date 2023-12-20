cmake_minimum_required(VERSION 3.16)
project(openframeworks)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/utils.cmake)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(OF_IS_MAIN_PROJECT ON)
else()
    set(OF_IS_MAIN_PROJECT OFF)
endif()

# Here are all the options you can set for customizing your build
# BUILD_SHARED_LIBS can be set to build a dll
option(BUILD_EXAMPLES "Add all OpenFrameworks examples to the project" ${OF_IS_MAIN_PROJECT})
option(BUILD_TESTS "Add all OpenFrameworks examples to the project" ${OF_IS_MAIN_PROJECT})

# This defines the actual library
add_library(openframeworks)
add_library(of::openframeworks ALIAS openframeworks)

# This is another CMake script that adds the header locations and source files
include(${CMAKE_CURRENT_LIST_DIR}/libs/openFrameworks/CMakeLists.txt)

# This downloads and configures the dependencies for us
# include(${CMAKE_CURRENT_LIST_DIR}/cmake/download_and_link_deps.cmake)


#     of::tess2 
#     of::glm 
#     of::utf8
#     of::FreeImage
#     of::uriparser
#     of::openssl
#     of::curl
#     of::freetype
#     of::pugixml
#     of::json

# find_package(unofficial-libtess2 CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_BINARY_DIR}")
find_package(glm CONFIG REQUIRED)
find_package(utf8cpp CONFIG REQUIRED)
find_package(FreeImage CONFIG REQUIRED)
find_package(uriparser CONFIG REQUIRED)
find_package(curl CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(pugixml CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# C++ standard version and disabling non-standard compiler specific features
target_compile_features(openframeworks PUBLIC cxx_std_17)
set_target_properties(openframeworks PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(openframeworks PROPERTIES FOLDER "openframeworks")

# Preprocessor defines
target_compile_definitions(openframeworks PUBLIC 
    OF_USING_STD_FS         # Use the new C++17 filesystem library instead of boost
    UNICODE                 # WinAPI setup
    _UNICODE
    CURL_STATICLIB
    POCO_STATIC
    CAIRO_WIN32_STATIC_BUILD
)

# Compiler flags specific to MSVC
if(MSVC)
    target_compile_options(openframeworks PUBLIC "/Zc:__cplusplus")     # Force MSVC to set __cplusplus macro correctly
    target_compile_options(openframeworks PUBLIC "/MP")                 # Enable multi-core compilation
    # target_link_options(openframeworks PUBLIC "/ignore:4099")           # Suppress compiler warning that no .pdb file is available for debugging (in third-party libraries)
    # target_link_options(openframeworks PUBLIC "/NODEFAULTLIB:LIBCMT")      # Don't link to the default C runtime library (Conflicting with third-party libraries)
    # target_link_options(openframeworks PUBLIC "/NODEFAULTLIB:$<IF:$<CONFIG:Debug>,MSVCRT,MSVCRTd>")     # Don't link to the default C++ standard library (Conflicting with third-party libraries)
endif()

# Where compiled binaries should be placed
set_target_properties(openframeworks PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib/$<IF:$<CONFIG:Debug>,debug,release>"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib/$<IF:$<CONFIG:Debug>,debug,release>"
)

add_subdirectory(addons)

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
