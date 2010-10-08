set(ADDON_NAME ofxOsc)


if (APPLE)
  find_library(OSCPACK oscpack PATHS ${OPENFRAMEWORKS_ADDONS_DIRECTORY}/${ADDON_NAME}/libs/oscpack/lib/osx)
endif(APPLE)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
execute_process(COMMAND uname -m OUTPUT_VARIABLE ARCH)
  if ( ${ARCH} MATCHES "x86_64" )
    set (SUBLIBDIRSUFFIX "linux64")
  else()
    set(SUBLIBDIRSUFFIX "linux")
  endif()
  message(STATUS "${OPENFRAMEWORKS_ADDONS_DIRECTORY}/${ADDON_NAME}/libs/oscpack/lib/${SUBLIBDIRSUFFIX}")
  find_library(OSCPACK oscpack PATHS ${OPENFRAMEWORKS_ADDONS_DIRECTORY}/${ADDON_NAME}/libs/oscpack/lib/${SUBLIBDIRSUFFIX})
endif()

if ( OSCPACK-NOTFOUND )
  message (FATAL_ERROR "Cannot find ocspack library ${OSCPACK}")
else()
  message (STATUS "Found oscpack library: ${OSCPACK}")
  set (OPENFRAMEWORKS_LIBRARIES ${OPENFRAMEWORKS_LIBRARIES} ${OSCPACK})
endif()

find_path(OSCPACK_INCPATH NAMES OscTypes.h PATHS ${OPENFRAMEWORKS_ADDONS_DIRECTORY}/${ADDON_NAME}/libs/oscpack/include/osc
                                                             ${OPENFRAMEWORKS_INCLUDE_PREFIX})

find_path(IP_INCPATH NAMES UdpSocket.h PATHS ${OPENFRAMEWORKS_ADDONS_DIRECTORY}/${ADDON_NAME}/libs/oscpack/include/ip
                                                             ${OPENFRAMEWORKS_INCLUDE_PREFIX})

if ( DEFINED OSCPACK_INCPATH )
  set(OPENFRAMEWORKS_INCLUDES ${OPENFRAMEWORKS_INCLUDES} ${OSCPACK_INCPATH})
  message (STATUS "Found oscpack includes: ${OSCPACK_INCPATH}")
else()
  message (FATAL_ERROR "Cannot find oscpack includes")
endif()

if (DEFINED  IP_INCPATH)
  set(OPENFRAMEWORKS_INCLUDES ${OPENFRAMEWORKS_INCLUDES} ${IP_INCPATH})
  message( STATUS "Found ip includes: ${IP_INCPATH}")
else()
  message(FATAL_ERROR "Cannot find ip includes")
endif()

file( GLOB_RECURSE ADDON_SRC ${OPENFRAMEWORKS_ADDONS_DIRECTORY}/${ADDON_NAME}/src/*.cpp)
set(ADDONS_SOURCES ${ADDONS_SOURCES} ${ADDON_SRC})
set(ADDONS_INCLUDE_DIRS ${ADDONS_INCLUDE_DIRS} ${OPENFRAMEWORKS_ADDONS_DIRECTORY}/${ADDON_NAME}/src/)
