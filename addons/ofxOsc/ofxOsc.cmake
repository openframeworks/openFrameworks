# This is a sample cmake file to add to newly added OF addons
# simply copy this file to addon directory and customize lib
# finding and include finding.
#
# An addon include cmake file *MUST* set the following cmake variables:
# 
# - ADDONS_SOURCES: list of addon source files
# - ADDONS_INCLUDE_DIRS: list of directories to add to addon include path
# - ADDONS_LIBRARIES: list of library the addon depend on and should be linked with
# 

get_filename_component(ADDON_ROOT ${CMAKE_CURRENT_LIST_FILE} PATH)
get_filename_component(ADDON_NAME ${ADDON_ROOT} NAME)

message(STATUS "-------------------------------- Configuring OF Addon '${ADDON_NAME}'")

#########################################################################
# Add here your cmake code to fill-in described variables
# 
# this populate ADDON_SRC with the list of source files in src/
# this should work in most cases

if (APPLE)
  find_library(OSCPACK oscpack PATHS ${ADDON_ROOT}/libs/oscpack/lib/osx)
endif(APPLE)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
execute_process(COMMAND uname -m OUTPUT_VARIABLE ARCH)
  if ( ${ARCH} MATCHES "x86_64" )
    set (SUBLIBDIRSUFFIX "linux64")
  else()
    set(SUBLIBDIRSUFFIX "linux")
  endif()
  message(STATUS "${ADDON_ROOT}/libs/oscpack/lib/${SUBLIBDIRSUFFIX}")
  find_library(OSCPACK oscpack PATHS ${ADDON_ROOT}/libs/oscpack/lib/${SUBLIBDIRSUFFIX})
endif()

if ( OSCPACK-NOTFOUND )
  message (FATAL_ERROR "Cannot find ocspack library ${OSCPACK}")
else()
  message (STATUS "Found oscpack library: ${OSCPACK}")
  set (ADDON_LIBRARIES ${ADDON_LIBRARIES} ${OSCPACK})
endif()

find_path(OSCPACK_INCPATH NAMES OscTypes.h PATHS ${ADDON_ROOT}/libs/oscpack/include/osc)
find_path(IP_INCPATH NAMES UdpSocket.h PATHS ${ADDON_ROOT}/libs/oscpack/include/ip)

if ( DEFINED OSCPACK_INCPATH )
  set(ADDON_INCLUDES ${ADDON_INCLUDES} ${OSCPACK_INCPATH})
  message (STATUS "Found oscpack includes: ${OSCPACK_INCPATH}")
else()
  message (FATAL_ERROR "Cannot find oscpack includes")
endif()

if (DEFINED  IP_INCPATH)
  set(ADDON_INCLUDES ${ADDON_INCLUDES} ${IP_INCPATH})
  message( STATUS "Found ip includes: ${IP_INCPATH}")
else()
  message(FATAL_ERROR "Cannot find ip includes")
endif()

message("_--- ${ADDON_INCLUDES}")
file( GLOB_RECURSE ADDON_SRC ${ADDON_ROOT}/src/*.cpp)
#########################################################################


set(ADDONS_SOURCES ${ADDONS_SOURCES} ${ADDON_SRC})
set(ADDONS_INCLUDE_DIRS ${ADDONS_INCLUDE_DIRS} ${ADDON_INCLUDES} "${ADDON_ROOT}/src/")
set(ADDONS_LIBRARIES ${ADDONS_LIBRARIES} ${ADDON_LIBRARIES})
