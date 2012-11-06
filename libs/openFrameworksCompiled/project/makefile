# where is this file located with respect to the root?
OF_ROOT=../../..

# setup the shell
SHELL = /bin/sh

# if the user has not specified a special variant, then use the default variant
ifndef PLATFORM_VARIANT
	PLATFORM_VARIANT=default
endif

# confgure all core paths, excluding platform and project specific paths
include $(OF_ROOT)/libs/openFrameworksCompiled/project/makefileCommon/configure.core.paths.make

# determine the platform's architecture, os and form the platform-specific libarary subpath
include $(OF_SHARED_MAKEFILES_PATH)/configure.core.platform.make

include $(OF_SHARED_MAKEFILES_PATH)/configure.core.platform.paths.make

# generate a list of valid core platform variants from the files in the platform makefiles directory
AVAILABLE_PLATFORM_VARIANTS=$(shell find $(OF_PLATFORM_MAKEFILES)/configure.core*.make -maxdepth 1 -type f | sed -r 's/.*\.([^\.]*)\.make/\1/' )

# check to see if we have a file for the desired variant.  if not, quit and list the variants.
ifeq ($(findstring $(PLATFORM_VARIANT),$(AVAILABLE_PLATFORM_VARIANTS)),)
    $(error Platform Variant "$(PLATFORM_VARIANT)" is not valid. Valid variants include [$(strip $(AVAILABLE_PLATFORM_VARIANTS))])
endif

# include the platform specific user and platform configuration files
include $(OF_PLATFORM_MAKEFILES)/configure.core.$(PLATFORM_LIB_SUBPATH).$(PLATFORM_VARIANT).make

# prepare core flags, including platform flags
include $(OF_SHARED_MAKEFILES_PATH)/configure.core.flags.make

# compile core openFrameworks library
include $(OF_SHARED_MAKEFILES_PATH)/compile.core.make