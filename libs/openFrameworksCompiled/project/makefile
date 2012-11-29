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

# define the OF_SHARED_MAKEFILES_PATH location
OF_SHARED_MAKEFILES_PATH=./makefileCommon

# do all of the core cofiguration work.
# configuration work basically means creating lots of
# lists of source files, search paths, libraries, etc.
#
# configure.core.make calls other make modules that 
# determine the current platform, load platform specific
# configuration files, etc.
include $(OF_SHARED_MAKEFILES_PATH)/configure.core.make

# compile core openFrameworks library
# this is where the core targets reside
include $(OF_SHARED_MAKEFILES_PATH)/compile.core.make