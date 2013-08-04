################################################################################
# CONFIGURE CORE PLATFORM MAKEFILE
#   This file has linux common rules for all the platforms (x86_64, i386,armv6l 
#   and armv7l)
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

ifeq ($(PLATFORM_ARCH),armv6l)
	LINUX_ARM=1
endif

ifeq ($(PLATFORM_ARCH),armv7l)
	LINUX_ARM=1
endif

#check if gtk exists and add it
ifeq ($(CROSS_COMPILING),1)
	HAS_SYSTEM_GTK = $(shell export PKG_CONFIG_LIBDIR=$(PKG_CONFIG_LIBDIR);pkg-config gtk+-2.0 --exists; echo $$?)
else
	HAS_SYSTEM_GTK = $(shell pkg-config gtk+-2.0 --exists; echo $$?)
endif

#check if mpg123 exists and add it
ifeq ($(CROSS_COMPILING),1)
	HAS_SYSTEM_MPG123 = $(shell export PKG_CONFIG_LIBDIR=$(PKG_CONFIG_LIBDIR);pkg-config libmpg123 --exists; echo $$?)
else
	HAS_SYSTEM_MPG123 = $(shell pkg-config libmpg123 --exists; echo $$?)
endif

#check gstreamer version
ifndef GST_VERSION
	ifeq ($(CROSS_COMPILING),1)
		ifeq ($(shell export PKG_CONFIG_LIBDIR=$(PKG_CONFIG_LIBDIR);pkg-config gstreamer-1.0 --exists; echo $$?),0)
		    GST_VERSION = 1.0
$(info GSTVERSION=$(GST_VERSION))
		else
		    GST_VERSION = 0.10
$(info GSTVERSION=$(GST_VERSION))
$(info $(shell export PKG_CONFIG_LIBDIR=$(PKG_CONFIG_LIBDIR);pkg-config gstreamer-1.0 --exists; echo $$?))
		endif
	else
		ifeq ($(shell pkg-config gstreamer-1.0 --exists; echo $$?),0)
		    GST_VERSION = 1.0
		else
		    GST_VERSION = 0.10
		endif
	endif
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

# add OF_USING_GTK define IF we have it defined as a system library
ifeq ($(HAS_SYSTEM_GTK),0)
    PLATFORM_DEFINES += OF_USING_GTK
endif

# add OF_USING_MPG123 define IF we have it defined as a system library
ifeq ($(HAS_SYSTEM_MPG123),0)
    PLATFORM_DEFINES += OF_USING_MPG123
endif


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

PLATFORM_REQUIRED_ADDONS =

################################################################################
# PLATFORM CFLAGS
#   This is a list of fully qualified CFLAGS required when compiling for this 
#   platform. These flags will always be added when compiling a project or the 
#   core library.  These flags are presented to the compiler AFTER the 
#   PLATFORM_OPTIMIZATION_CFLAGS below. 
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################

# Code Generation Option Flags (http://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html)
PLATFORM_CFLAGS = -Wall 


################################################################################
# PLATFORM LDFLAGS
#   This is a list of fully qualified LDFLAGS required when linking for this 
#   platform. These flags will always be added when linking a project. 
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################

PLATFORM_LDFLAGS = -Wl,-rpath=./libs:./bin/libs -Wl,--as-needed -Wl,--gc-sections




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

# DEBUG Debugging options (http://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html)
PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = -g3

################################################################################
# PLATFORM CORE EXCLUSIONS
#   During compilation, these makefiles will generate lists of sources, headers 
#   and third party libraries to be compiled and linked into a program or core 
#   library. The PLATFORM_CORE_EXCLUSIONS is a list of fully qualified file 
#   paths that will be used to exclude matching paths and files during list 
#   generation.
#
#   Each item in the PLATFORM_CORE_EXCLUSIONS list will be treated as a complete
#   string unless teh user adds a wildcard (%) operator to match subdirectories.
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

ifeq ($(LINUX_ARM),1)
	PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/app/ofAppGlutWindow.cpp
else
	PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/app/ofAppEGLWindow.cpp
