# import all of the project configurations
include configure.project.make

# uncomment to see debug prints
MAKEFILE_DEBUG=1

PLATFORM_CONFIG_SUFFIX=raspberrypi

# confgure all core paths, excluding platform and project specific paths
include $(OF_ROOT)/libs/openFrameworksCompiled/shared_makefiles/configure.core.paths.make

# determine the platform's architecture, os and form the platform-specific libarary subpath
include $(OF_SHARED_MAKEFILES_PATH)/configure.platform.make

include $(OF_SHARED_MAKEFILES_PATH)/configure.platform.paths.make

# include the platform specific user and platform configuration files
include $(PLATFORM_MAKEFILES_PATH)/configure.user.$(PLATFORM_CONFIG_SUFFIX).make
include $(PLATFORM_MAKEFILES_PATH)/configure.platform.$(PLATFORM_CONFIG_SUFFIX).make


# determine the platform-specific paths

#=========================================================================================

# gather all of the addons flags
include $(MAKEFILE_INCLUDES_PATH)/configure.project.addons.mk

# now we have our project project specific information, our source, our addons, etc.
include $(MAKEFILE_INCLUDES_PATH)/configure.core.paths.mk

# print debug information if needed
ifdef MAKEFILE_DEBUG
    $(info ---ALL_INCLUDES_SO_FAR--- )
    $(foreach v, $(ALL_INCLUDES_SO_FAR),$(info $(v)))
    $(info ---OF_CORE_INCLUDES_CFLAGS--- )
    $(foreach v, $(OF_CORE_INCLUDES_CFLAGS),$(info $(v)))
    $(info ---OF_CORE_STATIC_LIB_CFLAGS--- )
    $(foreach v, $(OF_CORE_STATIC_LIB_CFLAGS),$(info $(v)))
    $(info ---OF_CORE_SHARED_LIB_CFLAGS--- )
    $(foreach v, $(OF_CORE_SHARED_LIB_CFLAGS),$(info $(v)))
    $(info ---OF_CORE_PKG_CONFIG_LIB_CFLAGS--- )
    $(foreach v, $(OF_CORE_PKG_CONFIG_LIB_CFLAGS),$(info $(v)))
endif


ifeq ($(findstring Debug,$(MAKECMDGOALS)),Debug)
	TARGET_NAME = Debug
	BIN_NAME = $(APPNAME)_debug
	TARGET = bin/$(BIN_NAME)
endif

ifeq ($(findstring Release,$(MAKECMDGOALS)),Release)
	TARGET_NAME = Release
	BIN_NAME = $(APPNAME)
	TARGET = bin/$(BIN_NAME)
endif

ifeq ($(MAKECMDGOALS),)
	TARGET_NAME = Release
	BIN_NAME = $(APPNAME)
	TARGET = bin/$(BIN_NAME)
endif


include $(MAKEFILE_INCLUDES_PATH)/makefile.compile.project














