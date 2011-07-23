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

ifeq ($(findstring Android,$(MAKECMDGOALS)),Android)
	include $(OF_ROOT)/libs/openFrameworksCompiled/project/android/paths.make
	ARCH = android
	ifeq ($(shell uname),Darwin)
		HOST_PLATFORM = darwin-x86
	else
		HOST_PLATFORM = linux-x86
	endif
endif

ifeq ($(ARCH),android)
	COMPILER_OPTIMIZATION = $(ANDROID_COMPILER_OPTIMIZATION)
	NDK_PLATFORM = android-8
else
	COMPILER_OPTIMIZATION = $(USER_COMPILER_OPTIMIZATION)
endif




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
SOURCES = $(shell find $(SOURCE_DIRS) -name "*.cpp" -or -name "*.c" -or -name "*.cc")
OBJFILES = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(SOURCES))))

ifneq (,$(USER_SOURCE_DIR))
	USER_SOURCES = $(shell find $(USER_SOURCE_DIR) -name "*.cpp" -or -name "*.c" -or -name "*.cc")
	USER_OBJFILES =  $(subst $(USER_SOURCE_DIR)/, ,$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(USER_SOURCES)))))
endif

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
		SYSTEMLIBS += $(shell pkg-config gtk+-2.0 --libs)
	endif

	#check if mpg123 exists and add it
	MPG123 = $(shell pkg-config libmpg123 --exists; echo $$?)
	ifeq ($(MPG123),0)
		CFLAGS += -DOF_USING_MPG123
		SYSTEMLIBS += -lmpg123
	endif
endif
LIB_STATIC = $(shell ls $(OF_ROOT)/libs/*/lib/$(LIBSPATH)/*.a  2> /dev/null | grep -v openFrameworksCompiled | grep -v Poco)
LIB_SHARED = $(shell ls $(OF_ROOT)/libs/*/lib/$(LIBSPATH)/*.so  2> /dev/null | grep -v openFrameworksCompiled | sed "s/.*\\/lib\([^/]*\)\.so/-l\1/")
LIB_STATIC += $(OF_ROOT)/libs/poco/lib/$(LIBSPATH)/libPocoNet.a ../../../libs/poco/lib/$(LIBSPATH)/libPocoXML.a ../../../libs/poco/lib/$(LIBSPATH)/libPocoUtil.a ../../../libs/poco/lib/$(LIBSPATH)/libPocoFoundation.a
LIB_PATHS_FLAGS = $(shell ls -d $(OF_ROOT)/libs/*/lib/$(LIBSPATH) | sed "s/\(\.*\)/-L\1/")

CFLAGS += -Wall -fexceptions
CFLAGS += -I.
CFLAGS += $(INCLUDES_FLAGS)
CFLAGS += $(CORE_INCLUDE_FLAGS)



ifeq ($(ARCH),android)
	LDFLAGS = --sysroot=$(SYSROOT) -nostdlib -L"$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/libs/armeabi"
	SYSTEMLIBS += -lstdc++ -lsupc++ -lgcc -lz -lGLESv1_CM -llog -ldl -lm -lc
else
	LDFLAGS = -Wl,-rpath=./libs
	SYSTEMLIBS += $(shell pkg-config  jack glew gstreamer-0.10 gstreamer-video-0.10 gstreamer-base-0.10 gstreamer-app-0.10 libudev --libs)
	SYSTEMLIBS += -lglut -lGL -lasound -lopenal -lsndfile -lvorbis -lFLAC -logg -lfreeimage
endif


