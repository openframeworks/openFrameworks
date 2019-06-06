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
	HAS_SYSTEM_GTK3 = $(shell export PKG_CONFIG_LIBDIR=$(PKG_CONFIG_LIBDIR);pkg-config gtk+-3.0 --exists; echo $$?)
	HAS_SYSTEM_GTK2 = $(shell export PKG_CONFIG_LIBDIR=$(PKG_CONFIG_LIBDIR);pkg-config gtk+-2.0 --exists; echo $$?)
else
	HAS_SYSTEM_GTK3 = $(shell pkg-config gtk+-3.0 --exists; echo $$?)
	HAS_SYSTEM_GTK2 = $(shell pkg-config gtk+-2.0 --exists; echo $$?)
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
			ifdef MAKEFILE_DEBUG
                $(info GSTVERSION=$(GST_VERSION))
			endif
		else
			GST_VERSION = 0.10
			ifdef MAKEFILE_DEBUG
                $(info GSTVERSION=$(GST_VERSION))
                $(info $(shell export PKG_CONFIG_LIBDIR=$(PKG_CONFIG_LIBDIR);pkg-config gstreamer-1.0 --exists; echo $$?))
			endif
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
ifeq ($(HAS_SYSTEM_GTK2),0)
	PLATFORM_DEFINES += OF_USING_GTK
endif
ifeq ($(HAS_SYSTEM_GTK3),0)
	PLATFORM_DEFINES += OF_USING_GTK
endif

# add OF_USING_MPG123 define IF we have it defined as a system library
ifeq ($(HAS_SYSTEM_MPG123),0)
	PLATFORM_DEFINES += OF_USING_MPG123
endif

# add OF_USE_GST_GL if requested
ifdef USE_GST_GL
	PLATFORM_DEFINES += OF_USE_GST_GL
endif

ifdef OF_USING_STD_FS
	PLATFORM_DEFINES += "OF_USING_STD_FS=1"
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
# find out version of gcc:
# < 4.7.x  c++0x
# >= 4.7.x c++11
# >= 4.9.x c++14
# other compilers c++11 by now
ifeq ($(CXX),g++)
	GCC_MAJOR_EQ_4 := $(shell expr `gcc -dumpversion | cut -f1 -d.` \= 4)
	GCC_MAJOR_GT_4 := $(shell expr `gcc -dumpversion | cut -f1 -d.` \> 4)
	GCC_MINOR_GTEQ_7 := $(shell expr `gcc -dumpversion | cut -f2 -d.` \<= 7)
	GCC_MINOR_GTEQ_9 := $(shell expr `gcc -dumpversion | cut -f2 -d.` \>= 9)
	ifeq ("$(GCC_MAJOR_EQ_4)","1")
		ifeq ("$(GCC_MINOR_GTEQ_7)","1")
			PLATFORM_CFLAGS = -Wall -Werror=return-type -std=c++0x -DHAS_TLS=0
		else
			ifeq ("$(GCC_MINOR_GTEQ_9)","1")
				PLATFORM_CFLAGS = -Wall -Werror=return-type -std=c++14 -DGCC_HAS_REGEX
			else
				PLATFORM_CFLAGS = -Wall -Werror=return-type -std=c++11
			endif
		endif
	endif
	ifeq ("$(GCC_MAJOR_GT_4)","1")
		PLATFORM_CFLAGS = -Wall -Werror=return-type -std=c++14 -DGCC_HAS_REGEX
	endif
else
	ifeq ($(CXX),g++-5)
		PLATFORM_CFLAGS = -Wall -Werror=return-type -std=c++14 -DGCC_HAS_REGEX
	else
		ifeq ($(CXX),g++-4.9)
			PLATFORM_CFLAGS = -Wall -Werror=return-type -std=c++14 -DGCC_HAS_REGEX
		else
			ifeq ($(CXX),g++-4.8)
				PLATFORM_CFLAGS = -Wall -Werror=return-type -std=c++11
			else
				PLATFORM_CFLAGS = -Wall -Werror=return-type -std=c++11
			endif
		endif
	endif
endif


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

ifndef PROJECT_OPTIMIZATION_CFLAGS_RELEASE
	# RELEASE Debugging options (http://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html)
	PLATFORM_OPTIMIZATION_CFLAGS_RELEASE = -O3

	ifneq ($(LINUX_ARM),1)
		PLATFORM_OPTIMIZATION_CFLAGS_RELEASE += -march=native -mtune=native
	endif
