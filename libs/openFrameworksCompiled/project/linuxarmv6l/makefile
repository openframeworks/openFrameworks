# define the openframeworks root
OF_ROOT =../../../..

# uncomment to see debug prints
MAKEFILE_DEBUG=1

#=========================================================================================
# define the root of the project folder (usually we are in this folder already)
OF_COMPILED_PROJECT_PATH = $(OF_ROOT)/libs/openFrameworksCompiled/project

# define the location of the shared makefiles
MAKEFILE_INCLUDES_PATH = $(OF_COMPILED_PROJECT_PATH)/makefile_includes

ifdef MAKEFILE_DEBUG
    $(info OF_ROOT=$(realpath $(OF_ROOT)))
endif

# do all basic configurations
include $(MAKEFILE_INCLUDES_PATH)/makefile.configure.paths
include $(MAKEFILE_INCLUDES_PATH)/makefile.configure.architecture
include $(MAKEFILE_INCLUDES_PATH)/makefile.configure.platform
include $(MAKEFILE_INCLUDES_PATH)/makefile.configure.libspath

# configured by default
ifdef PLATFORM_SPECIFIER
    PLATFORM_CONFIG_SUFFIX=.$(PLATFORM_SPECIFIER)
endif

PLATFORM_CONFIG_SUFFIX=.raspberrypi

OF_COMPILED_PROJECT_PATH_PLATFORM=$(OF_COMPILED_PROJECT_PATH)/$(PLATFORM_LIB_SUBPATH)

# include the platform specific user and platform configuration files
include $(OF_COMPILED_PROJECT_PATH_PLATFORM)/makefile.configure.user$(PLATFORM_CONFIG_SUFFIX)
include $(OF_COMPILED_PROJECT_PATH_PLATFORM)/makefile.configure.platform$(PLATFORM_CONFIG_SUFFIX)

# configure all of the core-related paths, sources, headers, etc.
include $(MAKEFILE_INCLUDES_PATH)/makefile.configure.core.paths

# print debug information if needed
ifdef MAKEFILE_DEBUG
    $(info ---FULLY_QUALIFIED_CORE_SOURCE_EXCLUSIONS---)
    $(foreach v, $(FULLY_QUALIFIED_CORE_SOURCE_EXCLUSIONS),$(info $(v)))
    $(info ---ALL_CORE_SOURCE_PATHS---)
    $(foreach v, $(ALL_CORE_SOURCE_PATHS),$(info $(v)))
    $(info ---FILTERED_CORE_SOURCE_PATHS--- )
    $(foreach v, $(FILTERED_CORE_SOURCE_PATHS),$(info $(v)))
    $(info ---OF_CORE_SOURCES--- )
    $(foreach v, $(OF_CORE_SOURCES),$(info $(v)))
    $(info ---UNFILTERED_THIRDPARTY_HEADER_PATHS--- )
    $(foreach v, $(UNFILTERED_THIRDPARTY_HEADER_PATHS),$(info $(v)))
    $(info ---CLEANED_THIRDPARTY_HEADER_EXCLUSIONS--- )
    $(foreach v, $(CLEANED_THIRDPARTY_HEADER_EXCLUSIONS),$(info $(v)))
endif

# make the core!
include $(MAKEFILE_INCLUDES_PATH)/makefile.compile.core
