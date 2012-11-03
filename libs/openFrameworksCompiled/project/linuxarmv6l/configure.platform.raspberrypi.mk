##########################################################################################
# PLATFORM SPACE
#   This file is meant to allow for platform specific 
#   modifications to the openFrameworks build environment.
#
##########################################################################################

##########################################################################################
# PLATFORM DEFINES
#   Create a space-delimited list of DEFINES.  The list will be converted into 
#   CFLAGS with the "-D" flag later in the makefile.  An example of fully qualified flag
#   might look something like this: -DTARGET_OPENGLES2
#
#   When making a list as below, make sure to leave a trailing space.
##########################################################################################
PLATFORM_DEFINES =

# defines used inside openFrameworks libs.
PLATFORM_DEFINES += TARGET_NO_SOUND
PLATFORM_DEFINES += TARGET_NO_GLUT
PLATFORM_DEFINES += TARGET_NO_GLU
PLATFORM_DEFINES += TARGET_NO_X11
PLATFORM_DEFINES += TARGET_OPENGLES
PLATFORM_DEFINES += TARGET_OPENGLES2
PLATFORM_DEFINES += TARGET_RASPBERRY_PI

# from raspberry pi examples
PLATFORM_DEFINES += STANDALONE
PLATFORM_DEFINES += PIC
PLATFORM_DEFINES += _REENTRANT
PLATFORM_DEFINES += _LARGEFILE64_SOURCE
PLATFORM_DEFINES += _FILE_OFFSET_BITS=64
PLATFORM_DEFINES += _FORTIFY_SOURCE
PLATFORM_DEFINES += __STDC_CONSTANT_MACROS
PLATFORM_DEFINES += __STDC_LIMIT_MACROS
PLATFORM_DEFINES += TARGET_POSIX
PLATFORM_DEFINES += HAVE_LIBOPENMAX=2
PLATFORM_DEFINES += OMX
PLATFORM_DEFINES += OMX_SKIP64BIT
PLATFORM_DEFINES += USE_EXTERNAL_OMX
PLATFORM_DEFINES += HAVE_LIBBCM_HOST
PLATFORM_DEFINES += USE_EXTERNAL_LIBBCM_HOST
PLATFORM_DEFINES += USE_VCHIQ_ARM


##########################################################################################
# This is the name of the addon to exclude during addons searching excludes.
##########################################################################################
PLATFORM_RROJECT_ADDON_EXCLUSIONS += ofxRaspberryPi

##########################################################################################
# PLATFORM CXX
##########################################################################################

#PLATFORM_CXX=

##########################################################################################
# PLATFORM OPTIMIZATION FLAGS
#    Use full CFLAG style flags here (i.e. -march=native, etc)
#    These flags are added AFTER any flags defined in the core library makefile.
#
#        PLATFORM_OPTIMIZATION_CFLAGS_REQUIRED flags are applied to all targets.
#        PLATFORM_OPTIMIZATION_CFLAGS_RELEASE flags are applied to all RELEASE targets.
#        PLATFORM_OPTIMIZATION_CFLAGS_DEBUG flags are applied to all DEBUG targets.
#
#    When making a list as below, make sure to leave a trailing space.
##########################################################################################

PLATFORM_OPTIMIZATION_CFLAGS_REQUIRED =
PLATFORM_OPTIMIZATION_CFLAGS_REQUIRED += -march=armv6
PLATFORM_OPTIMIZATION_CFLAGS_REQUIRED += -mfpu=vfp
PLATFORM_OPTIMIZATION_CFLAGS_REQUIRED += -mfloat-abi=hard
PLATFORM_OPTIMIZATION_CFLAGS_REQUIRED += -fPIC
PLATFORM_OPTIMIZATION_CFLAGS_REQUIRED += -ftree-vectorize
PLATFORM_OPTIMIZATION_CFLAGS_REQUIRED += -Wno-psabi
PLATFORM_OPTIMIZATION_CFLAGS_REQUIRED += -pipe

#PLATFORM_OPTIMIZATION_CFLAGS_RELEASE=
#PLATFORM_OPTIMIZATION_CFLAGS_DEBUG=

##########################################################################################
# PLATFORM HEADER SEARCH PATH
#   These are header search paths that are platform specific and are  specified 
#   using valid paths.  The include flag (i.e. -I) is added automatically.
#
#   When making a list, be sure to leave a trailing space.
##########################################################################################

PLATFORM_HEADER_SEARCH_PATHS =
# Broadcom hardware interface library
PLATFORM_HEADER_SEARCH_PATHS += /opt/vc/include
#PLATFORM_HEADER_SEARCH_PATHS+=/opt/vc/include/IL
PLATFORM_HEADER_SEARCH_PATHS += /opt/vc/include/interface/vcos/pthreads

# add the raspberry pi addon directory
PLATFORM_HEADER_SEARCH_PATHS += $(OF_ADDONS_PATH)/ofxRaspberryPi/src

##########################################################################################
# PLATFORM LIBRARY SEARCH PATH
#   These are special libraries assocated with the above header search paths
#   Do not use full flag syntax, that will be added automatically later
#   These paths are ABSOLUTE.
#   Simply use space delimited paths.
#   !!!ALWAYS USE TRAILING SPACES!!!
##########################################################################################

PLATFORM_LIBRARY_SEARCH_PATHS =
PLATFORM_LIBRARY_SEARCH_PATHS += /opt/vc/lib

