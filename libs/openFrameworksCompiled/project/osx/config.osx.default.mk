
##########################################################################################
# CONFIGURE CORE PLATFORM  MAKEFILE
#   This file is where we make platform and architecture specific configurations.
#   This file can be specified for a generic architecture or can be defined as variants.
#    For instance, normally this file will be located in a platform specific subpath such
#   as $(OF_ROOT)/libs/openFrameworksComplied/linux64.
#
#   This file will then be a generic platform file like:
#
#        configure.core.linux64.default.make
#
#   Or it can specify a specific platform variant like:
#
#        configure.core.armv6l.raspberrypi.make
#
##########################################################################################

##########################################################################################
# PLATFORM SPECIFIC CHECKS
#   This is a platform defined section to create internal flags to enable or disable
#   the addition of various features within this makefile.  For instance, on Linux,
#   we check to see if there GTK+-2.0 is defined, allowing us to include that library
#   and generate DEFINES that are interpreted as ifdefs within the openFrameworks
#   core source code.
##########################################################################################

PLATFORM_PROJECT_DEBUG_BIN_NAME=$(APPNAME)_debug
PLATFORM_PROJECT_RELEASE_BIN_NAME=$(APPNAME)
PLATFORM_RUN_COMMAND = cd bin/$(BIN_NAME).app/Contents/MacOS/;./$(BIN_NAME)

##########################################################################################
# PLATFORM DEFINES
#   Create a list of DEFINES for this platform.  The list will be converted into
#   CFLAGS with the "-D" flag later in the makefile.  An example of fully qualified flag
#   might look something like this: -DTARGET_OPENGLES2
#
#   DEFINES are used throughout the openFrameworks code, especially when making
#   #ifdef decisions for cross-platform compatibility.  For instance, when chosing a
#   video playback framework, the openFrameworks base classes look at the DEFINES
#   to determine what source files to include or what default player to use.
#
# Note: Be sure to leave a leading space when using a += operator to add items to the list
##########################################################################################

PLATFORM_DEFINES = __MACOSX_CORE__

##########################################################################################
# PLATFORM REQUIRED ADDON
#   This is a list of addons required for this platform.  This list is used to EXCLUDE
#   addon source files when compiling projects, while INCLUDING their header files.
#   During core library compilation, this is used to include required addon header files
#   as needed within the core.
#
#   For instance, if you are compiling for Android, you would add ofxAndroid here.
#   If you are compiling for Raspberry Pi, you would add ofxRaspberryPi here.
#
# Note: Be sure to leave a leading space when using a += operator to add items to the list
##########################################################################################

PLATFORM_REQUIRED_ADDONS =

##########################################################################################
# PLATFORM CFLAGS
#   This is a list of fully qualified CFLAGS required when compiling for this platform.
#   These flags will always be added when compiling a project or the core library.  These
#   Flags are presented to the compiler AFTER the PLATFORM_OPTIMIZATION_CFLAGS below.
#
# Note: Be sure to leave a leading space when using a += operator to add items to the list
##########################################################################################

ifndef MAC_OS_MIN_VERSION
	MAC_OS_MIN_VERSION = 10.7
endif

ifndef MAC_OS_STD_LIB
	MAC_OS_STD_LIB = libc++
endif

# Link against libstdc++ to silence tr1/memory errors on latest versions of osx
PLATFORM_CFLAGS = -stdlib=$(MAC_OS_STD_LIB)

# Warning Flags (http://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html)
PLATFORM_CFLAGS += -Wall

# Code Generation Option Flags (http://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html)
PLATFORM_CFLAGS += -fexceptions

ifeq ($(shell xcode-select -print-path 2> /dev/null; echo $$?),0)
	MAC_OS_XCODE_ROOT=$(shell xcode-select -print-path)

	MAC_OS_SDK_PATH=$(MAC_OS_XCODE_ROOT)/Platforms/MacOSX.platform/Developer/SDKs

	ifndef MAC_OS_SDK
		ifeq ($(wildcard $(MAC_OS_SDK_PATH)/MacOSX10.10.sdk),$(MAC_OS_SDK_PATH)/MacOSX10.10.sdk)
			MAC_OS_SDK=10.10
		else ifeq ($(wildcard $(MAC_OS_SDK_PATH)/MacOSX10.9.sdk),$(MAC_OS_SDK_PATH)/MacOSX10.9.sdk)
			MAC_OS_SDK=10.9
		else ifeq ($(wildcard $(MAC_OS_SDK_PATH)/MacOSX10.8.sdk),$(MAC_OS_SDK_PATH)/MacOSX10.8.sdk)
			MAC_OS_SDK=10.8
		else ifeq ($(wildcard $(MAC_OS_SDK_PATH)/MacOSX10.7.sdk),$(MAC_OS_SDK_PATH)/MacOSX10.7.sdk)
			MAC_OS_SDK=10.7
		endif
	endif

	ifndef MAC_OS_SDK_ROOT
		MAC_OS_SDK_ROOT = $(MAC_OS_SDK_PATH)/MacOSX$(MAC_OS_SDK).sdk
	endif
