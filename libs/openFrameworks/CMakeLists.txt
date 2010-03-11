# vim:set ft=cmake:syn on
cmake_minimum_required(VERSION 2.6)
project(openFrameworks)
set(OPENFRAMEWORKS_VERSION "0.6")

set(OPENFRAMEWORKS_SEARCH_PREFIX /usr/ /usr/local/ /opt/local CACHE FILEPATH "Openframeworks prefix search")

set(OPENFRAMEWORKS_INSTALL_PREFIX "/usr/local" CACHE FILEPATH "Openframeworks install prefix")
set(OPENFRAMEWORKS_PREFIX ${OPENFRAMEWORKS_INSTALL_PREFIX} CACHE FILEPATH "Openframeworks additional prefix to check for dependencies (rtaudio, fmodex ...)")
if ( APPLE ) 
    set ( CMAKE_OSX_ARCHITECTURES "i386" CACHE STRING "Architecture" )
    set ( OPENFRAMEWORKS_BUILD_FRAMEWORK ON CACHE BOOL "Compile openFrameworks as a standard mac os x framework")
    set ( OPENFRAMEWORKS_FRAMEWORKS_DIR CACHE FILEPATH "Openframework framework destination")
endif ()

if ( CMAKE_SYSTEM_NAME MATCHES "Linux" )
    set(OPENFRAMEWORKS_USE_GSTREAMER_VIDCAP ON CACHE BOOL "Use Gstreamner for VideoGrabber")
    set(OPENFRAMEWORKS_USE_UNICAP_VIDCAP OFF CACHE BOOL "Use Unicap for VideoGrabber")
    set(OPENFRAMEWORKS_USE_V4L_VIDCAP OFF CACHE BOOL "Use v4l api for VideoGrabber")

    set ( VIDCAP_DEFINE "" )
    if (OPENFRAMEWORKS_USE_V4L_VIDCAP )
        set( VIDCAP_DEFINE -DOF_CAPTURE_V4L )
    endif ()
    if ( OPENFRAMEWORKS_USE_UNICAP_VIDCAP ) 
        set ( VIDCAP_DEFINE -DOF_CAPTURE_UNICAP ) 
    endif ()
    if ( OPENFRAMEWORKS_USE_GSTREAMER_VIDCAP ) 
        set ( VIDCAP_DEFINE -DOF_CAPTURE_GSTREAMER )
    endif ()
    add_definitions( ${VIDCAP_DEFINE} )
    message("Grabber config ${VIDCAP_DEFINE}")
endif() 

get_filename_component(OPENFRAMEWORKS_INSTALL_PREFIX ${OPENFRAMEWORKS_INSTALL_PREFIX} ABSOLUTE)
set(OPENFRAMEWORKS_LIB_DIR "${OPENFRAMEWORKS_INSTALL_PREFIX}/lib")
set(OPENFRAMEWORKS_INCLUDE_DIR "${OPENFRAMEWORKS_INSTALL_PREFIX}/include")
set(OPENFRAMEWORKS_PKGCONFIG_DIR "${OPENFRAMEWORKS_INSTALL_PREFIX}/lib/pkgconfig" CACHE FILEPATH "Pkgconfig modules path (PKG_CONFIG_PATH)")

# set (OPENFRAMEWORKS_PREFIX "/opt/openFrameworks")

include(openFrameworks.cmake)
if ( OPENFRAMEWORKS_USE_GSTREAMER_VIDCAP )
    include(FindPkgConfig)
    pkg_search_module(HAL  hal)
    if ( NOT HAL_LIBRARIES )
        message(FATAL_ERROR "You selected Gtreamer video capture API but cannot find HAL library")
    endif()
    
    pkg_search_module(UDEV libudev)
    if ( NOT UDEV_LIBRARIES )
        message(FATAL_ERROR "You selected Gtreamer video capture API but cannot find UDEV library")
    endif()
 
    set (OPENFRAMEWORKS_INCLUDES ${OPENFRAMEWORKS_INCLUDES} ${UDEV_INCLUDE_DIRS} ${HAL_INCLUDE_DIRS})
    set (OPENFRAMEWORKS_LIBRARIES ${HAL_LIBRARIES} ${UDEV_LIBRARIES} ${OPENFRAMEWORKS_LIBRARIES})