##########################################################################################
# PLATFORM LIBRARIES
#   These are special libraries assocated with the above header search paths
#   Do not use full flag syntax, that will be added automatically later
#   These paths are ABSOLUTE.
#   Simply use space delimited paths.
#   !!!ALWAYS USE TRAILING SPACES!!!
##########################################################################################

PLATFORM_LIBRARIES =
# raspberry pi specific
PLATFORM_LIBRARIES += GLESv2
PLATFORM_LIBRARIES += EGL
PLATFORM_LIBRARIES += openmaxil
PLATFORM_LIBRARIES += bcm_host
PLATFORM_LIBRARIES += vcos
PLATFORM_LIBRARIES += vchiq_arm

#openframeworks core third party
# these can be added if they are part of the core, but are 
# in a different search path location.  Otherwise, all libraries that
# have not been filtered out with PLATFORM_THIRDPARTY_HEADER_EXCLUSIONS
# will be automatically included. 
#PLATFORM_LIBRARIES+=

#static libraries (fully qualified paths)
PLATFORM_STATIC_LIBRARIES =
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/poco/lib/$(PLATFORM_LIB_SUBPATH)/libPocoNet.a
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/libs/poco/lib/$(PLATFORM_LIB_SUBPATH)/libPocoXML.a
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/libs/poco/lib/$(PLATFORM_LIB_SUBPATH)/libPocoUtil.a
PLATFORM_STATIC_LIBRARIES += $(OF_LIBS_PATH)/libs/poco/lib/$(PLATFORM_LIB_SUBPATH)/libPocoFoundation.a
 
 
PLATFORM_SHARED_LIBRARIES =


#openframeworks core third party
PLATFORM_PKG_CONFIG_LIBRARIES =
PLATFORM_PKG_CONFIG_LIBRARIES += cairo
PLATFORM_PKG_CONFIG_LIBRARIES += zlib

##########################################################################################
# PLATFORM ARCHITECTURE
#   This will override the architecture information generated by 
#   makefile.configure.architecture

##########################################################################################

#PLATFORM_ARCH =

##########################################################################################
# PLATFORM OS
#   If not defined here the platform kernel will be determined using (uname -s)
##########################################################################################

#PLATFORM_OS =

##########################################################################################
# PLATFORM LIBS PATH
#   This the the path within the (libs/) folders where the platform-specific library 
#   will live.  For example, 64-bit linux will be (linux64), 32-bit linux will be (linux)
##########################################################################################

#PLATFORM_LIBS_PATH =

##########################################################################################
# PLATFORM CORE SOURCE EXCLUSIONS
#   These are paths and filenames relative to the core library 
#   ($(OF_ROOT)/libs/openFrameworks) source folder.  When a directory is listed 
#   (i.e. /3d), ALL source files in that directory will be excluded.  
#   These relative paths will be fully qualified later.
#   !!!ALWAYS USE TRAILING SPACES!!!
##########################################################################################

PLATFORM_CORE_SOURCE_EXCLUSIONS =
PLATFORM_CORE_SOURCE_EXCLUSIONS += /app/ofAppGlutWindow.cpp
PLATFORM_CORE_SOURCE_EXCLUSIONS += /gl/ofFbo.cpp
PLATFORM_CORE_SOURCE_EXCLUSIONS += /sound
PLATFORM_CORE_SOURCE_EXCLUSIONS += /video

##########################################################################################
# PLATFORM CORE SOURCE INCLUSIONS
#   These are paths and filenames relative to the core library 
#   ($(OF_ROOT)/libs/openFrameworks) source folder. 
#   These relative paths will be fully qualified later.
#   !!!ALWAYS USE TRAILING SPACES!!!
##########################################################################################

PLATFORM_CORE_SOURCE_INCLUSIONS =

##########################################################################################
# PLATFORM CORE LIB HEADER EXCLUSIONS
#   These are paths and filenames relative to the ($(OF_ROOT)/libs/) folder.
#   When a directory is listed (i.e. /3d), ALL source files in that directory will
#   be excluded.
#   These relative paths will be fully qualified later.
##########################################################################################

PLATFORM_THIRDPARTY_HEADER_EXCLUSIONS =
PLATFORM_THIRDPARTY_HEADER_EXCLUSIONS += /portaudio
PLATFORM_THIRDPARTY_HEADER_EXCLUSIONS += /rtAudio
PLATFORM_THIRDPARTY_HEADER_EXCLUSIONS += /assimp
PLATFORM_THIRDPARTY_HEADER_EXCLUSIONS += /glu
PLATFORM_THIRDPARTY_HEADER_EXCLUSIONS += /videoInput
PLATFORM_THIRDPARTY_HEADER_EXCLUSIONS += /fmodex
PLATFORM_THIRDPARTY_HEADER_EXCLUSIONS += /poco/include/Poco
PLATFORM_THIRDPARTY_HEADER_EXCLUSIONS += /poco/include/CppUnit
PLATFORM_THIRDPARTY_HEADER_EXCLUSIONS += /quicktime
PLATFORM_THIRDPARTY_HEADER_EXCLUSIONS += /glut
PLATFORM_THIRDPARTY_HEADER_EXCLUSIONS += /glew


##########################################################################################
# PLATFORM FRAMEWORKS
#   These are special frameworks used in OSX.  These will be prefixed with -framework
#   Do not use full flag syntax, that will be added automatically later
#   These paths are ABSOLUTE.
##########################################################################################

PLATFORM_LIBRARIES =


