cmake_minimum_required(VERSION 2.8)
set(PROJECT "polygonExample")
project(${PROJECT})

include(../../../cmake/OpenFrameworksConfig.cmake)
include(../../../cmake/OpenFrameworks.cmake)

include_directories( src
                     ${OPENFRAMEWORKS_ROOT}/libs/openFrameworks/
                     ${OPENFRAMEWORKS_ROOT}/libs/openFrameworks/app
                     ${OPENFRAMEWORKS_ROOT}/libs/openFrameworks/communication
                     ${OPENFRAMEWORKS_ROOT}/libs/openFrameworks/events
                     ${OPENFRAMEWORKS_ROOT}/libs/openFrameworks/graphics
                     ${OPENFRAMEWORKS_ROOT}/libs/openFrameworks/sound
                     ${OPENFRAMEWORKS_ROOT}/libs/openFrameworks/utils
                     ${OPENFRAMEWORKS_ROOT}/libs/openFrameworks/video
                     ${OPENFRAMEWORKS_INCLUDES}
                     )


find_library(OF_LIB openFrameworks PATHS ${OPENFRAMEWORKS_ROOT}/libs/openFrameworks)
add_executable(${PROJECT} src/testApp.cpp src/main.cpp )

target_link_libraries(${PROJECT} ${OF_LIB} ${OPENFRAMEWORKS_LIBRARIES})
