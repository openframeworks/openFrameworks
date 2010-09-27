cmake_minimum_required(VERSION 2.8)
project(firmataExample)

find_library(OF_LIB openFrameworks PATHS ./../../../libs/openFrameworks)

set(OPENFRAMEWORKS_TRY_USE_LOCAL_LIBRARIES TRUE)

set(OPENFRAMEWORKS_LOCAL_SEARCH_PREFIX ../../../libs/fmodex
                                 ../../../libs/FreeImage
                                 ../../../libs/freetype
                                 ../../../libs/glee
                                 ../../../libs/glu
                                 ../../../libs/glut
                                 ../../../libs/gstappsink
                                 ../../../libs/poco
                                 ../../../libs/quicktime
                                 ../../../libs/rtAudio
                                 ../../../libs/unicap
                                 ../../../libs/videoInput CACHE FILEPATH "OpenFrameworks local library search prefixes")

include(../../../libs/openFrameworks/openFrameworks.cmake)

include_directories( src
                     ./../../../libs/openFrameworks/
                     ./../../../libs/openFrameworks/app
                     ./../../../libs/openFrameworks/communication
                     ./../../../libs/openFrameworks/events
                     ./../../../libs/openFrameworks/graphics
                     ./../../../libs/openFrameworks/sound
                     ./../../../libs/openFrameworks/utils
                     ./../../../libs/openFrameworks/video
                     ${OPENFRAMEWORKS_INCLUDES}
                     )


add_executable(firmataExample src/testApp.cpp src/main.cpp )
target_link_libraries(firmataExample ${OF_LIB} ${OPENFRAMEWORKS_LIBRARIES})
