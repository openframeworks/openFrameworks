##########################################################################################
# Project User Defined Variables
#   This file is a place where you can add project-specific compiler flags,
#   includes, defines, etc.  See below for more information.
#
##########################################################################################


PROJECT_EXTERNAL_SOURCE_PATHS=

PROJECT_SOURCE_EXCLUSIONS=bin .xcodeproj obj 
# add others here
PROJECT_SOURCE_EXCLUSIONS+= 

##########################################################################################
# PROJECT DEFINES
#   Create a space-delimited list of DEFINES.  The list will be converted into 
#   CFLAGS with the "-D" flag later in the makefile.  An example of fully qualified flag
#   might look something like this: -DTARGET_OPENGLES2
#
#   When making a list as below, make sure to leave a trailing space.
##########################################################################################

PROJECT_DEFINES=

##########################################################################################
# USER OPTIMIZATION FLAGS
#    Use full CFLAG style flags here (i.e. -march=native, etc)
#    These flags are added AFTER any flags defined in the core library makefile.
#
#        PROJECT_OPTIMIZATION_CFLAGS_REQUIRED flags are applied to all targets.
#        PROJECT_OPTIMIZATION_CFLAGS_RELEASE flags are applied to all RELEASE targets.
#        PROJECT_OPTIMIZATION_CFLAGS_DEBUG flags are applied to all DEBUG targets.
#
#    When making a list as below, make sure to leave a trailing space.
##########################################################################################

PROJECT_OPTIMIZATION_CFLAGS_REQUIRED=
PROJECT_OPTIMIZATION_CFLAGS_RELEASE=
PROJECT_OPTIMIZATION_CFLAGS_DEBUG=

##########################################################################################
# PLATFORM HEADER SEARCH PATH
#   These are header search paths that are platform specific and are  specified 
#   using valid paths.  The include flag (i.e. -I) is added automatically.
#
#   When making a list, be sure to leave a trailing space.
##########################################################################################

# this will define all of the directories in your project's src folder as header search paths
PROJECT_HEADER_SEARCH_PATHS=$(shell find src -type d))

# add others here
PROJECT_HEADER_SEARCH_PATHS+=

##########################################################################################
# PROJECT LIBRARY SEARCH PATH
#   These are special libraries assocated with the above header search paths
#   Do not use full flag syntax, that will be added automatically later
#   These paths are ABSOLUTE.
#   Simply use space delimited paths.
#   !!!ALWAYS USE TRAILING SPACES!!!
##########################################################################################

PROJECT_LIBRARY_SEARCH_PATHS=

##########################################################################################
# PROJECT LIBRARIES
#   These are special libraries assocated with the above header search paths
#   Do not use full flag syntax, that will be added automatically later
#   These paths are ABSOLUTE.
#   Simply use space delimited paths.
#   !!!ALWAYS USE TRAILING SPACES!!!
##########################################################################################

PROJECT_LIBRARIES=
PROJECT_STATIC_LIBRARIES=
PROJECT_PKG_CONFIG_LIBRARIES=

##########################################################################################
# PROJECT LIBS PATH
#   This the the path within the (libs/) folders where the platform-specific library 
#   will live.  For example, 64-bit linux will be (linux64), 32-bit linux will be (linux)
##########################################################################################

PROJECT_LIBS_PATH=

##########################################################################################
# Android Specific
##########################################################################################

#PROJECT_ANDROID_COMPILER_OPTIMIZATION=
#PROJECT_NDK_PLATFORM=
# uncomment this for custom package name, must be the same as the java package that contains OFActivity
#PKGNAME = cc.openframeworks.$(APPNAME)

##########################################################################################
# Custom Application Name
#   If you don't want to use the project folder's name.
##########################################################################################
#APPNAME=myappname
