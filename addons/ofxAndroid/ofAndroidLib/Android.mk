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
LOCAL_CFLAGS    := -Werror -std=c++17

LOCAL_SRC_FILES := $(JNI_SRC_PATH)/ofAppAndroidWindow.cpp $(JNI_SRC_PATH)/ofxAndroidAccelerometer.cpp $(JNI_SRC_PATH)/ofxAndroidLogChannel.cpp
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/ofxAndroidSoundPlayer.cpp $(JNI_SRC_PATH)/ofxAndroidSoundStream.cpp $(JNI_SRC_PATH)/ofxAndroidUtils.cpp
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/ofxAndroidVibrator.cpp $(JNI_SRC_PATH)/ofxAndroidVideoGrabber.cpp $(JNI_SRC_PATH)/ofxAndroidVideoPlayer.cpp

LOCAL_SRC_FILES := $(JNI_SRC_PATH)/ofxAndroid.h $(JNI_SRC_PATH)/ofxAndroidApp.h $(JNI_SRC_PATH)/ofxAndroidCircBuffer.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/ofAppAndroidWindow.h $(JNI_SRC_PATH)/ofxAndroidAccelerometer.h $(JNI_SRC_PATH)/ofxAndroidLogChannel.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/ofxAndroidSoundPlayer.h $(JNI_SRC_PATH)/ofxAndroidSoundStream.h $(JNI_SRC_PATH)/ofxAndroidUtils.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/ofxAndroidVibrator.h $(JNI_SRC_PATH)/ofxAndroidVideoGrabber.h $(JNI_SRC_PATH)/ofxAndroidVideoPlayer.h


LOCAL_LDLIBS    := -llog -lGLESv2

include $(BUILD_SHARED_LIBRARY)
