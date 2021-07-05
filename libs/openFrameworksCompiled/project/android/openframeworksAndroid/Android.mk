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

OF_ROOT := $(call shorten_path,./../../../../../)
LIBS_ROOT := $(call shorten_path,$(OF_ROOT)/libs)

LOCAL_PATH := $(call my-dir)

JNI_SRC_PATH := $(LIBS_ROOT)/openFrameworks

include $(CLEAR_VARS)

LOCAL_MODULE    := openframeworksAndroid

LOCAL_CFLAGS    := -std=c17 -Werror -fno-short-enums -fPIE -fPIC -fuse-ld=gold
LOCAL_CPPFLAGS  := -std=c++17 -stdlib=libc++ -Werror -fno-short-enums -fPIE -fPIC -fuse-ld=gold
LOCAL_LDLIBS    := -llog -lGLESv2 -lGLESv1_CM -lOpenSLES
LOCAL_LDLIBS    := libGLESv3
LOCAL_LDLIBS    := -lz -lgcc -lc -lm -ldl -llog -lc++ -lc++abi
LOCAL_LDLIBS    := -legl -ljnigraphics -lcamera2ndk

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
    LOCAL_LDLIBS := unwind
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


# 3d
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/3d/of3dPrimitives.cpp $(JNI_SRC_PATH)/3d/of3dUtils.cpp $(JNI_SRC_PATH)/3d/ofCamera.cpp
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/3d/ofEasyCam.cpp $(JNI_SRC_PATH)/3d/ofNode.cpp

LOCAL_SRC_FILES := $(JNI_SRC_PATH)/3d/of3dPrimitives.h $(JNI_SRC_PATH)/3d/of3dUtils.h $(JNI_SRC_PATH)/3d/ofCamera.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/3d/ofEasyCam.h $(JNI_SRC_PATH)/3d/ofNode.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/3d/ofMesh.inl

# app
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/app/ofAppBaseWindow.cpp $(JNI_SRC_PATH)/app/ofAppRunner.h $(JNI_SRC_PATH)/app/ofMainLoop.cpp

LOCAL_SRC_FILES := $(JNI_SRC_PATH)/app/ofAppBaseWindow.h $(JNI_SRC_PATH)/app/ofAppRunner.h $(JNI_SRC_PATH)/app/ofMainLoop.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/app/ofWindowSettings.h

# communication
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/communication/ofSerial.cpp $(JNI_SRC_PATH)/communication/ofSerial.h

# events

LOCAL_SRC_FILES := $(JNI_SRC_PATH)/events/ofEvents.cpp
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/events/ofEvents.h $(JNI_SRC_PATH)/events/ofEventUtils.h $(JNI_SRC_PATH)/events/ofEvent.h

# gl
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/gl/ofBufferObject.cpp $(JNI_SRC_PATH)/gl/ofFbo.cpp $(JNI_SRC_PATH)/gl/ofGLProgrammableRenderer.cpp
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/gl/ofGLRenderer.cpp $(JNI_SRC_PATH)/gl/ofGLUtils.cpp $(JNI_SRC_PATH)/gl/ofLight.cpp $(JNI_SRC_PATH)/gl/ofMaterial.cpp
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/gl/ofShader.cpp $(JNI_SRC_PATH)/gl/ofTexture.cpp $(JNI_SRC_PATH)/gl/ofVbo.cpp $(JNI_SRC_PATH)/gl/ofVboMesh.cpp

LOCAL_SRC_FILES := $(JNI_SRC_PATH)/gl/ofBufferObject.h $(JNI_SRC_PATH)/gl/ofFbo.h $(JNI_SRC_PATH)/gl/ofGLProgrammableRenderer.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/gl/ofGLRenderer.h $(JNI_SRC_PATH)/gl/ofGLUtils.h $(JNI_SRC_PATH)/gl/ofLight.h $(JNI_SRC_PATH)/gl/ofMaterial.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/gl/ofShader.h $(JNI_SRC_PATH)/gl/ofTexture.h $(JNI_SRC_PATH)/gl/ofVbo.h $(JNI_SRC_PATH)/gl/ofVboMesh.h

# graphics
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/graphics/of3dGraphics.cpp $(JNI_SRC_PATH)/graphics/of3dGraphics.h 
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/graphics/ofBitmapFont.cpp $(JNI_SRC_PATH)/graphics/ofBitmapFont.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/graphics/ofCairoRenderer.cpp $(JNI_SRC_PATH)/graphics/ofCairoRenderer.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/graphics/ofGraphics.cpp $(JNI_SRC_PATH)/graphics/ofGraphics.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/graphics/ofGraphicsBaseTypes.cpp $(JNI_SRC_PATH)/graphics/ofGraphicsBaseTypes.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/graphics/ofImage.cpp $(JNI_SRC_PATH)/graphics/ofImage.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/graphics/ofPath.cpp $(JNI_SRC_PATH)/graphics/ofPath.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/graphics/ofPixels.cpp $(JNI_SRC_PATH)/graphics/ofPixels.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/graphics/ofRendererCollection.cpp $(JNI_SRC_PATH)/graphics/ofRendererCollection.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/graphics/ofTessellator.cpp $(JNI_SRC_PATH)/graphics/ofTessellator.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/graphics/ofTrueTypeFont.cpp $(JNI_SRC_PATH)/graphics/ofTrueTypeFont.h


