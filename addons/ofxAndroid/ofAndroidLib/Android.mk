#
# Copyright (C) 2016 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# Workaround a bug in abspath on Windows that existed in older versions of make
# (paths with drive letters were not handled properly in abspath). r21+ has a
# newer version of make that doesn't have this bug.
ifeq ($(call ndk-major-at-least,21),true)
    shorten_path = $(abspath $1)
else
    # Strip the drive letter, call abspath, prepend the drive letter.
    shorten_path = $(join $(filter %:,$(subst :,: ,$1)),$(abspath $(filter-out %:,$(subst :,: ,$1))))
endif

OFANDROIDLIB_ROOT := $(call shorten_path,./)

LOCAL_PATH := $(call my-dir)

JNI_SRC_PATH := $(OFANDROIDLIB_ROOT)/src/main/cpp

include $(CLEAR_VARS)

LOCAL_MODULE    := ofAndroid

LOCAL_CFLAGS    := -std=c17 -Werror -fno-short-enums -fPIE -fPIC -fuse-ld=gold

LOCAL_CPPFLAGS  := -std=c++17 -stdlib=libc++ -Werror -fno-short-enums -fPIE -fPIC -fuse-ld=gold
LOCAL_LDLIBS    := -llog -lGLESv2 -lGLESv1_CM -lOpenSLES
LOCAL_LDLIBS    := libGLESv3
LOCAL_LDLIBS    := -lz -lgcc -lc -lm -ldl -llog -lc++ -lc++abi
LOCAL_LDLIBS    := -ljnigraphics

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
    LOCAL_LDLIBS := -lunwind
    LOCAL_CFLAGS    :=  -march=armv7-a -mfloat-abi=softfp -mfpu=neon
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
    LOCAL_CFLAGS    :=  -target x86_64-linux-android
    LOCAL_CPPFLAGS  := -target x86_64-linux-android
    ABI := x86_64
endif


LOCAL_SRC_FILES := $(JNI_SRC_PATH)/ofAppAndroidWindow.cpp $(JNI_SRC_PATH)/ofxAndroidAccelerometer.cpp $(JNI_SRC_PATH)/ofxAndroidLogChannel.cpp
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/ofxAndroidSoundPlayer.cpp $(JNI_SRC_PATH)/ofxAndroidSoundStream.cpp $(JNI_SRC_PATH)/ofxAndroidUtils.cpp
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/ofxAndroidVibrator.cpp $(JNI_SRC_PATH)/ofxAndroidVideoGrabber.cpp $(JNI_SRC_PATH)/ofxAndroidVideoPlayer.cpp

LOCAL_SRC_FILES := $(JNI_SRC_PATH)/ofxAndroid.h $(JNI_SRC_PATH)/ofxAndroidApp.h $(JNI_SRC_PATH)/ofxAndroidCircBuffer.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/ofAppAndroidWindow.h $(JNI_SRC_PATH)/ofxAndroidAccelerometer.h $(JNI_SRC_PATH)/ofxAndroidLogChannel.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/ofxAndroidSoundPlayer.h $(JNI_SRC_PATH)/ofxAndroidSoundStream.h $(JNI_SRC_PATH)/ofxAndroidUtils.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/ofxAndroidVibrator.h $(JNI_SRC_PATH)/ofxAndroidVideoGrabber.h $(JNI_SRC_PATH)/ofxAndroidVideoPlayer.h


include $(BUILD_SHARED_LIBRARY)

