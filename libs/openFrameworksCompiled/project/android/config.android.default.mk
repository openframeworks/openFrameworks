################################################################################
# CONFIGURE CORE PLATFORM MAKEFILE
#   This file is where we make platform and architecture specific 
#   configurations. This file can be specified for a generic architecture or can
#   be defined as variants. For instance, normally this file will be located in 
#   a platform specific subpath such as 
#
#        $(OF_ROOT)/libs/openFrameworksComplied/linux64
#
#   This file will then be a generic platform file like:
#
#        configure.core.linux64.default.make
#
#   Or it can specify a specific platform variant like:
#
#        configure.core.linuxarmv6l.raspberrypi.make
#
################################################################################

################################################################################
# PLATFORM SPECIFIC CHECKS
#   This is a platform defined section to create internal flags to enable or 
#   disable the addition of various features within this makefile.  For 
#   instance, on Linux, we check to see if there GTK+-2.0 is defined, allowing 
#   us to include that library and generate DEFINES that are interpreted as 
#   ifdefs within the openFrameworks core source code.
################################################################################

#check if mpg123 exists and add it
#HAS_SYSTEM_MPG123 = $(shell pkg-config libmpg123 --exists; echo $$?)

include $(OF_ROOT)/libs/openFrameworksCompiled/project/android/paths.make
ARCH = android

ifndef ABIS_TO_COMPILE_RELEASE
	ABIS_TO_COMPILE_RELEASE = armv7 neon x86
endif

ifndef ABIS_TO_COMPILE_DEBUG
	ABIS_TO_COMPILE_DEBUG = armv7
endif


################################################################################
# PLATFORM DEFINES
#   Create a list of DEFINES for this platform.  The list will be converted into 
#   CFLAGS with the "-D" flag later in the makefile.  An example of fully 
#   qualified flag might look something like this: -DTARGET_OPENGLES2
#
#   DEFINES are used throughout the openFrameworks code, especially when making
#   #ifdef decisions for cross-platform compatibility.  For instance, when 
#   choosing a video playback framework, the openFrameworks base classes look at
#   the DEFINES to determine what source files to include or what default player 
#   to use.
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################

PLATFORM_DEFINES = 
PLATFORM_DEFINES = ANDROID

ifndef $(NDK_PLATFORM)
	NDK_PLATFORM = android-17
endif

ifndef $(SDK_TARGET)
	SDK_TARGET = android-17
endif

ifndef $(GCC_VERSION)
	GCC_VERSION = 4.8
endif

PROJECT_PATH=$(PWD)


ifeq ($(ABI),x86)
ANDROID_PREFIX=i686-linux-android-
TOOLCHAIN=x86-$(GCC_VERSION)
SYSROOT=$(NDK_ROOT)/platforms/$(NDK_PLATFORM)/arch-x86/
else
ANDROID_PREFIX=arm-linux-androideabi-
TOOLCHAIN=$(ANDROID_PREFIX)$(GCC_VERSION)
SYSROOT=$(NDK_ROOT)/platforms/$(NDK_PLATFORM)/arch-arm/
endif

ifeq ($(shell uname),Darwin)
ifneq ($(wildcard $(NDK_ROOT)/toolchains/$(TOOLCHAIN)/prebuilt/darwin-x86_64),)
	HOST_PLATFORM = darwin-x86_64
else
	HOST_PLATFORM = darwin-x86
endif
else ifneq (,$(findstring MINGW32_NT,$(shell uname)))
	HOST_PLATFORM = windows
	PWD = $(shell pwd)
else
ifneq ($(wildcard $(NDK_ROOT)/toolchains/$(TOOLCHAIN)/prebuilt/linux-x86_64),)
	HOST_PLATFORM = linux-x86_64
else
	HOST_PLATFORM = linux-x86
endif
endif

TOOLCHAIN_PATH=$(NDK_ROOT)/toolchains/$(TOOLCHAIN)/prebuilt/$(HOST_PLATFORM)/bin/

