get_filename_component(ADDON_ROOT ${CMAKE_CURRENT_LIST_FILE} PATH)
get_filename_component(ADDON_NAME ${ADDON_ROOT} NAME)

message(STATUS "-------------------------------- Configuring OF Addon '${ADDON_NAME}'")

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  execute_process(COMMAND uname -m OUTPUT_VARIABLE ARCH)
  if ( ${ARCH} MATCHES "x86_64" )
    set (SUBLIBDIRSUFFIX "linux64")
  else()
    set(SUBLIBDIRSUFFIX "linux")
  endif()
  find_library(OPENCV cv PATHS ${ADDON_ROOT}/libs/opencv/lib/${SUBLIBDIRSUFFIX})
  find_library(OPENCVAUX cvaux PATHS ${ADDON_ROOT}/libs/opencv/lib/${SUBLIBDIRSUFFIX})
  find_library(OPENCXCORE cxcore PATHS ${ADDON_ROOT}/libs/opencv/lib/${SUBLIBDIRSUFFIX})
elseif(APPLE)
 
  set(SUBLIBDIRSUFFIX "osx")
  set (CMAKE_FIND_LIBRARY_PREFIXES "lib" "")                                                                                                                           
  set (CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".a")

  find_library(OPENCV openCV PATHS ${ADDON_ROOT}/libs/opencv/lib/${SUBLIBDIRSUFFIX})
  if ( OPENCV-NOTFOUND)
    find_library(OPENCV cv PATHS ${ADDON_ROOT}/libs/opencv/lib/${SUBLIBDIRSUFFIX})
    find_library(OPENCVAUX cvaux PATHS ${ADDON_ROOT}/libs/opencv/lib/${SUBLIBDIRSUFFIX})
    find_library(OPENCXCORE cxcore PATHS ${ADDON_ROOT}/libs/opencv/lib/${SUBLIBDIRSUFFIX})
  endif()
endif()


if ( OPENCV-NOTFOUND OR OPENCVAUX-NOTFOUND OR OPENCXCORE-NOTFOUND )
  message (FATAL_ERROR "Cannot find OpenCV library")
else()
  message (STATUS "Found OpenCV library: ${OPENCV} ${OPENCVAUX} ${OPENCXCORE}")
  set (ADDONS_LIBRARIES ${ADDONS_LIBRARIES} ${OPENCV} ${OPENCVAUX} ${OPENCXCORE} )
endif()

find_path(OPENCV_INCPATH NAMES cv.h PATHS ${ADDON_ROOT}/libs/opencv/include/*
                                          /usr/include/*
                                          /usr/local/include/*)


if ( DEFINED OPENCV_INCPATH )
  set(ADDONS_INCLUDE_DIRS ${ADDONS_INCLUDE_DIRS} ${OPENCV_INCPATH})
  message (STATUS "Found OpenCV includes: ${ADDONS_INCLUDE_DIRS}")
else()
  message (FATAL_ERROR "Cannot find OpenCV include files")
endif()

file( GLOB_RECURSE ADDON_SRC ${ADDON_ROOT}/src/*.cpp)
set(ADDONS_SOURCES ${ADDONS_SOURCES} ${ADDON_SRC})
set(ADDONS_INCLUDE_DIRS ${ADDONS_INCLUDE_DIRS} "${ADDON_ROOT}/src/")
