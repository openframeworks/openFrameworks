ifeq ($(call ndk-major-at-least,21),true)
    shorten_path = $(abspath $1)
else
    # Strip the drive letter, call abspath, prepend the drive letter.
    shorten_path = $(join $(filter %:,$(subst :,: ,$1)),$(abspath $(filter-out %:,$(subst :,: ,$1))))
endif

OF_ROOT := $(call shorten_path,./../../../../../)
LIBS_ROOT := $(call shorten_path,$(OF_ROOT)/libs)

LOCAL_PATH := $(call my-dir)

SOURCE_PATH := $(LIBS_ROOT)/openFrameworks

# openFrameworks Library dependancies 

# curl / openssl for url loading 
# include $(CLEAR_VARS)
# LOCAL_MODULE := libcurl
# LOCAL_SRC_FILES := $(LIBS_ROOT)/curl/lib/android/$(TARGET_ARCH_ABI)/libcurl.a
# LOCAL_EXPORT_C_INCLUDES := $(LIBS_ROOT)/curl/include
# include $(PREBUILT_STATIC_LIBRARY)

# include $(CLEAR_VARS)
# LOCAL_MODULE := libopenssl
# LOCAL_SRC_FILES := $(LIBS_ROOT)/openssl/lib/android/$(TARGET_ARCH_ABI)/libopenssl.a
# LOCAL_EXPORT_C_INCLUDES := $(LIBS_ROOT)/openssl/include
# include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := FreeImage
LOCAL_SRC_FILES := $(LIBS_ROOT)/FreeImage/lib/android/$(TARGET_ARCH_ABI)/libfreeimage.a
LOCAL_EXPORT_C_INCLUDES := $(LIBS_ROOT)/FreeImage/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := freetype
LOCAL_SRC_FILES := $(LIBS_ROOT)/freetype/lib/android/$(TARGET_ARCH_ABI)/libfreetype.a
LOCAL_EXPORT_C_INCLUDES := $(LIBS_ROOT)/freetype/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := pugixml
LOCAL_SRC_FILES := $(LIBS_ROOT)/pugixml/lib/android/$(TARGET_ARCH_ABI)/libpugixml.a
LOCAL_EXPORT_C_INCLUDES := $(LIBS_ROOT)/pugixml/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := tess2
LOCAL_SRC_FILES := $(LIBS_ROOT)/tess2/lib/android/$(TARGET_ARCH_ABI)/libtess2.a
LOCAL_EXPORT_C_INCLUDES := $(LIBS_ROOT)/tess2/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := openFrameworksAndroid

LOCAL_CFLAGS    := -std=c17 -frtti -Wall -Werror -fno-short-enums -fPIE -fPIC -fuse-ld=gold -fexceptions -ffunction-sections -fdata-sections
LOCAL_CPPFLAGS  := -std=c++17 -stdlib=libc++ -frtti -Wall -Werror -fno-short-enums -fPIE -fPIC -fuse-ld=gold -fexceptions -ffunction-sections -fdata-sections
LOCAL_LDLIBS    := -llog -lGLESv2 -lGLESv1_CM -lOpenSLES -landroid
LOCAL_LDLIBS    := libGLESv3
LOCAL_LDLIBS    := -lz -lgcc -lc -lm -ldl -llog -lc++ -lc++abi
LOCAL_LDLIBS    := -ljnigraphics

LOCAL_C_INCLUDES += \
	$(SOURCE_PATH)/3d \
	$(SOURCE_PATH)/app \
	$(SOURCE_PATH)/communication \
	$(SOURCE_PATH)/events \
	$(SOURCE_PATH)/gl \
	$(SOURCE_PATH)/graphics \
	$(SOURCE_PATH)/math \
	$(SOURCE_PATH)/sound \
	$(SOURCE_PATH)/types \
	$(SOURCE_PATH)/utils \
	$(SOURCE_PATH)/video \
	$(OF_ROOT)/addons/ofxAndroid/ofAndroidLib/src/main/cpp \
	$(OF_ROOT)/addons/ofxAccelerometer/src \
	$(LIBS_ROOT)/FreeImage/include \
	$(LIBS_ROOT)/freetype/include \
	$(LIBS_ROOT)/freetype/include/freetype2 \
	$(LIBS_ROOT)/freetype/include/freetype2/config \
	$(LIBS_ROOT)/freetype/include/freetype2/internal \
	$(LIBS_ROOT)/freetype/include/freetype2/internal/services \
	$(LIBS_ROOT)/glm/include \
	$(LIBS_ROOT)/json/include \
	$(LIBS_ROOT)/tess2/include \
	$(LIBS_ROOT)/utf8/include \
	$(LIBS_ROOT)/pugixml/include \
	$(ADDONS_INCLUDES)

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
    LOCAL_CFLAGS    += -march=armv7-a -mfloat-abi=softfp -mfpu=neon
    LOCAL_CPPFLAGS  += -target armv7-none-linux-androideabi -march=armv7-a -mfloat-abi=softfp -mfpu=neon
    ABI := armv7
else ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
    LOCAL_CFLAGS    += -target aarch64-linux-android
    LOCAL_CPPFLAGS  += -target aarch64-linux-android -mfpu=neon
    ABI := arm64
else ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_CFLAGS    += -target i686-none-linux-android -march=i686 -msse3 -mstackrealign -mfpmath=sse -fno-stack-protector
    LOCAL_CPPFLAGS  += -target i686-none-linux-android -march=i686 -msse3 -mstackrealign -mfpmath=sse -fno-stack-protector
    ABI := x86
else ifeq ($(TARGET_ARCH_ABI),x86_64)
    LOCAL_CFLAGS    +=  -target x86_64-linux-android
    LOCAL_CPPFLAGS  += -target x86_64-linux-android
    ABI := x86_64
endif

# utils

LOCAL_SRC_FILES := $(SOURCE_PATH)/utils/ofConstants.h \
    $(SOURCE_PATH)/utils/ofFileUtils.cpp $(SOURCE_PATH)/utils/ofFileUtils.h \
    $(SOURCE_PATH)/utils/ofFpsCounter.cpp $(SOURCE_PATH)/utils/ofFpsCounter.h \
    $(SOURCE_PATH)/utils/ofJson.h \
    $(SOURCE_PATH)/utils/ofLog.cpp $(SOURCE_PATH)/utils/ofLog.h \
    $(SOURCE_PATH)/utils/ofMatrixStack.cpp $(SOURCE_PATH)/utils/ofMatrixStack.h \
    $(SOURCE_PATH)/utils/ofNoise.h \
    $(SOURCE_PATH)/utils/ofSystemUtils.cpp $(SOURCE_PATH)/utils/ofSystemUtils.h \
    $(SOURCE_PATH)/utils/ofThread.cpp $(SOURCE_PATH)/utils/ofThread.h \
    $(SOURCE_PATH)/utils/ofThreadChannel.h \
    $(SOURCE_PATH)/utils/ofTimer.cpp $(SOURCE_PATH)/utils/ofTimer.h \
    $(SOURCE_PATH)/utils/ofURLFileLoader.cpp $(SOURCE_PATH)/utils/ofURLFileLoader.h \
    $(SOURCE_PATH)/utils/ofUtils.cpp $(SOURCE_PATH)/utils/ofUtils.h \
    $(SOURCE_PATH)/utils/ofXml.cpp $(SOURCE_PATH)/utils/ofXml.h


# 3d
LOCAL_SRC_FILES += $(SOURCE_PATH)/3d/of3dPrimitives.cpp \
    $(SOURCE_PATH)/3d/of3dUtils.cpp \
    $(SOURCE_PATH)/3d/ofCamera.cpp \
    $(SOURCE_PATH)/3d/ofEasyCam.cpp \
    $(SOURCE_PATH)/3d/ofNode.cpp \
	$(SOURCE_PATH)/3d/of3dPrimitives.h \
	$(SOURCE_PATH)/3d/of3dUtils.h \
	$(SOURCE_PATH)/3d/ofCamera.h \
	$(SOURCE_PATH)/3d/ofEasyCam.h \
	$(SOURCE_PATH)/3d/ofNode.h \
	$(SOURCE_PATH)/3d/ofMesh.inl \
	$(SOURCE_PATH)/app/ofWindowSettings.h \
	$(SOURCE_PATH)/app/ofAppBaseWindow.cpp \
	$(SOURCE_PATH)/app/ofAppRunner.h \
	$(SOURCE_PATH)/app/ofMainLoop.cpp \
	$(SOURCE_PATH)/app/ofAppBaseWindow.h \
	$(SOURCE_PATH)/app/ofAppRunner.h \
	$(SOURCE_PATH)/app/ofMainLoop.h \