DATA_FILES = $(shell find bin/data -type f 2>/dev/null)
RESNAME=$(shell echo $(APPNAME)Resources | tr '[A-Z]' '[a-z]')
RESFILE=$(RESNAME).zip

ifeq ($(ABI),armv7)
	ABI_PATH = armeabi-v7a
	PLATFORM_PROJECT_RELEASE_TARGET = libs/$(ABI_PATH)/libOFAndroidApp.so
	PLATFORM_PROJECT_DEBUG_TARGET = libs/$(ABI_PATH)/libOFAndroidApp.so
endif

ifeq ($(ABI),armv5)
	ABI_PATH = armeabi
	PLATFORM_PROJECT_RELEASE_TARGET = libs/$(ABI_PATH)/libOFAndroidApp.so
	PLATFORM_PROJECT_DEBUG_TARGET = libs/$(ABI_PATH)/libOFAndroidApp.so
endif

ifeq ($(ABI),neon)
	ABI_PATH = armeabi-v7a
	PLATFORM_PROJECT_RELEASE_TARGET = libs/$(ABI_PATH)/libOFAndroidApp_neon.so
	PLATFORM_PROJECT_DEBUG_TARGET = libs/$(ABI_PATH)/libOFAndroidApp_neon.so
endif

ifeq ($(ABI),x86)
	ABI_PATH = x86
	PLATFORM_PROJECT_RELEASE_TARGET = libs/$(ABI_PATH)/libOFAndroidApp.so
	PLATFORM_PROJECT_DEBUG_TARGET = libs/$(ABI_PATH)/libOFAndroidApp.so
endif

PLATFORM_CORELIB_RELEASE_TARGET = $(OF_CORE_LIB_PATH)/$(ABI)/libopenFrameworks.a
PLATFORM_CORELIB_DEBUG_TARGET = $(OF_CORE_LIB_PATH)/$(ABI)/libopenFrameworksDebug.a


# add OF_USING_MPG123 define IF we have it defined as a system library
#ifeq ($(HAS_SYSTEM_MPG123),0)
#    PLATFORM_DEFINES += OF_USING_MPG123
#endif

################################################################################
# PLATFORM REQUIRED ADDON
#   This is a list of addons required for this platform.  This list is used to 
#   EXCLUDE addon source files when compiling projects, while INCLUDING their 
#   header files. During core library compilation, this is used to include 
#   required addon header files as needed within the core. 
#
#   For instance, if you are compiling for Android, you would add ofxAndroid 
#   here. If you are compiling for Raspberry Pi, you would add ofxRaspberryPi 
#   here.
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################

PLATFORM_REQUIRED_ADDONS = ofxAndroid

################################################################################
# PLATFORM CFLAGS
#   This is a list of fully qualified CFLAGS required when compiling for this 
#   platform. These flags will always be added when compiling a project or the 
#   core library.  These flags are presented to the compiler AFTER the 
#   PLATFORM_OPTIMIZATION_CFLAGS below. 
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################

# Warning Flags (http://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html)
PLATFORM_CFLAGS = -Wall -std=c++11

# Code Generation Option Flags (http://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html)
PLATFORM_CFLAGS += -nostdlib --sysroot=$(SYSROOT) -fno-short-enums -ffunction-sections -fdata-sections


ifeq ($(ABI),armv7)
	PLATFORM_CFLAGS += -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16
endif

ifeq ($(ABI),neon)
	PLATFORM_CFLAGS += -march=armv7-a -mfloat-abi=softfp -mfpu=neon
endif

ifeq ($(ABI),x86)
	PLATFORM_CFLAGS += -march=i686 -msse3 -mstackrealign -mfpmath=sse -fno-stack-protector
endif

################################################################################
# PLATFORM LDFLAGS
#   This is a list of fully qualified LDFLAGS required when linking for this 
#   platform. These flags will always be added when linking a project.
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################

