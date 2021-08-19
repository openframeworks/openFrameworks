ifeq ($(call ndk-major-at-least,21),true)
    shorten_path = $(abspath $1)
else
    # Strip the drive letter, call abspath, prepend the drive letter.
    shorten_path = $(join $(filter %:,$(subst :,: ,$1)),$(abspath $(filter-out %:,$(subst :,: ,$1))))
endif
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
SYSROOT=/Users/eyenine/Documents/android-ndk-r22b/toolchains/llvm/prebuilt/darwin-x86_64/sysroot
TOOLS_HOST=linux

PRJ_OF_ROOT := ./../../../../../..



PURE_OF_ROOT = /Users/eyenine/Documents/oFAndroid/openFrameworks/
CORE_OF_ROOT = /Users/eyenine/Documents/oFAndroid/openFrameworks/libs/openFrameworks

PRJ_ADDONS_PATH :=$(PURE_OF_ROOT)/addons
PRJ_SOURCE_PATH := $(PURE_OF_ROOT)/openFrameworks
PRJ_LIBS_ROOT := $(PURE_OF_ROOT)/libs

PRJ_OFX_ANDROID_PATH := $(PRJ_ADDONS_PATH)/ofxAndroid
PRJ_OFX_ANDROID_CPP_PATH := $(PRJ_OFX_ANDROID_PATH)/src

PRJ_ROOT := /../..


$(warning $(LOCAL_PATH))
$(warning $(PRJ_SOURCE_PATH))

#PROJECT_DIR := superhexagon-android
#JNI_SRC_PATH := $(LOCAL_PATH) #$(LOCAL_PATH)/src/main/cpp

#LOCAL_MODULE := openFrameworksAndroid
#LOCAL_SRC_FILES := $(LIBS_ROOT)/openFrameworksCompiled/lib/android/$(TARGET_ARCH_ABI)/libopenFrameworksAndroid.so
#include $(PREBUILT_SHARED_LIBRARY)
LOCAL_MODULE := superhexagon

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

LOCAL_CFLAGS    := -std=c17 -frtti -Wall -fno-short-enums -fPIE -fPIC -fuse-ld=gold -fexceptions -ffunction-sections -fdata-sections
LOCAL_CPPFLAGS  := -std=c++17 -stdlib=libc++ -frtti -Wall -fno-short-enums -fPIE -fPIC -fuse-ld=gold -fexceptions -ffunction-sections -fdata-sections
LOCAL_LDLIBS    += -lGLESv2 -lGLESv1_CM -lOpenSLES -landroid
LOCAL_LDLIBS    += -lGLESv3
LOCAL_LDLIBS    += -lz -lc -lm -ldl -llog -ljnigraphics

# Local Source Files (.CPP/.C Only, no headers listed)
LOCAL_SRC_FILES :=  \
	ofApp.cpp \
	main.cpp


# Include Directories
LOCAL_C_INCLUDES += \
	$(CORE_OF_ROOT)/3d \
	$(CORE_OF_ROOT)/app \
	$(CORE_OF_ROOT)/communication \
	$(CORE_OF_ROOT)/events \
	$(CORE_OF_ROOT)/gl \
	$(CORE_OF_ROOT)/graphics \
	$(CORE_OF_ROOT)/math \
	$(CORE_OF_ROOT)/sound \
	$(CORE_OF_ROOT)/types \
	$(CORE_OF_ROOT)/utils \
	$(CORE_OF_ROOT)/video \
	$(CORE_OF_ROOT) \
	$(PURE_OF_ROOT)/addons/ofxAndroid/src \
	$(PURE_OF_ROOT)/addons/ofxAccelerometer/src \
	$(PURE_OF_ROOT)/addons/ofxXmlSettings/src \
	$(PURE_OF_ROOT)/addons/ofxXmlSettings/libs \
	$(PRJ_LIBS_ROOT)/FreeImage/include \
	$(PRJ_LIBS_ROOT)/freetype/include \
	$(PRJ_LIBS_ROOT)/freetype/include/freetype2 \
	$(PRJ_LIBS_ROOT)/freetype/include/freetype2/freetype/config \
	$(PRJ_LIBS_ROOT)/freetype/include/freetype2/freetype/internal \
	$(PRJ_LIBS_ROOT)/freetype/include/freetype2/freetype/internal/services \
	$(PRJ_LIBS_ROOT)/glm/include \
	$(PRJ_LIBS_ROOT)/pugixml/include \
	$(PRJ_LIBS_ROOT)/json/include \
	$(PRJ_LIBS_ROOT)/tess2/include \
	$(PRJ_LIBS_ROOT)/utf8/include \
	$(PRJ_LIBS_ROOT)/uriparser/include \
	$(PRJ_LIBS_ROOT)/uriparser/include \
	$(LOCAL_PATH)


LOCAL_SHARED_LIBRARIES := openFrameworksAndroid
include $(BUILD_SHARED_LIBRARY)
include $(CLEAR_VARS)

#$(call import-add-path, $(LOCAL_PATH))
#$(call import-add-path, $(PRJ_ROOT))
$(call import-add-path, /Users/eyenine/Documents/ofAndroid/openFrameworks/libs/openFrameworksCompiled/project/android)
#$(call import-add-path, $(PRJ_OF_ROOT))
$(call import-add-path, /Users/eyenine/Documents/ofAndroid/openFrameworks/libs/openFrameworks)


include $(CLEAR_VARS)
# If you don't need your project to build with NDKs older than r21, you can omit
# this block.
ifneq ($(call ndk-major-at-least,21),true)
    $(call import-add-path,$(NDK_GRADLE_INJECTED_IMPORT_PATH))
endif

$(call import-module, openFrameworksAndroid)