endif()

set (OFX_SOURCE_FILES 
            utils/ofUtils.cpp
            utils/ofMath.cpp
            communication/ofArduino.cpp
            communication/ofSerial.cpp
            graphics/ofBitmapFont.cpp
            graphics/ofGraphics.cpp
            graphics/ofImage.cpp
            graphics/ofTexture.cpp
            graphics/ofTrueTypeFont.cpp
            sound/ofSoundStream.cpp
            sound/ofSoundPlayer.cpp
            app/ofAppRunner.cpp
            app/ofAppGlutWindow.cpp
            video/ofVideoPlayer.cpp
            video/ofGstUtils.cpp
            video/ofUCUtils.cpp
            video/ofVideoGrabber.cpp
            video/ofQtUtils.cpp
            utils/ofNoise.h
            utils/ofMath.h
            utils/ofUtils.h
            utils/ofTypes.h
            utils/ofConstants.h
            events/ofEvents.h
            events/ofEventUtils.h
            communication/ofSerial.h
            communication/ofArduino.h
            ofMain.h
            graphics/ofImage.h
            graphics/ofTexture.h
            graphics/ofBitmapFont.h
            graphics/ofGraphics.h
            graphics/ofTrueTypeFont.h
            sound/ofSoundStream.h
            sound/ofSoundPlayer.h
            app/ofAppBaseWindow.h
            app/ofAppRunner.h
            app/ofAppGlutWindow.h
            app/ofBaseApp.h
            video/ofVideoPlayer.h
            video/ofUCUtils.h
            video/ofQtUtils.h
            video/ofGstUtils.h
            video/ofVideoGrabber.h
)



set(ofincludes       . 
                     app
                     communication
                     events
                     graphics
                     sound
                     utils
                     video)

set(ofincludes_files 
            utils/ofNoise.h
            utils/ofMath.h
            utils/ofUtils.h
            utils/ofTypes.h
            utils/ofConstants.h
            events/ofEvents.h
            events/ofEventUtils.h
            communication/ofSerial.h
            communication/ofArduino.h
            ofMain.h
            graphics/ofImage.h
            graphics/ofTexture.h
            graphics/ofBitmapFont.h
            graphics/ofGraphics.h
            graphics/ofTrueTypeFont.h
            sound/ofSoundStream.h
            sound/ofSoundPlayer.h
            app/ofAppBaseWindow.h
            app/ofAppRunner.h
            app/ofAppGlutWindow.h
            app/ofBaseApp.h
            video/ofVideoPlayer.h
            video/ofUCUtils.h
            video/ofQtUtils.h
            video/ofGstUtils.h
            video/ofVideoGrabber.h)


include_directories( ${ofincludes} )
include_directories(${OPENFRAMEWORKS_INCLUDES})


##################################################################
## Configure library generation
add_library( openFrameworksShared SHARED ${OFX_SOURCE_FILES})
#add_library( openFrameworksStatic ${OFX_SOURCE_FILES})
#set_target_properties (openFrameworksStatic 
#                        PROPERTIES OUTPUT_NAME openFrameworks )

set_target_properties (openFrameworksShared 
                        PROPERTIES OUTPUT_NAME openFrameworks )

target_link_libraries(openFrameworksShared ${OPENFRAMEWORKS_LIBRARIES})
##################################################################
## Setup pkg-config file (openFrameworks.pc 
##################################################################

# other way to expand include dirs ?
foreach (dir ${OPENFRAMEWORKS_INCLUDES})
    set(inccmd "${inccmd} -I${dir}")    
endforeach()
set (inccmd "-pthread ${inccmd} -I${OPENFRAMEWORKS_INCLUDE_DIR}/openFrameworks" )