LOCAL_SRC_FILES += $(SOURCE_PATH)/communication/ofSerial.cpp $(SOURCE_PATH)/communication/ofSerial.h
LOCAL_SRC_FILES += $(SOURCE_PATH)/events/ofEvents.cpp
LOCAL_SRC_FILES := $(SOURCE_PATH)/events/ofEvents.h $(SOURCE_PATH)/events/ofEventUtils.h $(SOURCE_PATH)/events/ofEvent.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/gl/ofBufferObject.cpp $(SOURCE_PATH)/gl/ofFbo.cpp $(SOURCE_PATH)/gl/ofGLProgrammableRenderer.cpp
LOCAL_SRC_FILES := $(SOURCE_PATH)/gl/ofGLRenderer.cpp $(SOURCE_PATH)/gl/ofGLUtils.cpp $(SOURCE_PATH)/gl/ofLight.cpp $(SOURCE_PATH)/gl/ofMaterial.cpp
LOCAL_SRC_FILES := $(SOURCE_PATH)/gl/ofShader.cpp $(SOURCE_PATH)/gl/ofTexture.cpp $(SOURCE_PATH)/gl/ofVbo.cpp $(SOURCE_PATH)/gl/ofVboMesh.cpp
LOCAL_SRC_FILES := $(SOURCE_PATH)/gl/ofBufferObject.h $(SOURCE_PATH)/gl/ofFbo.h $(SOURCE_PATH)/gl/ofGLProgrammableRenderer.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/gl/ofGLRenderer.h $(SOURCE_PATH)/gl/ofGLUtils.h $(SOURCE_PATH)/gl/ofLight.h $(SOURCE_PATH)/gl/ofMaterial.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/graphics/of3dGraphics.cpp $(SOURCE_PATH)/graphics/of3dGraphics.h 
LOCAL_SRC_FILES := $(SOURCE_PATH)/graphics/ofBitmapFont.cpp $(SOURCE_PATH)/graphics/ofBitmapFont.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/graphics/ofCairoRenderer.cpp $(SOURCE_PATH)/graphics/ofCairoRenderer.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/graphics/ofGraphics.cpp $(SOURCE_PATH)/graphics/ofGraphics.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/graphics/ofGraphicsBaseTypes.cpp $(SOURCE_PATH)/graphics/ofGraphicsBaseTypes.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/graphics/ofImage.cpp $(SOURCE_PATH)/graphics/ofImage.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/graphics/ofPath.cpp $(SOURCE_PATH)/graphics/ofPath.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/graphics/ofPixels.cpp $(SOURCE_PATH)/graphics/ofPixels.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/graphics/ofRendererCollection.cpp $(SOURCE_PATH)/graphics/ofRendererCollection.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/graphics/ofTessellator.cpp $(SOURCE_PATH)/graphics/ofTessellator.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/graphics/ofTrueTypeFont.cpp $(SOURCE_PATH)/graphics/ofTrueTypeFont.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/math/ofMath.cpp $(SOURCE_PATH)/math/ofMath.h 
LOCAL_SRC_FILES := $(SOURCE_PATH)/math/ofMathConstants.cpp $(SOURCE_PATH)/math/ofMathConstants.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/math/ofMatrix3x3.cpp $(SOURCE_PATH)/math/ofMatrix3x3.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/math/ofMatrix4x4.cpp $(SOURCE_PATH)/math/ofMatrix4x4.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/math/ofQuaternion.cpp $(SOURCE_PATH)/math/ofQuaternion.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/math/ofVec2f.cpp $(SOURCE_PATH)/math/ofVectorMath.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/math/ofVec3f.cpp $(SOURCE_PATH)/math/ofVec3f.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/math/ofVec4f.cpp $(SOURCE_PATH)/math/ofVec4f.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/sound/ofSoundBaseTypes.h $(SOURCE_PATH)/sound/ofSoundUtils.h 
LOCAL_SRC_FILES := $(SOURCE_PATH)/sound/ofSoundBuffer.cpp $(SOURCE_PATH)/sound/ofSoundBuffer.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/sound/ofSoundPlayer.cpp $(SOURCE_PATH)/sound/ofSoundPlayer.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/sound/ofSoundStream.cpp $(SOURCE_PATH)/sound/ofSoundStream.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/sound/ofOpenALSoundPlayer.cpp $(SOURCE_PATH)/sound/ofOpenALSoundPlayer.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/types/ofBaseTypes.h 
LOCAL_SRC_FILES := $(SOURCE_PATH)/types/ofColor.cpp $(SOURCE_PATH)/types/ofParameter.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/types/ofParameter.cpp $(SOURCE_PATH)/types/ofParameter.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/types/ofParameterGroup.cpp $(SOURCE_PATH)/types/ofParameterGroup.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/types/ofPoint.cpp $(SOURCE_PATH)/types/ofPoint.h
LOCAL_SRC_FILES := $(SOURCE_PATH)/types/ofRectangle.h 
LOCAL_SRC_FILES := $(SOURCE_PATH)/types/ofTypes.h
LOCAL_SRC_FILES += $(SOURCE_PATH)/video/ofVideoBaseTypes.h
LOCAL_SRC_FILES += $(SOURCE_PATH)/video/ofVideoGrabber.cpp $(SOURCE_PATH)/video/ofVideoGrabber.h
LOCAL_SRC_FILES += $(SOURCE_PATH)/video/ofVideoPlayer.cpp $(SOURCE_PATH)/video/ofVideoPlayer.h


LOCAL_SRC_FILES += $(SOURCE_PATH)/video/ofVideoBaseTypes.h
LOCAL_SRC_FILES += $(SOURCE_PATH)/video/ofVideoGrabber.cpp $(SOURCE_PATH)/video/ofVideoGrabber.h
LOCAL_SRC_FILES += $(SOURCE_PATH)/video/ofVideoPlayer.cpp $(SOURCE_PATH)/video/ofVideoPlayer.h

LOCAL_STATIC_LIBRARIES := freetype
LOCAL_STATIC_LIBRARIES := FreeImage
LOCAL_STATIC_LIBRARIES := pugixml
LOCAL_STATIC_LIBRARIES := tess2

include $(BUILD_SHARED_LIBRARY)



