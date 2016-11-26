#------------------------------------------------------------------------------
# This file detects the current platform, creates the needed variables for that
# platform, generates the variables for the paths to the code, addons, libraries...
# and the list of source code for the core and common include search paths for the
# core and projects. It's included from both the core makefile and the projects
# makefile


# #####################  PLATFORM DETECTION ###################################
# determine the platform's architecture, os and form the platform-specific libarary subpath
#   If they haven't already been defined, this file will generate the following
#   variables
#
#   $(PLATFORM_OS) (e.g. Linux, Darwin, etc.).
#   $(PLATFORM_ARCH) (e.g. armv6l, x86_64, i386, etc.).
#   $(PLATFORM_LIB_SUBPATH) (e.g. linuxarmv6l, osx, linux64, linux, etc)
################################################################################
#


SHELL ?= /bin/sh
OF_ROOT ?=  $(realpath ../../..)
PLATFORM_VARIANT ?= default

ifeq ($(CC),$(EMSCRIPTEN)/emcc)
	PLATFORM_OS=emscripten
endif
PLATFORM_OS ?= $(shell uname -s)

FIND=find
HOST_OS=$(shell uname -s)
$(info HOST_OS=${HOST_OS})

#check for Raspbian as armv7l needs to use armv6l architecture
ifeq ($(wildcard $(RPI_ROOT)/etc/*-release), /etc/os-release)
	ifeq ($(shell grep ID=raspbian $(RPI_ROOT)/etc/*-release),ID=raspbian)
		IS_RASPBIAN=1
	endif
endif

ifdef IS_RASPBIAN
	PLATFORM_ARCH=armv6l
	HOST_ARCH=armv6l
	ifdef RPI_ROOT
		CROSS_COMPILING=1
	endif
else
	HOST_ARCH=$(shell uname -m)
	ifndef PLATFORM_ARCH
		# determine from the uname
		PLATFORM_ARCH=$(shell uname -m)
	endif
	ifndef CROSS_COMPILING
		ifneq ($(HOST_ARCH),$(PLATFORM_ARCH))
			CROSS_COMPILING=1
		else
			CROSS_COMPILING=0
		endif
	endif
endif

#$(info PLATFORM_ARCH=$(PLATFORM_ARCH))
#$(info PLATFORM_OS=$(PLATFORM_OS))
#$(info HOST_ARCH=$(HOST_ARCH))
#$(info HOST_OS=$(HOST_OS))
#$(info CROSS_COMPILING=$(CROSS_COMPILING))
#$(info PLATFORM_VARIANT=$(PLATFORM_VARIANT))
#$(info IS_RASPBIAN=$(IS_RASPBIAN))

# if not defined, construct the default PLATFORM_LIB_SUBPATH
ifndef PLATFORM_LIB_SUBPATH
	# determine from the arch
	ifeq ($(PLATFORM_OS),Linux)
		ifeq ($(PLATFORM_ARCH),x86_64)
			PLATFORM_LIB_SUBPATH=linux64
		else ifeq ($(PLATFORM_ARCH),armv6l)
			PLATFORM_LIB_SUBPATH=linuxarmv6l
		else ifeq ($(PLATFORM_ARCH),armv7l)
			PLATFORM_LIB_SUBPATH=linuxarmv7l
		else ifeq ($(PLATFORM_ARCH),i386)
			PLATFORM_LIB_SUBPATH=linux
		else ifeq ($(PLATFORM_ARCH),i686)
			PLATFORM_LIB_SUBPATH=linux
		else
			$(error This makefile does not support your architecture $(PLATFORM_ARCH))
		endif
		SHARED_LIB_EXTENSION=so
	else ifneq (,$(findstring MINGW32_NT,$(PLATFORM_OS)))
		PLATFORM_LIB_SUBPATH=msys2
		SHARED_LIB_EXTENSION=dll
	else ifneq (,$(findstring MSYS_NT,$(PLATFORM_OS)))
		PLATFORM_LIB_SUBPATH=msys2
		SHARED_LIB_EXTENSION=dll
	else ifneq (,$(findstring MINGW64_NT,$(PLATFORM_OS)))
		PLATFORM_LIB_SUBPATH=msys2
		SHARED_LIB_EXTENSION=dll
	else ifeq ($(PLATFORM_OS),Android)
		PLATFORM_LIB_SUBPATH=android
		SHARED_LIB_EXTENSION=so
	else ifeq ($(PLATFORM_OS),Darwin)
		PLATFORM_LIB_SUBPATH=osx
		SHARED_LIB_EXTENSION=dylib
	else ifeq ($(PLATFORM_OS),emscripten)
		PLATFORM_LIB_SUBPATH=emscripten
		SHARED_LIB_EXTENSION=so
	else
		$(error This makefile does not support your operating system)
	endif
endif


# TODO: add appropriate list of platform suffixes
# these variables will actually be used during compilation
# http://en.wikipedia.org/wiki/Library_(computing)#File_naming
#ifndef PLATFORM_SHARED_LIB_SUFFIXES
# i.e. if osx check for .dylib AND .so
# if linux, just look for so
#endif
#ifndef PLATFORM_STATIC_LIB_SUFFIXES
#endif

# if desired, print the variables
ifdef MAKEFILE_DEBUG
	$(info =================== config.mk platform detection ================)
	$(info PLATFORM_ARCH=$(PLATFORM_ARCH))
	$(info PLATFORM_OS=$(PLATFORM_OS))
	$(info PLATFORM_VARIANT=$(PLATFORM_VARIANT))
	$(info PLATFORM_LIB_SUBPATH=$(PLATFORM_LIB_SUBPATH))
endif


# #####################  PATHS  ###############################################
# confgure all core paths, excluding platform and project specific paths
#
#   $(OF_ADDONS_PATH)
#   $(OF_EXAMPLES_PATH)
#   $(OF_APPS_PATH)
#   $(OF_LIBS_PATH)
#
#   $(OF_LIBS_OPENFRAMEWORKS_PATH)
#   $(OF_LIBS_OF_COMPILED_PATH)
#   $(OF_LIBS_OF_COMPILED_PROJECT_PATH)
#   $(OF_MAKEFILE_INCLUDES_PATH)
#
#   $(OF_ROOT) must be defined previously.
################################################################################


################################################################################
# create path definitions
ifndef OF_ADDONS_PATH
	OF_ADDONS_PATH=$(OF_ROOT)/addons
endif
ifndef OF_EXAMPLES_PATH
	OF_EXAMPLES_PATH=$(OF_ROOT)/examples
endif
ifndef OF_APPS_PATH
	OF_APPS_PATH=$(OF_ROOT)/apps
endif
ifndef OF_LIBS_PATH
	OF_LIBS_PATH=$(OF_ROOT)/libs
endif

################################################################################
ifndef OF_LIBS_OPENFRAMEWORKS_PATH
	OF_LIBS_OPENFRAMEWORKS_PATH=$(OF_LIBS_PATH)/openFrameworks
endif
ifndef OF_LIBS_OF_COMPILED_PATH
	OF_LIBS_OF_COMPILED_PATH=$(OF_LIBS_OPENFRAMEWORKS_PATH)Compiled
endif
ifndef OF_LIBS_OF_COMPILED_PROJECT_PATH
	OF_LIBS_OF_COMPILED_PROJECT_PATH=$(OF_LIBS_OF_COMPILED_PATH)/project
endif
ifndef OF_SHARED_MAKEFILES_PATH
	OF_SHARED_MAKEFILES_PATH=$(OF_LIBS_OF_COMPILED_PROJECT_PATH)/makefileCommon
endif

ifdef OF_LIBS_OF_COMPILED_PROJECT_PATH
	OF_PLATFORM_MAKEFILES=$(OF_LIBS_OF_COMPILED_PROJECT_PATH)/$(PLATFORM_LIB_SUBPATH)
else
	$(error OF_LIBS_OF_COMPILED_PATH is not defined)
endif

ifndef OF_CORE_LIB_PATH
	OF_CORE_LIB_PATH=$(OF_LIBS_OF_COMPILED_PATH)/lib/$(PLATFORM_LIB_SUBPATH)
endif

################################################################################
# print debug information if needed
ifdef MAKEFILE_DEBUG
	$(info =================== config.mk paths =============================)
	$(info OF_ADDONS_PATH=$(OF_ADDONS_PATH))
	$(info OF_EXAMPLES_PATH=$(OF_EXAMPLES_PATH))
	$(info OF_APPS_PATH=$(OF_APPS_PATH))
	$(info OF_LIBS_PATH=$(OF_LIBS_PATH))
	$(info OF_LIBS_OPENFRAMEWORKS_PATH=$(OF_LIBS_OPENFRAMEWORKS_PATH))
	$(info OF_LIBS_OF_COMPILED_PATH=$(OF_LIBS_OF_COMPILED_PATH))
	$(info OF_LIBS_OF_COMPILED_PROJECT_PATH=$(OF_LIBS_OF_COMPILED_PROJECT_PATH))
	$(info OF_SHARED_MAKEFILES_PATH=$(OF_SHARED_MAKEFILES_PATH))
	$(info OF_PLATFORM_MAKEFILES=$(OF_PLATFORM_MAKEFILES))
	$(info OF_CORE_LIB_PATH=$(OF_CORE_LIB_PATH))
endif


ifeq ($(wildcard $(OF_LIBS_OF_COMPILED_PROJECT_PATH)/$(PLATFORM_LIB_SUBPATH)),)
$(error This package doesn't support your platform, probably you downloaded the wrong package?)
endif

# generate a list of valid core platform variants from the files in the platform makefiles directory
AVAILABLE_PLATFORM_VARIANTS=$(shell $(FIND) $(OF_PLATFORM_MAKEFILES)/config.*.mk -maxdepth 1 -type f | sed -E 's/.*\.([^\.]*)\.mk/\1/' )
AVAILABLE_PLATFORM_VARIANTS+=default

# check to see if we have a file for the desired variant.  if not, quit and list the variants.
ifeq ($(findstring $(PLATFORM_VARIANT),$(AVAILABLE_PLATFORM_VARIANTS)),)
	$(error Platform Variant "$(PLATFORM_VARIANT)" is not valid. Valid variants include [$(strip $(AVAILABLE_PLATFORM_VARIANTS))])
endif

# include the platform specific user and platform configuration files
include $(OF_PLATFORM_MAKEFILES)/config.$(PLATFORM_LIB_SUBPATH).$(PLATFORM_VARIANT).mk

ifdef ABI_PATH
	ABI_LIB_SUBPATH=$(PLATFORM_LIB_SUBPATH)/$(strip $(ABI_PATH))
else
	ABI_LIB_SUBPATH=$(PLATFORM_LIB_SUBPATH)
endif

PLATFORM_PKG_CONFIG ?= pkg-config


################################ FLAGS #########################################
# define the location of the core path
#TODO: make sure all of the right checks are here.
ifndef PLATFORM_CORE_EXCLUSIONS
	$(error PLATFORM_CORE_EXCLUSIONS not defined)
endif

ifndef OF_LIBS_OPENFRAMEWORKS_PATH
	$(error OF_LIBS_OPENFRAMEWORKS_PATH not defined)
endif

################################################################################
# CLEAN CORE EXCLUSIONS
################################################################################

# take from the platform core exclusions and strip and collapse spaces
CORE_EXCLUSIONS = $(strip $(PLATFORM_CORE_EXCLUSIONS))

################################################################################
# OF CORE HEADER INCLUDES (-I ...)
################################################################################

# find all of the source directories
# grep -v "/\.[^\.]" will exclude all .hidden folders and files
ALL_OF_CORE_SOURCE_PATHS=$(shell $(FIND) $(OF_LIBS_OPENFRAMEWORKS_PATH) -maxdepth 1 -mindepth 1 -type d | grep -v "/\.[^\.]" )

# create a list of core source PATHS, filtering out any  items that have a match in the CORE_EXCLUSIONS list
OF_CORE_SOURCE_PATHS=$(filter-out $(CORE_EXCLUSIONS),$(ALL_OF_CORE_SOURCE_PATHS))

# create our core include paths from the source directory paths,
# these have already been filtered and processed according to rules.
# plus the root so that we don't miss the ofMain.h.
OF_CORE_HEADER_PATHS = $(OF_LIBS_OPENFRAMEWORKS_PATH) $(OF_CORE_SOURCE_PATHS)

# add folders or single files to exclude fromt he compiled lib
# grep -v "/\.[^\.]" will exclude all .hidden folders and files
ALL_OF_CORE_THIRDPARTY_HEADER_PATHS = $(shell $(FIND) $(OF_LIBS_PATH)/*/include -type d | grep -v "/\.[^\.]")

# filter out all excluded files / folders that were defined above
OF_CORE_THIRDPARTY_HEADER_PATHS = $(filter-out $(CORE_EXCLUSIONS),$(ALL_OF_CORE_THIRDPARTY_HEADER_PATHS))

# generate the list of core includes
# 1. Add the header search paths defined by the platform config files.
OF_CORE_INCLUDES_CFLAGS = $(addprefix -I,$(PLATFORM_HEADER_SEARCH_PATHS))
# 2. Add all of the system library search paths defined by the platform config files.
CORE_PKG_CONFIG_LIBRARIES =
CORE_PKG_CONFIG_LIBRARIES += $(PLATFORM_PKG_CONFIG_LIBRARIES)
CORE_PKG_CONFIG_LIBRARIES += $(PROJECT_PKG_CONFIG_LIBRARIES)

ifneq ($(strip $(CORE_PKG_CONFIG_LIBRARIES)),)
ifneq ($(strip $(PKG_CONFIG_LIBDIR)),)
$(info checking pkg-config libraries: $(CORE_PKG_CONFIG_LIBRARIES))
$(info with PKG_CONFIG_LIBDIR=$(PKG_CONFIG_LIBDIR))
FAILED_PKG=$(shell export PKG_CONFIG_LIBDIR=$(PKG_CONFIG_LIBDIR); for pkg in $(CORE_PKG_CONFIG_LIBRARIES); do $(PLATFORM_PKG_CONFIG) $$pkg --cflags > /dev/null; if [ $$? -ne 0 ]; then echo $$pkg; return; fi; done; echo 0)
else
$(info checking pkg-config libraries: $(CORE_PKG_CONFIG_LIBRARIES))
$(info with PKG_CONFIG_LIBDIR=$(PKG_CONFIG_LIBDIR))
FAILED_PKG=$(shell for pkg in $(CORE_PKG_CONFIG_LIBRARIES); do $(PLATFORM_PKG_CONFIG) $$pkg --cflags > /dev/null; if [ $$? -ne 0 ]; then echo $$pkg; return; fi; done; echo 0)
endif
	ifneq ($(FAILED_PKG),0)
$(error couldn't find $(FAILED_PKG) pkg-config package or it's dependencies, did you run the latest install_dependencies.sh?)
	endif
	ifeq ($(CROSS_COMPILING),1)
		OF_CORE_INCLUDES_CFLAGS += $(patsubst -I$(SYSROOT)$(SYSROOT)%,-I$(SYSROOT)% ,$(patsubst -I%,-I$(SYSROOT)% ,$(shell export PKG_CONFIG_LIBDIR=$(PKG_CONFIG_LIBDIR);$(PLATFORM_PKG_CONFIG) "$(CORE_PKG_CONFIG_LIBRARIES)" --cflags)))
	else
		OF_CORE_INCLUDES_CFLAGS += $(shell $(PLATFORM_PKG_CONFIG) "$(CORE_PKG_CONFIG_LIBRARIES)" --cflags)
	endif
endif

# 3. Add all of the standard OF third party library headers (these have already been filtered above according to the platform config files)
OF_CORE_INCLUDES_CFLAGS += $(addprefix -I,$(OF_CORE_THIRDPARTY_HEADER_PATHS))
# 4. Add all of the core OF headers(these have already been filtered above according to the platform config files)
OF_CORE_INCLUDES_CFLAGS += $(addprefix -I,$(OF_CORE_HEADER_PATHS))


################################################################################
# OF CORE DEFINES
################################################################################

OF_CORE_DEFINES_CFLAGS=$(addprefix -D,$(PLATFORM_DEFINES))

################################################################################
# OF PLATFORM CFLAGS
################################################################################

# gather any platform CFLAGS
OF_CORE_BASE_CFLAGS=$(PLATFORM_CFLAGS)
OF_CORE_BASE_CXXFLAGS=$(PLATFORM_CXXFLAGS)


################################################################################
# CORE SOURCE FILES
################################################################################

# search the directories in the source folders for all .cpp files
# filter out all excluded files / folders that were defined above
# grep -v "/\.[^\.]" will exclude all .hidden folders and files
OF_CORE_SOURCE_FILES=$(filter-out $(CORE_EXCLUSIONS),$(shell $(FIND) $(OF_CORE_SOURCE_PATHS) -name "*.cpp" -or -name "*.mm" -or -name "*.m" | grep -v "/\.[^\.]"))
OF_CORE_HEADER_FILES=$(filter-out $(CORE_EXCLUSIONS),$(shell $(FIND) $(OF_CORE_SOURCE_PATHS) -name "*.h" | grep -v "/\.[^\.]"))




################################################################################
# DEBUG INFO
################################################################################
ifdef MAKEFILE_DEBUG
	$(info ========================= config.mk flags ========================)
	$(info ---OF_CORE_DEFINES_CFLAGS---)
	$(foreach v, $(OF_CORE_DEFINES_CFLAGS),$(info $(v)))

	$(info ---OF_CORE_INCLUDES_CFLAGS---)
	$(foreach v, $(OF_CORE_INCLUDES_CFLAGS),$(info $(v)))

	$(info ---OF_CORE_FRAMEWORKS_CFLAGS---)
	$(foreach v, $(OF_CORE_FRAMEWORKS_CFLAGS),$(info $(v)))

	$(info ---OF_CORE_SOURCE_FILES---)
	$(foreach v, $(OF_CORE_SOURCE_FILES),$(info $(v)))

	$(info ---OF_CORE_HEADER_FILES---)
	$(foreach v, $(OF_CORE_HEADER_FILES),$(info $(v)))

	$(info ---PLATFORM_CORE_EXCLUSIONS---)
	$(foreach v, $(PLATFORM_CORE_EXCLUSIONS),$(info $(v)))
endif


