# Sets the minimum version of CMake required to build the native
# library.
cmake_minimum_required(VERSION 3.22.1)

project(ofapp LANGUAGES CXX)
set(TARGET_ANDROID TRUE)

set(LOCAL_PATH ${CMAKE_SOURCE_DIR})
set(PRJ_OF_ROOT ${LOCAL_PATH}/../../../../../../../)

set(PURE_OF_ROOT ${LOCAL_PATH}/../../../../../../../)
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

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(TARGET_ANDROID ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c17 -Oz -DNDEBUG -frtti --warn-uninitialized -fno-short-enums -Wextra -fPIE -fPIC -fuse-ld=gold -fexceptions -ffunction-sections -fdata-sections -Wall -Wextra -Wfloat-equal -Wundef -Werror -fverbose-asm -Wint-to-pointer-cast -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wcast-qual -Wmissing-prototypes -Wstrict-overflow=5 -Wwrite-strings -Wconversion --pedantic-errors")
set(CMAKE_CPP_FLAGS "${CMAKE_C_FLAGS} -std=c++17 -Oz -DNDEBUG -stdlib=libc++ --warn-uninitialized -frtti -Wextra -fno-short-enums -fPIE -fPIC -fuse-ld=gold -fexceptions -ffunction-sections -fdata-sections -Wall -Wextra -Wfloat-equal -Wundef -Werror -fverbose-asm -Wint-to-pointer-cast -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wcast-qual -Wmissing-prototypes -Wstrict-overflow=5 -Wwrite-strings -Wconversion --pedantic-errors")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-export-dynamic")

print_all_variables()

# Creates the project's shared lib: libnative-lib.so.
# The lib is loaded by this project's Java code in MainActivity.java:
#     System.loadLibrary("native-lib");
# The lib name in both places must match.
add_library( ofapp #name
        SHARED # type of library
        # src files for project (just c/cpp)
        ${CMAKE_SOURCE_DIR}/main.cpp
        ${CMAKE_SOURCE_DIR}/ofApp.cpp
        )


# Specifies a path to native header files
include_directories(
        # openFrameworks headers
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
        # openFrameworks addons includes
        ${PURE_OF_ROOT}/addons/ofxAndroid/src
        ${PURE_OF_ROOT}/addons/ofxAccelerometer/src
        ${PURE_OF_ROOT}/addons/ofxXmlSettings/src
        ${PURE_OF_ROOT}/addons/ofxXmlSettings/libs
        # openFrameworks Libs includes
        ${PRJ_LIBS_ROOT}/FreeImage/include
        ${PRJ_LIBS_ROOT}/freetype/include
        ${PRJ_LIBS_ROOT}/freetype/include/freetype2
        ${PRJ_LIBS_ROOT}/freetype/include/freetype2/freetype/config
        ${PRJ_LIBS_ROOT}/freetype/include/freetype2/freetype/internal
        ${PRJ_LIBS_ROOT}/freetype/include/freetype2/freetype/internal/services
        ${PRJ_LIBS_ROOT}/glm/include
        ${PRJ_LIBS_ROOT}/pugixml/include
        ${PRJ_LIBS_ROOT}/json/include
        ${PRJ_LIBS_ROOT}/tess2/include
        ${PRJ_LIBS_ROOT}/utf8/include
        ${PRJ_LIBS_ROOT}/uriparser/include
        ${CMAKE_SOURCE_DIR}/
        ${CMAKE_SOURCE_DIR}/
        ${OF_ANDROID}
)

find_library(android-lib android)
find_library(log-lib log)
find_library(GLES2-lib GLESv2)

#find_library(GLES1-lib GLESv1_CM)
#find_library(GLES3-lib GLESv3)


target_link_libraries(ofapp
        EGL
        GLESv2
        log
        c
        m
        z
        dl
#        GLESv3
        )

target_link_libraries( ofapp
        ${android-lib} )
target_link_libraries( ofapp
        ${GLES2-lib} )
target_link_libraries( ofapp
        ${log-lib} )
#target_link_libraries( ofApp
#        ${GLES3-lib} )
#target_link_libraries( ofApp
#        ${GLES1-lib} )

# Finally link in openFrameworks Library for each ABI
target_link_libraries( ofapp
        ${OF_ANDROID_OUTPUT}/${ANDROID_ABI}/libopenFrameworksAndroid.so)