# math
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/math/ofMath.cpp $(JNI_SRC_PATH)/math/ofMath.h 
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/math/ofMathConstants.cpp $(JNI_SRC_PATH)/math/ofMathConstants.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/math/ofMatrix3x3.cpp $(JNI_SRC_PATH)/math/ofMatrix3x3.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/math/ofMatrix4x4.cpp $(JNI_SRC_PATH)/math/ofMatrix4x4.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/math/ofQuaternion.cpp $(JNI_SRC_PATH)/math/ofQuaternion.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/math/ofVec2f.cpp $(JNI_SRC_PATH)/math/ofVectorMath.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/math/ofVec3f.cpp $(JNI_SRC_PATH)/math/ofVec3f.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/math/ofVec4f.cpp $(JNI_SRC_PATH)/math/ofVec4f.h


# sound
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/sound/ofSoundBaseTypes.h $(JNI_SRC_PATH)/sound/ofSoundUtils.h 
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/sound/ofSoundBuffer.cpp $(JNI_SRC_PATH)/sound/ofSoundBuffer.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/sound/ofSoundPlayer.cpp $(JNI_SRC_PATH)/sound/ofSoundPlayer.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/sound/ofSoundStream.cpp $(JNI_SRC_PATH)/sound/ofSoundStream.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/sound/ofOpenALSoundPlayer.cpp $(JNI_SRC_PATH)/sound/ofOpenALSoundPlayer.h

# types
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/types/ofBaseTypes.h 
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/types/ofColor.cpp $(JNI_SRC_PATH)/types/ofParameter.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/types/ofParameter.cpp $(JNI_SRC_PATH)/types/ofParameter.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/types/ofParameterGroup.cpp $(JNI_SRC_PATH)/types/ofParameterGroup.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/types/ofPoint.cpp $(JNI_SRC_PATH)/types/ofPoint.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/types/ofRectangle.h 
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/types/ofTypes.h 

# utils
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/utils/ofConstants.h 
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/utils/ofFileUtils.cpp $(JNI_SRC_PATH)/utils/ofFileUtils.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/utils/ofFpsCounter.cpp $(JNI_SRC_PATH)/utils/ofFpsCounter.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/utils/ofJson.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/utils/ofLog.cpp $(JNI_SRC_PATH)/utils/ofLog.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/utils/ofMatrixStack.cpp $(JNI_SRC_PATH)/utils/ofMatrixStack.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/utils/ofNoise.h 
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/utils/ofSystemUtils.cpp $(JNI_SRC_PATH)/utils/ofSystemUtils.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/utils/ofThread.cpp $(JNI_SRC_PATH)/utils/ofThread.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/utils/ofThreadChannel.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/utils/ofTimer.cpp $(JNI_SRC_PATH)/utils/ofTimer.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/utils/ofURLFileLoader.cpp $(JNI_SRC_PATH)/utils/ofURLFileLoader.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/utils/ofUtils.cpp $(JNI_SRC_PATH)/utils/ofUtils.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/utils/ofXml.cpp $(JNI_SRC_PATH)/utils/ofXml.h


# video
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/video/ofVideoBaseTypes.h 
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/video/ofVideoGrabber.cpp $(JNI_SRC_PATH)/video/ofVideoGrabber.h
LOCAL_SRC_FILES := $(JNI_SRC_PATH)/video/ofVideoPlayer.cpp $(JNI_SRC_PATH)/video/ofVideoPlayer.h

#include $(PREBUILT_SHARED_LIBRARY)
include $(BUILD_STATIC_LIBRARY)

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

# include $(CLEAR_VARS)
# LOCAL_MODULE := glm
# #LOCAL_SRC_FILES := $(LIBS_ROOT)/glm/lib/android/$(TARGET_ARCH_ABI)/libglm.a
# LOCAL_EXPORT_C_INCLUDES := $(LIBS_ROOT)/glm/include
# include $(PREBUILT_STATIC_LIBRARY)

# include $(CLEAR_VARS)
# LOCAL_MODULE := json
# #LOCAL_SRC_FILES := $(LIBS_ROOT)/json/lib/android/$(TARGET_ARCH_ABI)/libjson.a
# LOCAL_EXPORT_C_INCLUDES := $(LIBS_ROOT)/json/include
# include $(PREBUILT_STATIC_LIBRARY)

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