else
	# xcode-select was not set, assume command line tools are installed
	MAC_OS_SDK_ROOT=
endif

# Optimization options (http://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)
#PLATFORM_CFLAGS += -finline-functions
#PLATFORM_CFLAGS += -funroll-all-loops
#PLATFORM_CFLAGS += -Os

#PLATFORM_CFLAGS += -arch i386

# other osx
PLATFORM_CFLAGS += -fpascal-strings

ifdef MAC_OS_SDK_ROOT
	PLATFORM_CFLAGS += -isysroot $(MAC_OS_SDK_ROOT)
	PLATFORM_CFLAGS += -F$(MAC_OS_SDK_ROOT)/System/Library/Frameworks
endif

PLATFORM_CFLAGS += -mmacosx-version-min=$(MAC_OS_MIN_VERSION)

PLATFORM_CXXFLAGS += -x objective-c++
PLATFORM_CXXFLAGS += -std=c++11

ifeq ($(USE_GST),1)
	PLATFORM_CFLAGS += -I/Library/Frameworks/Gstreamer.framework/Headers
endif


################################################################################
# PLATFORM LDFLAGS
#   This is a list of fully qualified LDFLAGS required when linking for this
#   platform. These flags will always be added when linking a project.
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################

PLATFORM_LDFLAGS = -stdlib=$(MAC_OS_STD_LIB)

#PLATFORM_LDFLAGS += -arch i386
#PLATFORM_LDFLAGS += -F$(OF_LIBS_PATH)/glut/lib/osx/

PLATFORM_LDFLAGS += -mmacosx-version-min=$(MAC_OS_MIN_VERSION) -v

##########################################################################################
# PLATFORM OPTIMIZATION CFLAGS
#   These are lists of CFLAGS that are target-specific.  While any flags could be
#   conditionally added, they are usually limited to optimization flags.  These flags are
#   added BEFORE the PLATFORM_CFLAGS.
#
#    PLATFORM_OPTIMIZATION_CFLAGS_RELEASE flags are only applied to RELEASE targets.
#
#    PLATFORM_OPTIMIZATION_CFLAGS_DEBUG flags are only applied to DEBUG targets.
#
# Note: Be sure to leave a leading space when using a += operator to add items to the list
##########################################################################################

# RELEASE Debugging options (http://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html)
PLATFORM_OPTIMIZATION_CFLAGS_RELEASE = -Os -DNDEBUG

# DEBUG Debugging options (http://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html)
PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = -g3

# Architecture / Machine Flags (http://gcc.gnu.org/onlinedocs/gcc/Submodel-Options.html)
ifeq ($(shell gcc -march=native -S -o /dev/null -xc /dev/null 2> /dev/null; echo $$?),0)
	#PLATFORM_OPTIMIZATION_CFLAGS_RELEASE += -march=native
	PLATFORM_OPTIMIZATION_CFLAGS_RELEASE += -mtune=native
endif

##########################################################################################
# PLATFORM CORE EXCLUSIONS
#   During compilation, these makefiles will generate lists of sources, headers and
#   third party libraries to be compiled and linked into a program or core library.
#   The PLATFORM_CORE_EXCLUSIONS is a list of fully qualified file paths that will be used
#   to exclude matching paths and files during list generation.
#
#   Each item in the PLATFORM_CORE_EXCLUSIONS list will be treated as a complete string
#   unless teh user adds a wildcard (%) operator to match subdirectories.  GNU make only
#   allows one wildcard for matching.  The second wildcard (%) is treated literally.
#
# Note: Be sure to leave a leading space when using a += operator to add items to the list
##########################################################################################

PLATFORM_CORE_EXCLUSIONS =

# core sources
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofDirectShowGrabber.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofDirectShowPlayer.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofQtKitGrabber.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofQtKitPlayer.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofQtKitMovieRenderer.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofQtKitPlayer.mm
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofQtUtils.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofQuicktimeGrabber.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofQuicktimePlayer.cpp

