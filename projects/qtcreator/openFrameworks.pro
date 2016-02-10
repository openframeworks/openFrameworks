#-------------------------------------------------
#
# Project created by QtCreator 2015-09-26T10:40:31
#
#-------------------------------------------------

QT       -= core gui
TEMPLATE = lib
CONFIG += staticlib

OF_ROOT=$$PWD/../../../..
OF_COMPILED=$$OF_ROOT/libs/openFrameworksCompiled
OF_SRC=$$OF_ROOT/libs/openFrameworks

SOURCES += \
    $$OF_SRC/3d/of3dPrimitives.cpp \
    $$OF_SRC/3d/of3dUtils.cpp \
    $$OF_SRC/3d/ofCamera.cpp \
    $$OF_SRC/3d/ofEasyCam.cpp \
    $$OF_SRC/3d/ofMesh.cpp \
    $$OF_SRC/3d/ofNode.cpp \
    $$OF_SRC/app/ofAppGLFWWindow.cpp \
    $$OF_SRC/app/ofAppGlutWindow.cpp \
    $$OF_SRC/app/ofAppNoWindow.cpp \
    $$OF_SRC/app/ofAppRunner.cpp \
    $$OF_SRC/app/ofMainLoop.cpp \
    $$OF_SRC/communication/ofArduino.cpp \
    $$OF_SRC/communication/ofSerial.cpp \
    $$OF_SRC/events/ofEvents.cpp \
    $$OF_SRC/gl/ofBufferObject.cpp \
    $$OF_SRC/gl/ofFbo.cpp \
    $$OF_SRC/gl/ofGLProgrammableRenderer.cpp \
    $$OF_SRC/gl/ofGLRenderer.cpp \
    $$OF_SRC/gl/ofGLUtils.cpp \
    $$OF_SRC/gl/ofLight.cpp \
    $$OF_SRC/gl/ofMaterial.cpp \
    $$OF_SRC/gl/ofShader.cpp \
    $$OF_SRC/gl/ofTexture.cpp \
    $$OF_SRC/gl/ofVbo.cpp \
    $$OF_SRC/gl/ofVboMesh.cpp \
    $$OF_SRC/graphics/of3dGraphics.cpp \
    $$OF_SRC/graphics/ofBitmapFont.cpp \
    $$OF_SRC/graphics/ofCairoRenderer.cpp \
    $$OF_SRC/graphics/ofGraphics.cpp \
    $$OF_SRC/graphics/ofImage.cpp \
    $$OF_SRC/graphics/ofPath.cpp \
    $$OF_SRC/graphics/ofPixels.cpp \
    $$OF_SRC/graphics/ofPolyline.cpp \
    $$OF_SRC/graphics/ofRendererCollection.cpp \
    $$OF_SRC/graphics/ofTessellator.cpp \
    $$OF_SRC/graphics/ofTrueTypeFont.cpp \
    $$OF_SRC/math/ofMath.cpp \
    $$OF_SRC/math/ofMatrix3x3.cpp \
    $$OF_SRC/math/ofMatrix4x4.cpp \
    $$OF_SRC/math/ofQuaternion.cpp \
    $$OF_SRC/math/ofVec2f.cpp \
    $$OF_SRC/math/ofVec4f.cpp \
    $$OF_SRC/sound/ofBaseSoundStream.cpp \
    $$OF_SRC/sound/ofFmodSoundPlayer.cpp \
    $$OF_SRC/sound/ofOpenALSoundPlayer.cpp \
    $$OF_SRC/sound/ofRtAudioSoundStream.cpp \
    $$OF_SRC/sound/ofSoundBuffer.cpp \
    $$OF_SRC/sound/ofSoundPlayer.cpp \
    $$OF_SRC/sound/ofSoundStream.cpp \
    $$OF_SRC/types/ofBaseTypes.cpp \
    $$OF_SRC/types/ofColor.cpp \
    $$OF_SRC/types/ofParameter.cpp \
    $$OF_SRC/types/ofParameterGroup.cpp \
    $$OF_SRC/types/ofRectangle.cpp \
    $$OF_SRC/utils/ofFileUtils.cpp \
    $$OF_SRC/utils/ofFpsCounter.cpp \
    $$OF_SRC/utils/ofLog.cpp \
    $$OF_SRC/utils/ofMatrixStack.cpp \
    $$OF_SRC/utils/ofSystemUtils.cpp \
    $$OF_SRC/utils/ofThread.cpp \
    $$OF_SRC/utils/ofTimer.cpp \
    $$OF_SRC/utils/ofURLFileLoader.cpp \
    $$OF_SRC/utils/ofUtils.cpp \
    $$OF_SRC/utils/ofXml.cpp \
    $$OF_SRC/video/ofGstUtils.cpp \
    $$OF_SRC/video/ofGstVideoGrabber.cpp \
    $$OF_SRC/video/ofGstVideoPlayer.cpp \
    $$OF_SRC/video/ofVideoGrabber.cpp \
    $$OF_SRC/video/ofVideoPlayer.cpp