else
	PLATFORM_OPTIMIZATION_CFLAGS_RELEASE = $(PROJECT_OPTIMIZATION_CFLAGS_RELEASE)
endif

ifndef PROJECT_OPTIMIZATION_CFLAGS_DEBUG
	# DEBUG Debugging options (http://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html)
	PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = -g3
endif

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
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofDirectShowPlayer.cpp

ifeq ($(LINUX_ARM),1)
	PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/app/ofAppGlutWindow.cpp
else
	PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/app/ofAppEGLWindow.cpp
endif

# third party
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/glew/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/cairo/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/videoInput/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/freetype/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/FreeImage/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/assimp/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/rtAudio/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openssl/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/boost/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/glfw/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/curl/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/uriparser/%

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

	#PLATFORM_LIBRARIES += gstgl-1.0
	#PLATFORM_LIBRARIES += SM
	#PLATFORM_LIBRARIES += ICE
endif
ifneq ($(PLATFORM_ARCH),armv6l)
	PLATFORM_LIBRARIES += X11
	PLATFORM_LIBRARIES += Xrandr
	PLATFORM_LIBRARIES += Xxf86vm
	PLATFORM_LIBRARIES += Xi
	PLATFORM_LIBRARIES += Xcursor
	PLATFORM_LIBRARIES += dl
	PLATFORM_LIBRARIES += pthread
endif

PLATFORM_LIBRARIES += freeimage
ifeq ($(OF_USING_STD_FS),1)
PLATFORM_LIBRARIES += stdc++fs
else
PLATFORM_LIBRARIES += boost_filesystem
PLATFORM_LIBRARIES += boost_system
endif
PLATFORM_LIBRARIES += pugixml
PLATFORM_LIBRARIES += uriparser

#static libraries (fully qualified paths)
PLATFORM_STATIC_LIBRARIES =

# shared libraries
PLATFORM_SHARED_LIBRARIES =

#openframeworks core third party

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
# PLATFORM_PKG_CONFIG_LIBRARIES += openssl
PLATFORM_PKG_CONFIG_LIBRARIES += libcurl

ifeq ($(CROSS_COMPILING),1)
	ifeq "$(shell export PKG_CONFIG_LIBDIR=$(PKG_CONFIG_LIBDIR); pkg-config --exists glfw3 && echo 1)" "1"
		PLATFORM_PKG_CONFIG_LIBRARIES += glfw3
		PLATFORM_LIBRARIES += Xinerama
	endif
else
	ifeq "$(shell pkg-config --exists glfw3 && echo 1)" "1"
		PLATFORM_PKG_CONFIG_LIBRARIES += glfw3
		PLATFORM_LIBRARIES += Xinerama
	endif
endif

ifeq ($(CROSS_COMPILING),1)
	ifeq "$(shell export PKG_CONFIG_LIBDIR=$(PKG_CONFIG_LIBDIR); pkg-config --exists rtaudio && echo 1)" "1"
		PLATFORM_PKG_CONFIG_LIBRARIES += rtaudio
	else
		PLATFORM_LIBRARIES += rtaudio
	endif
else
	ifeq "$(shell pkg-config --exists rtaudio && echo 1)" "1"
		PLATFORM_PKG_CONFIG_LIBRARIES += rtaudio
	else
		PLATFORM_LIBRARIES += rtaudio
	endif
endif


ifneq ($(LINUX_ARM),1)
	PLATFORM_PKG_CONFIG_LIBRARIES += libpulse-simple
	PLATFORM_PKG_CONFIG_LIBRARIES += alsa
	PLATFORM_PKG_CONFIG_LIBRARIES += gl
	PLATFORM_PKG_CONFIG_LIBRARIES += glu
	PLATFORM_PKG_CONFIG_LIBRARIES += glew
endif


# conditionally add GTK
ifeq ($(HAS_SYSTEM_GTK3),0)
	PLATFORM_PKG_CONFIG_LIBRARIES += gtk+-3.0
else
	ifeq ($(HAS_SYSTEM_GTK2),0)
		PLATFORM_PKG_CONFIG_LIBRARIES += gtk+-2.0
	endif
endif

# conditionally add mpg123
ifeq ($(HAS_SYSTEM_MPG123),0)
	PLATFORM_PKG_CONFIG_LIBRARIES += libmpg123
endif

# conditionally add gstreamer-gl
ifdef USE_GST_GL
	PLATFORM_PKG_CONFIG_LIBRARIES += gstreamer-gl-$(GST_VERSION)
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
