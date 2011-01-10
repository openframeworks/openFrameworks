nobase_include_HEADERS= \
			openFrameworks/app/ofAppBaseWindow.h \
			openFrameworks/app/ofAppGlutWindow.h \
			openFrameworks/app/ofAppRunner.h \
			openFrameworks/app/ofBaseApp.h \
			openFrameworks/communication/ofArduino.h \
			openFrameworks/communication/ofSerial.h \
			openFrameworks/events/ofEventUtils.h \
			openFrameworks/events/ofEvents.h \
			openFrameworks/graphics/ofBitmapFont.h \
			openFrameworks/graphics/ofGraphics.h \
			openFrameworks/graphics/ofImage.h \
			openFrameworks/graphics/ofTexture.h \
			openFrameworks/graphics/ofTrueTypeFont.h \
			openFrameworks/ofMain.h \
			openFrameworks/sound/ofSoundPlayer.h \
			openFrameworks/sound/ofSoundStream.h \
			openFrameworks/utils/ofConstants.h \
			openFrameworks/utils/ofMath.h \
			openFrameworks/utils/ofNoise.h \
			openFrameworks/utils/ofTypes.h \
			openFrameworks/utils/ofUtils.h \
			openFrameworks/video/ofBaseVideoGrabber.h \
			openFrameworks/video/ofBaseVideoPlayer.h \
			openFrameworks/video/ofDirectShowGrabber.h \
			openFrameworks/video/ofGStreamerGrabber.h \
			openFrameworks/video/ofGStreamerPlayer.h \
			openFrameworks/video/ofGstUtils.h \
			openFrameworks/video/ofQtUtils.h \
			openFrameworks/video/ofQuickTimeGrabber.h \
			openFrameworks/video/ofQuickTimePlayer.h \
			openFrameworks/video/ofUnicapGrabber.h \
			openFrameworks/video/ofVideoGrabber.h \
			openFrameworks/video/ofVideoPlayer.h

lib_LTLIBRARIES=libopenFrameworks.la
libopenFrameworks_la_CXXFLAGS= \
			       ${gstreamer_CFLAGS} \
			       ${freetype_CFLAGS}
libopenFrameworks_la_LDFLAGS= \
			      -version-info ${OFX_LIB_VERSION} \
			      ${gstreamer_LDFLAGS} \
			      ${freetype_LDFLAGS}
libopenFrameworks_la_SOURCES= \
			      openFrameworks/app/ofAppGlutWindow.cpp \
			      openFrameworks/app/ofAppRunner.cpp \
			      openFrameworks/communication/ofArduino.cpp \
			      openFrameworks/communication/ofSerial.cpp \
			      openFrameworks/events/ofEvents.cpp \
			      openFrameworks/graphics/ofBitmapFont.cpp \
			      openFrameworks/graphics/ofGraphics.cpp \
			      openFrameworks/graphics/ofImage.cpp \
			      openFrameworks/graphics/ofTexture.cpp \
			      openFrameworks/graphics/ofTrueTypeFont.cpp \
			      openFrameworks/sound/ofSoundStream.cpp \
			      openFrameworks/utils/ofMath.cpp \
			      openFrameworks/utils/ofUtils.cpp \
			      openFrameworks/video/ofBaseVideoGrabber.cpp \
			      openFrameworks/video/ofBaseVideoPlayer.cpp \
			      openFrameworks/video/ofDirectShowGrabber.cpp \
			      openFrameworks/video/ofGStreamerGrabber.cpp \
			      openFrameworks/video/ofGStreamerPlayer.cpp \
			      openFrameworks/video/ofGstUtils.cpp \
			      openFrameworks/video/ofQtUtils.cpp \
			      openFrameworks/video/ofUnicapGrabber.cpp \
			      openFrameworks/video/ofVideoGrabber.cpp \
			      openFrameworks/video/ofVideoPlayer.cpp