endif

# third party
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/glew/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/glu/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/include/Poco
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/include/CppUnit
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/include/Poco/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/include/CppUnit/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/quicktime/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/videoInput/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/freetype/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/FreeImage/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/assimp/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/glut/%

ifeq ($(USE_FMOD),0)
	PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/fmodex/%
	PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/sound/ofFmodSoundPlayer.cpp
endif

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

ifneq ($(LINUX_ARM),1)
	PLATFORM_LIBRARIES += glut
endif
ifneq ($(PLATFORM_ARCH),armv6l)
    PLATFORM_LIBRARIES += X11 
    PLATFORM_LIBRARIES += Xrandr
    PLATFORM_LIBRARIES += Xxf86vm 
    PLATFORM_LIBRARIES += Xi 
endif
    
PLATFORM_LIBRARIES += freeimage

#static libraries (fully qualified paths)
PLATFORM_STATIC_LIBRARIES =
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/poco/lib/$(ABI_LIB_SUBPATH)/libPocoNetSSL.a
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/poco/lib/$(ABI_LIB_SUBPATH)/libPocoNet.a
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/poco/lib/$(ABI_LIB_SUBPATH)/libPocoCrypto.a
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/poco/lib/$(ABI_LIB_SUBPATH)/libPocoUtil.a
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/poco/lib/$(ABI_LIB_SUBPATH)/libPocoXML.a
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/poco/lib/$(ABI_LIB_SUBPATH)/libPocoFoundation.a

# shared libraries 
PLATFORM_SHARED_LIBRARIES =

#openframeworks core third party

#ifneq ($(CROSS_COMPILING),1)
	PLATFORM_PKG_CONFIG_LIBRARIES =
	PLATFORM_PKG_CONFIG_LIBRARIES += cairo
	PLATFORM_PKG_CONFIG_LIBRARIES += zlib
	PLATFORM_PKG_CONFIG_LIBRARIES += gstreamer-app-$(GST_VERSION)
	PLATFORM_PKG_CONFIG_LIBRARIES += gstreamer-$(GST_VERSION)
	PLATFORM_PKG_CONFIG_LIBRARIES += gstreamer-video-$(GST_VERSION)
	PLATFORM_PKG_CONFIG_LIBRARIES += gstreamer-base-$(GST_VERSION)
	PLATFORM_PKG_CONFIG_LIBRARIES += libudev
	PLATFORM_PKG_CONFIG_LIBRARIES += freetype2
	PLATFORM_PKG_CONFIG_LIBRARIES += fontconfig
	PLATFORM_PKG_CONFIG_LIBRARIES += sndfile
	PLATFORM_PKG_CONFIG_LIBRARIES += openal
	PLATFORM_PKG_CONFIG_LIBRARIES += portaudio-2.0
	PLATFORM_PKG_CONFIG_LIBRARIES += openssl
	PLATFORM_PKG_CONFIG_LIBRARIES += libpulse-simple
	PLATFORM_PKG_CONFIG_LIBRARIES += alsa

	ifneq ($(LINUX_ARM),1)
		PLATFORM_PKG_CONFIG_LIBRARIES += gl
		PLATFORM_PKG_CONFIG_LIBRARIES += glu
		PLATFORM_PKG_CONFIG_LIBRARIES += glew
	endif
	
	# conditionally add GTK
	ifeq ($(HAS_SYSTEM_GTK),0)
	    PLATFORM_PKG_CONFIG_LIBRARIES += gtk+-2.0
	endif
	
	# conditionally add mpg123
	ifeq ($(HAS_SYSTEM_MPG123),0)
	    PLATFORM_PKG_CONFIG_LIBRARIES += libmpg123
	endif
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
# These will override the architecture vars generated by configure.platform.mk
################################################################################
#PLATFORM_ARCH =
#PLATFORM_OS =
#PLATFORM_LIBS_PATH =

################################################################################
# PLATFORM CXX
#    Don't want to use a default compiler?
################################################################################
#PLATFORM_CXX=

afterplatform: after
	@echo 