PLATFORM_LDFLAGS =
PLATFORM_LDFLAGS += --sysroot=$(SYSROOT) -nostdlib -L"$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/$(GCC_VERSION)/libs/$(ABI_PATH)"
#ifeq ($(HOST_PLATFORM),linux-x86)
#	PLATFORM_LDFLAGS += -fuse-ld=gold
#endif

ifneq ($(ABI),x86)
PLATFORM_LDFLAGS += -Wl,--fix-cortex-a8 
endif
PLATFORM_LDFLAGS += -shared -Wl,--no-undefined -Wl,--as-needed -Wl,--gc-sections -Wl,--exclude-libs,ALL

################################################################################
# PLATFORM OPTIMIZATION CFLAGS
#   These are lists of CFLAGS that are target-specific.  While any flags could 
#   be conditionally added, they are usually limited to optimization flags.  
#   These flags are added BEFORE the PLATFORM_CFLAGS.
#
#   PLATFORM_OPTIMIZATION_CFLAGS_RELEASE flags are only applied to 
#      RELEASE targets.
#
#   PLATFORM_OPTIMIZATION_CFLAGS_DEBUG flags are only applied to 
#      DEBUG targets.
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################

# RELEASE Debugging options (http://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html)
PLATFORM_OPTIMIZATION_CFLAGS_RELEASE = -Os

# RELEASE options
PLATFORM_OPTIMIZATION_LDFLAGS_RELEASE = -s

# DEBUG Debugging options (http://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html)
PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = -O0 -g -D_DEBUG

################################################################################
# PLATFORM CORE EXCLUSIONS
#   During compilation, these makefiles will generate lists of sources, headers 
#   and third party libraries to be compiled and linked into a program or core 
#   library. The PLATFORM_CORE_EXCLUSIONS is a list of fully qualified file 
#   paths that will be used to exclude matching paths and files during list 
#   generation.
#
#   Each item in the PLATFORM_CORE_EXCLUSIONS list will be treated as a complete
#   string unless the user adds a wildcard (%) operator to match subdirectories.
#   GNU make only allows one wildcard for matching.  The second wildcard (%) is
#   treated literally.
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################

PLATFORM_CORE_EXCLUSIONS =

# core sources
PLATFORM_CORE_EXCLUSIONS += %.mm
PLATFORM_CORE_EXCLUSIONS += %.m
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofQtUtils.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofQuickTimeGrabber.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofQuickTimePlayer.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofDirectShowGrabber.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofDirectShowPlayer.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofGstUtils.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofGstVideoGrabber.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofGstVideoPlayer.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/app/ofAppGlutWindow.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/app/ofAppEGLWindow.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/app/ofAppGLFWWindow.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/graphics/ofCairoRenderer.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/sound/ofFmodSoundPlayer.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/sound/ofOpenALSoundPlayer.cpp

# third party
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/glew/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/include/Poco
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/include/CppUnit
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/include/Poco/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/include/CppUnit/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/quicktime/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/videoInput/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/glu/include
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/fmodex/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/kiss/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/assimp/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/glut/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/portaudio/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/rtAudio/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/lib/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openssl/lib/%

# android project folders
PROJECT_EXCLUSIONS += ./gen
PROJECT_EXCLUSIONS += ./gen/%
PROJECT_EXCLUSIONS += ./jni
PROJECT_EXCLUSIONS += ./srcJava
PROJECT_EXCLUSIONS += ./srcJava/%
PROJECT_EXCLUSIONS += ./res
PROJECT_EXCLUSIONS += ./res/%
PROJECT_EXCLUSIONS += ./assets
PROJECT_EXCLUSIONS += ./assets/%
PROJECT_EXCLUSIONS += ./libs



################################################################################
# PLATFORM HEADER SEARCH PATHS
#   These are header search paths that are platform specific and are specified 
#   using fully-qualified paths.  The include flag (i.e. -I) is prefixed 
#   automatically. These are usually not required, but may be required by some 
#   experimental platforms such as the raspberry pi or other other embedded 
#   architectures.
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################

