# Sets the minimum version of CMake required to build the native
# library.
cmake_minimum_required(VERSION 3.22.1)

# important the projectName is ofApp to be linked - see relinker
project(ofApp LANGUAGES CXX)
set(TARGET_ANDROID TRUE)

set(LOCAL_PATH ${CMAKE_SOURCE_DIR})
set(OF_APP_SRC_PATH ${CMAKE_SOURCE_DIR}/../../src)
set(PRJ_OF_ROOT ${LOCAL_PATH}/../../../../)
set(PURE_OF_ROOT ${LOCAL_PATH}/../../../../../)
set(CORE_OF_ROOT ${PURE_OF_ROOT}/libs/openFrameworks)
set(LIBS_ROOT ${PURE_OF_ROOT}/libs)
set(PRJ_ADDONS_PATH ${PURE_OF_ROOT}/addons)
set(PRJ_SOURCE_PATH ${LIBS_ROOT}/openFrameworks)
set(PRJ_LIBS_ROOT ${PURE_OF_ROOT}/libs)
set(OF_ANDROID ${PURE_OF_ROOT}/libs/openFrameworksCompiled/project/android)
set(OF_ANDROID_OUTPUT ${PURE_OF_ROOT}/libs/openFrameworksCompiled/lib/android)
set(PRJ_OFX_ANDROID_PATH ${PRJ_ADDONS_PATH}/ofxAndroid)
set(PRJ_OFX_ANDROID_CPP_PATH ${PRJ_OFX_ANDROID_PATH}/src)

macro(print_all_variables)
    message(STATUS "print_all_variables------------------------------------------{")
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
    message(STATUS "print_all_variables------------------------------------------}")
endmacro()

# Custom function to check if the library is built
function(check_library)
    if (NOT TARGET openFrameworksAndroid)
        message(STATUS "openFrameworksAndroid Library not found. Building library...")

        # Invoke the build process for the library
        execute_process(
                COMMAND ${CMAKE_COMMAND} --build ${OF_ANDROID}/
                RESULT_VARIABLE result
        )
        if (result)
            message(FATAL_ERROR "Failed to build the library.")
        endif ()
    endif ()
endfunction()

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(TARGET_ANDROID ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ferror-limit=0 -std=c17 -Oz -Wall -fno-short-enums -fPIE -fPIC -fexceptions -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ferror-limit=0 -std=c++23 -Oz -stdlib=libc++ -Wall -fno-short-enums -fPIE -fPIC -fexceptions -ffunction-sections -fdata-sections")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-export-dynamic")
print_all_variables()
set(OF_LIBRARY_TYPE "SHARED")  # or "STATIC"
if(OF_LIBRARY_TYPE STREQUAL "SHARED")
    set(LIB_EXTENSION ".so")
elseif(OF_LIBRARY_TYPE STREQUAL "STATIC")
    set(LIB_EXTENSION ".a")
endif()

add_library( ${PROJECT_NAME}
        ${OF_LIBRARY_TYPE}
        # src files for project (just c/cpp)
        ${OF_APP_SRC_PATH}/main.cpp
        ${OF_APP_SRC_PATH}/ofApp.cpp
)

include_directories(
        ${PRJ_SOURCE_PATH}/3d
        ${PRJ_SOURCE_PATH}/app
        ${PRJ_SOURCE_PATH}/communication
        ${PRJ_SOURCE_PATH}/events
        ${PRJ_SOURCE_PATH}/gl
        ${PRJ_SOURCE_PATH}/graphics
        ${PRJ_SOURCE_PATH}/math
        ${PRJ_SOURCE_PATH}/sound
        ${PRJ_SOURCE_PATH}/types
        ${PRJ_SOURCE_PATH}/utils
        ${PRJ_SOURCE_PATH}/video
        ${PRJ_SOURCE_PATH}
        ${PRJ_OFX_ANDROID_CPP_PATH}/
        ${PURE_OF_ROOT}/addons/ofxAndroid/src
        ${PURE_OF_ROOT}/addons/ofxAccelerometer/src
        ${PRJ_LIBS_ROOT}/FreeImage/include
        ${PRJ_LIBS_ROOT}/freetype/include
        ${PRJ_LIBS_ROOT}/freetype/include/freetype2
        ${PRJ_LIBS_ROOT}/freetype/include/freetype2/freetype/config
        ${PRJ_LIBS_ROOT}/freetype/include/freetype2/freetype/internal
        ${PRJ_LIBS_ROOT}/freetype/include/freetype2/freetype/internal/services
        ${PRJ_LIBS_ROOT}/glm/include
        ${PRJ_LIBS_ROOT}/brotli/include
        ${PRJ_LIBS_ROOT}/json/include
        ${PRJ_LIBS_ROOT}/libpng/include
        ${PRJ_LIBS_ROOT}/fmt/include
        ${PRJ_LIBS_ROOT}/pugixml/include
        ${PRJ_LIBS_ROOT}/json/include
        ${PRJ_LIBS_ROOT}/tess2/include
        ${PRJ_LIBS_ROOT}/utf8/include
        ${PRJ_LIBS_ROOT}/tess2/include
        ${PRJ_LIBS_ROOT}/zlib/include
        ${PRJ_LIBS_ROOT}/uriparser/include
        ${PRJ_LIBS_ROOT}/openssl/include
        ${PRJ_LIBS_ROOT}/curl/include
        ${CMAKE_SOURCE_DIR}/
        ${OF_APP_SRC_PATH}/
        ${OF_ANDROID}
)

# Load add-on include directories from Gradle
if(DEFINED ADDON_INCLUDE_DIRS)
    foreach(DIR ${ADDON_INCLUDE_DIRS})
        include_directories(${DIR})
    endforeach()
endif()
if(DEFINED ADDON_LIBS)
    message(STATUS "Adding addon libraries: ${ADDON_LIBS}")
    foreach(LIB ${ADDON_LIBS})
        target_link_libraries(${PROJECT_NAME} ${LIB})
    endforeach()
endif()

find_library(ANDROID_LIB NAMES android)
find_library(LOG_LIB NAMES log)
find_library(GLES1_LIB NAMES GLESv1_CM)
find_library(GLES2_LIB NAMES GLESv2)
find_library(GLES3_LIB NAMES GLESv3)

target_link_libraries(${PROJECT_NAME}
        android
        log
        m
        z
        dl
)
target_link_libraries(${PROJECT_NAME}
        ${ANDROID_LIB}
        ${GLES2_LIB}
        ${GLES3_LIB}
        ${GLES1_LIB}
        ${LOG_LIB}
)

target_link_libraries( ${PROJECT_NAME}
        ${OF_ANDROID_OUTPUT}/${ANDROID_ABI}/libopenFrameworksAndroid${LIB_EXTENSION})
