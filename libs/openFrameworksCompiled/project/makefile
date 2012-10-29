# define the openframeworks root
OF_ROOT=../../..

# uncomment to see debug prints
MAKEFILE_DEBUG=1

#=========================================================================================
# define the folder for the makefile includes
MAKEFILE_INCLUDES_PATH=makefile_includes

ifdef MAKEFILE_DEBUG
    $(info OF_ROOT=$(realpath $(OF_ROOT)))
endif

# do all configs
include $(MAKEFILE_INCLUDES_PATH)/config.paths
include $(MAKEFILE_INCLUDES_PATH)/config.arch
include $(MAKEFILE_INCLUDES_PATH)/config.platform
include $(MAKEFILE_INCLUDES_PATH)/config.libspath

# configured by default
ifdef PLATFORM_SPECIFIER
    PLATFORM_CONFIG_SUFFIX=.$(PLATFORM_SPECIFIER)
endif

PLATFORM_CONFIG_SUFFIX=.raspberrypi

# include the platform specific user and platform configuration files
include $(PLATFORM_LIBS_PATH)/config.user$(PLATFORM_CONFIG_SUFFIX)
include $(PLATFORM_LIBS_PATH)/config.platform$(PLATFORM_CONFIG_SUFFIX)

# make the core!
include $(MAKEFILE_INCLUDES_PATH)/makefile.core