PLATFORM_HEADER_SEARCH_PATHS =
PLATFORM_HEADER_SEARCH_PATHS += "$(SYSROOT)/usr/include/" 
PLATFORM_HEADER_SEARCH_PATHS += "$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/include" 
PLATFORM_HEADER_SEARCH_PATHS += "$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/$(GCC_VERSION)/include" 
PLATFORM_HEADER_SEARCH_PATHS += "$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/libs/$(ABI_PATH)/include" 
PLATFORM_HEADER_SEARCH_PATHS += "$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/$(GCC_VERSION)/libs/$(ABI_PATH)/include"
PLATFORM_HEADER_SEARCH_PATHS += "$(NDK_ROOT)/sources/crystax/include/" 
PLATFORM_HEADER_SEARCH_PATHS += "$(OF_ROOT)/libs/glu/include_android"
PLATFORM_HEADER_SEARCH_PATHS += "$(OF_ROOT)/addons/ofxAndroid/src"

################################################################################
# PLATFORM LIBRARIES
#   These are library names/paths that are platform specific and are specified 
#   using names or paths.  The library flag (i.e. -l) is prefixed automatically.
#
#   PLATFORM_LIBRARIES are libraries that can be found in the library search 
#       paths.
#   PLATFORM_STATIC_LIBRARIES is a list of required static libraries.
#   PLATFORM_SHARED_LIBRARIES is a list of required shared libraries.
#   PLATFORM_PKG_CONFIG_LIBRARIES is a list of required libraries that are 
#       under system control and are easily accesible via the package 
#       configuration utility (i.e. pkg-config)
#
#   See the helpfile for the -l flag here for more information:
#       http://gcc.gnu.org/onlinedocs/gcc/Link-Options.html
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################

PLATFORM_LIBRARIES = 
PLATFORM_LIBRARIES += OpenSLES
PLATFORM_LIBRARIES += supc++ 
PLATFORM_LIBRARIES += z 
PLATFORM_LIBRARIES += GLESv1_CM 
PLATFORM_LIBRARIES += GLESv2 
PLATFORM_LIBRARIES += log 
PLATFORM_LIBRARIES += dl 
PLATFORM_LIBRARIES += m 
PLATFORM_LIBRARIES += c 
PLATFORM_LIBRARIES += gnustl_static
PLATFORM_LIBRARIES += gcc

#static libraries (fully qualified paths)
PLATFORM_STATIC_LIBRARIES =
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/poco/lib/$(ABI_LIB_SUBPATH)/libPocoNetSSL.a
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/poco/lib/$(ABI_LIB_SUBPATH)/libPocoNet.a
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/poco/lib/$(ABI_LIB_SUBPATH)/libPocoCrypto.a
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/poco/lib/$(ABI_LIB_SUBPATH)/libPocoUtil.a
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/poco/lib/$(ABI_LIB_SUBPATH)/libPocoXML.a
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/poco/lib/$(ABI_LIB_SUBPATH)/libPocoFoundation.a
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/openssl/lib/$(ABI_LIB_SUBPATH)/libssl.a
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/openssl/lib/$(ABI_LIB_SUBPATH)/libcrypto.a

# shared libraries 
PLATFORM_SHARED_LIBRARIES =

#openframeworks core third party
PLATFORM_PKG_CONFIG_LIBRARIES =

# conditionally add mpg123
#ifeq ($(HAS_SYSTEM_MPG123),0)
#    PLATFORM_PKG_CONFIG_LIBRARIES += libmpg123
#endif

################################################################################
# PLATFORM LIBRARY SEARCH PATHS
#   These are library search paths that are platform specific and are specified 
#   using fully-qualified paths.  The lib search flag (i.e. -L) is prefixed 
#   automatically. The -L paths are used to find libraries defined above with 
#   the -l flag.
#
#   See the the following link for more information on the -L flag:
#       http://gcc.gnu.org/onlinedocs/gcc/Directory-Options.html 
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################

