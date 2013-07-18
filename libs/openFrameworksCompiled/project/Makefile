################################################################################
# Makefile (openFrameworks Core)
################################################################################
#
#   The openFrameworks makefile system is designed to facilitate simple 
#   platform-specific configurations.
#
#   Read the documentation or simply execute :
#
#   make help 
#
#   for a brief help file.
#
################################################################################

################################################################################
# MAKEFILE_DEBUG
#   This variable controls the debug output throughout the makefile system.
#   To see debug output, set:
#  
#       MAKEFILE_DEBUG:=1
#
################################################################################

ifndef MAKEFILE_DEBUG
    MAKEFILE_DEBUG:=
endif

################################################################################
# LEGACY_SUPPORT
#   This variable controls the legacy support mode for the pre-0.8.0 makefile
#   system.  To enable legacy support, set:
#  
#       LEGACY_SUPPORT:=1
#
################################################################################

ifndef LEGACY_SUPPORT
    LEGACY_SUPPORT:=
endif


################################################################################
# LEGACY_SUPPORT
#   This variable controls the legacy support mode for the pre-0.8.0 makefile
#   system.  To enable legacy support, set:
#  
#       LEGACY_SUPPORT:=1
#
################################################################################

# Call the legacy support file (See file for description)
# include $(OF_SHARED_MAKEFILES_PATH)/config.legacy.mk


################################################################################
# OF_SHARED_MAKEFILES_PATH
#   A path pointing to the location of all of the shared makefiles.  These 
#   files are usually located here:
#  
#    $(PATH_OF_ROOT)/libs/openFrameworksCompiled/project/makefileCommon
#
#   We set a relative directory below because variables like $(PATH_OF_ROOT)
#   may still be undefined when this Makefile is executed.
#
################################################################################

OF_SHARED_MAKEFILES_PATH:=./makefileCommon

# Call the common configuration file (See file for description)
include $(OF_SHARED_MAKEFILES_PATH)/config.common.mk

# Call the core configuration file (See file for description)
include $(OF_SHARED_MAKEFILES_PATH)/config.core.mk

# Call the core compilation file (See file for description)
include $(OF_SHARED_MAKEFILES_PATH)/compile.core.mk
