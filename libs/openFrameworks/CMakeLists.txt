# vim:set ft=cmake:syn on
cmake_minimum_required(VERSION 2.6)
project(openFrameworks)
set(OPENFRAMEWORKS_VERSION "0.6")

#### FIND DEPENDENCIES 
include(./../../cmake/OpenFrameworksConfig.cmake)
include(./../../cmake/OpenFrameworks.cmake)
##########################################################################

set(OPENFRAMEWORKS_LIBRARY_INSTALL_PREFIX "/usr/local/" CACHE STRING "OpenFrameworks library install prefix")

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
            video/ofQtUtils.h
            video/ofVideoGrabber.h
)
if ( LINUX ) 
  if(OPENFRAMEWORKS_VIDCAP_METHOD MATCHES "gstreamer")
    set (OFX_SOURCE_FILES ${OFX_SOURCE_FILES}
      video/ofGstUtils.cpp
      video/ofGstUtils.h )
# Unicap related files where removed 
#  elseif (OPENFRAMEWORKS_VIDCAP_METHOD MATCHES "unicap")
#    set (OFX_SOURCE_FILES ${OFX_SOURCE_FILES}
#        video/ofUCUtils.cpp
#        video/ofUCUtils.h )
  endif()

endif ( LINUX )


include_directories( . 
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
            video/ofQtUtils.h
            video/ofGstUtils.h
            video/ofVideoGrabber.h)


include_directories(${OPENFRAMEWORKS_INCLUDES})


add_library( openFrameworksStatic ${OFX_SOURCE_FILES})
set_target_properties (openFrameworksStatic 
                        PROPERTIES OUTPUT_NAME openFrameworks )
target_link_libraries(openFrameworksStatic ${OPENFRAMEWORKS_LIBRARIES})


install(FILES ${ofincludes_files}
    DESTINATION "${OPENFRAMEWORKS_LIBRARY_INSTALL_PREFIX}/include/openFrameworks")

install(TARGETS openFrameworksStatic 
    DESTINATION "${OPENFRAMEWORKS_LIBRARY_INSTALL_PREFIX}/lib/")



message(STATUS "OF INCLUDE ${OPENFRAMEWORKS_INCLUDES}")
message(STATUS "OF LIBRARIES ${OPENFRAMEWORKS_LIBRARIES}")