PLATFORM_LIBRARY_SEARCH_PATHS =

################################################################################
# PLATFORM FRAMEWORKS
#   These are a list of platform frameworks.  
#   These are used exclusively with Darwin/OSX.
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################
#PLATFORM_FRAMEWORKS =

################################################################################
# PLATFORM FRAMEWORK SEARCH PATHS
#   These are a list of platform framework search paths.  
#   These are used exclusively with Darwin/OSX.
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################
#PLATFORM_FRAMEWORKS_SEARCH_PATHS =

################################################################################
# LOW LEVEL CONFIGURATION BELOW
#   The following sections should only rarely be modified.  They are meant for 
#   developers who need fine control when, for instance, creating a platform 
#   specific makefile for a new openFrameworks platform, such as raspberry pi. 
################################################################################

################################################################################
# PLATFORM CONFIGURATIONS
# These will override the architecture vars generated by configure.platform.make
################################################################################
#PLATFORM_ARCH =
#PLATFORM_OS =
#PLATFORM_LIBS_PATH =

################################################################################
# PLATFORM CXX
#    Don't want to use a default compiler?
################################################################################
#PLATFORM_CXX=

PLATFORM_CC=$(NDK_ROOT)/toolchains/$(TOOLCHAIN)/prebuilt/$(HOST_PLATFORM)/bin/$(ANDROID_PREFIX)gcc
PLATFORM_CXX=$(NDK_ROOT)/toolchains/$(TOOLCHAIN)/prebuilt/$(HOST_PLATFORM)/bin/$(ANDROID_PREFIX)g++
PLATFORM_AR=$(NDK_ROOT)/toolchains/$(TOOLCHAIN)/prebuilt/$(HOST_PLATFORM)/bin/$(ANDROID_PREFIX)ar

#ifeq (,$(findstring MINGW32_NT,$(shell uname)))
ZIPWINDOWS=..\\..\\..\\..\\..\\libs\\openFrameworksCompiled\\project\\android\\windows\\zip -r ../../res/raw/$(RESFILE)
#endif