#foreach(dir ${ofincludes})
#    set(inccmd "${inccmd} -I${OPENFRAMEWORKS_INSTALL_PREFIX}/${dir}")
#endforeach()
set ( prefix ${OPENFRAMEWORKS_INSTALL_PREFIX})
set ( libdir ${OPENFRAMEWORKS_LIB_DIR})
set ( includedir ${OPENFRAMEWORKS_INCLUDE_DIR}/openFrameworks )
# set 
set(flags "${inccmd}")
if ( APPLE ) 
    set(libs  "-framework OpenGL")
endif ()

configure_file("openFrameworks.pc.in" "openFrameworks.pc")


#################################################################
## Install things

# install library
#target_link_libraries(openFrameworksStatic ${OPENFRAMEWORKS_LIBRARIES})
if ( APPLE )
    if (OPENFRAMEWORKS_BUILD_FRAMEWORK)
        set_target_properties(openFrameworksShared
                PROPERTIES FRAMEWORK TRUE
                 OUTPUT_NAME openFrameworks 
                 PUBLIC_HEADER "${ofincludes_files}"
                 INSTALL_NAME_DIR "${OPENFRAMEWORKS_FRAMEWORKS_DIR}"#LIBRARY             DESTINATION  "./lib"
                )
        
        install(TARGETS openFrameworksShared FRAMEWORK DESTINATION ${OPENFRAMEWORKS_FRAMEWORKS_DIR})
    endif()
endif()

if(NOT OPENFRAMEWORKS_BUILD_FRAMEWORK )
    install(FILES ${ofincludes_files}
        DESTINATION "${OPENFRAMEWORKS_INCLUDE_DIR}/openFrameworks")
    install(FILES openFrameworks.pc
        DESTINATION ${OPENFRAMEWORKS_PKGCONFIG_DIR})
    install(TARGETS openFrameworksShared 
        DESTINATION ${OPENFRAMEWORKS_LIB_DIR})
else()
    #install(TARGETS openFrameworksShared 
    #    DESTINATION ${OPENFRAMEWORKS_FRAMEWORKS_DIR})
endif()







###################################################################
message(STATUS "====================== Configuration for OpenFrameworks ====================================")
message(STATUS "")
message(STATUS " Build on ${CMAKE_SYSTEM_NAME}")
message(STATUS "")
message(STATUS "    Compiler:                  ${CMAKE_COMPILER}")
message(STATUS "    C++ flags (Release):       ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "    C++ flags (Debug):         ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
if(WIN32)
message(STATUS "    Linker flags (Release):    ${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
message(STATUS "    Linker flags (Debug):      ${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
else()
message(STATUS "    Linker flags (Release):    ${CMAKE_SHARED_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS_RELEASE}")
message(STATUS "    Linker flags (Debug):      ${CMAKE_SHARED_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")
endif()
message(STATUS "    Linking with the following libraries: ")
message(STATUS "")
message(STATUS "        ${OPENFRAMEWORKS_LIBRARIES}")
message(STATUS "")
message(STATUS "")
message(STATUS "    Compiling with the following includes paths" )
message(STATUS "")
foreach(inc ${OPENFRAMEWORKS_INCLUDES})
message(STATUS "        - ${inc}")
endforeach()
message(STATUS "")
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
message(STATUS "")
message(STATUS " Configuration used for video capture : ${VIDCAP_DEFINE}")
message(STATUS "")
endif()
message(STATUS "")
message(STATUS "============================================================================================ Enjoy !")
message(STATUS "")
message(STATUS "")
#target_link_libraries(openFrameworksStatic ${OPENFRAMEWORKS_LIBRARIES})
#if ( APPLE )
#    set_target_properties(openFrameworksShared
#            PROPERTIES    FRAMEWORK               TRUE
#            PROPERTIES OUTPUT_NAME openFrameworks 
#            LIBRARY             DESTINATION  "./lib"
#            # PUBLIC_HEADER           "${FLAT_HDRS}"
#            )
#    install(TARGETS openFrameworksShared
#            FRAMEWORK           DESTINATION framewk)
#
#endif()