ifneq ($(USE_GST),1)
	PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofGstUtils.cpp
	PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofGstVideoGrabber.cpp
	PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofGstVideoPlayer.cpp
endif
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/app/ofAppEGLWindow.cpp


# third party
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/include/Poco
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/include/CppUnit
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/include/Poco/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/include/CppUnit/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/boost/include/boost/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/videoInput/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/quicktime/%

# third party static libs (this may not matter due to exclusions in poco's libsorder.make)
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/glut/lib/$(PLATFORM_LIB_SUBPATH)/%

ifeq ($(USE_FMOD),0)
	PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/fmodex/%
	PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/sound/ofFmodSoundPlayer.cpp
	PLATFORM_CFLAGS += -DUSE_FMOD=0
endif

##########################################################################################
# PLATFORM HEADER SEARCH PATHS
#   These are header search paths that are platform specific and are specified
#   using fully-qualified paths.  The include flag (i.e. -I) is prefixed automatically.
#   These are usually not required, but may be required by some experimental platforms
#   such as the raspberry pi or other other embedded architectures.
#
# Note: Be sure to leave a leading space when using a += operator to add items to the list
##########################################################################################

PLATFORM_HEADER_SEARCH_PATHS =

##########################################################################################
# PLATFORM LIBRARIES
#   These are library names/paths that are platform specific and are specified
#   using names or paths.  The library flag (i.e. -l) is prefixed automatically.
#
#   PLATFORM_LIBRARIES are libraries that can be found in the library search paths.
#   PLATFORM_STATIC_LIBRARIES is a list of required static libraries.
#   PLATFORM_SHARED_LIBRARIES is a list of required shared libraries.
#   PLATFORM_PKG_CONFIG_LIBRARIES is a list of required libraries that are under system
#       control and are easily accesible via the package configuration utility
#       (i.e. pkg-config)
#
#   See the helpfile for the -l flag here for more information:
#       http://gcc.gnu.org/onlinedocs/gcc/Link-Options.html
#
# Note: Be sure to leave a leading space when using a += operator to add items to the list
##########################################################################################

PLATFORM_LIBRARIES = objc

#static libraries (fully qualified paths)
PLATFORM_STATIC_LIBRARIES =

# shared libraries
PLATFORM_SHARED_LIBRARIES =


##########################################################################################
# PLATFORM LIBRARY SEARCH PATHS
#   These are library search paths that are platform specific and are specified
#   using fully-qualified paths.  The lib search flag (i.e. -L) is prefixed automatically.
#   The -L paths are used to find libraries defined above with the -l flag.
#
#   See the the following link for more information on the -L flag:
#       http://gcc.gnu.org/onlinedocs/gcc/Directory-Options.html
#
# Note: Be sure to leave a leading space when using a += operator to add items to the list
##########################################################################################

PLATFORM_LIBRARY_SEARCH_PATHS =

##########################################################################################
# PLATFORM FRAMEWORKS
#   These are a list of platform frameworks.
#   These are used exclusively with Darwin/OSX.
#
# Note: Be sure to leave a leading space when using a += operator to add items to the list
##########################################################################################

PLATFORM_FRAMEWORKS =
PLATFORM_FRAMEWORKS += Accelerate
PLATFORM_FRAMEWORKS += QTKit
PLATFORM_FRAMEWORKS += GLUT
PLATFORM_FRAMEWORKS += AGL
PLATFORM_FRAMEWORKS += ApplicationServices
PLATFORM_FRAMEWORKS += AudioToolbox
PLATFORM_FRAMEWORKS += CoreAudio
PLATFORM_FRAMEWORKS += CoreFoundation
PLATFORM_FRAMEWORKS += CoreServices
PLATFORM_FRAMEWORKS += OpenGL
PLATFORM_FRAMEWORKS += IOKit
PLATFORM_FRAMEWORKS += Cocoa
PLATFORM_FRAMEWORKS += CoreVideo
PLATFORM_FRAMEWORKS += AVFoundation
PLATFORM_FRAMEWORKS += CoreMedia
PLATFORM_FRAMEWORKS += QuartzCore

ifeq ($(USE_GST),1)
	PLATFORM_FRAMEWORKS += GStreamer
endif

##########################################################################################
# PLATFORM FRAMEWORK SEARCH PATHS
#   These are a list of platform framework search paths.
#   These are used exclusively with Darwin/OSX.
#
# Note: Be sure to leave a leading space when using a += operator to add items to the list
##########################################################################################

PLATFORM_FRAMEWORKS_SEARCH_PATHS = /System/Library/Frameworks

