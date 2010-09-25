# - Find openFrameworks dependencies 
#
# Find openframeworks dependencies on Linux and OS X 
# This code set the following variables
# OPENFRAMEWORKS_LIBRARIES 
# OPENFRAMEWORKS_INCLUDES 

# Check the following dependencies 
cmake_minimum_required(VERSION 2.6)

cmake_policy(SET CMP0011 NEW)

#IF("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
#SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}"  )
#SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG}"  )
#ENDIF("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
 
# set (OPENFRAMEWORKS_PREFIX "/opt/openFrameworks")
# Set default libraries search path
if ( DEFINED OPENFRAMEWORKS_SEARCH_PREFIX ) 
    foreach(PFX ${OPENFRAMEWORKS_SEARCH_PREFIX})
        MESSAGE ( STATUS "Searching libraries in ${PFX}" )
        set (OPENFRAMEWORKS_LIBRARY_PREFIX ${OPENFRAMEWORKS_LIBRARY_PREFIX} ${PFX}/lib)
        set (OPENFRAMEWORKS_INCLUDE_PREFIX ${OPENFRAMEWORKS_INCLUDE_PREFIX} ${PFX}/include)
    endforeach()
endif( DEFINED OPENFRAMEWORKS_SEARCH_PREFIX ) 

if ( OPENFRAMEWORKS_TRY_USE_LOCAL_LIBRARIES AND DEFINED OPENFRAMEWORKS_LOCAL_SEARCH_PREFIX)
  if ( APPLE )
    set (SUBLIBDIRSUFFIX "osx")
  endif(APPLE)
  if ( CMAKE_SYSTEM_NAME MATCHES "Linux" )
    execute_process(COMMAND uname -m OUTPUT_VARIABLE ARCH)
    if ( ${ARCH} MATCHES "x86_64" )
      set (SUBLIBDIRSUFFIX "linux64")
    else()
      set(SUBLIBDIRSUFFIX "linux")
    endif()
    
  endif()
  foreach(PFX ${OPENFRAMEWORKS_LOCAL_SEARCH_PREFIX})
    message( STATUS "Searching local library in ${PFX}/lib/${SUBLIBDIRSUFFIX}" )
    set (OPENFRAMEWORKS_LIBRARY_PREFIX ${OPENFRAMEWORKS_LIBRARY_PREFIX} ${PFX}/lib/${SUBLIBDIRSUFFIX})
    set (OPENFRAMEWORKS_INCLUDE_PREFIX ${OPENFRAMEWORKS_INCLUDE_PREFIX} ${PFX}/include)
    set (CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${OPENFRAMEWORKS_LIBRARY_PREFIX} ${PFX}/lib/${SUBLIBDIRSUFFIX})
  endforeach()
endif()

if ( APPLE )
    set (CMAKE_FIND_LIBRARY_PREFIXES "" "lib")
    set (CMAKE_FIND_LIBRARY_SUFFIXES ".dylib" ".a" ".lib", ".so")
endif ()

MACRO (find_lib_inc varname libname incfile)
    string(TOLOWER ${libname} libnamelow)
     
    find_library(${varname}_LIBRARIES ${libname} PATHS ${OPENFRAMEWORKS_LIBRARY_PREFIX})

    if (NOT ${varname}_LIBRARIES})
        find_library(${varname}_LIBRARIES ${libnamelow} PATHS ${OPENFRAMEWORKS_LIBRARY_PREFIX})
    endif()

    find_path(${varname}_INCLUDES NAMES ${incfile} 
        PATHS ${OPENFRAMEWORKS_INCLUDE_PREFIX}
            /usr/local/X11R6/include
            /usr/local/X11/include
            /usr/X11/include
            /sw/include
            /opt/local/include
            /usr/freeware/include   
            /usr/include/${libnamelow}
            /usr/local/include/${libnamelow}
            /opt/local/include/${libnamelow}
            /usr/include/${libname}
            /usr/local/include/${libname}
            /opt/local/include/${libname}
    )
    if ( NOT ${varname}_LIBRARIES ) 
        message (FATAL_ERROR "Cannot find ${libname} library")
    else()
        message ( STATUS "Found ${libname} library: "${${varname}_LIBRARIES})
        set ( OPENFRAMEWORKS_LIBRARIES ${OPENFRAMEWORKS_LIBRARIES} ${${varname}_LIBRARIES} )
    endif()
    if ( NOT ${varname}_INCLUDES ) 
        message (FATAL_ERROR "Cannot find ${incfile}" )
    else ()
        set ( OPENFRAMEWORKS_INCLUDES ${OPENFRAMEWORKS_INCLUDES} ${${varname}_INCLUDES} )
        message ( STATUS "Found ${incfile} (${${varname}_INCLUDES})")
    endif()
