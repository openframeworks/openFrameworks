# openFrameworks universal makefile
#
# make help : shows this message
# make Debug:  makes the application with debug symbols
# make Release: makes the app with optimizations
# make: the same as make Release
# make CleanDebug: cleans the Debug target
# make CleanRelease: cleans the Release target
# make clean: cleans everything
#
# edit the following  vars to customize the makefile



ifeq ($(findstring Android,$(MAKECMDGOALS)),Android)
	include paths.make
	ARCH = android
	ifeq ($(shell uname),Darwin)
		HOST_PLATFORM = darwin-x86
	else
		HOST_PLATFORM = linux-x86
	endif
endif

ifeq ($(ARCH),android)
	COMPILER_OPTIMIZATION = -Os
	NDK_PLATFORM = android-8
else
	COMPILER_OPTIMIZATION = -march=native -mtune=native -finline-functions -funroll-all-loops  -Os
endif
USER_CFLAGS = 





# you shouldn't modify anything below this line




SHELL =  /bin/sh

ifneq ($(ARCH),android)
	CXX =  g++
	ARCH = $(shell uname -m)
	ifeq ($(ARCH),x86_64)
		LIBSPATH=linux64
	else
		LIBSPATH=linux
	endif
else
	LIBSPATH = android
	#NDK_ROOT = $(shell cat ndk_path.make)
	TOOLCHAIN=arm-linux-androideabi-4.4.3
	TOOLCHAIN_PATH=$(NDK_ROOT)/toolchains/$(TOOLCHAIN)/prebuilt/$(HOST_PLATFORM)/bin/
	ANDROID_PREFIX=arm-linux-androideabi-
	CC=$(NDK_ROOT)/toolchains/$(TOOLCHAIN)/prebuilt/$(HOST_PLATFORM)/bin/$(ANDROID_PREFIX)gcc
	CXX=$(NDK_ROOT)/toolchains/$(TOOLCHAIN)/prebuilt/$(HOST_PLATFORM)/bin/$(ANDROID_PREFIX)g++
	AR=$(NDK_ROOT)/toolchains/$(TOOLCHAIN)/prebuilt/$(HOST_PLATFORM)/bin/$(ANDROID_PREFIX)ar
	SYSROOT=$(NDK_ROOT)/platforms/$(NDK_PLATFORM)/arch-arm/
	CFLAGS += -nostdlib --sysroot=$(SYSROOT) -fno-short-enums 
	CFLAGS += -I"$(NDK_ROOT)/platforms/$(NDK_PLATFORM)/arch-arm/usr/include" -I"$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/include/" -I"$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/libs/armeabi/include"
	CFLAGS += -DANDROID
endif

SOURCE_DIRS = $(shell find ../../../openFrameworks -maxdepth 1 -mindepth 1 -type d )
ifeq ($(ARCH),android)
	SOURCES = $(shell find $(SOURCE_DIRS) -name "*.cpp" | grep -v "ofAppGlutWindow.cpp" | grep -v ofGstVideoPlayer.cpp | grep -v ofQuickTimeGrabber.cpp | grep -v ofVideoPlayer.cpp | grep -v ofGstVideoGrabber.cpp | grep -v ofOpenALSoundPlayer.cpp)
else
	SOURCES = $(shell find $(SOURCE_DIRS) -name "*.cpp") 
