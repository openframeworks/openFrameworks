
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
if(APPLE)
    set(OPENFRAMEWORKS_LIBRARIES_SEARCH_PATHS 
                                     ${OPENFRAMEWORKS_ROOT}/libs/fmodex/lib/osx
                                     ${OPENFRAMEWORKS_ROOT}/libs/FreeImage/lib/osx
                                     ${OPENFRAMEWORKS_ROOT}/libs/freetype/lib/osx
                                     ${OPENFRAMEWORKS_ROOT}/libs/glee/lib/osx
                                     ${OPENFRAMEWORKS_ROOT}/libs/glu/lib/osx
                                     ${OPENFRAMEWORKS_ROOT}/libs/glut/lib/osx
                                     ${OPENFRAMEWORKS_ROOT}/libs/gstappsink/lib/osx
                                     ${OPENFRAMEWORKS_ROOT}/libs/poco/lib/osx
                                     ${OPENFRAMEWORKS_ROOT}/libs/quicktime/lib/osx
                                     ${OPENFRAMEWORKS_ROOT}/libs/rtAudio/lib/osx
                                     ${OPENFRAMEWORKS_ROOT}/libs/unicap/lib/osx
                                     ${OPENFRAMEWORKS_ROOT}/libs/videoInput/lib/osx
                                     
                                     CACHE STRING "OpenFrameworks library search paths")
elseif( "${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
  execute_process(COMMAND uname -m OUTPUT_VARIABLE ARCH)
  if ( ${ARCH} MATCHES "x86_64" )
    set (SUBLIBDIRSUFFIX "linux64")
  else()
    set(SUBLIBDIRSUFFIX "linux")
  endif()

     set(OPENFRAMEWORKS_LIBRARIES_SEARCH_PATHS 
                                     ${OPENFRAMEWORKS_ROOT}/libs/fmodex/lib/${SUBLIBDIRSUFFIX}
                                     ${OPENFRAMEWORKS_ROOT}/libs/FreeImage/lib/${SUBLIBDIRSUFFIX}
                                     ${OPENFRAMEWORKS_ROOT}/libs/freetype/lib/${SUBLIBDIRSUFFIX}
                                     ${OPENFRAMEWORKS_ROOT}/libs/glee/lib/${SUBLIBDIRSUFFIX}
                                     ${OPENFRAMEWORKS_ROOT}/libs/glu/lib/${SUBLIBDIRSUFFIX}
                                     ${OPENFRAMEWORKS_ROOT}/libs/glut/lib/${SUBLIBDIRSUFFIX}
                                     ${OPENFRAMEWORKS_ROOT}/libs/gstappsink/lib/${SUBLIBDIRSUFFIX}
                                     ${OPENFRAMEWORKS_ROOT}/libs/poco/lib/${SUBLIBDIRSUFFIX}
                                     ${OPENFRAMEWORKS_ROOT}/libs/rtAudio/lib/${SUBLIBDIRSUFFIX}
                                     CACHE STRING "OpenFrameworks library search paths")
endif() 


