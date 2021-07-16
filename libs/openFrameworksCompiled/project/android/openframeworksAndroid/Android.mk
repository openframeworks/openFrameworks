ifeq ($(call ndk-major-at-least,21),true)
    shorten_path = $(abspath $1)
else
    # Strip the drive letter, call abspath, prepend the drive letter.
    shorten_path = $(join $(filter %:,$(subst :,: ,$1)),$(abspath $(filter-out %:,$(subst :,: ,$1))))
endif

OF_ROOT := $(call shorten_path,./../../../../../)
LIBS_ROOT := $(call shorten_path,$(OF_ROOT)/libs)

LOCAL_PATH := $(call my-dir)

ADDONS_PATH := $(call shorten_path,$(OF_ROOT)/addons)

SOURCE_PATH := $(LIBS_ROOT)/openFrameworks

OFX_ANDROID_PATH := $(ADDONS_PATH)/ofxAndroid
OFX_ANDROID_CPP_PATH := $(OFX_ANDROID_PATH)/ofAndroidLib/src/main/cpp

SYSROOT=/Users/eyenine/Documents/android-ndk-r22b/toolchains/llvm/prebuilt/darwin-x86_64/sysroot
TOOLS_HOST=linux

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
LOCAL_MODULE := uriparser
LOCAL_SRC_FILES := $(LIBS_ROOT)/uriparser/lib/android/$(TARGET_ARCH_ABI)/liburiparser.a
LOCAL_EXPORT_C_INCLUDES := $(LIBS_ROOT)/uriparser/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := openFrameworksAndroid

ifeq ($(TARGET_PLATFORM),android-21)
	TARGET_API=21
endif

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
    LOCAL_CFLAGS    += -march=armv7-a -mfloat-abi=softfp -mfpu=neon
    LOCAL_CPPFLAGS  += -target armv7-linux-androideabi -march=armv7-a -mfloat-abi=softfp -mfpu=neon
    ABI := armv7
    LIBRARY_ARCHITECTURE = arm-$(TOOLS_HOST)-androideabi
else ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
    LOCAL_CFLAGS    += -target aarch64-linux-android
    LOCAL_CPPFLAGS  += -target aarch64-linux-android -mfpu=neon
    ABI := arm64
    LIBRARY_ARCHITECTURE = aarch64-$(TOOLS_HOST)-android
else ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_CFLAGS    += -target i686-linux-android -march=i686 -msse3 -mstackrealign -mfpmath=sse -fno-stack-protector
    LOCAL_CPPFLAGS  += -target i686-linux-android -march=i686 -msse3 -mstackrealign -mfpmath=sse -fno-stack-protector
    ABI := x86
    LIBRARY_ARCHITECTURE = i686-$(TOOLS_HOST)-android
else ifeq ($(TARGET_ARCH_ABI),x86_64)
    LOCAL_CFLAGS    +=  -target x86_64-linux-android
    LOCAL_CPPFLAGS  += -target x86_64-linux-android
    ABI := x86_64
    LIBRARY_ARCHITECTURE = x86_64-$(TOOLS_HOST)-android
endif

LOCAL_CFLAGS    := -std=c17 --sysroot=$(SYSROOT) -frtti -Wall -fno-short-enums -fPIE -fPIC -fuse-ld=gold -fexceptions -ffunction-sections -fdata-sections
LOCAL_CPPFLAGS  := -std=c++17 --sysroot=$(SYSROOT) -stdlib=libc++ -frtti -Wall -fno-short-enums -fPIE -fPIC -fuse-ld=gold -fexceptions -ffunction-sections -fdata-sections
LOCAL_LDLIBS    += -L$(SYSROOT)/$(LIBRARY_ARCHITECTURE)/$(TARGET_API)  -lGLESv2 -lGLESv1_CM -lOpenSLES -landroid
LOCAL_LDLIBS    += -lGLESv3
LOCAL_LDLIBS    += -lz -lc -lm -ldl -llog -ljnigraphics
#LOCAL_LDLIBS    +=  -lc++_static -lc++ -lc++abi -lgcc -lc++abi -landroid_support

LOCAL_STATIC_LIBRARIES += freetype
LOCAL_STATIC_LIBRARIES += FreeImage
LOCAL_STATIC_LIBRARIES += pugixml
LOCAL_STATIC_LIBRARIES += tess2
LOCAL_STATIC_LIBRARIES += uriparser

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
	$(LIBS_ROOT)/pugixml/include \
	$(LIBS_ROOT)/json/include \
	$(LIBS_ROOT)/tess2/include \
	$(LIBS_ROOT)/utf8/include \
	$(LIBS_ROOT)/uriparser/include \
	$(ADDONS_INCLUDES)






LOCAL_SRC_FILES += \
    $(SOURCE_PATH)/utils/ofFileUtils.cpp \
    $(SOURCE_PATH)/utils/ofFpsCounter.cpp \
    $(SOURCE_PATH)/utils/ofLog.cpp \
    $(SOURCE_PATH)/utils/ofMatrixStack.cpp  \
    $(SOURCE_PATH)/utils/ofSystemUtils.cpp \
    $(SOURCE_PATH)/utils/ofThread.cpp \
    $(SOURCE_PATH)/utils/ofTimer.cpp \
    $(SOURCE_PATH)/utils/ofURLFileLoader.cpp \
    $(SOURCE_PATH)/utils/ofUtils.cpp \
    $(SOURCE_PATH)/utils/ofXml.cpp

