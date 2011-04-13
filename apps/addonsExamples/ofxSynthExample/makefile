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
#
# this should work with any OF app, just copy any example
# change the name of the folder and it should compile
# only .cpp support, don't use .c files
# it will look for files in any folder inside the application
# folder except that in the EXCLUDE_FROM_SOURCE variable
# it doesn't autodetect include paths yet
# add the include paths in the USER_CFLAGS variable
# using the gcc syntax: -Ipath
#
# to add addons to your application, edit the addons.make file
# in this directory and add the names of the addons you want to
# include
#
# edit the following  vars to customize the makefile

include config.make
include $(OF_ROOT)/libs/openFrameworksCompiled/project/android/paths.make

ifeq ($(findstring Android,$(MAKECMDGOALS)),Android)
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
	COMPILER_OPTIMIZATION = $(USER_COMPILER_OPTIMIZATION)
endif
EXCLUDE_FROM_SOURCE="bin,.xcodeproj,obj"




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
	ifeq ($(findstring Release_arm7,$(MAKECMDGOALS)),Release_arm7)
		LIBSPATH =android/armeabi-v7a
	else
		LIBSPATH =android/armeabi
	endif
	#NDK_ROOT = $(shell cat $(OF_ROOT)/libs/openFrameworksCompiled/project/android/ndk_path.make)
	#SDK_ROOT = $(shell cat $(OF_ROOT)/libs/openFrameworksCompiled/project/android/sdk_path.make)
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

NODEPS = clean
SED_EXCLUDE_FROM_SRC = $(shell echo  $(EXCLUDE_FROM_SOURCE) | sed s/\,/\\\\\|/g)
SOURCE_DIRS = $(shell find . -maxdepth 1 -mindepth 1 -type d | grep -v $(SED_EXCLUDE_FROM_SRC) | sed s/.\\///)
SOURCES = $(shell find $(SOURCE_DIRS) -name "*.cpp" -or -name "*.c")
OBJFILES = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))
APPNAME = $(shell basename `pwd`)
CORE_INCLUDES = $(shell find $(OF_ROOT)/libs/openFrameworks/ -type d)
CORE_INCLUDE_FLAGS = $(addprefix -I,$(CORE_INCLUDES))
INCLUDES = $(shell find $(OF_ROOT)/libs/*/include -type d | grep -v glu | grep -v quicktime | grep -v poco)
INCLUDES_FLAGS = $(addprefix -I,$(INCLUDES))
INCLUDES_FLAGS += -I$(OF_ROOT)/libs/poco/include
ifeq ($(ARCH),android)
	INCLUDES_FLAGS += -I$(OF_ROOT)/libs/glu/include_android
else
 	INCLUDES_FLAGS += -I$(OF_ROOT)/libs/glu/include
	INCLUDES_FLAGS += $(shell pkg-config  glew gstreamer-0.10 gstreamer-video-0.10 gstreamer-base-0.10 libudev --cflags)
	#check if gtk exists and add it
	GTK = $(shell pkg-config gtk+-2.0 --exists; echo $$?)
	ifeq ($(GTK),0)
		CFLAGS += $(shell pkg-config gtk+-2.0 --cflags) -DOF_USING_GTK
		LIBS += $(shell pkg-config gtk+-2.0 --libs)
	endif

	#check if mpg123 exists and add it
	MPG123 = $(shell pkg-config libmpg123 --exists; echo $$?)
	ifeq ($(MPG123),0)
		CFLAGS += -DOF_USING_MPG123
		LIBS += -lmpg123
	endif
endif
LIB_STATIC = $(shell ls $(OF_ROOT)/libs/*/lib/$(LIBSPATH)/*.a | grep -v openFrameworksCompiled | grep -v poco| sed "s/.*\\/lib\([^/]*\)\.a/-l\1/")
LIB_SHARED = $(shell ls $(OF_ROOT)/libs/*/lib/$(LIBSPATH)/*.so | grep -v openFrameworksCompiled| sed "s/.*\\/lib\([^/]*\)\.so/-l\1/")
LIB_STATIC += $(OF_ROOT)/libs/poco/lib/$(LIBSPATH)/libPocoNet.a ../../../libs/poco/lib/$(LIBSPATH)/libPocoXML.a ../../../libs/poco/lib/$(LIBSPATH)/libPocoUtil.a ../../../libs/poco/lib/$(LIBSPATH)/libPocoFoundation.a
LIB_PATHS_FLAGS = $(shell ls -d $(OF_ROOT)/libs/*/lib/$(LIBSPATH) | sed "s/\(\.*\)/-L\1/")
ifeq ($(ARCH),android)
	LIB_PATHS_FLAGS += -L$(OF_ROOT)/libs/openFrameworksCompiled/lib/android
