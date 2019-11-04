
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

MINGW_PREFIX ?= /mingw32
PLATFORM_CFLAGS += -std=gnu++14 -DUNICODE -D_UNICODE
#PLATFORM_CFLAGS += -IC:/msys64/mingw32/include/gstreamer-1.0 -DOF_VIDEO_PLAYER_GSTREAMER
ifdef USE_CCACHE
	CC = ccache $(MINGW_PREFIX)/bin/gcc
	CXX = ccache $(MINGW_PREFIX)/bin/g++
else
	CC = $(MINGW_PREFIX)/bin/gcc
	CXX = $(MINGW_PREFIX)/bin/g++
endif
FIND ?= /usr/bin/find
PLATFORM_AR = $(MINGW_PREFIX)/bin/ar
PLATFORM_LD = $(MINGW_PREFIX)/bin/ld
PLATFORM_RESOURCE_COMPILER = $(MINGW_PREFIX)/bin/windres
PLATFORM_PKG_CONFIG = $(MINGW_PREFIX)/bin/pkg-config


PLATFORM_PROJECT_DEBUG_BIN_NAME=$(APPNAME)_debug.exe
PLATFORM_PROJECT_RELEASE_BIN_NAME=$(APPNAME).exe
PLATFORM_PROJECT_RELEASE_TARGET = bin/$(PLATFORM_PROJECT_RELEASE_BIN_NAME)
PLATFORM_PROJECT_DEBUG_TARGET = bin/$(PLATFORM_PROJECT_DEBUG_BIN_NAME)
PLATFORM_RUN_COMMAND = cd bin;./$(BIN_NAME)

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

#PLATFORM_DEFINES = OF_USING_STD_FS
ifeq ($(OF_USE_POCO),1)
	PLATFORM_DEFINES += POCO_STATIC
endif

ifeq ($(MSYSTEM),MINGW64)
	PLATFORM_DEFINES += OF_SOUND_PLAYER_OPENAL
	PLATFORM_DEFINES += OF_USING_MPG123
endif

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

# Link against libstdc++ to silence tr1/memory errors on latest versions of osx
#PLATFORM_CFLAGS = -stdlib=libstdc++

# Warning Flags (http://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html)
PLATFORM_CFLAGS += -Wall

# Code Generation Option Flags (http://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html)
PLATFORM_CFLAGS += -fexceptions

# Architecture / Machine Flags (http://gcc.gnu.org/onlinedocs/gcc/Submodel-Options.html)
ifeq ($(shell gcc -march=native -S -o /dev/null -xc /dev/null 2> /dev/null; echo $$?),0)
	PLATFORM_CFLAGS += -march=native
	PLATFORM_CFLAGS += -mtune=native
endif


################################################################################
# PLATFORM LDFLAGS
#   This is a list of fully qualified LDFLAGS required when linking for this
#   platform. These flags will always be added when linking a project.
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################


#PLATFORM_LDFLAGS += -arch i386
PLATFORM_LDFLAGS += -lpthread
ifndef DEBUG
	PLATFORM_LDFLAGS += -mwindows
endif
ifeq ($(findstring OF_USING_STD_FS, $(PLATFORM_DEFINES)),OF_USING_STD_FS)
	PLATFORM_LDFLAGS += -lstdc++fs
endif


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
PLATFORM_OPTIMIZATION_CFLAGS_RELEASE = -Os

# DEBUG Debugging options (http://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html)
PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = -g3 #-D_GLIBCXX_DEBUG

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

PLATFORM_CORE_EXCLUSIONS += %.mm
PLATFORM_CORE_EXCLUSIONS += %.m
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofQtUtils.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofQuickTimeGrabber.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofQuickTimePlayer.cpp
#PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofDirectShowPlayer.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofGstUtils.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofGstVideoGrabber.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofGstVideoPlayer.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/app/ofAppEGLWindow.cpp

# third party
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/glew/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/cairo/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/freetype/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/FreeImage/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openssl/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/boost/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/glfw/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/curl/%
# FMOD is not supported on MINGW64
ifeq ($(MSYSTEM),MINGW64)
	PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/fmod/%
