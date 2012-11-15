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

PLATFORM_REQUIRED_ADDON = ofxRaspberryPi

##########################################################################################
# PLATFORM CXX
##########################################################################################

#PLATFORM_CXX=

################################################################################
# PLATFORM CFLAGS
#   This is a list of fully qualified CFLAGS required when compiling for this 
#   platform. These flags will always be added when compiling a project or the 
#   core library.  These flags are presented to the compiler AFTER the 
#   PLATFORM_OPTIMIZATION_CFLAGS below. 
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################

PLATFORM_CFLAGS =
PLATFORM_CFLAGS += -march=armv6
PLATFORM_CFLAGS += -mfpu=vfp
PLATFORM_CFLAGS += -mfloat-abi=hard
PLATFORM_CFLAGS += -fPIC
PLATFORM_CFLAGS += -ftree-vectorize
PLATFORM_CFLAGS += -Wno-psabi
PLATFORM_CFLAGS += -pipe

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
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/app/ofAppGlutWindow.cpp
#PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/gl/ofFbo.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/sound/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofQtUtils.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofQuickTimeGrabber.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofQuickTimePlayer.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofDirectShowGrabber.cpp

# third party

PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/portaudio/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/rtAudio/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/assimp/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/glu/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/videoInput/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/fmodex/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/include/Poco/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/include/CppUnit/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/quicktime/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/glut/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/glew/%


# third party static libs (this may not matter due to exclusions in poco's libsorder.make)
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/lib/$(PLATFORM_LIB_SUBPATH)/libPocoCrypto.a
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/lib/$(PLATFORM_LIB_SUBPATH)/libPocoData.a
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/lib/$(PLATFORM_LIB_SUBPATH)/libPocoDataMySQL.a
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/lib/$(PLATFORM_LIB_SUBPATH)/libPocoDataODBC.a
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/lib/$(PLATFORM_LIB_SUBPATH)/libPocoDataSQLite.a
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/lib/$(PLATFORM_LIB_SUBPATH)/libPocoNetSSL.a
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/poco/lib/$(PLATFORM_LIB_SUBPATH)/libPocoZip.a

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
# raspberry pi specific
PLATFORM_LIBRARIES += GLESv2
PLATFORM_LIBRARIES += EGL
PLATFORM_LIBRARIES += openmaxil
PLATFORM_LIBRARIES += bcm_host
PLATFORM_LIBRARIES += vcos
PLATFORM_LIBRARIES += vchiq_arm
PLATFORM_LIBRARIES += freeimage

#openframeworks core third party
# these can be added if they are part of the core, but are 
# in a different search path location.  Otherwise, all libraries that
# have not been filtered out with PLATFORM_THIRDPARTY_HEADER_EXCLUSIONS
# will be automatically included. 
#PLATFORM_LIBRARIES +=

#static libraries (fully qualified paths)
PLATFORM_STATIC_LIBRARIES =

# shared libraries 
PLATFORM_SHARED_LIBRARIES =

#openframeworks core third party
PLATFORM_PKG_CONFIG_LIBRARIES =
PLATFORM_PKG_CONFIG_LIBRARIES += cairo
PLATFORM_PKG_CONFIG_LIBRARIES += zlib
PLATFORM_PKG_CONFIG_LIBRARIES += gstreamer-app-0.10
PLATFORM_PKG_CONFIG_LIBRARIES += gstreamer-0.10
PLATFORM_PKG_CONFIG_LIBRARIES += gstreamer-video-0.10
PLATFORM_PKG_CONFIG_LIBRARIES += gstreamer-base-0.10

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
# Broadcom hardware interface library
PLATFORM_HEADER_SEARCH_PATHS += /opt/vc/include
PLATFORM_HEADER_SEARCH_PATHS += /opt/vc/include/GLES2
PLATFORM_HEADER_SEARCH_PATHS += /opt/vc/include/GLES
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


##########################################################################################
# PLATFORM FRAMEWORKS
#   These are special frameworks used in OSX.  These will be prefixed with -framework
#   Do not use full flag syntax, that will be added automatically later
#   These paths are ABSOLUTE.
##########################################################################################

#PLATFORM_FRAMEWORKS =