endif

CFLAGS += -Wall -fexceptions
CFLAGS += -I.
CFLAGS += $(INCLUDES_FLAGS)
CFLAGS += $(CORE_INCLUDE_FLAGS)



LIBS += $(LIB_SHARED)
LIBS += $(LIB_STATIC)
ifeq ($(ARCH),android)
	LDFLAGS = $(LIB_PATHS_FLAGS) --sysroot=$(SYSROOT) -nostdlib -L"$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/libs/armeabi"
	LIBS += -lstdc++ -lsupc++ -lGLU -lgcc -lz -lGLESv1_CM -llog -ldl -lm -lc
else
	LDFLAGS = $(LIB_PATHS_FLAGS) -Wl,-rpath=./libs
	LIBS += $(shell pkg-config  glew gstreamer-0.10 gstreamer-video-0.10 gstreamer-base-0.10 gstreamer-app-0.10 libudev --libs)
	LIBS += -lglut -lGL -lGLU -lasound -lopenal -lsndfile -lvorbis -lFLAC -logg -lfreeimage
endif


ifeq ($(findstring addons.make,$(wildcard *.make)),addons.make)
	ADDONS_INCLUDES = $(shell find $(OF_ROOT)/addons/*/src/ -type d)
	ADDONS_INCLUDES += $(shell find $(OF_ROOT)/addons/*/libs/ -type d)
	ADDONSCFLAGS = $(addprefix -I,$(ADDONS_INCLUDES))

	ifeq ($(findstring libsorder.make,$(wildcard $(OF_ROOT)/addons/*/libs/*/lib/$(LIBSPATH)/libsorder.make)),libsorder.make)
		ADDONS_LIBS_W_ORDER = $(shell cat $(OF_ROOT)/addons/*/libs/*/lib/$(LIBSPATH)/libsorder.make)
	endif

	SED_EXCLUDE_LIBS = $(addsuffix \ \\\| , $(ADDONS_LIBS_W_ORDER))
	ADDONS_LIBS_STATICS = $(shell ls $(OF_ROOT)/addons/*/libs/*/lib/$(LIBSPATH)/*.a | grep -v "$(SED_EXCLUDE_LIBS) null")
	ADDONS_LIBS_STATICS += $(addprefix -l, $(ADDONS_LIBS_W_ORDER))
	ADDONS_LIBS_STATICS += $(addprefix -L, $(shell ls $(OF_ROOT)/addons/*/libs/*/lib/$(LIBSPATH)/libsorder.make | sed s/libsorder.make//g))
	ADDONS_LIBS_SHARED = $(shell ls $(OF_ROOT)/addons/*/libs/*/lib/$(LIBSPATH)/*.so)
	ADDONSLIBS = $(ADDONS_LIBS_STATICS)
	ADDONSLIBS += $(ADDONS_LIBS_SHARED)

	ADDONS = $(shell cat addons.make)
	ADDONS_REL_DIRS = $(addsuffix /src, $(ADDONS))
	ADDONS_LIBS_REL_DIRS = $(addsuffix /libs, $(ADDONS))
	ADDONS_DIRS = $(addprefix $(OF_ROOT)/addons/, $(ADDONS_REL_DIRS) )
	ADDONS_LIBS_DIRS = $(addprefix $(OF_ROOT)/addons/, $(ADDONS_LIBS_REL_DIRS) )
	ADDONS_SOURCES = $(shell find $(ADDONS_DIRS) -name "*.cpp" -or -name "*.c")
	ADDONS_SOURCES += $(shell find $(ADDONS_LIBS_DIRS) -name "*.cpp" -or -name "*.c" 2>/dev/null)
	ADDONS_OBJFILES = $(subst $(OF_ROOT)/, ,$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(ADDONS_SOURCES))))
endif


ifeq ($(findstring Debug,$(MAKECMDGOALS)),Debug)
	TARGET_CFLAGS = -g
	TARGET_LIBS = -lopenFrameworksDebug
	TARGET_NAME = Debug
endif

ifeq ($(findstring Release,$(MAKECMDGOALS)),Release)
	TARGET_CFLAGS = $(COMPILER_OPTIMIZATION)
	TARGET_LIBS = -lopenFrameworks
	TARGET_NAME = Release
endif

ifeq ($(ARCH),android)
	ifeq ($(findstring Debug,$(MAKECMDGOALS)),Debug)
		TARGET = libs/armeabi/libOFAndroidApp.so
		TARGET_LIBS = -lopenFrameworksDebug
		LDFLAGS += -Wl,--fix-cortex-a8 -shared
	endif

	ifeq ($(findstring Release,$(MAKECMDGOALS)),Release)
	    TARGET = libs/armeabi/libOFAndroidApp.so
		TARGET_LIBS = -lopenFrameworks
		LDFLAGS += -Wl,--fix-cortex-a8 -shared
	endif

	ifeq ($(findstring Release_arm7,$(MAKECMDGOALS)),Release_arm7)
		TARGET_NAME = Release_arm7
		TARGET_CFLAGS += -march=armv7-a -mfloat-abi=softfp -mfpu=neon
	    TARGET = libs/armeabi-v7a/libOFAndroidApp.so
		TARGET_LIBS = -lopenFrameworks_arm7
		#LDFLAGS += -shared  already included through Release
	endif

	ifeq ($(findstring TestLink,$(MAKECMDGOALS)),TestLink)
		TARGET_NAME = Debug
		TARGET_LIBS = -lopenFrameworksDebug
		LDFLAGS += -Wl,--entry=main,--fix-cortex-a8
	    BIN_NAME = $(APPNAME)
	    TARGET = obj/$(BIN_NAME)
	endif
else
	ifeq ($(findstring Debug,$(MAKECMDGOALS)),Debug)
	    BIN_NAME = $(APPNAME)_debug
		TARGET = bin/$(BIN_NAME)
	endif

	ifeq ($(findstring Release,$(MAKECMDGOALS)),Release)
	    BIN_NAME = $(APPNAME)
		TARGET = bin/$(BIN_NAME)
	endif

	ifeq ($(MAKECMDGOALS),)
		TARGET_NAME = Release
	    BIN_NAME = $(APPNAME)
		TARGET = bin/$(BIN_NAME)
		TARGET_LIBS = -lopenFrameworks
	endif
endif

ifeq ($(MAKECMDGOALS),clean)
	TARGET = bin/$(APPNAME)_debug bin/$(APPNAME)
endif

OBJ_OUTPUT = obj/$(TARGET_NAME)/
CLEANTARGET = clean$(TARGET_NAME)
OBJS = $(addprefix $(OBJ_OUTPUT), $(OBJFILES))
DEPFILES = $(patsubst %.o,%.d,$(OBJS))
ifeq ($(findstring addons.make,$(wildcard *.make)),addons.make)
	ADDONS_OBJS = $(addprefix $(OBJ_OUTPUT), $(ADDONS_OBJFILES))
    DEPFILES += $(patsubst %.o,%.d,$(ADDONS_OBJS))
endif

.PHONY: Debug Release all after afterDebugAndroid afterReleaseAndroid

Release: $(TARGET) after

Debug: $(TARGET) after

all:
	$(MAKE) Release



DebugAndroid: $(TARGET)

ReleaseAndroid: $(TARGET)

Release_arm7Android: $(TARGET) afterReleaseAndroid

TestLinkAndroid: $(TARGET) afterDebugAndroid

AndroidDebug:
	$(MAKE) DebugAndroid
	$(MAKE) TestLinkAndroid

AndroidRelease:
	$(MAKE) ReleaseAndroid
	$(MAKE) Release_arm7Android


#This rule does the compilation
#$(OBJS): $(SOURCES)
$(OBJ_OUTPUT)%.o: %.cpp
	@echo "compiling object for: " $<
	mkdir -p $(@D)
	$(CXX) -c $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.d -o$@ -c $<

$(OBJ_OUTPUT)%.o: %.c
	@echo "compiling object for: " $<
	mkdir -p $(@D)
	$(CC) -c $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.d -o$@ -c $<


$(OBJ_OUTPUT)%.o: $(OF_ROOT)/%.cpp
	@echo "compiling addon object for" $<
	mkdir -p $(@D)
	$(CXX) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.d -o $@ -c $<

$(OBJ_OUTPUT)%.o: $(OF_ROOT)/%.c
	@echo "compiling addon object for" $<
	mkdir -p $(@D)
	$(CC) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.d -o $@ -c $<

$(TARGET): $(OBJS) $(ADDONS_OBJS)
	@echo "linking" $(TARGET) $(GTK)
	$(CXX) -o $@ $(OBJS) $(ADDONS_OBJS) $(LDFLAGS) $(USER_LDFLAGS) $(TARGET_LIBS) $(ADDONSLIBS) $(USER_LIBS) $(LIBS)

-include $(DEPFILES)

.PHONY: clean cleanDebug cleanRelease CleanAndroid
clean:
	rm -Rf obj
	rm -f -v $(TARGET)
	rm -Rf -v bin/libs

$(CLEANTARGET):
	rm -Rf -v $(OBJ_OUTPUT)
	rm -f -v $(TARGET)

CleanAndroid:
	rm -Rf obj
	rm -f -v libs/armeabi-v7a/libOFAndroidApp.so
	rm -f -v libs/armeabi/libOFAndroidApp.so
	rm -f -v obj/$(APPNAME)


afterDebugAndroid:$(TARGET)
	if [ -f libs/armeabi-v7a/libOFAndroidApp.so ]; then rm libs/armeabi-v7a/libOFAndroidApp.so; fi
	#touch AndroidManifest.xml

afterReleaseAndroid:$(TARGET)
	if [ -f obj/$(BIN_NAME) ]; then rm obj/$(BIN_NAME); fi
	#touch AndroidManifest.xml

AndroidInstall:
	if [ -f obj/$(BIN_NAME) ]; then rm obj/$(BIN_NAME); fi
	#touch AndroidManifest.xml
	$(SDK_ROOT)/tools/android update project --target $(NDK_PLATFORM) --path $(PROJECT_PATH)
	ant debug
	cp bin/OFActivity-debug.apk bin/$(APPNAME).apk
	#if [ "$(shell $(SDK_ROOT)/platform-tools/adb get-state)" = "device" ]; then
	$(SDK_ROOT)/platform-tools/adb install -r bin/$(APPNAME).apk;
	#fi
	$(SDK_ROOT)/platform-tools/adb shell am start -a android.intent.action.MAIN -n cc.openframeworks.$(APPNAME)/cc.openframeworks.$(APPNAME).OFActivity

after:$(TARGET)
	cp -r $(OF_ROOT)/export/$(LIBSPATH)/libs bin/
	@echo
	@echo "     compiling done"
	@echo "     to launch the application"
	@echo
	@echo "     cd bin"
	@echo "     ./$(BIN_NAME)"
	@echo


.PHONY: help
help:
	@echo
	@echo openFrameworks universal makefile
	@echo
	@echo targets:
	@echo "make Debug:		builds the application with debug symbols"
	@echo "make Release:		builds the app with optimizations"
	@echo "make:			= make Release"
	@echo "make all:		= make Release"
	@echo "make CleanDebug:	cleans the Debug target"
	@echo "make CleanRelease:	cleans the Release target"
	@echo "make clean:		cleans everything"
	@echo
	@echo this should work with any OF app, just copy any example
	@echo change the name of the folder and it should compile
	@echo "only .cpp support, don't use .c files"
	@echo it will look for files in any folder inside the application
	@echo folder except that in the EXCLUDE_FROM_SOURCE variable.
	@echo "it doesn't autodetect include paths yet"
	@echo "add the include paths editing the var USER_CFLAGS"
	@echo at the beginning of the makefile using the gcc syntax:
	@echo -Ipath
	@echo
	@echo to add addons to your application, edit the addons.make file
	@echo in this directory and add the names of the addons you want to
	@echo include
	@echo