afterplatform:$(RESFILE)
	@if [ -f obj/$(BIN_NAME) ]; then rm obj/$(BIN_NAME); fi
	
	@echo copying debugging binaries for $(ABIS_TO_COMPILE)
	@if [ "$(findstring neon,$(ABIS_TO_COMPILE))" = "neon" ]; then \
		cp $(OF_ROOT)/libs/openFrameworksCompiled/project/android/libneondetection.so libs/armeabi-v7a/; \
		cp $(NDK_ROOT)/prebuilt/android-arm/gdbserver/gdbserver libs/armeabi-v7a; \
	fi
	
	@if [ "$(findstring armv5,$(ABIS_TO_COMPILE))" = "armv5" ]; then \
		cp $(NDK_ROOT)/prebuilt/android-arm/gdbserver/gdbserver libs/armeabi; \
	fi
	
	@if [ "$(findstring armv7,$(ABIS_TO_COMPILE))" = "armv7" ]; then \
		cp $(NDK_ROOT)/prebuilt/android-arm/gdbserver/gdbserver libs/armeabi-v7a; \
	fi
	
	@if [ "$(findstring armv7,$(ABIS_TO_COMPILE))" = "x86" ]; then \
		cp $(NDK_ROOT)/prebuilt/android-x86/gdbserver/gdbserver libs/x86; \
	fi
	
	
	@if [ "$(findstring armv5,$(ABIS_TO_COMPILE))" = "armv5" ]; then \
		echo create gdb.setup for armeabi; \
		echo "set solib-search-path $(PWD)/obj/local/armeabi:$(PWD)/libs/armeabi" > libs/armeabi/gdb.setup; \
		echo "set sysroot $(SYSROOT)" >> libs/armeabi/gdb.setup; \
		echo "directory $(NDK_ROOT)/platforms/$(NDK_PLATFORM)/arch-arm/usr/include" >> libs/armeabi/gdb.setup; \
		echo "directory $(PWD)/src" >> libs/armeabi/gdb.setup; \
		echo "directory $(NDK_ROOT)/sources/cxx-stl/system" >> libs/armeabi/gdb.setup; \
		echo "directory $(PWD)/libs/armeabi" >> libs/armeabi/gdb.setup; \
		echo "" >> libs/armeabi/gdb.setup; \
	fi
	
	@if [ "$(findstring armv7,$(ABIS_TO_COMPILE))" = "armv7" ]; then \
		echo create gdb.setup for armeabi-v7a; \
		echo "set solib-search-path $(PWD)/obj/local/armeabi-v7a:$(PWD)/libs/armeabi-v7a" > libs/armeabi-v7a/gdb.setup; \
		echo "set sysroot $(SYSROOT)" >> libs/armeabi-v7a/gdb.setup; \
		echo "directory $(NDK_ROOT)/platforms/$(NDK_PLATFORM)/arch-arm/usr/include" >> libs/armeabi-v7a/gdb.setup; \
		echo "directory $(PWD)/src" >> libs/armeabi-v7a/gdb.setup; \
		echo "directory $(NDK_ROOT)/sources/cxx-stl/system" >> libs/armeabi-v7a/gdb.setup; \
		echo "directory $(PWD)/libs/armeabi-v7a" >> libs/armeabi-v7a/gdb.setup ; \
		echo "" >> libs/armeabi-v7a/gdb.setup; \
	fi
	
	@if [ "$(findstring x86,$(ABIS_TO_COMPILE))" = "x86" ]; then \
		echo create gdb.setup for x86; \
		echo "set solib-search-path $(PWD)/obj/local/x86:$(PWD)/libs/x86" > libs/x86/gdb.setup; \
		echo "set sysroot $(SYSROOT)" >> libs/x86/gdb.setup; \
		echo "directory $(NDK_ROOT)/platforms/$(NDK_PLATFORM)/arch-arm/usr/include" >> libs/x86/gdb.setup; \
		echo "directory $(PWD)/src" >> libs/x86/gdb.setup; \
		echo "directory $(NDK_ROOT)/sources/cxx-stl/system" >> libs/x86/gdb.setup; \
		echo "directory $(PWD)/libs/x86" >> libs/x86/gdb.setup ; \
		echo "" >> libs/x86/gdb.setup; \
	fi
	
	@echo creating Android.mk and Application.mk
	@if [ ! -d jni ]; then mkdir jni; fi
	@ABIS=""; \
	if [ "$(findstring armv5,$(ABIS_TO_COMPILE))" = "armv5" ]; then \
		ABIS="$$ABIS armeabi"; \
	else \
		rm -r libs/armeabi 2> /dev/null; \
	fi; \
	if [ "$(findstring armv7,$(ABIS_TO_COMPILE))" = "armv7" ] || [ "$(findstring neon,$(ABIS_TO_COMPILE))" = "neon" ]; then \
		ABIS="$$ABIS armeabi-v7a"; \
	elif [ "$(findstring armv7,$(ABIS_TO_COMPILE))" = "armv7" ]; then \
		rm libs/armeabi-v7a/libOFAndroidApp_neon.so 2> /dev/null; \
		rm libs/armeabi-v7a/libneondetection.so 2> /dev/null; \
	elif [ "$(findstring neon,$(ABIS_TO_COMPILE))" = "neon" ]; then \
		rm libs/armeabi-v7a/libOFAndroidApp.so 2> /dev/null; \
	else \
		rm -r libs/armeabi-v7a 2> /dev/null; \
	fi; \
	if [ "$(findstring x86,$(ABIS_TO_COMPILE))" = "x86" ]; then \
		ABIS="$$ABIS x86"; \
	else \
		rm -r libs/x86 2> /dev/null; \
	fi; \
	echo "APP_ABI := $$ABIS" > jni/Application.mk; \
	echo "LOCAL_MODULE := OFAndroidApp" > jni/Android.mk
		
	
	
	#@echo updating ofAndroidLib project
	#@cd $(OF_ROOT)/addons/ofxAndroid/ofAndroidLib; \
	#if [ "$(HOST_PLATFORM)" = "windows" ]; then \
	#	cmd //c $(SDK_ROOT)/tools/android.bat update project --target $(SDK_TARGET) --path .; \
	#else \
	#	$(SDK_ROOT)/tools/android update project --target $(SDK_TARGET) --path .; \
	#fi
	
	#@echo updating current project
	#@cd $(PROJECT_PATH); \
	#if [ "$(HOST_PLATFORM)" = "windows" ]; then \
	#	cmd //c $(SDK_ROOT)/tools/android.bat update project --target $(SDK_TARGET) --path .; \
	#else \
	#	$(SDK_ROOT)/tools/android update project --target $(SDK_TARGET) --path .; \
	#fi
	
