################################################################################
# OPENFRAMEWORKS MAKEFILE
#   The openFrameworks makefile system is desiged for ease of use, and 
#   particularly for straight-forward platform-based configurations.
#
#   Read the documentation or simply execute :
#
#   make help 
#
#   for a brief help file.
#
################################################################################

# define the OF_SHARED_MAKEFILES location
OF_SHARED_MAKEFILES_PATH=./makefileCommon

# do all of the core cofiguration work.
# configuration work basically means creating lots of
# lists of source files, search paths, libraries, etc.
#
ifndef OF_ROOT
    OF_ROOT=../../..
endif

ifndef SHELL
    SHELL := /bin/sh
endif

# If the user has not specified a special variant at the command line, use the 
# default variant. To define a specific variant, use a command like:
#
#     make PLATFORM_VARIANT=pandaboard
#
# and make sure that the corresponding platform variant file is available.

ifndef PLATFORM_VARIANT
	PLATFORM_VARIANT=default
endif

# include the global configuration file
include $(OF_SHARED_MAKEFILES_PATH)/config.shared.make

# compile core openFrameworks library check for platform specific compile rules
# if there's none use common ones
ifeq ($(wildcard $(OF_PLATFORM_MAKEFILES)/compile.core.make),) 
    include $(OF_SHARED_MAKEFILES_PATH)/compile.core.make
else 
    include $(OF_PLATFORM_MAKEFILES)/compile.core.make
endif
