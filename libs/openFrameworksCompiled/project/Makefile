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

MAKEFILE_DEBUG:=1

################################################################################
# PATH_OF_SHARED_MAKEFILES
#   A path pointing to the location of all of the shared makefiles.  These 
#   files are usually located here:
#  
#    $(PATH_OF_ROOT)/libs/openFrameworksCompiled/project/makefileCommon
#
#   We set a relative directory below because variables like $(PATH_OF_ROOT)
#   may still be undefined when this Makefile is executed.
#
################################################################################

PATH_OF_SHARED_MAKEFILES:=./makefileCommon

# Call the shared configuration file (See file for description)
include $(PATH_OF_SHARED_MAKEFILES)/config.shared.mk

# Call the core compilation file (See file for description)
include $(PATH_OF_SHARED_MAKEFILES)/compile.core.mk