LOCAL_SRC_FILES += $(SOURCE_PATH)/sound/ofSoundBuffer.cpp
LOCAL_SRC_FILES += $(SOURCE_PATH)/sound/ofSoundPlayer.cpp
LOCAL_SRC_FILES += $(SOURCE_PATH)/sound/ofSoundStream.cpp
LOCAL_SRC_FILES += $(SOURCE_PATH)/sound/ofOpenALSoundPlayer.cpp

# 3d
LOCAL_SRC_FILES += $(SOURCE_PATH)/3d/of3dPrimitives.cpp \
    $(SOURCE_PATH)/3d/of3dUtils.cpp \
    $(SOURCE_PATH)/3d/ofCamera.cpp \
    $(SOURCE_PATH)/3d/ofEasyCam.cpp \
    $(SOURCE_PATH)/3d/ofNode.cpp \
    $(SOURCE_PATH)/app/ofMainLoop.cpp \
    $(SOURCE_PATH)/app/ofAppRunner.cpp \
	$(SOURCE_PATH)/app/ofBaseApp.cpp



LOCAL_SRC_FILES += $(SOURCE_PATH)/communication/ofSerial.cpp
LOCAL_SRC_FILES += $(SOURCE_PATH)/events/ofEvents.cpp

LOCAL_SRC_FILES += $(SOURCE_PATH)/gl/ofBufferObject.cpp \
	$(SOURCE_PATH)/gl/ofFbo.cpp \
	$(SOURCE_PATH)/gl/ofGLProgrammableRenderer.cpp \
	$(SOURCE_PATH)/gl/ofGLRenderer.cpp
LOCAL_SRC_FILES +=  \
	$(SOURCE_PATH)/gl/ofGLUtils.cpp \
	$(SOURCE_PATH)/gl/ofLight.cpp \
	$(SOURCE_PATH)/gl/ofMaterial.cpp
	# $(SOURCE_PATH)/gl/ofGLRenderer.cpp
LOCAL_SRC_FILES += $(SOURCE_PATH)/gl/ofShader.cpp \
	$(SOURCE_PATH)/gl/ofTexture.cpp \
	$(SOURCE_PATH)/gl/ofVbo.cpp \
	$(SOURCE_PATH)/gl/ofVboMesh.cpp

LOCAL_SRC_FILES += $(SOURCE_PATH)/graphics/of3dGraphics.cpp \
	$(SOURCE_PATH)/graphics/ofBitmapFont.cpp \
	$(SOURCE_PATH)/graphics/ofGraphics.cpp
LOCAL_SRC_FILES += $(SOURCE_PATH)/graphics/ofGraphicsBaseTypes.cpp
LOCAL_SRC_FILES += $(SOURCE_PATH)/graphics/ofImage.cpp
LOCAL_SRC_FILES += $(SOURCE_PATH)/graphics/ofPath.cpp
LOCAL_SRC_FILES += $(SOURCE_PATH)/graphics/ofPixels.cpp
LOCAL_SRC_FILES += $(SOURCE_PATH)/graphics/ofRendererCollection.cpp
LOCAL_SRC_FILES += $(SOURCE_PATH)/graphics/ofTessellator.cpp
LOCAL_SRC_FILES += $(SOURCE_PATH)/graphics/ofTrueTypeFont.cpp
LOCAL_SRC_FILES += $(SOURCE_PATH)/math/ofMath.cpp

LOCAL_SRC_FILES += $(SOURCE_PATH)/math/ofMatrix3x3.cpp
LOCAL_SRC_FILES += $(SOURCE_PATH)/math/ofMatrix4x4.cpp
LOCAL_SRC_FILES += $(SOURCE_PATH)/math/ofQuaternion.cpp
LOCAL_SRC_FILES += $(SOURCE_PATH)/math/ofVec2f.cpp

LOCAL_SRC_FILES += $(SOURCE_PATH)/math/ofVec4f.cpp


LOCAL_SRC_FILES += $(SOURCE_PATH)/types/ofColor.cpp
LOCAL_SRC_FILES += $(SOURCE_PATH)/types/ofParameter.cpp
LOCAL_SRC_FILES += $(SOURCE_PATH)/types/ofParameterGroup.cpp
LOCAL_SRC_FILES += $(SOURCE_PATH)/types/ofRectangle.cpp
LOCAL_SRC_FILES += $(SOURCE_PATH)/video/ofVideoBaseTypes.cpp \
	$(SOURCE_PATH)/video/ofVideoGrabber.cpp \
	$(SOURCE_PATH)/video/ofVideoPlayer.cpp


LOCAL_SRC_FILES += $(OFX_ANDROID_CPP_PATH)/ofAppAndroidWindow.cpp \
	$(OFX_ANDROID_CPP_PATH)/ofxAndroidAccelerometer.cpp \
	$(OFX_ANDROID_CPP_PATH)/ofxAndroidLogChannel.cpp \
	$(OFX_ANDROID_CPP_PATH)/ofxAndroidSoundPlayer.cpp \
	$(OFX_ANDROID_CPP_PATH)/ofxAndroidSoundStream.cpp \
	$(OFX_ANDROID_CPP_PATH)/ofxAndroidUtils.cpp \
	$(OFX_ANDROID_CPP_PATH)/ofxAndroidVibrator.cpp \
	$(OFX_ANDROID_CPP_PATH)/ofxAndroidVideoGrabber.cpp \
	$(OFX_ANDROID_CPP_PATH)/ofxAndroidVideoPlayer.cpp

LOCAL_SRC_FILES += $(ADDONS_PATH)/ofxAccelerometer/src/ofxAccelerometer.cpp


include $(BUILD_SHARED_LIBRARY)



