###############################################################################
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
#        configure.linux64.default.make
#
#   Or it can specify a specific platform variant like:
#
#        configure.linuxarmv6l.raspberrypi.make
#
################################################################################

################################################################################
# include common rules
#
#   all linux systems have several rules in common so most of them are included
#   from the following file
#
################################################################################

include $(OF_SHARED_MAKEFILES_PATH)/config.linux.common.mk


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

# defines used inside openFrameworks libs.
PLATFORM_DEFINES += TARGET_RASPBERRY_PI

# TODO many of these are not relevant to openFrameworks (were just pasted from hello_pi examples)
# from raspberry pi examples
PLATFORM_DEFINES += STANDALONE
PLATFORM_DEFINES += PIC
PLATFORM_DEFINES += _REENTRANT
PLATFORM_DEFINES += _LARGEFILE64_SOURCE
PLATFORM_DEFINES += _FILE_OFFSET_BITS=64
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
# PLATFORM REQUIRED ADDONS
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

#PLATFORM_REQUIRED_ADDONS = ofxRaspberryPi

################################################################################
# PLATFORM CFLAGS
#   This is a list of fully qualified CFLAGS required when compiling for this
#   platform. These flags will always be added when compiling a project or the
#   core library.  These flags are presented to the compiler AFTER the
#   PLATFORM_OPTIMIZATION_CFLAGS below.
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################

PLATFORM_CFLAGS += -march=armv6
PLATFORM_CFLAGS += -mfpu=vfp
PLATFORM_CFLAGS += -mfloat-abi=hard
PLATFORM_CFLAGS += -fPIC
PLATFORM_CFLAGS += -ftree-vectorize
PLATFORM_CFLAGS += -Wno-psabi
PLATFORM_CFLAGS += -pipe


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

# raspberry pi specific
PLATFORM_LIBRARIES += GLESv2
PLATFORM_LIBRARIES += GLESv1_CM
PLATFORM_LIBRARIES += EGL
PLATFORM_LIBRARIES += openmaxil
PLATFORM_LIBRARIES += bcm_host
PLATFORM_LIBRARIES += vcos
PLATFORM_LIBRARIES += vchiq_arm
PLATFORM_LIBRARIES += pcre
PLATFORM_LIBRARIES += rt
PLATFORM_LIBRARIES += X11
PLATFORM_LIBRARIES += dl


PLATFORM_LDFLAGS += -pthread


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

# Broadcom hardware interface library
PLATFORM_HEADER_SEARCH_PATHS += $(RPI_ROOT)/opt/vc/include
PLATFORM_HEADER_SEARCH_PATHS += $(RPI_ROOT)/opt/vc/include/IL
PLATFORM_HEADER_SEARCH_PATHS += $(RPI_ROOT)/opt/vc/include/interface/vcos/pthreads
PLATFORM_HEADER_SEARCH_PATHS += $(RPI_ROOT)/opt/vc/include/interface/vmcs_host/linux

##########################################################################################
# PLATFORM LIBRARY SEARCH PATH
#   These are special libraries assocated with the above header search paths
#   Do not use full flag syntax, that will be added automatically later
#   These paths are ABSOLUTE.
#   Simply use space delimited paths.
#   Note: Leave a leading space when adding list items with the += operator
##########################################################################################

PLATFORM_LIBRARY_SEARCH_PATHS += $(RPI_ROOT)/opt/vc/lib


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

PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/app/ofAppGLFWWindow.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/sound/ofFmodSoundPlayer.cpp

ifeq ($(CROSS_COMPILING),1)
$(info detected cross compiling $(CROSS_COMPILING))
	ifdef TOOLCHAIN_ROOT
		#You have specified TOOLCHAIN_ROOT with an environment variable
	else
		TOOLCHAIN_ROOT = /opt/cross/bin
	endif

	ifdef GCC_PREFIX
		#You have specified GCC_PREFIX with an environment variable
	else
		GCC_PREFIX = arm-linux-gnueabihf
	endif

    PLATFORM_CXX = $(TOOLCHAIN_ROOT)/bin/$(GCC_PREFIX)-g++
	PLATFORM_CC = $(TOOLCHAIN_ROOT)/bin/$(GCC_PREFIX)-gcc
	PLATFORM_AR = $(TOOLCHAIN_ROOT)/bin/$(GCC_PREFIX)-ar
	PLATFORM_LD = $(TOOLCHAIN_ROOT)/bin/$(GCC_PREFIX)-ld

	SYSROOT=$(RPI_ROOT)

	PLATFORM_CFLAGS += --sysroot=$(SYSROOT)

	PLATFORM_HEADER_SEARCH_PATHS += $(SYSROOT)/usr/include/c++/4.9
	PLATFORM_HEADER_SEARCH_PATHS += $(SYSROOT)/usr/include/$(GCC_PREFIX)/c++/4.9

	PLATFORM_LIBRARY_SEARCH_PATHS += $(SYSROOT)/usr/lib/$(GCC_PREFIX)
	PLATFORM_LIBRARY_SEARCH_PATHS += $(SYSROOT)/usr/lib/gcc/$(GCC_PREFIX)/4.9

	PLATFORM_LDFLAGS += --sysroot=$(SYSROOT)
	PLATFORM_LDFLAGS += -Xlinker -rpath-link=$(SYSROOT)/usr/lib/$(GCC_PREFIX)
	PLATFORM_LDFLAGS += -Xlinker -rpath-link=$(SYSROOT)/lib/$(GCC_PREFIX)
	PLATFORM_LDFLAGS += -Xlinker -rpath-link=$(SYSROOT)/opt/vc/lib
	PLATFORM_LDFLAGS += -Xlinker -rpath-link=$(SYSROOT)/usr/lib/arm-linux-gnueabihf/pulseaudio

	PKG_CONFIG_LIBDIR=$(SYSROOT)/usr/lib/pkgconfig:$(SYSROOT)/usr/lib/$(GCC_PREFIX)/pkgconfig:$(SYSROOT)/usr/share/pkgconfig

endif
