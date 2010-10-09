# - Find openFrameworks dependencies 
#
# Find openframeworks dependencies on Linux and OS X 
# This code set the following variables
# Configuration variables:
#
# Search:
# -includes
# -libraries
# With the following parameters :
# - library_name
# - include_files
# - prefixes
#
# INPUT_VARIABLES :
#   FOR LINUX:
#     OPENFRAMEWORKS_VIDCAP_METHOD
#
#   OPENFRAMEWORKS_LIBRARIES_SEARCH_PATHS 
#   OPENFRAMEWORKS_INCLUDES_SEARCH_PATHS
# 
# OUTPUT VARIABLES:
#   OPENFRAMEWORKS_INCLUDES
#   OPENFRAMEWORKS_LIBRARIES
#

#######################################################
#######################################################
#--- Common libraries
#######################################################
#######################################################

message(STATUS "Looking for libraries in : ${OPENFRAMEWORKS_LIBRARIES_SEARCH_PATHS}")
message(STATUS "Logging for includes in  : ${OPENFRAMEWORKS_INCLUDES_SEARCH_PATHS}")

if ( APPLE )
    set (CMAKE_FIND_LIBRARY_PREFIXES "" "lib")
    set (CMAKE_FIND_LIBRARY_SUFFIXES ".dylib" ".a" ".lib", ".so")
    #set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-m32")
    set (CMAKE_OSX_ARCHITECTURES "i386")
elseif(LINUX)
    message(STATUS "RUNNING LINUX")
    set (CMAKE_FIND_LIBRARY_PREFIXES "lib" "")
    set (CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".a")
endif ()

set (CMAKE_LIBRARY_PATH ${OPENFRAMEWORKS_LIBRARIES_SEARCH_PATHS} /usr/lib /usr/local/lib )
set (CMAKE_INCLUDE_PATH ${OPENFRAMEWORKS_INCLUDES_SEARCH_PATHS} /usr/include /usr/local/include )


#######################################################
#
# Find include file and fail if not found 
# 
macro(of_find_include includefile)
  find_path(${includefile}_INCPATH NAMES ${includefile} PATHS ${OPENFRAMEWORKS_INCLUDES_SEARCH_PATHS})
  if ( ${${includefile}_INCPATH} STREQUAL "${includefile}_INCPATH-NOTFOUND")
    message(FATAL_ERROR "Cannot find ${includefile}")
  else()
    message(STATUS "Found ${includefile} in ${${includefile}_INCPATH}")
  endif()
  set (OPENFRAMEWORKS_INCLUDES ${OPENFRAMEWORKS_INCLUDES} ${${includefile}_INCPATH})
endmacro(of_find_include)


#######################################################
####
# Find library and fail if not found 
#
macro(of_find_library libname)
  string(TOLOWER ${libname} libnamelow)
     
  find_library(${libname}_LIB NAMES ${libname} PATHS ${OPENFRAMEWORKS_LIBRARIES_SEARCH_PATHS})
  if (NOT ${libname}_LIB)
    find_library(${libname}_LIB NAMES ${libnamelow} PATHS ${OPENFRAMEWORKS_LIBRARIES_SEARCH_PATHS})
  endif()
  
  if (NOT ${libname}_LIB)
    message(FATAL_ERROR "Cannot find '${libname}' library")
  else()
    message(STATUS "Found '${libname}' library in ${${libname}_LIB}")
    set(OPENFRAMEWORKS_LIBRARIES ${OPENFRAMEWORKS_LIBRARIES} ${${libname}_LIB})
  endif()
endmacro()


#######################################################
####
# Find library and include 
#
macro(of_find_lib_and_include libname incname)
  of_find_library(${libname})
  of_find_include(${incname})
endmacro(of_find_lib_and_include)


#######################################################
###
# Use pkgconfig to find module and fail if not found 
#
macro(of_find_pkgconfig_lib pkgname)
  include(FindPkgConfig)
  pkg_check_modules(${pkgname}_pkg REQUIRED ${pkgname})
  if ( DEFINED ${pkgname}_pkg_FOUND )
    set (OPENFRAMEWORKS_LIBRARIES ${OPENFRAMEWORKS_LIBRARIES} ${${pkgname}_pkg_LIBRARIES}
                                  ${${pkgname}_pkg_LDFLAGS}  ${${pkgname}_pkg_LDFLAGS_OTHER})
    set (OPENFRAMEWORKS_INCLUDES ${OPENFRAMEWORKS_INCLUDES} ${${pkgname}_pkg_INCLUDE_DIRS})
    set (OPENFRAMEWORKS_CXX_FLAGS ${OPENFRAMEWORKS_CXX_FLAGS} ${${pkgname}_pkg_CFLAGS})
  else()
    message( FATAL_ERROR "Cannot find pkgconfig file for ${pkgconfig}. Check your PKG_CONFIG_PATH environment variable.")
  endif()
