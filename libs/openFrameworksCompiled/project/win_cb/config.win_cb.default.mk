
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
PLATFORM_RUN_COMMAND = 

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

PLATFORM_DEFINES =

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

# Optimization options (http://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)
PLATFORM_CFLAGS += -finline-functions
#PLATFORM_CFLAGS += -funroll-all-loops
#PLATFORM_CFLAGS += -Os

PLATFORM_CFLAGS += -funroll-loops 
PLATFORM_CFLAGS += -mssse3
PLATFORM_CFLAGS += -fmessage-length=0

PLATFORM_CFLAGS += -Wno-multichar
PLATFORM_CFLAGS += -g 

################################################################################
# PLATFORM LDFLAGS
#   This is a list of fully qualified LDFLAGS required when linking for this 
#   platform. These flags will always be added when linking a project.
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################


#PLATFORM_LDFLAGS += -arch i386

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
PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = -g3

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
#PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofDirectShowGrabber.cpp

PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofDirectShowPlayer.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofGstUtils.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofGstVideoGrabber.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofGstVideoPlayer.cpp

PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/app/ofAppEGLWindow.cpp

PLATFORM_CORE_EXCLUSIONS += %.mm
PLATFORM_CORE_EXCLUSIONS += %.m

# third party

PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/include/Poco
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/include/CppUnit
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/include/Poco/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/include/CppUnit/%
#PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/videoInput/%
#PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/quicktime/%


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

PLATFORM_LIBRARIES += opengl32 gdi32 msimg32 glu32 dsound winmm dxguid strmiids 
PLATFORM_LIBRARIES += uuid ole32 oleaut32 setupapi wsock32 ws2_32 Iphlpapi Comdlg32

#static libraries (fully qualified paths)
# libssl and libcrypto need to come *after* Poco libs for the linker, and the Poco libs 
# need to be linked in the right order.
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/poco/lib/$(ABI_LIB_SUBPATH)/libPocoNetSSL.a
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/poco/lib/$(ABI_LIB_SUBPATH)/libPocoNet.a
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/poco/lib/$(ABI_LIB_SUBPATH)/libPocoCrypto.a
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/poco/lib/$(ABI_LIB_SUBPATH)/libPocoUtil.a
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/poco/lib/$(ABI_LIB_SUBPATH)/libPocoXML.a
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/poco/lib/$(ABI_LIB_SUBPATH)/libPocoFoundation.a
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/poco/lib/$(ABI_LIB_SUBPATH)/libPocoZip.a
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/openssl/lib/$(ABI_LIB_SUBPATH)/libssl.a
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/openssl/lib/$(ABI_LIB_SUBPATH)/libcrypto.a
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/videoInput/lib/$(ABI_LIB_SUBPATH)/videoInputLib.a 
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/FreeImage/lib/$(ABI_LIB_SUBPATH)/FreeImage.lib 
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/quicktime/lib/$(ABI_LIB_SUBPATH)/qtmlClient.lib 
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/glew/lib/$(ABI_LIB_SUBPATH)/libglew32.a 
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/glu/lib/$(ABI_LIB_SUBPATH)/glu32.lib

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

afterplatform: after
	@echo 