##########################################################################################
# LOW LEVEL CONFIGURATION BELOW
#   The following sections should only rarely be modified.  They are meant for developers
#   why need fine control when, for instance, creating a platform specific makefile for
#   a new openFrameworks platform, such as raspberry pi.
##########################################################################################

##########################################################################################
# PLATFORM CONFIGURATIONS
#   These will override the architecture vars generated by configure.platform.make
##########################################################################################

#PLATFORM_ARCH =
#PLATFORM_OS =
#PLATFORM_LIBS_PATH =

################################################################################
# PLATFORM CXX
#    Don't want to use a default compiler?
################################################################################

# PLATFORM_CXX =

################################################################################
# PLATFORM CC
#    Don't want to use a default compiler?
################################################################################
#PLATFORM_CC=

afterplatform: $(TARGET_NAME)
	@rm -rf bin/$(BIN_NAME).app
	@mkdir -p bin/$(BIN_NAME).app
	@mkdir -p bin/$(BIN_NAME).app/Contents
	@mkdir -p bin/$(BIN_NAME).app/Contents/MacOS
	@mkdir -p bin/$(BIN_NAME).app/Contents/Resources

	@echo '<?xml version="1.0" encoding="UTF-8"?>' > bin/$(BIN_NAME).app/Contents/Info.plist
	@echo '<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">' >> bin/$(BIN_NAME).app/Contents/Info.plist
	@echo '<plist version="1.0">' >> bin/$(BIN_NAME).app/Contents/Info.plist
	@echo '<dict>' >> bin/$(BIN_NAME).app/Contents/Info.plist
	@echo '  <key>CFBundleGetInfoString</key>' >> bin/$(BIN_NAME).app/Contents/Info.plist
	@echo '  <string>$(BIN_NAME).app</string>' >> bin/$(BIN_NAME).app/Contents/Info.plist
	@echo '  <key>CFBundleExecutable</key>' >> bin/$(BIN_NAME).app/Contents/Info.plist
	@echo '  <string>$(BIN_NAME)</string>' >> bin/$(BIN_NAME).app/Contents/Info.plist
	@echo '  <key>CFBundleIdentifier</key>' >> bin/$(BIN_NAME).app/Contents/Info.plist
	@echo '  <string>com.your-company-name.www</string>' >> bin/$(BIN_NAME).app/Contents/Info.plist
	@echo '  <key>CFBundleName</key>' >> bin/$(BIN_NAME).app/Contents/Info.plist
	@echo '  <string>$(BIN_NAME)</string>' >> bin/$(BIN_NAME).app/Contents/Info.plist
	@echo '  <key>CFBundleShortVersionString</key>' >> bin/$(BIN_NAME).app/Contents/Info.plist
	@echo '  <string>0.01</string>' >> bin/$(BIN_NAME).app/Contents/Info.plist
	@echo '  <key>CFBundleInfoDictionaryVersion</key>' >> bin/$(BIN_NAME).app/Contents/Info.plist
	@echo '  <string>6.0</string>' >> bin/$(BIN_NAME).app/Contents/Info.plist
	@echo '  <key>CFBundlePackageType</key>' >> bin/$(BIN_NAME).app/Contents/Info.plist
	@echo '  <string>APPL</string>' >> bin/$(BIN_NAME).app/Contents/Info.plist
	@echo '  <key>IFMajorVersion</key>' >> bin/$(BIN_NAME).app/Contents/Info.plist
	@echo '  <integer>0</integer>' >> bin/$(BIN_NAME).app/Contents/Info.plist
	@echo '  <key>IFMinorVersion</key>' >> bin/$(BIN_NAME).app/Contents/Info.plist
	@echo '  <integer>1</integer>' >> bin/$(BIN_NAME).app/Contents/Info.plist
	@echo '</dict>' >> bin/$(BIN_NAME).app/Contents/Info.plist
	@echo '</plist>' >> bin/$(BIN_NAME).app/Contents/Info.plist

	@echo TARGET=$(TARGET)


	@mv $(TARGET) bin/$(BIN_NAME).app/Contents/MacOS
	
ifneq ($(USE_FMOD),0)
	@cp -r $(OF_EXPORT_PATH)/$(ABI_LIB_SUBPATH)/libs/* bin/$(BIN_NAME).app/Contents/MacOS
endif
	
	@echo
	@echo "     compiling done"
	@echo "     to launch the application"
	@echo
	@echo "     cd bin/$(BIN_NAME).app/Contents/MacOS/"
	@echo "     ./$(BIN_NAME)"
	@echo "     "
	@echo "     - or -"
	@echo "     "
	@echo "     make $(RUN_TARGET)"
	@echo
