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
# PLATFORM CFLAGS
#   This is a list of fully qualified CFLAGS required when compiling for this 
#   platform. These flags will always be added when compiling a project or the 
#   core library.  These flags are presented to the compiler AFTER the 
#   PLATFORM_OPTIMIZATION_CFLAGS below. 
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################

PLATFORM_CFLAGS += -march=native
PLATFORM_CFLAGS += -mtune=native
#PLATFORM_CFLAGS += -std=c++11
