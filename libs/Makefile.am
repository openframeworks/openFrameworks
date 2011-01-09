AM_CPPFLAGS= -I${top_srcdir}/libs/openFrameworks \
	     -I${top_srcdir}/libs/openFrameworks/utils \
	     -I${top_srcdir}/libs/openFrameworks/events \
	     -I${top_srcdir}/libs/openFrameworks/graphics \
	     -I${top_srcdir}/libs/openFrameworks/app \
	     -I${top_srcdir}/libs/openFrameworks/video \
	     -I${top_srcdir}/libs/openFrameworks/communication \
	     -I${top_srcdir}/libs/openFrameworks/sound

include_HEADERS= \
			      libs/openFrameworks/app/ofAppBaseWindow.h \
			      libs/openFrameworks/app/ofAppGlutWindow.h \
			      libs/openFrameworks/app/ofAppRunner.h \
			      libs/openFrameworks/app/ofBaseApp.h \
			      libs/openFrameworks/communication/ofArduino.h \
			      libs/openFrameworks/communication/ofSerial.h \
			      libs/openFrameworks/events/ofEventUtils.h \
			      libs/openFrameworks/events/ofEvents.h \
			      libs/openFrameworks/graphics/ofBitmapFont.h \
			      libs/openFrameworks/graphics/ofGraphics.h \
			      libs/openFrameworks/graphics/ofImage.h \
			      libs/openFrameworks/graphics/ofTexture.h \
			      libs/openFrameworks/graphics/ofTrueTypeFont.h \
			      libs/openFrameworks/ofMain.h \
			      libs/openFrameworks/sound/ofSoundPlayer.h \
			      libs/openFrameworks/sound/ofSoundStream.h \
			      libs/openFrameworks/utils/ofConstants.h \
			      libs/openFrameworks/utils/ofMath.h \
			      libs/openFrameworks/utils/ofNoise.h \
			      libs/openFrameworks/utils/ofTypes.h \
			      libs/openFrameworks/utils/ofUtils.h \
			      libs/openFrameworks/video/ofBaseVideoGrabber.h \
			      libs/openFrameworks/video/ofBaseVideoPlayer.h \
			      libs/openFrameworks/video/ofDirectShowGrabber.h \
			      libs/openFrameworks/video/ofGStreamerGrabber.h \
			      libs/openFrameworks/video/ofGStreamerPlayer.h \
			      libs/openFrameworks/video/ofGstUtils.h \
			      libs/openFrameworks/video/ofQtUtils.h \
			      libs/openFrameworks/video/ofQuickTimeGrabber.h \
			      libs/openFrameworks/video/ofQuickTimePlayer.h \
			      libs/openFrameworks/video/ofUnicapGrabber.h \
			      libs/openFrameworks/video/ofVideoGrabber.h \
			      libs/openFrameworks/video/ofVideoPlayer.h

lib_LTLIBRARIES=libopenFrameworks.la
libopenFrameworks_la_CXXFLAGS= \
			       ${gstreamer_CFLAGS} \
			       ${freetype_CFLAGS}
libopenFrameworks_la_LDFLAGS= \
			      -version-info 0:6:0 \
			      ${gstreamer_LDFLAGS} \
			      ${freetype_LDFLAGS}
libopenFrameworks_la_SOURCES= \
			      libs/openFrameworks/app/ofAppGlutWindow.cpp \
			      libs/openFrameworks/app/ofAppRunner.cpp \
			      libs/openFrameworks/communication/ofArduino.cpp \
			      libs/openFrameworks/communication/ofSerial.cpp \
			      libs/openFrameworks/events/ofEvents.cpp \
			      libs/openFrameworks/graphics/ofBitmapFont.cpp \
			      libs/openFrameworks/graphics/ofGraphics.cpp \
			      libs/openFrameworks/graphics/ofImage.cpp \
			      libs/openFrameworks/graphics/ofTexture.cpp \
			      libs/openFrameworks/graphics/ofTrueTypeFont.cpp \
			      libs/openFrameworks/sound/ofSoundStream.cpp \
			      libs/openFrameworks/utils/ofMath.cpp \
			      libs/openFrameworks/utils/ofUtils.cpp \
			      libs/openFrameworks/video/ofBaseVideoGrabber.cpp \
			      libs/openFrameworks/video/ofBaseVideoPlayer.cpp \
			      libs/openFrameworks/video/ofDirectShowGrabber.cpp \
			      libs/openFrameworks/video/ofGStreamerGrabber.cpp \
			      libs/openFrameworks/video/ofGStreamerPlayer.cpp \
			      libs/openFrameworks/video/ofGstUtils.cpp \
			      libs/openFrameworks/video/ofQtUtils.cpp \
			      libs/openFrameworks/video/ofUnicapGrabber.cpp \
			      libs/openFrameworks/video/ofVideoGrabber.cpp \
			      libs/openFrameworks/video/ofVideoPlayer.cpp
