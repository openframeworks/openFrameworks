################################################################################
# CONFIGURE PROJECT MAKEFILE
#   This file is where we make project specific configurations.
################################################################################

################################################################################
# Custom Application Name
#   Will default to the project's folder name if not defined.
################################################################################

#APPNAME=myappname

################################################################################
# PROJECT ROOT
#   If not defined, the project root is assumed to be "this" directory (i.e. ./)
################################################################################

PROJECT_ROOT = .

################################################################################
# PROJECT SPECIFIC CHECKS
#   This is a project defined section to create internal makefile flags to 
#   conditionally enable or disable the addition of various features within 
#   this makefile.  For instance, if you want to make changes based on whether
#   GTK is installed, one might test that here and create a variable to check. 
################################################################################
# None

# ################################################################################
# # PROJECT EXTERNAL SOURCE PATHS
# #   These are fully qualified paths that are not within the PROJECT_ROOT folder.
# #   Like source folders in the PROJECT_ROOT, these paths are subject to 
# #   exlclusion via the PROJECT_EXLCUSIONS list.
# #
# #   Note: Leave a leading space when adding list items with the += operator
# ################################################################################

# PROJECT_EXTERNAL_SOURCE_PATHS = 

################################################################################
# PROJECT EXCLUSIONS
#   These makefiles assume that all folders in your current project directory 
#   and any listed in the PROJECT_EXTERNAL_SOURCH_PATHS are are valid locations
#   to look for source code. The any folders or files that match any of the 
#   items in the PROJECT_EXCLUSIONS list below will be ignored.
#
#   Each item in the PROJECT_EXCLUSIONS list will be treated as a complete 
#   string unless teh user adds a wildcard (%) operator to match subdirectories.
#   GNU make only allows one wildcard for matching.  The second wildcard (%) is
#   treated literally.
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################

PROJECT_EXCLUSIONS =
PROJECT_EXCLUSIONS += $(PROJECT_ROOT)/bin%
PROJECT_EXCLUSIONS += $(PROJECT_ROOT)/obj%
PROJECT_EXCLUSIONS += $(PROJECT_ROOT)/%.xcodeproj

################################################################################
# PROJECT LINKER FLAGS
#	These flags will be sent to the linker when compiling the executable.
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################

# Currently, shared libraries that are needed are copied to the 
# $(PROJECT_ROOT)/bin/libs directory.  The following LDFLAGS tell the linker to
# add a runtime path to search for those shared libraries, since they aren't 
# incorporated directly into the final executable application binary.
# TODO: should this be a default setting?
PROJECT_LDFLAGS=-Wl,-rpath=./libs

################################################################################
# PROJECT DEFINES
#   Create a space-delimited list of DEFINES. The list will be converted into 
#   CFLAGS with the "-D" flag later in the makefile.
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################

PROJECT_DEFINES = 

################################################################################
# PROJECT CFLAGS
#   This is a list of fully qualified CFLAGS required when compiling for this 
#   project.  These CFLAGS will be used IN ADDITION TO the PLATFORM_CFLAGS 
#   defined in your platform specific core configuration files. These flags are
#   presented to the compiler BEFORE the PROJECT_OPTIMIZATION_CFLAGS below. 
#
#   Note: Before adding PROJECT_CFLAGS, note that the PLATFORM_CFLAGS defined in 
#   your platform specific configuration file will be applied by default and 
#   further flags here may not be needed.
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################

PROJECT_CFLAGS = 

################################################################################
# PROJECT OPTIMIZATION CFLAGS
#   These are lists of CFLAGS that are target-specific.  While any flags could 
#   be conditionally added, they are usually limited to optimization flags. 
#   These flags are added BEFORE the PROJECT_CFLAGS.
#
#   PROJECT_OPTIMIZATION_CFLAGS_RELEASE flags are only applied to RELEASE targets.
#
#   PROJECT_OPTIMIZATION_CFLAGS_DEBUG flags are only applied to DEBUG targets.
#
#   Note: Before adding PROJECT_OPTIMIZATION_CFLAGS, please note that the 
#   PLATFORM_OPTIMIZATION_CFLAGS defined in your platform specific configuration 
#   file will be applied by default and further optimization flags here may not 
#   be needed.
#
#   Note: Leave a leading space when adding list items with the += operator
################################################################################

PROJECT_OPTIMIZATION_CFLAGS_RELEASE=
PROJECT_OPTIMIZATION_CFLAGS_DEBUG=

