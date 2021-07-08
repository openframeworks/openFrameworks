ifeq ($(call ndk-major-at-least,21),true)
    shorten_path = $(abspath $1)
else
    # Strip the drive letter, call abspath, prepend the drive letter.
    shorten_path = $(join $(filter %:,$(subst :,: ,$1)),$(abspath $(filter-out %:,$(subst :,: ,$1))))
endif

OFANDROIDLIB_ROOT := $(call shorten_path,./)

LOCAL_PATH := $(call my-dir)

JNI_SRC_PATH := $(OFANDROIDLIB_ROOT)/src/main/cpp

OF_ROOT := $(OFANDROIDLIB_ROOT)../../../
LIBS_ROOT := $(call shorten_path,$(OF_ROOT)/libs)
OF_SOURCE_PATH := $(LIBS_ROOT)/openFrameworks

include $(CLEAR_VARS)

LOCAL_MODULE    := ofAndroid

LOCAL_CFLAGS    := -std=c17 -Werror -fno-short-enums -fPIE -fPIC -fuse-ld=gold

LOCAL_CPPFLAGS  := -std=c++17 -stdlib=libc++ -Werror -fno-short-enums -fPIE -fPIC -fuse-ld=gold
LOCAL_LDLIBS    := -llog -lGLESv2 -lGLESv1_CM -lOpenSLES -landroid
LOCAL_LDLIBS    := libGLESv3
LOCAL_LDLIBS    := -lz -lgcc -lc -lm -ldl -llog -lc++ -lc++abi
LOCAL_LDLIBS    := -ljnigraphics

LOCAL_C_INCLUDES += \
	$(OF_SOURCE_PATH)/3d \
	$(OF_SOURCE_PATH)/app \
	$(OF_SOURCE_PATH)/communication \
	$(OF_SOURCE_PATH)/events \
	$(OF_SOURCE_PATH)/gl \
	$(OF_SOURCE_PATH)/graphics \
	$(OF_SOURCE_PATH)/math \
	$(OF_SOURCE_PATH)/sound \
	$(OF_SOURCE_PATH)/types \
	$(OF_SOURCE_PATH)/utils \
	$(OF_SOURCE_PATH)/video \
	$(OF_ROOT)/addons/ofxAndroid/ofAndroidLib/src/main/cpp \
	$(OF_ROOT)/addons/ofxAccelerometer/src \

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
    LOCAL_CFLAGS    := -march=armv7-a -mfloat-abi=softfp -mfpu=neon
    LOCAL_CPPFLAGS  := -target armv7-none-linux-androideabi -march=armv7-a -mfloat-abi=softfp -mfpu=neon
    ABI := armv7
else ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
    LOCAL_CFLAGS    := -target aarch64-linux-android
    LOCAL_CPPFLAGS  := -target aarch64-linux-android -mfpu=neon
    ABI := arm64
else ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_CFLAGS    := -target i686-none-linux-android -march=i686 -msse3 -mstackrealign -mfpmath=sse -fno-stack-protector
    LOCAL_CPPFLAGS  := -target i686-none-linux-android -march=i686 -msse3 -mstackrealign -mfpmath=sse -fno-stack-protector
    ABI := x86
else ifeq ($(TARGET_ARCH_ABI),x86_64)
    LOCAL_CFLAGS    := -target x86_64-linux-android
    LOCAL_CPPFLAGS  := -target x86_64-linux-android
    ABI := x86_64
endif

include $(CLEAR_VARS)
LOCAL_MODULE := openFrameworksAndroid
LOCAL_SRC_FILES := $(LIBS_ROOT)/FreeImage/lib/android/$(TARGET_ARCH_ABI)/libfreeimage.a
LOCAL_EXPORT_C_INCLUDES := $(LIBS_ROOT)/FreeImage/include
include $(PREBUILT_STATIC_LIBRARY)


LOCAL_SRC_FILES += $(JNI_SRC_PATH)/ofAppAndroidWindow.cpp \
	$(JNI_SRC_PATH)/ofxAndroidAccelerometer.cpp \
	$(JNI_SRC_PATH)/ofxAndroidLogChannel.cpp \
	$(JNI_SRC_PATH)/ofxAndroidSoundPlayer.cpp \
	$(JNI_SRC_PATH)/ofxAndroidSoundStream.cpp \
	$(JNI_SRC_PATH)/ofxAndroidUtils.cpp \
	$(JNI_SRC_PATH)/ofxAndroidVibrator.cpp \
	$(JNI_SRC_PATH)/ofxAndroidVideoGrabber.cpp \
	$(JNI_SRC_PATH)/ofxAndroidVideoPlayer.cpp

LOCAL_SRC_FILES += $(JNI_SRC_PATH)/ofxAndroid.h \
	$(JNI_SRC_PATH)/ofxAndroidApp.h \
	$(JNI_SRC_PATH)/ofxAndroidCircBuffer.h \
	$(JNI_SRC_PATH)/ofAppAndroidWindow.h \
	$(JNI_SRC_PATH)/ofxAndroidAccelerometer.h \
	$(JNI_SRC_PATH)/ofxAndroidLogChannel.h \
	$(JNI_SRC_PATH)/ofxAndroidSoundPlayer.h \
	$(JNI_SRC_PATH)/ofxAndroidSoundStream.h \
	$(JNI_SRC_PATH)/ofxAndroidUtils.h \
	$(JNI_SRC_PATH)/ofxAndroidVibrator.h \
	$(JNI_SRC_PATH)/ofxAndroidVideoGrabber.h \
	$(JNI_SRC_PATH)/ofxAndroidVideoPlayer.h

LOCAL_STATIC_LIBRARIES := openFrameworksAndroid

include $(BUILD_SHARED_LIBRARY)