endmacro(of_find_pkgconfig_lib)


#######################################################
####
# Find glu library
#
macro(of_find_glu)
  include(FindGLU)
  message(STATUS "GLUT ${GLU_LIBRARY}")
  if ( GLU_LIBRARY )
    message (STATUS "Found GLU")
    set(OPENFRAMEWORKS_INCLUDES ${OPENFRAMEWORKS_INCLUDES} ${GLU_INCLUDE_PATH})
    set(OPENFRAMEWORKS_LIBRARIES ${OPENFRAMEWORKS_LIBRARIES} ${GLU_LIBRARY})
  else ( GLU_LIBRARY )
    message ( WARNING "Cannot find GLU")
  endif (GLU_LIBRARY)
endmacro(of_find_glu)


#######################################################
####
# Find glut library
#
macro(of_find_glut)
  # find GLUT 
  include(FindGLUT)
  if ( GLUT_FOUND )
      message ( STATUS "Found GLUT ${GLUT_LIBRARIES}")
      set(OPENFRAMEWORKS_INCLUDES ${OPENFRAMEWORKS_INCLUDES} ${GLUT_INCLUDE_DIR})
      set(OPENFRAMEWORKS_LIBRARIES ${OPENFRAMEWORKS_LIBRARIES} ${GLUT_LIBRARIES})
  else ( GLUT_FOUND )
      message ( FATAL_ERROR  "Cannot find GLUT")
  endif (GLUT_FOUND)
endmacro(of_find_glut)

macro(of_find_freetype)
  include (FindFreetype)
  if (NOT FREETYPE_FOUND)
      message( FATAL_ERROR "Cannot find freetype 2 library")
  else()
      message( STATUS "Found Freetype2" )
      set(OPENFRAMEWORKS_INCLUDES ${OPENFRAMEWORKS_INCLUDES} ${FREETYPE_INCLUDE_DIRS})
      set(OPENFRAMEWORKS_LIBRARIES ${OPENFRAMEWORKS_LIBRARIES} ${FREETYPE_LIBRARIES})
  endif()
endmacro()



   
#######################################################
#######################################################
#######################################################
#######################################################

of_find_lib_and_include(fmodex "fmod.h")
of_find_lib_and_include(PocoFoundation "Poco/Poco.h")
of_find_lib_and_include(GLee GLee.h)
of_find_lib_and_include(RtAudio RtAudio.h)
of_find_lib_and_include(FreeImage FreeImage.h)


if ( LINUX ) 
  of_find_library(asound)
  of_find_library(raw1394)
  of_find_lib_and_include(avcodec avcodec.h)
  if ( ${OPENFRAMEWORKS_VIDCAP_METHOD} MATCHES "unicap" )
    of_find_pkgconfig_lib(unicap)
  elseif( ${OPENFRAMEWORKS_VIDCAP_METHOD} MATCHES "gstreamer" )
    of_find_pkgconfig_lib(gstreamer-app-0.10)
    of_find_pkgconfig_lib(gstreamer-video-0.10)
  endif()
  of_find_pkgconfig_lib(libudev)
  of_find_pkgconfig_lib(gstreamer-app-0.10)
  of_find_pkgconfig_lib(gstreamer-video-0.10)
  of_find_pkgconfig_lib(libavformat)
  of_find_pkgconfig_lib(libswscale)
endif( LINUX )


if ( APPLE )
    include(FindQuickTime)
    if ( ${QUICKTIME_FOUND} MATCHES "YES" )
      message(STATUS "Found QuickTime library")
      message(STATUS "   QuickTime library: ${QUICKTIME_LIBRARY}")
      message(STATUS "   QuickTime includes: ${QUICKTIME_INCLUDE_DIR}")
      set(OPENFRAMEWORKS_INCLUDES ${OPENFRAMEWORKS_INCLUDES} ${QUICKTIME_INCLUDE_DIR})
      set(OPENFRAMEWORKS_LIBRARIES ${OPENFRAMEWORKS_LIBRARIES} ${QUICKTIME_LIBRARY})
    else()
      message(FATAL_ERROR "Cannot find quicktime library")
    endif()
    of_find_library("coreaudio")
    of_find_library("opengl")
    of_find_library("carbon")
endif(APPLE)

of_find_freetype()

of_find_glu()
of_find_glut()