$(RESFILE): $(DATA_FILES)
	@echo compressing and copying resources from bin/data into res
	cd $(PROJECT_PATH); \
	if [ -d "bin/data" ]; then \
		mkdir -p res/raw; \
		rm res/raw/$(RESNAME).zip; \
		cd bin/data; \
		if [ "$(HOST_PLATFORM)" = "windows" ]; then \
			echo "Windows Platform. Running Zip..."; \
			cmd //c $(ZIPWINDOWS) * && exit; \
		else \
			zip -r ../../res/raw/$(RESNAME).zip *; \
		fi; \
		cd ../..; \
	fi

install:	
	cd $(OF_ROOT)/addons/ofxAndroid/ofAndroidLib; \
	echo installing on $(HOST_PLATFORM); \
	if [ "$(HOST_PLATFORM)" = "windows" ]; then \
	cmd //c $(SDK_ROOT)/tools/android.bat update project --target $(SDK_TARGET) --path .; \
	else \
	$(SDK_ROOT)/tools/android update project --target $(SDK_TARGET) --path .; \
	fi 
	cd $(PROJECT_PATH); \
	if [ -d "bin/data" ]; then \
		mkdir -p res/raw; \
		rm res/raw/$(RESNAME).zip; \
		cd bin/data; \
		if [ "$(HOST_PLATFORM)" = "windows" ]; then \
			echo "Windows Platform. Running Zip..."; \
			cmd //c $(ZIPWINDOWS) * && exit; \
		else \
			zip -r ../../res/raw/$(RESNAME).zip; *; \
		fi; \
		cd ../..; \
	fi 
	if [ -f obj/$(BIN_NAME) ]; then rm obj/$(BIN_NAME); fi
	#touch AndroidManifest.xml
	if [ "$(HOST_PLATFORM)" = "windows" ]; then \
	cmd //c $(SDK_ROOT)/tools/android.bat update project --target $(SDK_TARGET) --path .; \
	else \
	$(SDK_ROOT)/tools/android update project --target $(SDK_TARGET) --path .; \
	fi
	
	#rm -r $(addprefix bin/,$(shell ls bin/ | grep -v ^data$))
	
	if [ "$(HOST_PLATFORM)" = "windows" ]; then \
	#$(ANT_BIN)/ant clean; \
	$(ANT_BIN)/ant debug install; \
	else \
	#ant clean; \
	ant debug install; \
	fi
	cp bin/OFActivity-debug.apk bin/$(APPNAME).apk
	#if [ "$(shell $(SDK_ROOT)/platform-tools/adb get-state)" = "device" ]; then
	#$(SDK_ROOT)/platform-tools/adb uninstall $(PKGNAME)
	$(SDK_ROOT)/platform-tools/adb install -r bin/$(APPNAME).apk;
	#fi
	$(SDK_ROOT)/platform-tools/adb shell am start -a android.intent.action.MAIN -n $(PKGNAME)/$(PKGNAME).OFActivity
	
