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

PLATFORM_PROJECT_RELEASE_TARGET = bin/$(BIN_NAME)/index.html
PLATFORM_PROJECT_DEBUG_TARGET = bin/$(BIN_NAME)/index.html
BYTECODECORE=1
PLATFORM_CORELIB_DEBUG_TARGET = $(OF_CORE_LIB_PATH)/libopenFrameworksDebug.bc
PLATFORM_CORELIB_RELEASE_TARGET = $(OF_CORE_LIB_PATH)/libopenFrameworks.bc

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

PLATFORM_DEFINES = __EMSCRIPTEN__

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

PLATFORM_REQUIRED_ADDONS = ofxEmscripten

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
PLATFORM_CFLAGS =
PLATFORM_CXXFLAGS = -Wall -std=c++17 -Wno-warn-absolute-paths

################################################################################
# PLATFORM LDFLAGS
#   This is a list of fully qualified LDFLAGS required when linking for this
#   platform. These flags will always be added when linking a project.
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################

ifdef PROJECT_EMSCRIPTEN_TOTAL_MEMORY
	PLATFORM_EMSCRIPTEN_TOTAL_MEMORY=$(PROJECT_EMSCRIPTEN_TOTAL_MEMORY)
else
	PLATFORM_EMSCRIPTEN_TOTAL_MEMORY=134217728
endif

CUR_CC = $(CC)
CC := $(CUR_CC) -r

ifdef USE_CCACHE
	ifeq ($(findstring ccache, $(CC)),)
		ORIGINAL_CC = $(CC)
		CC := ccache $(ORIGINAL_CC)
		ORIGINAL_CXX = $(CXX)
		CXX := ccache $(ORIGINAL_CXX)
	endif
endif

PLATFORM_LDFLAGS = -Wl --gc-sections --preload-file bin/data@data --emrun --bind --profiling-funcs -s USE_FREETYPE=1 -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s WEBGL2_BACKWARDS_COMPATIBILITY_EMULATION=1 -s FULL_ES2 -sFULL_ES3=1
PLATFORM_LDFLAGS += --js-library $(OF_ADDONS_PATH)/ofxEmscripten/libs/html5video/lib/emscripten/library_html5video.js
PLATFORM_LDFLAGS += --js-library $(OF_ADDONS_PATH)/ofxEmscripten/libs/html5audio/lib/emscripten/library_html5audio.js

ifdef PROJECT_EMSCRIPTEN_TEMPLATE
	PLATFORM_LDFLAGS += --shell-file $(PROJECT_EMSCRIPTEN_TEMPLATE)
else
	PLATFORM_LDFLAGS += --shell-file $(OF_LIBS_PATH)/openFrameworksCompiled/project/emscripten/template.html
endif

PLATFORM_OPTIMIZATION_LDFLAGS_RELEASE = -O3 -s TOTAL_MEMORY=$(PLATFORM_EMSCRIPTEN_TOTAL_MEMORY) --memory-init-file 1

PLATFORM_OPTIMIZATION_LDFLAGS_DEBUG = -g3 -s TOTAL_MEMORY=134217728 --memory-init-file 1  -s DEMANGLE_SUPPORT=1 -s ASSERTIONS=2

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
PLATFORM_OPTIMIZATION_CFLAGS_RELEASE = -O3

# DEBUG Debugging options (http://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html)
PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = -g3 -s DEMANGLE_SUPPORT=1 -s ASSERTIONS=2

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

# core sources
PLATFORM_CORE_EXCLUSIONS += %.mm
PLATFORM_CORE_EXCLUSIONS += %.m
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/graphics/ofGraphicsCairo.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofDirectShowGrabber.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofDirectShowGrabber.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofDirectShowPlayer.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofGstUtils.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofGstVideoGrabber.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofGstVideoPlayer.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/video/ofMediaFoundationPlayer.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/sound/ofMediaFoundationSoundPlayer.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/communication/%.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/sound/ofFmodSoundPlayer.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/sound/ofOpenALSoundPlayer.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/sound/ofRtAudioSoundStream.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/app/ofAppGlutWindow.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/app/ofAppGLFWWindow.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/app/ofAppEGLWindow.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/graphics/ofCairoRenderer.cpp
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/openFrameworks/gl/ofGLRenderer.cpp
# third party
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/glew/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/boost/include/boost/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/videoInput/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/fmod/%
PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/freetype/lib/%

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
PLATFORM_HEADER_SEARCH_PATHS += "$(OF_ROOT)/addons/ofxEmscripten/src"

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


#static libraries (fully qualified paths)
PLATFORM_STATIC_LIBRARIES =

# shared libraries
PLATFORM_SHARED_LIBRARIES =
#PLATFORM_SHARED_LIBRARIES += $(OF_LIBS_PATH)/freetype/lib/$(ABI_LIB_SUBPATH)/libfreetype.bc


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

afterplatform: $(TARGET_NAME)
	@echo
	@echo "     compiling done"
	@echo "     to launch the application on the default browser, run:"
	@echo
	@echo "     emrun bin/$(BIN_NAME)"
	@echo "     "
	@echo "     some browsers, like safari, don't support webgl"
	@echo "     "
	@emrun --list_browsers 2>/dev/null
	@echo