ENDMACRO(find_lib_inc)
    
find_lib_inc(POCO PocoFoundation "Poco/Poco.h")
find_lib_inc(GLEE GLee GLee.h)
find_lib_inc(FMODEX fmodex "fmod.h")
find_lib_inc(RTAUDIO RtAudio RtAudio.h)
find_lib_inc(FREEIMAGE FreeImage FreeImage.h)


# freetype 
include (FindFreetype)
if (NOT FREETYPE_FOUND)
    message( FATAL_ERROR "Cannot find freetype 2 library")
else()
    message( STATUS "Found Freetype2" )
endif()

if ( APPLE ) 
# find opengl framework
    FIND_PATH( GLU_INCLUDES 
            glu.h
            /System/Library/Frameworks/OpenGL.framework/Versions/A/Headers)

    if (GLU_INCLUDES)
        message(STATUS "Found GLU")
        set ( GLU_LIBRARY "-framework OpenGL")
    else()
        message(FATAL_ERROR "Cannot Find GLU")
    endif()
else()
    # find GLU
    include(FindGLU)
    if ( GLU_LIBRARY )
        message (STATUS "Found GLU")
    else ( GLU_LIBRARY )
        message ( WARNING "Cannot find GLU")
    endif (GLU_LIBRARY)
endif()
        
# find GLUT 
include(FindGLUT)
if ( GLUT_LIBRARIES )
    message ( STATUS "Found GLUT ${GLUT_LIBRARIES}")
else ( GLUT_LIBRARIES )
    message ( FATAL_ERROR  "Cannot find GLUT")
endif (GLUT_LIBRARIES)


if ( APPLE ) 
    #include(FindQuickTime)
#    FIND_PATH( QUICKTIME_INCLUDES 
#            QuickTime.h
#            /System/Library/Frameworks/QuickTime.framework/Versions/A/Headers)
#    if ( QUICKTIME_INCLUDES )
#        message ( STATUS "Found QuickTime framework")
#        set (QUICKTIME_LIBRARIES "-framework QuickTime")
#    else()
#        message (WARNING "Cannot find quicktime")
#    endif()
    
    find_library(QUICKTIME_LIBRARIES QuickTime)
    if ( QUICKTIME_LIBRARIES )
        message (STATUS "Found quicktime")
    else()
        message( FATAL_ERROR "Cannot find QuickTime")
    endif()


    ### COREAUDIO
    FIND_LIBRARY(COREAUDIO_LIBRARY CoreAudio)
    if ( COREAUDIO_LIBRARY)
        message (STATUS "Found CoreAudio library")
    else ()
        message (FATAL_ERROR "Cannot find CoreAudio")
    endif()
    ### CARBON                                                                                                                    
    FIND_LIBRARY(CARBON_LIBRARY Carbon)
    if ( CARBON_LIBRARY ) 
        message(STATUS "Found Carbon library")
    else()
        message( FATAL_ERROR "Cannot find Carbon")
    endif ()
    ### COREFOUNDATION
    FIND_LIBRARY(COREFOUNDATION_LIBRARY CoreFoundation )
    if (COREFOUNDATION_LIBRARY)
        message(STATUS "Found CoreFoundation library")
    else()
        message(FATAL_ERROR "Cannot find CoreFoundation")
    endif()

endif()

