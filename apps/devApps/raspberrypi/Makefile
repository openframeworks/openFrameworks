# import all of the project configurations
include config.make

#include $(OF_ROOT)/libs/openFrameworksCompiled/project/makefileCommon/Makefile.examples

# define the openframeworks root if it has not been defined
ifndef OF_ROOT
    OF_ROOT=../../..
endif

# uncomment to see debug prints
MAKEFILE_DEBUG=1

#=========================================================================================
# define the root of the project folder
OF_COMPILED_PROJECT_PATH=$(OF_ROOT)/libs/openFrameworksCompiled/project

# define the location of the shared makefiles
MAKEFILE_INCLUDES_PATH=$(OF_COMPILED_PROJECT_PATH)/makefile_includes

ifdef MAKEFILE_DEBUG
    $(info OF_ROOT=$(realpath $(OF_ROOT)))
endif

# do all configs
include $(MAKEFILE_INCLUDES_PATH)/makefile.configure.paths
include $(MAKEFILE_INCLUDES_PATH)/makefile.configure.architecture
include $(MAKEFILE_INCLUDES_PATH)/makefile.configure.platform
include $(MAKEFILE_INCLUDES_PATH)/makefile.configure.libspath

# configured by default
#ifdef PLATFORM_SPECIFIER
#    PLATFORM_CONFIG_SUFFIX=.$(PLATFORM_SPECIFIER)
#endif

PLATFORM_CONFIG_SUFFIX=.raspberrypi

OF_COMPILED_PROJECT_PATH_PLATFORM=$(OF_COMPILED_PROJECT_PATH)/$(PLATFORM_LIB_SUBPATH)

# include the platform specific user and platform configuration files
include $(OF_COMPILED_PROJECT_PATH_PLATFORM)/makefile.configure.user$(PLATFORM_CONFIG_SUFFIX)
include $(OF_COMPILED_PROJECT_PATH_PLATFORM)/makefile.configure.platform$(PLATFORM_CONFIG_SUFFIX)

#=========================================================================================

# gather all of the addons flags
include $(MAKEFILE_INCLUDES_PATH)/makefile.configure.project.addons

# now we have our project project specific information, our source, our addons, etc.

include $(MAKEFILE_INCLUDES_PATH)/makefile.configure.core.paths

ALL_INCLUDES_SO_FAR=$(ALL_OF_CORE_HEADER_PATHS) $(ADDONS_INCLUDES)

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

include $(MAKEFILE_INCLUDES_PATH)/makefile.compile.project