ifeq ($(findstring addons.make,$(wildcard *.make)),addons.make)
	ifneq ($(ARCH),android)
		ADDONS = $(shell cat addons.make | grep -v ofxAndroid)
	else
		ADDONS = $(shell cat addons.make)
	endif
	
	ifneq ($(strip $(ADDONS)),)
	    ADDONS_REL_DIRS = $(addsuffix /src, $(ADDONS))
	    ADDONS_LIBS_REL_DIRS = $(addsuffix /libs, $(ADDONS))
	    ADDONS_DIRS = $(addprefix $(OF_ROOT)/addons/, $(ADDONS_REL_DIRS) )
	    ADDONS_LIBS_DIRS = $(addprefix $(OF_ROOT)/addons/, $(ADDONS_LIBS_REL_DIRS) )
	    ADDONS_BIN_LIBS_DIRS = $(addsuffix /*/lib/$(LIBSPATH), $(ADDONS_LIBS_DIRS) )
	
	    ADDONS_INCLUDES = $(ADDONS_DIRS)
	    ADDONS_INCLUDES = $(ADDONS_LIBS_DIRS)
	    ADDONS_INCLUDES += $(shell find $(ADDONS_DIRS) -type d 2> /dev/null)
	    ADDONS_INCLUDES += $(shell find $(ADDONS_LIBS_DIRS) -type d 2> /dev/null)
	    ADDONSCFLAGS = $(addprefix -I,$(ADDONS_INCLUDES))

	    ifeq ($(findstring libsorder.make,$(shell find $(ADDONS_BIN_LIBS_DIRS) -name libsorder.make 2> /dev/null)),libsorder.make)
		    ADDONS_LIBS_W_ORDER = $(shell cat $(shell find $(ADDONS_BIN_LIBS_DIRS) -name libsorder.make 2> /dev/null))
		    EXCLUDE_LIBS_FILTER = $(addprefix %,$(addsuffix .a,$(ADDONS_LIBS_W_ORDER)))
		    ADDONS_LIBS_STATICS = $(filter-out $(EXCLUDE_LIBS_FILTER), $(shell find $(ADDONS_BIN_LIBS_DIRS) -name *.a))
	        ADDONS_LIBS_STATICS += $(addprefix -l, $(ADDONS_LIBS_W_ORDER))
        	ADDONS_LIBS_STATICS += $(addprefix -L, $(shell find $(ADDONS_BIN_LIBS_DIRS) -name libsorder.make 2> /dev/null | sed s/libsorder.make//g))
	    else
		    ADDONS_LIBS_STATICS = $(shell find $(ADDONS_BIN_LIBS_DIRS) -name *.a 2> /dev/null)
	    endif

	    ADDONS_LIBS_SHARED = $(shell find $(ADDONS_BIN_LIBS_DIRS) -name *.so 2> /dev/null)
	    ADDONSLIBS = $(ADDONS_LIBS_STATICS)
	    ADDONSLIBS += $(ADDONS_LIBS_SHARED)


	    ADDONS_SOURCES = $(shell find $(ADDONS_DIRS) -name "*.cpp" -or -name "*.c" 2> /dev/null)
	    ADDONS_SOURCES += $(shell find $(ADDONS_LIBS_DIRS) -name "*.cpp" -or -name "*.c" -or -name "*.cc" 2>/dev/null)
	    ADDONS_OBJFILES = $(subst $(OF_ROOT)/, ,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(ADDONS_SOURCES)))))
	endif
endif


ifeq ($(findstring Debug,$(MAKECMDGOALS)),Debug)
	TARGET_CFLAGS = -g
	TARGET_LIBS = $(OF_ROOT)/libs/openFrameworksCompiled/lib/$(LIBSPATH)/libopenFrameworksDebug.a
	TARGET_NAME = Debug
endif

ifeq ($(findstring Release,$(MAKECMDGOALS)),Release)
	TARGET_CFLAGS = $(COMPILER_OPTIMIZATION)
	TARGET_LIBS = $(OF_ROOT)/libs/openFrameworksCompiled/lib/$(LIBSPATH)/libopenFrameworks.a
	TARGET_NAME = Release
endif

ifeq ($(ARCH),android)
	ifeq ($(findstring Debug,$(MAKECMDGOALS)),Debug)
		TARGET = libs/armeabi/libOFAndroidApp.so
		TARGET_LIBS = $(OF_ROOT)/libs/openFrameworksCompiled/lib/$(ARCH)/libopenFrameworksDebug.a
		LDFLAGS += -Wl,--fix-cortex-a8 -shared
		USER_LIBS = $(USER_LIBS_ARM)
	endif

	ifeq ($(findstring Release,$(MAKECMDGOALS)),Release)
	    TARGET = libs/armeabi/libOFAndroidApp.so
		TARGET_LIBS = $(OF_ROOT)/libs/openFrameworksCompiled/lib/$(ARCH)/libopenFrameworks.a
		LDFLAGS += -Wl,--fix-cortex-a8 -shared
		USER_LIBS = $(USER_LIBS_ARM)
	endif

	ifeq ($(findstring Release_arm7,$(MAKECMDGOALS)),Release_arm7)
		TARGET_NAME = Release_arm7
		TARGET_CFLAGS += -march=armv7-a -mfloat-abi=softfp -mthumb
	    TARGET = libs/armeabi-v7a/libOFAndroidApp.so
		TARGET_LIBS = $(OF_ROOT)/libs/openFrameworksCompiled/lib/$(ARCH)/libopenFrameworks_arm7.a
		USER_LIBS = $(USER_LIBS_ARM7)
	endif

	ifeq ($(findstring Release_neon,$(MAKECMDGOALS)),Release_neon)
		TARGET_NAME = Release_neon
		TARGET_CFLAGS += -march=armv7-a -mfloat-abi=softfp -mfpu=neon
	    TARGET = libs/armeabi-v7a/libOFAndroidApp_neon.so
		TARGET_LIBS = $(OF_ROOT)/libs/openFrameworksCompiled/lib/$(ARCH)/libopenFrameworks_neon.a
		USER_LIBS = $(USER_LIBS_NEON)
	endif
	
	ifeq ($(findstring TestLink,$(MAKECMDGOALS)),TestLink)
		TARGET_NAME = Debug
		TARGET_LIBS = $(OF_ROOT)/libs/openFrameworksCompiled/lib/$(ARCH)/libopenFrameworksDebug.a
		LDFLAGS += -Wl,--entry=main,--fix-cortex-a8
	    BIN_NAME = $(APPNAME)
	    TARGET = obj/$(BIN_NAME)
		USER_LIBS = $(USER_LIBS_ARM)
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
		TARGET_LIBS = $(OF_ROOT)/libs/openFrameworksCompiled/lib/$(LIBSPATH)/libopenFrameworks.a
	endif
endif

ifeq ($(MAKECMDGOALS),clean)
	TARGET = bin/$(APPNAME)_debug bin/$(APPNAME)
	TARGET_NAME = Release
endif


OBJ_OUTPUT = obj/$(ARCH)$(TARGET_NAME)/
CLEANTARGET = clean$(TARGET_NAME)

OBJS = $(addprefix $(OBJ_OUTPUT), $(OBJFILES))
DEPFILES = $(patsubst %.o,%.d,$(OBJS))

USER_OBJS = $(addprefix $(OBJ_OUTPUT), $(USER_OBJFILES))
DEPFILES += $(patsubst %.o,%.d,$(USER_OBJS))

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

Release_arm7Android: $(TARGET)

Release_neonAndroid: $(TARGET) afterReleaseAndroid

TestLinkAndroid: $(TARGET) afterDebugAndroid

AndroidDebug:
	$(MAKE) DebugAndroid
	$(MAKE) TestLinkAndroid

AndroidRelease:
	$(MAKE) ReleaseAndroid
	$(MAKE) Release_arm7Android
	$(MAKE) Release_neonAndroid


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

$(OBJ_OUTPUT)%.o: %.cc
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
	
$(OBJ_OUTPUT)%.o: $(OF_ROOT)/%.cc
	@echo "compiling addon object for" $<
	mkdir -p $(@D)
	$(CC) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.d -o $@ -c $<

$(OBJ_OUTPUT)%.o: $(USER_SOURCE_DIR)/%.c
	@echo "compiling object for: " $<
	mkdir -p $(@D)
	$(CC) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.d -o$@ -c $<
	
$(OBJ_OUTPUT)%.o: $(USER_SOURCE_DIR)/%.cc
	@echo "compiling object for: " $<
	mkdir -p $(@D)
	$(CC) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.d -o$@ -c $<

$(OBJ_OUTPUT)%.o: $(USER_SOURCE_DIR)/%.cpp
	@echo "compiling object for: " $<
	mkdir -p $(@D)
	$(CXX) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.d -o$@ -c $<
	
$(TARGET): $(OBJS) $(ADDONS_OBJS) $(USER_OBJS) $(TARGET_LIBS) $(LIB_STATIC)
	@echo 'linking $(TARGET)'
	mkdir -p $(@D)
	$(CXX) -o $@ $(OBJS) $(ADDONS_OBJS) $(USER_OBJS) $(LDFLAGS) $(USER_LDFLAGS) $(TARGET_LIBS) $(ADDONSLIBS) $(USER_LIBS) $(LIB_STATIC) $(LIB_PATHS_FLAGS) $(LIB_SHARED) $(SYSTEMLIBS)

-include $(DEPFILES)

.PHONY: clean cleanDebug cleanRelease CleanAndroid
clean:
	rm -rf $(OBJ_OUTPUT)
	rm -f $(TARGET)
	rm -r bin/libs
	
$(CLEANTARGET):
	rm -rf $(OBJ_OUTPUT)
	rm -f $(TARGET)
	rm -rf bin/libs

CleanAndroid:
	rm -Rf obj
	rm -f libs/armeabi-v7a/libOFAndroidApp.so
	rm -f libs/armeabi/libOFAndroidApp.so
	rm -f obj/$(APPNAME)


afterDebugAndroid:$(TARGET)
	@if [ -d libs/armeabi-v7a ]; then rm -r libs/armeabi-v7a; fi
	
	@cp $(NDK_ROOT)/toolchains/arm-linux-androideabi-4.4.3/prebuilt/gdbserver libs/armeabi
	
	#create gdb.setup for armeabi
	@echo "set solib-search-path $(PWD)/obj/local/armeabi:$(PWD)/libs/armeabi" > libs/armeabi/gdb.setup
	@echo "directory $(NDK_ROOT)/platforms/$(NDK_PLATFORM)/arch-arm/usr/include" >> libs/armeabi/gdb.setup
	@echo "directory $(PWD)/src" >> libs/armeabi/gdb.setup
	@echo "directory $(NDK_ROOT)/sources/cxx-stl/system" >> libs/armeabi/gdb.setup
	@echo "directory $(PWD)/libs/armeabi" >> libs/armeabi/gdb.setup 
	@echo "" >> libs/armeabi/gdb.setup 
	
	@if [ ! -d jni ]; then mkdir jni; fi
	@echo "APP_ABI := armeabi" > jni/Application.mk
	@echo "#LOCAL_MODULE := OFAndroidApp" > jni/Android.mk

afterReleaseAndroid:$(TARGET)
	@if [ -f obj/$(BIN_NAME) ]; then rm obj/$(BIN_NAME); fi
	
	@cp $(OF_ROOT)/libs/openFrameworksCompiled/project/android/libneondetection.so libs/armeabi-v7a/
	@cp $(NDK_ROOT)/toolchains/arm-linux-androideabi-4.4.3/prebuilt/gdbserver libs/armeabi-v7a
	
	#create gdb.setup for armeabi-v7a
	@echo "set solib-search-path $(PWD)/obj/local/armeabi-v7a:$(PWD)/libs/armeabi-v7a" > libs/armeabi-v7a/gdb.setup
	@echo "directory $(NDK_ROOT)/platforms/$(NDK_PLATFORM)/arch-arm/usr/include" >> libs/armeabi-v7a/gdb.setup
	@echo "directory $(PWD)/src" >> libs/armeabi-v7a/gdb.setup
	@echo "directory $(NDK_ROOT)/sources/cxx-stl/system" >> libs/armeabi-v7a/gdb.setup
	@echo "directory $(PWD)/libs/armeabi-v7a" >> libs/armeabi-v7a/gdb.setup 
	@echo "" >> libs/armeabi-v7a/gdb.setup 
	
	@if [ ! -d jni ]; then mkdir jni; fi
	@echo "APP_ABI := armeabi armeabi-v7a" > jni/Application.mk
	@echo "#LOCAL_MODULE := OFAndroidApp" > jni/Android.mk

RESNAME=$(shell echo $(APPNAME)Resources | tr '[A-Z]' '[a-z]')

AndroidInstall:	
	if [ -d "bin/data" ]; then \
		mkdir -p res/raw; \
		rm res/raw/$(RESNAME).zip; \
		cd bin/data; \
		zip -r ../../res/raw/$(RESNAME).zip *; \
		cd ../..; \
	fi 
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
