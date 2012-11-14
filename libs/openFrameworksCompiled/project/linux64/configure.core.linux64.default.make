################################################################################
# CONFIGURE CORE PLATFORM  MAKEFILE
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

#check if gtk exists and add it
HAS_SYSTEM_GTK = $(shell pkg-config gtk+-2.0 --exists; echo $$?)
#check if mpg123 exists and add it
HAS_SYSTEM_MPG123 = $(shell pkg-config libmpg123 --exists; echo $$?)

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

PLATFORM_REQUIRED_ADDON =

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
PLATFORM_CFLAGS = -Wall

# Code Generation Option Flags (http://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html)
PLATFORM_CFLAGS += -fexceptions

# Architecture / Machine Flags (http://gcc.gnu.org/onlinedocs/gcc/Submodel-Options.html)
PLATFORM_CFLAGS += -march=native
PLATFORM_CFLAGS += -mtune=native

# Optimization options (http://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)
PLATFORM_CFLAGS += -finline-functions
#PLATFORM_CFLAGS += -funroll-all-loops
PLATFORM_CFLAGS += -Os

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
PLATFORM_OPTIMIZATION_CFLAGS_RELEASE =

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
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofQtUtils.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofQuickTimeGrabber.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofQuickTimePlayer.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofDirectShowGrabber.cpp

# third party
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/glew/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/glu/include_android/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/glu/include_ios/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/include/Poco/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/include/CppUnit/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/quicktime/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/videoInput/%

# third party static libs (this may not matter due to exclusions in poco's libsorder.make)
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/lib/$(PLATFORM_LIB_SUBPATH)/libPocoCrypto.a
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/lib/$(PLATFORM_LIB_SUBPATH)/libPocoData.a
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/lib/$(PLATFORM_LIB_SUBPATH)/libPocoDataMySQL.a
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/lib/$(PLATFORM_LIB_SUBPATH)/libPocoDataODBC.a
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/lib/$(PLATFORM_LIB_SUBPATH)/libPocoDataSQLite.a
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/lib/$(PLATFORM_LIB_SUBPATH)/libPocoNetSSL.a
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/lib/$(PLATFORM_LIB_SUBPATH)/libPocoZip.a

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
PLATFORM_LIBRARIES += GL
PLATFORM_LIBRARIES += glut
PLATFORM_LIBRARIES += asound
PLATFORM_LIBRARIES += openal
PLATFORM_LIBRARIES += sndfile
PLATFORM_LIBRARIES += vorbis
PLATFORM_LIBRARIES += FLAC
PLATFORM_LIBRARIES += ogg
PLATFORM_LIBRARIES += freeimage

#static libraries (fully qualified paths)
PLATFORM_STATIC_LIBRARIES =

# shared libraries 
PLATFORM_SHARED_LIBRARIES =

#openframeworks core third party
PLATFORM_PKG_CONFIG_LIBRARIES =
PLATFORM_PKG_CONFIG_LIBRARIES += jack
PLATFORM_PKG_CONFIG_LIBRARIES += glu
PLATFORM_PKG_CONFIG_LIBRARIES += cairo
PLATFORM_PKG_CONFIG_LIBRARIES += zlib
PLATFORM_PKG_CONFIG_LIBRARIES += gstreamer-app-0.10
PLATFORM_PKG_CONFIG_LIBRARIES += gstreamer-0.10
PLATFORM_PKG_CONFIG_LIBRARIES += gstreamer-video-0.10
PLATFORM_PKG_CONFIG_LIBRARIES += gstreamer-base-0.10
PLATFORM_PKG_CONFIG_LIBRARIES += libudev
PLATFORM_PKG_CONFIG_LIBRARIES += glew

# conditionally add GTK
ifeq ($(HAS_SYSTEM_GTK),0)
    PLATFORM_PKG_CONFIG_LIBRARIES += gtk+-2.0
endif


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