HEADERS += \
    $$OF_SRC/3d/of3dPrimitives.h \
    $$OF_SRC/3d/of3dUtils.h \
    $$OF_SRC/3d/ofCamera.h \
    $$OF_SRC/3d/ofEasyCam.h \
    $$OF_SRC/3d/ofMesh.h \
    $$OF_SRC/3d/ofNode.h \
    $$OF_SRC/app/ofAppBaseWindow.h \
    $$OF_SRC/app/ofAppEGLWindow.h \
    $$OF_SRC/app/ofAppGLFWWindow.h \
    $$OF_SRC/app/ofAppGlutWindow.h \
    $$OF_SRC/app/ofAppNoWindow.h \
    $$OF_SRC/app/ofAppRunner.h \
    $$OF_SRC/app/ofBaseApp.h \
    $$OF_SRC/app/ofIcon.h \
    $$OF_SRC/app/ofMainLoop.h \
    $$OF_SRC/app/ofWindowSettings.h \
    $$OF_SRC/communication/ofArduino.h \
    $$OF_SRC/communication/ofSerial.h \
    $$OF_SRC/events/ofEvent.h \
    $$OF_SRC/events/ofEvents.h \
    $$OF_SRC/events/ofEventUtils.h \
    $$OF_SRC/gl/ofBufferObject.h \
    $$OF_SRC/gl/ofFbo.h \
    $$OF_SRC/gl/ofGLProgrammableRenderer.h \
    $$OF_SRC/gl/ofGLRenderer.h \
    $$OF_SRC/gl/ofGLUtils.h \
    $$OF_SRC/gl/ofLight.h \
    $$OF_SRC/gl/ofMaterial.h \
    $$OF_SRC/gl/ofShader.h \
    $$OF_SRC/gl/ofTexture.h \
    $$OF_SRC/gl/ofVbo.h \
    $$OF_SRC/gl/ofVboMesh.h \
    $$OF_SRC/graphics/of3dGraphics.h \
    $$OF_SRC/graphics/ofBitmapFont.h \
    $$OF_SRC/graphics/ofCairoRenderer.h \
    $$OF_SRC/graphics/ofGraphics.h \
    $$OF_SRC/graphics/ofImage.h \
    $$OF_SRC/graphics/ofPath.h \
    $$OF_SRC/graphics/ofPixels.h \
    $$OF_SRC/graphics/ofPolyline.h \
    $$OF_SRC/graphics/ofRendererCollection.h \
    $$OF_SRC/graphics/ofTessellator.h \
    $$OF_SRC/graphics/ofTrueTypeFont.h \
    $$OF_SRC/math/ofMath.h \
    $$OF_SRC/math/ofMatrix3x3.h \
    $$OF_SRC/math/ofMatrix4x4.h \
    $$OF_SRC/math/ofQuaternion.h \
    $$OF_SRC/math/ofVec2f.h \
    $$OF_SRC/math/ofVec3f.h \
    $$OF_SRC/math/ofVec4f.h \
    $$OF_SRC/math/ofVectorMath.h \
    $$OF_SRC/sound/ofBaseSoundPlayer.h \
    $$OF_SRC/sound/ofBaseSoundStream.h \
    $$OF_SRC/sound/ofFmodSoundPlayer.h \
    $$OF_SRC/sound/ofOpenALSoundPlayer.h \
    $$OF_SRC/sound/ofRtAudioSoundStream.h \
    $$OF_SRC/sound/ofSoundBuffer.h \
    $$OF_SRC/sound/ofSoundPlayer.h \
    $$OF_SRC/sound/ofSoundStream.h \
    $$OF_SRC/sound/ofSoundUtils.h \
    $$OF_SRC/types/ofBaseTypes.h \
    $$OF_SRC/types/ofColor.h \
    $$OF_SRC/types/ofParameter.h \
    $$OF_SRC/types/ofParameterGroup.h \
    $$OF_SRC/types/ofPoint.h \
    $$OF_SRC/types/ofRectangle.h \
    $$OF_SRC/types/ofTypes.h \
    $$OF_SRC/utils/ofConstants.h \
    $$OF_SRC/utils/ofFileUtils.h \
    $$OF_SRC/utils/ofFpsCounter.h \
    $$OF_SRC/utils/ofLog.h \
    $$OF_SRC/utils/ofMatrixStack.h \
    $$OF_SRC/utils/ofNoise.h \
    $$OF_SRC/utils/ofSystemUtils.h \
    $$OF_SRC/utils/ofThread.h \
    $$OF_SRC/utils/ofThreadChannel.h \
    $$OF_SRC/utils/ofTimer.h \
    $$OF_SRC/utils/ofURLFileLoader.h \
    $$OF_SRC/utils/ofUtils.h \
    $$OF_SRC/utils/ofXml.h \
    $$OF_SRC/video/ofGstUtils.h \
    $$OF_SRC/video/ofGstVideoGrabber.h \
    $$OF_SRC/video/ofGstVideoPlayer.h \
    $$OF_SRC/video/ofVideoGrabber.h \
    $$OF_SRC/video/ofVideoPlayer.h \
    $$OF_SRC/ofMain.h

include(commonRules.pro)

unix {
    DESTDIR=$$OF_COMPILED/lib/linux64
    CONFIG( debug, debug|release ){
        OBJECTS_DIR=$$DESTDIR/obj/Debug
        TARGET=openFrameworksDebug
    }else{
        OBJECTS_DIR=$$DESTDIR/obj/Release
        TARGET=openFrameworks
    }
}