endif
OBJFILES = $(patsubst ../../../%.cpp,%.o,$(SOURCES))
CORE_INCLUDES = $(shell find ../../../openFrameworks/ -type d)
CORE_INCLUDE_FLAGS = $(addprefix -I,$(CORE_INCLUDES))
INCLUDES = $(shell find ../../../*/include -type d | grep -v glu | grep -v quicktime | grep -v poco)
INCLUDES_FLAGS = $(addprefix -I,$(INCLUDES))
INCLUDES_FLAGS += -I../../../poco/include
ifeq ($(ARCH),android)
	INCLUDES_FLAGS += -I../../../glu/include_android
 	INCLUDES_FLAGS += -I../../../../addons/ofxAndroid/src
else
 	INCLUDES_FLAGS += -I../../../glu/include
endif

CFLAGS += -Wall -fexceptions
CFLAGS += $(INCLUDES_FLAGS)
CFLAGS += $(CORE_INCLUDE_FLAGS)


ifneq ($(ARCH),android)
	CFLAGS += $(shell pkg-config  gstreamer-0.10 gstreamer-video-0.10 gstreamer-base-0.10 libudev --cflags)
	#check if gtk exists and add it
	GTK = $(shell pkg-config gtk+-2.0 --exists; echo $$?)
	ifeq ($(GTK),0)
		CFLAGS += $(shell pkg-config gtk+-2.0 --cflags) -DOF_USING_GTK
	endif
	#check if mpg123 exists and add it
	MPG123 = $(shell pkg-config libmpg123 --exists; echo $$?)
	ifeq ($(MPG123),0)
		CFLAGS += -DOF_USING_MPG123
	endif
endif

ifeq ($(findstring Debug,$(MAKECMDGOALS)),Debug)
	TARGET_CFLAGS = -g3
	TARGET_NAME = $(MAKECMDGOALS)
	TARGET = ../../lib/$(LIBSPATH)/libopenFrameworksDebug.a
endif

ifeq ($(findstring Release,$(MAKECMDGOALS)),Release)
	TARGET_CFLAGS = $(COMPILER_OPTIMIZATION)
	TARGET_NAME = $(MAKECMDGOALS)
	TARGET = ../../lib/$(LIBSPATH)/libopenFrameworks.a
endif

ifeq ($(findstring Release_arm7,$(MAKECMDGOALS)),Release_arm7)
	TARGET_CFLAGS += -march=armv7-a -mfloat-abi=softfp 
	TARGET = ../../lib/$(LIBSPATH)/libopenFrameworks_arm7.a
endif

ifeq ($(findstring Release_neon,$(MAKECMDGOALS)),Release_neon)
	TARGET_CFLAGS += -march=armv7-a -mfloat-abi=softfp -mfpu=neon
	TARGET = ../../lib/$(LIBSPATH)/libopenFrameworks_neon.a
endif

ifeq ($(MAKECMDGOALS),)
	TARGET_CFLAGS = $(COMPILER_OPTIMIZATION)
	TARGET_NAME = Release
	TARGET = ../../lib/$(LIBSPATH)/libopenFrameworks.a
endif

ifeq ($(MAKECMDGOALS),clean)
	TARGET = ../../lib/$(LIBSPATH)/libopenFrameworks.a ../../lib/$(LIBSPATH)/libopenFrameworksDebug.a
endif

ifeq ($(MAKECMDGOALS),CleanAndroid)
	TARGET = ../../lib/$(LIBSPATH)/libopenFrameworks.a ../../lib/$(LIBSPATH)/libopenFrameworksDebug.a ../../lib/$(LIBSPATH)/libopenFrameworks_arm7.a ../../lib/$(LIBSPATH)/libopenFrameworks_neon.a
endif

CLEANTARGET = $(addprefix Clean,$(TARGET_NAME))
OBJ_OUTPUT = obj/$(TARGET_NAME)/
DEPFILES = $(addprefix $(OBJ_OUTPUT),$(patsubst ../../../%.cpp,%.d,$(SOURCES)))
OBJS = $(addprefix $(OBJ_OUTPUT), $(OBJFILES))

.PHONY: all Debug Release after clean CleanDebug CleanRelease help DebugAndroid ReleaseAndroid Release_arm7Android Release_neonAndroid CleanAndroid

Release: $(TARGET) 

Debug: $(TARGET) 

DebugAndroid: $(TARGET)

ReleaseAndroid: $(TARGET)

Release_arm7Android: $(TARGET)

Release_neonAndroid: $(TARGET)

all: 
	$(MAKE) Debug
	$(MAKE) Release
	
android:
	$(MAKE) DebugAndroid
	$(MAKE) ReleaseAndroid
	$(MAKE) Release_arm7Android
	$(MAKE) Release_neonAndroid

#This rule does the compilation
$(OBJ_OUTPUT)%.o: ../../../%.cpp 
	@echo "compiling object for " $<
	mkdir -p $(@D)
	$(CXX) $(TARGET_CFLAGS) $(CFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.d -o $@ -c $<

$(TARGET) : $(OBJS) 
	echo "creating " $(TARGET)
	$(AR) -r "$@" $(OBJS)

-include $(DEPFILES)

#.PHONY: clean CleanDebug CleanRelease
clean:
	rm -Rf obj
	rm -f -v $(TARGET)
	
$(CLEANTARGET):
	rm -Rf -v $(OBJ_OUTPUT)
	rm -f -v $(TARGET)
	
CleanAndroid:
	rm -Rf obj
	rm -f -v $(TARGET)

#.PHONY: help
help:
 
	@echo 
	@echo openFrameworks compiled library makefile
	@echo
	@echo targets:
	@echo "make Debug:		builds the library with debug symbols"
	@echo "make Release:		builds the library with optimizations"
	@echo "make:			= make Release"
	@echo "make all:		= make Debug + make Release"
	@echo "make CleanDebug:	cleans the Debug target"
	@echo "make CleanRelease:	cleans the Release target"
	@echo "make clean:		cleans everything"
	@echo
