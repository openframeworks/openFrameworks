
get_filename_component(OPENFRAMEWORKS_CMAKE_DIR_ROOT ${CMAKE_CURRENT_LIST_FILE} PATH)
get_filename_component(OPENFRAMEWORKS_ROOT "${OPENFRAMEWORKS_CMAKE_DIR_ROOT}/../" ABSOLUTE)
set(OPENFRAMEWORKS_ADDONS_ROOT "${OPENFRAMEWORKS_ROOT}/addons/")

set(OPENFRAMEWORKS_INCLUDES_SEARCH_PATHS 
                                 ${OPENFRAMEWORKS_ROOT}/libs/fmodex/include
                                 ${OPENFRAMEWORKS_ROOT}/libs/FreeImage/include
                                 ${OPENFRAMEWORKS_ROOT}/libs/freetype/include
                                 ${OPENFRAMEWORKS_ROOT}/libs/glee/include
                                 ${OPENFRAMEWORKS_ROOT}/libs/glu/include
                                 ${OPENFRAMEWORKS_ROOT}/libs/glut/include
                                 ${OPENFRAMEWORKS_ROOT}/libs/gstappsink/include
                                 ${OPENFRAMEWORKS_ROOT}/libs/poco/include
                                 ${OPENFRAMEWORKS_ROOT}/libs/quicktime/include
                                 ${OPENFRAMEWORKS_ROOT}/libs/rtAudio/include
                                 ${OPENFRAMEWORKS_ROOT}/libs/unicap/include
                                 ${OPENFRAMEWORKS_ROOT}/libs/videoInput/include
                                 /usr/include/*
                                 
                                 CACHE STRING "OpenFrameworks includes search path")

set(OPENFRAMEWORKS_LIBRARIES_SEARCH_PATHS 
                                 ${OPENFRAMEWORKS_ROOT}/libs/fmodex/lib/linux64
                                 ${OPENFRAMEWORKS_ROOT}/libs/FreeImage/lib/linux64
                                 ${OPENFRAMEWORKS_ROOT}/libs/freetype/lib/linux64
                                 ${OPENFRAMEWORKS_ROOT}/libs/glee/lib/linux64
                                 ${OPENFRAMEWORKS_ROOT}/libs/glu/lib/linux64
                                 ${OPENFRAMEWORKS_ROOT}/libs/glut/lib/linux64
                                 ${OPENFRAMEWORKS_ROOT}/libs/gstappsink/lib/linux64
                                 ${OPENFRAMEWORKS_ROOT}/libs/poco/lib/linux64
                                 ${OPENFRAMEWORKS_ROOT}/libs/quicktime/lib/linux64
                                 ${OPENFRAMEWORKS_ROOT}/libs/rtAudio/lib/linux64
                                 ${OPENFRAMEWORKS_ROOT}/libs/unicap/lib/linux64
                                 ${OPENFRAMEWORKS_ROOT}/libs/videoInput/lib/linux64
                                 
                                 CACHE STRING "OpenFrameworks library search paths")