if ( CMAKE_SYSTEM_NAME MATCHES "Linux" ) 
    include(FindPkgConfig)
    if ( OPENFRAMEWORKS_USE_UNICAP_VIDCAP )
      pkg_check_modules(UNICAP libunicap)
      if ( UNICAP_LIBRARIES )
          message ( STATUS "Found unicap")
          set ( OPENFRAMEWORKS_LIBRARIES ${OPENFRAMEWORKS_LIBRARIES} ${UNICAP_LIBRARIES} )
          set ( OPENFRAMEWORKS_INCLUDES ${OPENFRAMEWORKS_INCLUDES} ${UNICAP_INCLUDE_DIRS} ) 
      else()
          message ( FATAL_ERROR "Cannot find unicap")
      endif()
    endif()

    pkg_check_modules(GST gstreamer-app-0.10)
    if ( GST_LIBRARIES ) 
        message ( STATUS "Found Gstreamer")
        set (  OPENFRAMEWORKS_LIBRARIES ${OPENFRAMEWORKS_LIBRARIES}  ${GST_LIBRARIES} )
        set ( OPENFRAMEWORKS_INCLUDES ${OPENFRAMEWORKS_INCLUDES} ${GST_INCLUDE_DIRS} ) 
    else()
        message( FATAL_ERROR "Cannot find Gstreamer-app")
    endif()
    
    pkg_check_modules(GSTVIDEO gstreamer-video-0.10)
    if ( GSTVIDEO_LIBRARIES ) 
        message ( STATUS "Found Gstreamer-video")
        set (  OPENFRAMEWORKS_LIBRARIES ${OPENFRAMEWORKS_LIBRARIES}  ${GSTVIDEO_LIBRARIES} )
        set ( OPENFRAMEWORKS_INCLUDES ${OPENFRAMEWORKS_INCLUDES} ${GSTVIDEO_INCLUDE_DIRS} ) 
    else()
        message( FATAL_ERROR "Cannot find Gstreamer-video")
    endif()


    pkg_check_modules(AVFORMAT libavformat)
    if ( AVFORMAT_LIBRARIES)
        message ( STATUS "Found libavformat  ${AVFORMAT_LIBRARIES} ${AVFORMAT_LDFLAGS} ${AVFORMAT_LDFLAGS_OTHER}")
        set ( OPENFRAMEWORKS_LIBRARIES ${OPENFRAMEWORKS_LIBRARIES} ${AVFORMAT_LIBRARIES})
        set ( OPENFRAMEWORKS_INCLUDES ${OPENFRAMEWORKS_INCLUDES} ${AVFORMAT_INCLUDE_DIRS})
    else()
        message ( FATAL_ERROR "Cannot find libavformat" ) 
    endif()

    pkg_check_modules(SWSCALE libswscale)
    if ( SWSCALE_LIBRARIES)
        message ( STATUS "Found libswscale")
        set ( OPENFRAMEWORKS_LIBRARIES ${OPENFRAMEWORKS_LIBRARIES} ${SWSCALE_LIBRARIES})
        set ( OPENFRAMEWORKS_INCLUDES ${OPENFRAMEWORKS_INCLUDES} ${SWSCALE_INCLUDE_DIRS})
    else()
        message ( FATAL_ERROR "Cannot find libswscale")
    endif()
   
    ### LIBASOUND 
    find_library(ASOUND asound)
    if (ASOUND)
        set ( OPENFRAMEWORKS_LIBRARIES ${OPENFRAMEWORKS_LIBRARIES} ${ASOUND})
        message ( STATUS "Found asound ${ASOUND}")
    else()
        message (FATAL_ERROR "Cannot find asound library")
    endif()

### LIBRAW1394
    find_library(RAW1394 raw1394)
    if (RAW1394)
        set ( OPENFRAMEWORKS_LIBRARIES ${OPENFRAMEWORKS_LIBRARIES} ${RAW1394})
        message ( STATUS "Found raw1394")
    else()
        message (FATAL_ERROR "Cannot find raw1394 library")
    endif()


    #find_lib_inc(UNICAP unicap unicap.h)
endif()
SET(OPENFRAMEWORKS_LIBRARIES
    ${OPENFRAMEWORKS_LIBRARIES}
            ${FREETYPE_LIBRARIES}
            ${GLUT_LIBRARIES}
            ${GLU_LIBRARY}
      )


set(OPENFRAMEWORKS_INCLUDES ${OPENFRAMEWORKS_INCLUDES}
    ${FREETYPE_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}/freetype
    ${OPENFRAMEWORKS_INCLUDE_PREFIX})

if ( APPLE ) 
    set (OPENFRAMEWORKS_LIBRARIES ${OPENFRAMEWORKS_LIBRARIES}
    ${COREAUDIO_LIBRARY}
    ${COREFOUNDATION_LIBRARY}
    ${CARBON_LIBRARY}
    ${QUICKTIME_LIBRARIES})
endif()

if ( CMAKE_SYSTEM_NAME MATCHES "Linux" )
    set ( OPENFRAMEWORKS_LIBRARIES ${OPENFRAMEWORKS_LIBRARIES}
    ${UNICAP_LIBRARIES})

endif()
    

#message("${OPENFRAMEWORKS_LIBRARIES}")
#message("${OPENFRAMEWORKS_INCLUDES}")
