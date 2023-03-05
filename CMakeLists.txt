cmake_minimum_required(VERSION 3.25)
project(openframeworks)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/utils.cmake)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are a really bad idea. If you don't know what you're doing, please create a 'build' folder and call 'cmake ..' from within.")
endif()

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(OF_IS_MAIN_PROJECT ON)
else()
    set(OF_IS_MAIN_PROJECT OFF)
endif()

# Here are all the options you can set for customizing your build
option(BUILD_SHARED_LIBS "Build OF as a shared library instead of a static one" OFF)
option(BUILD_EXAMPLES "Add all OpenFrameworks examples to the project" ${OF_IS_MAIN_PROJECT})
option(BUILD_TESTS "Add all OpenFrameworks examples to the project" ${OF_IS_MAIN_PROJECT})

# This defines the actual library
if (BUILD_SHARED_LIBS)
    add_library(openframeworks SHARED)
else()
    add_library(openframeworks STATIC)
endif()
add_library(of::openframeworks ALIAS openframeworks)

# This is another CMake script that adds the header locations and source files
include(${CMAKE_CURRENT_LIST_DIR}/libs/openFrameworks/ofSources.cmake)

# C++ standard version and disabling non-standard compiler specific features
target_compile_features(openframeworks PUBLIC cxx_std_17)
set_target_properties(openframeworks PROPERTIES CXX_EXTENSIONS OFF)

# Preprocessor defines
target_compile_definitions(openframeworks PUBLIC 
    OF_USING_STD_FS         # Use the new C++17 filesystem library instead of boost
    UNICODE                 # WinAPI setup
    _UNICODE
    CURL_STATICLIB
    POCO_STATIC
)

# Compiler flags specific to MSVC
if(MSVC)
    target_compile_options(openframeworks PUBLIC "/Zc:__cplusplus")    # Force MSVC to set macro correctly
    target_compile_options(openframeworks PUBLIC "/MP")                # Enable multi-core compilation
    # Ignore linker warning that no .pdb file is available for debugging
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ignore:4099 /NODEFAULTLIB:LIBCMT /NODEFAULTLIB:MSVCRT")
endif()

# Output everything into a bin folder
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Folder layout in Visual Studio & Co
set_target_properties(openframeworks PROPERTIES FOLDER "openframeworks")

# This downloads and configures the dependencies for us
include(${CMAKE_CURRENT_LIST_DIR}/cmake/download_deps.cmake)

# And link to all the dependencies we depend upon
target_link_libraries(openframeworks 
    of::glew 
    of::tess2 
    of::cairo 
    of::glm 
    of::utf8
    of::glfw
    of::glut
    of::fmod
    of::FreeImage
    of::rtAudio
    of::videoInput
    of::uriparser
    of::openssl
    of::curl
    of::freetype
    of::pugixml
    of::json
)

if(WIN32)       # Linking to WinAPI system libraries
    target_link_libraries(openframeworks winmm ws2_32 wldap32 crypt32 dsound.lib)
endif()

# Other dependencies
find_package(OpenGL)
target_include_directories(openframeworks PUBLIC ${OPENGL_INCLUDE_DIRS})
target_link_libraries(openframeworks ${OPENGL_LIBRARIES})

add_subdirectory(addons)

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()