endif
#PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/glm/%


##########################################################################################
# PLATFORM HEADER SEARCH PATHS
#   These are header search paths that are platform specific and are specified
#   using fully-qualified paths.  The include flag (i.e. -I) is prefixed automatically.
#   These are usually not required, but may be required by some experimental platforms
#   such as the raspberry pi or other other embedded architectures.
#
# Note: Be sure to leave a leading space when using a += operator to add items to the list
##########################################################################################

# PLATFORM_HEADER_SEARCH_PATHS =

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

PLATFORM_LIBRARIES += ksuser opengl32 gdi32 msimg32 glu32 dsound winmm strmiids #dxguid
PLATFORM_LIBRARIES += uuid ole32 oleaut32 setupapi wsock32 ws2_32 Iphlpapi Comdlg32
PLATFORM_LIBRARIES += freeimage boost_filesystem-mt boost_system-mt freetype cairo
#PLATFORM_LIBRARIES += gstapp-1.0 gstvideo-1.0 gstbase-1.0 gstreamer-1.0 gobject-2.0 glib-2.0 intl

#static libraries (fully qualified paths)
#PLATFORM_STATIC_LIBRARIES += somestaticlib

PLATFORM_PKG_CONFIG_LIBRARIES =
PLATFORM_PKG_CONFIG_LIBRARIES += cairo
PLATFORM_PKG_CONFIG_LIBRARIES += zlib
PLATFORM_PKG_CONFIG_LIBRARIES += openssl
PLATFORM_PKG_CONFIG_LIBRARIES += glew
PLATFORM_PKG_CONFIG_LIBRARIES += glfw3
#PLATFORM_PKG_CONFIG_LIBRARIES += gstreamer-1.0
PLATFORM_PKG_CONFIG_LIBRARIES += libcurl
ifeq ($(findstring OF_SOUND_PLAYER_OPENAL, $(PLATFORM_DEFINES)),OF_SOUND_PLAYER_OPENAL)
	PLATFORM_PKG_CONFIG_LIBRARIES += openal
endif
ifeq ($(findstring OF_USING_MPG123, $(PLATFORM_DEFINES)),OF_USING_MPG123)
	PLATFORM_PKG_CONFIG_LIBRARIES += sndfile
	PLATFORM_PKG_CONFIG_LIBRARIES += libmpg123
endif

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


# PLATFORM_CXX = clang
# PLATFORM_CC = clang


################################################################################
# PLATFORM CC
#    Don't want to use a default compiler?
################################################################################
#PLATFORM_CC=

################################################################################
# PLATFORM ICON
#    If not set by the project by PROJECT_(RELEASE|DEBUG)_ICON, use OF defaults
################################################################################
PLATFORM_RELEASE_ICON = $(OF_PLATFORM_MAKEFILES)/icon.ico
PLATFORM_DEBUG_ICON = $(OF_PLATFORM_MAKEFILES)/icon-debug.ico

copy_dlls:
	@echo "     copying dlls to bin"

	@ntldd --recursive $(wildcard bin/$(APPNAME)*.exe) | sed -e 's:\\:/:g' | grep -F "$(MINGW_PREFIX)" | cut -d">" -f2 |cut -d" " -f2 >dlllist
	
	@while read -r dll; do \
		test -e "$$dll" && cp "$$dll" ./bin; \
    done <dlllist
	@echo "     `wc -l <dlllist` dlls copied"
	@rm dlllist
	
afterplatform: $(TARGET_NAME)
	@if [ -e $(OF_LIBS_PATH)/*/lib/$(PLATFORM_LIB_SUBPATH)/*.$(SHARED_LIB_EXTENSION) ]; then cp $(OF_LIBS_PATH)/*/lib/$(PLATFORM_LIB_SUBPATH)/*.$(SHARED_LIB_EXTENSION) bin/; fi
	@echo
	@echo "     compiling done"
	@echo "     to launch the application"
	@echo
	@echo "     cd bin"
	@echo "     ./$(BIN_NAME)"
	@echo ""
	@echo "     - or -"
	@echo ""
	@echo "     make $(RUN_TARGET)"
	@echo
