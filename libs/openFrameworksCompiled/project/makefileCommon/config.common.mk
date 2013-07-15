################################################################################
# config.shared.mk
################################################################################
#
#   This file detects the current platform and creates all shared variables
#   required for the compilation of the core libraries, addons and projects. 
#   This file is referenced by the core makefile:
#
#      $(PATH_OF_ROOT)/libs/openFrameworksCompiled/project/Makefile
#
#   and also the project makefile:
#    
#      $(PATH_OF_PROJECT)/Makefile
#
#   Many of the shared variables produced by this file can be set by the user
#   before this files is called.  See below for examples.
#
################################################################################

################################################################################
# SHELL (conditionally set)
#   The SHELL variable is used when embedded shell scripts are called.  These
#   appear in the form:
#
#       MY_VAR = $(shell my_shell_command)
#
################################################################################
ifndef SHELL
    SHELL := /bin/sh
endif

################################################################################
# PATH_OF_ROOT (conditionally set)
#   The PATH_OF_ROOT is used throughout the script to locate the root folder 
#   of the openFrameworks installation directory.  This variable is often set
#   within custom project files such as $(PATH_OF_PROJECT)/config.make when
#   a project is in a non-standard location.
#
################################################################################
ifndef PATH_OF_ROOT
    PATH_OF_ROOT := ../../..
endif

################################################################################
# PLATFORM_VARIANT (conditionally set)
#   (e.g. default, raspberrypi, beagleboneblack, etc.)
# PLATFORM_OS (conditionally set)
#   (e.g. Linux, Darwin, etc.)
# PLATFORM_ARCH (conditionally set)
#   (e.g. armv6l, x86_64, i386, etc.)
#
#   Determine the build's variant, the platform's architecture, operating 
#   system and architecture.  These variables  are configurable on the command 
#   line during the make process. For instance, to override the autodetected 
#   platform architecture onc might specify:
#
#       make PLATFORM_ARCH=i386
#
#   Likewise, if the one would like to specify a platform variant, one might
#   use the following command:
#
#       make PLATFORM_VARIANT=raspberrypi
#
#   The PLATFORM_VARIANT is used to choose the approprate platform config file.
#   This is important at a device level.  For instance, Beagle Bone and Panda
#   Board computers shared architecture (armv7l) OS (Linux), yet they require
#   slightly different makefile configurations.
#
#   See the platform configuration files for more information.
#
################################################################################
ifndef PLATFORM_VARIANT
    PLATFORM_VARIANT = default
endif

ifndef PLATFORM_OS
    PLATFORM_OS = $(shell uname -s)
endif

ifndef PLATFORM_ARCH
    PLATFORM_ARCH = $(shell uname -m)
endif

################################################################################
# PLATFORM_LIB_SUBPATH (conditionally set)
#   (e.g. linuxarmv6l, osx, linux64, linux, etc.)
#
#   The PLATFORM_LIB_SUBPATH is the subpath used throughout the openFrameworks
#   filesystem to differentiate binary libraries compiled for different 
#   hardware architectures and operating systems.  These PLATFORM_LIB_SUBPATH
#   names are usually a combination of the operating system name and the 
#   architecture type.  The following section sets these variables if the user
#   has not already set the PLATFORM_LIB_SUBPATH variable.
#
################################################################################
ifndef PLATFORM_LIB_SUBPATH
    # check linux flavors
    ifeq ($(PLATFORM_OS),Linux)
        ifeq ($(PLATFORM_ARCH),x86_64)
            PLATFORM_LIB_SUBPATH=linux64
        else ifeq ($(PLATFORM_ARCH),armv6l)
            PLATFORM_LIB_SUBPATH=linuxarmv6l
        else ifeq ($(PLATFORM_ARCH),armv7l)
            PLATFORM_LIB_SUBPATH=linuxarmv7l
        else ifeq ($(PLATFORM_ARCH),i386)
            PLATFORM_LIB_SUBPATH=linux
        else ifeq ($(PLATFORM_ARCH),i686)
            PLATFORM_LIB_SUBPATH=linux
        else
            $(error                                                            \
                This makefile does not support                                 \
                your architecture $(PLATFORM_ARCH)                             \
            )
        endif
    # check android flavors 
    else ifeq ($(PLATFORM_OS),Android)
        PLATFORM_LIB_SUBPATH=android
    # check darwin flavors
    else ifeq ($(PLATFORM_OS),Darwin)
        ifeq ($(PLATFORM_ARCH),x86_64)
            # TODO: this will be updated as soon as we support 64bit on OSX 
            # PLATFROM_LIB_SUBPATH=osx64
            $(error                                                            \
                This makefile does not support your architecture               \
                $(PLATFORM_ARCH). If you are on OSX, perhaps you               \
                meant to specify > "make PLATFORM_ARCH=i386"                   \
            )
        else ifeq ($(PLATFORM_ARCH),i386)
            PLATFORM_LIB_SUBPATH=osx
        else
            $(error                                                            \
                This makefile does not support                                 \
                your architecture $(PLATFORM_ARCH)                             \
            )
        endif
    else
        $(error                                                                \
            This makefile does not support                                     \
            your operating system $(PLATFORM_OS)                               \
        )
    endif
endif

################################################################################
# DEBUG ########################################################################
################################################################################
ifdef MAKEFILE_DEBUG
    $(info =================config.shared.mk PLATFORM DETECTION================)
    $(info PLATFORM_ARCH=$(PLATFORM_ARCH))
    $(info PLATFORM_OS=$(PLATFORM_OS))
    $(info PLATFORM_VARIANT=$(PLATFORM_VARIANT))
    $(info PLATFORM_LIB_SUBPATH=$(PLATFORM_LIB_SUBPATH))
    $(info )
endif

################################################################################
# PATH GENERATION
################################################################################
# The following section builds common paths required for compilation.  This
# section relies heavily upon the PATH_OF_ROOT variable.  In most cases
# these paths can be manually overriden as make variables.
################################################################################

################################################################################
# PATH_OF_ADDONS (conditionally set, immediately assigned)
#   The standard location for openFrameworks addons.
################################################################################
ifndef PATH_OF_ADDONS
    PATH_OF_ADDONS := $(PATH_OF_ROOT)/addons
endif

################################################################################
# PATH_OF_EXAMPLES (conditionally set, immediately assigned)
#   The standard location for openFrameworks examples.
################################################################################
ifndef PATH_OF_EXAMPLES
    PATH_OF_EXAMPLES := $(PATH_OF_ROOT)/examples
endif

################################################################################
# PATH_OF_APPS (conditionally set, immediately assigned)
#   The standard location for openFrameworks apps.
################################################################################
ifndef PATH_OF_APPS
    PATH_OF_APPS := $(PATH_OF_ROOT)/apps
endif

################################################################################
# PATH_OF_LIBS (conditionally set, immediately assigned)
#   The standard location for openFrameworks core and 3rd party libraries.
################################################################################
ifndef PATH_OF_LIBS
    PATH_OF_LIBS := $(PATH_OF_ROOT)/libs
endif

################################################################################
# PATH_OF_LIBS_OPENFRAMEWORKS (conditionally set, immediately assigned)
#   The standard location for openFrameworks core source and header files.
################################################################################
ifndef PATH_OF_LIBS_OPENFRAMEWORKS
    PATH_OF_LIBS_OPENFRAMEWORKS :=                                             \
        $(PATH_OF_LIBS)/openFrameworks
endif

################################################################################
# PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED (conditionally set, immediately assigned)
#   The standard base location for the compiled openFrameworks static libs
#   libs will be stored in a platform-based subdirectory.
################################################################################
ifndef PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED
    PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED :=                                    \
        $(PATH_OF_LIBS)/openFrameworksCompiled
endif

################################################################################
# PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED_PROJECT
#       (conditionally set, immediately assigned)
#   The standard location for openFrameworks project files, including makefiles.
################################################################################
ifndef PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED_PROJECT
    PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED_PROJECT :=                            \
        $(PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED)/project
endif

################################################################################
# PATH_OF_SHARED_MAKEFILES (conditionally set, immediately assigned)
#   The standard location for openFrameworks shared makefiles.
################################################################################
ifndef PATH_OF_SHARED_MAKEFILES
    PATH_OF_SHARED_MAKEFILES :=                                                \
        $(PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED_PROJECT)/makefileCommon
endif

################################################################################
# PATH_OF_PLATFORM_MAKEFILES (conditionally set, immediately assigned)
#   The standard location for openFrameworks platform-specific makefiles.
################################################################################
ifndef PATH_OF_PLATFORM_MAKEFILES
    PATH_OF_PLATFORM_MAKEFILES :=                                              \
        $(PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED_PROJECT)/$(PLATFORM_LIB_SUBPATH)
endif

################################################################################
# PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED_LIB_PLATFORM_LIB_SUBPATH
#        (conditionally set, immediately assigned)
#   The standard location for openFrameworks core platform-specific compiled
#   static libraries.
################################################################################
ifndef PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED_LIB_PLATFORM_LIB_SUBPATH
    PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED_LIB_PLATFORM_LIB_SUBPATH :=           \
        $(PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED)/lib/$(PLATFORM_LIB_SUBPATH)
endif

################################################################################
# DEBUG ########################################################################
################################################################################
ifdef MAKEFILE_DEBUG
    $(info =================config.shared.mk PATHS=============================)
    $(info PATH_OF_ADDONS=$(PATH_OF_ADDONS))
    $(info PATH_OF_EXAMPLES=$(PATH_OF_EXAMPLES))
    $(info PATH_OF_APPS=$(PATH_OF_APPS))
    $(info PATH_OF_LIBS=$(PATH_OF_LIBS))
    $(info PATH_OF_LIBS_OPENFRAMEWORKS=$(PATH_OF_LIBS_OPENFRAMEWORKS))
    $(info PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED=$(PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED))
    $(info PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED_PROJECT=$(PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED_PROJECT))
    $(info PATH_OF_SHARED_MAKEFILES=$(PATH_OF_SHARED_MAKEFILES))
    $(info PATH_OF_PLATFORM_MAKEFILES=$(PATH_OF_PLATFORM_MAKEFILES))
    $(info PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED_LIB_PLATFORM_LIB_SUBPATH=$(PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED_LIB_PLATFORM_LIB_SUBPATH))
    $(info )
endif
################################################################################

################################################################################
# PLATFORM VARIANT CONFIGURATION
################################################################################
# In this section, we check and see what platform configuration files are 
# available.  After determing the available configuration options, we check
# the available configuration files against the PLATFORM_VARIANT variable above.
# In most cases this will simply be "default".  But users have the option to 
# play with platform-specific configurations by creating different platform
# configuration files.
################################################################################

################################################################################
# AVAILABLE_PLATFORM_VARIANTS
#   A list of available platform-specific configuration files for the 
#   architecure / os determined above.  These are determined using a simple
#   shell command.  Steps:
#
#   1. Use the "find" command to locate all matching files
#       find $(PATH_OF_PLATFORM_MAKEFILES)/config.*.mk \
#   2. Limit the search to the single folder and only find files
#       -maxdepth 1 -type f \
#   3. Extract the variant type name from the file by piping the results into
#      a simple regex extractor
#       | sed -E 's/.*\.([^\.]*)\.mk/\1/'\
#
#   4. Look inside the collected values and see if the current PLATFORM_VARIANT
#      actually exists as an option in the AVAILABLE_PLATFORM_VARIANTS list:
#         ($(findstring $(PLATFORM_VARIANT),$(AVAILABLE_PLATFORM_VARIANTS)),)
#   5. If not, throw an error and display the variants that were found.
#
################################################################################

AVAILABLE_PLATFORM_VARIANTS :=                                                 \
    $(shell                                                                    \
        find $(PATH_OF_PLATFORM_MAKEFILES)/config.*.mk                         \
        -maxdepth 1                                                            \
        -type f                                                                \
        | sed -E 's/.*\.([^\.]*)\.mk/\1/'                                      \
    )

ifeq ($(findstring $(PLATFORM_VARIANT),$(AVAILABLE_PLATFORM_VARIANTS)),)
    $(error                                                                    \
        Platform Variant "$(PLATFORM_VARIANT)"                                 \
        is not valid. Valid variants include                                   \
        [$(strip $(AVAILABLE_PLATFORM_VARIANTS))]                              \
    )
endif

################################################################################
# CALL THE PLATFORM VARIANT CONFIGURATION FILE
################################################################################
# This section will call the platform-specific configuration file.  
# Among other variables, the PATH_ABI is specified in the platform specific
# configuration file.  This is important in the case of Android projects in
# particular where multiple ABIs (application binary interface) are needed.
#
# See the documentation inside any of the files with the format:
#
#     $(PATH_OF_PLATFORM_MAKEFILES)/config.$(PLATFORM_LIB_SUBPATH).$(PLATFORM_VARIANT).mk 
#
################################################################################

# include the platform specific user and platform configuration files
include $(PATH_OF_PLATFORM_MAKEFILES)/config.$(PLATFORM_LIB_SUBPATH).$(PLATFORM_VARIANT).mk

################################################################################
# PLATFORM_SHARED_LIBRARY_EXTENSION (conditionally set, immediately assigned)
# PLATFORM_STATIC_LIBRARY_EXTENSION (conditionally set, immediately assigned)
# PLATFORM_LIBRARY_PREFIX (conditionally set, immediately assigned)
#   These variables are usually set in the platform specific config file, but
#   we set them to a default value if they are not already set.
################################################################################

ifndef PLATFORM_SHARED_LIBRARY_EXTENSION
    PLATFORM_SHARED_LIBRARY_EXTENSION := so
endif

ifndef PLATFORM_STATIC_LIBRARY_EXTENSION
    PLATFORM_STATIC_LIBRARY_EXTENSION := a
endif

ifndef PLATFORM_LIBRARY_PREFIX
    PLATFORM_LIBRARY_PREFIX := lib
endif

################################################################################
# ABI_LIB_SUBPATH (conditionally set, immediately assigned)
#   The ABI_LIB_SUBPATH is a subpath used for locating the final static binary
#   libraries.  If the PATH_ABI variable is NOT set in the platform-specific
#   configuration file, then ABI_LIB_SUBPATH will simply be equal to 
#   PLATFORM_LIB_SUBPATH.  Currently Android platforms make use of the 
#   ABI_LIB_SUBPATH variable.
################################################################################

ifdef PATH_ABI
    ABI_LIB_SUBPATH := $(PLATFORM_LIB_SUBPATH)/$(strip $(PATH_ABI))
else
    ABI_LIB_SUBPATH := $(PLATFORM_LIB_SUBPATH)
endif

################################################################################
# DEBUG ########################################################################
################################################################################
ifdef MAKEFILE_DEBUG
    $(info =================config.shared.mk PLATFORM_VARIANT CONFIG===========)
    $(info Available Platform Variants = $(AVAILABLE_PLATFORM_VARIANTS))
    $(info                                                                     \
        Including :                                                            \
        $(PATH_OF_PLATFORM_MAKEFILES)/config.$(PLATFORM_LIB_SUBPATH).$(PLATFORM_VARIANT).mk \
    )
    $(info PATH_ABI=$(PATH_ABI))
    $(info ABI_LIB_SUBPATH=$(ABI_LIB_SUBPATH))
    $(info )
endif
################################################################################

################################################################################
# CONSTANTS
#   A set of constants that are used for some of the more gnarly make action. 
################################################################################

EMPTY_SPACE :=
EMPTY_SPACE += 

TRUE := NON_EMPTY_STRING
FALSE := 

ESCAPED_DELIMITER := <?--DELIMITER--?>

FIND_TYPE_DIRECTORY := d
FIND_TYPE_FILE := f

################################################################################
# FUNCTION FUNC_DO_NOTHING
#   A "do nothing" function sometimes called to help make if/else statements 
#   more readable. 
################################################################################

define FUNC_DO_NOTHING
endef

################################################################################
# FUNCTION FUNC_REMOVE_DUPLICATE_ADDONS
#   Define a function to remove duplicates without using $(sort ..), 
#   because ($sort ...) will place the list in lexicographic order.  In 
#   many cases we want to respect an existing order.
#   
#   This function is not for the faint of heart.
#
#   For more information about $(strip ...), $(word, ...), see:
#
#       http://www.gnu.org/software/make/manual/html_node/Text-Functions.html
#
#   For more information about $(call ...) see:
#
#       http://www.gnu.org/software/make/manual/html_node/Call-Function.html
#
################################################################################

define FUNC_REMOVE_DUPLICATES_PRESERVE_ORDER
    $(if $1,                                                                   \
        $(strip                                                                \
            $(word 1,$1)                                                       \
            $(call FUNC_REMOVE_DUPLICATES_PRESERVE_ORDER,$(filter-out $(word 1,$1),$1))\
        ),                                                                     \
        $(call FUNC_DO_NOTHING)                                                \
    )                                                                          \

endef

################################################################################
# FUNCTION FUNC_RECURSIVE_FIND_SOURCES
#   A function that will recursively search for source files beginning in a 
#   given directory.
#
#   Example Usage:
#   
#       THE_SOURCES_FOUND :=                                                   \
#                      $(call                                                  \
#                           FUNC_RECURSIVE_FIND_SOURCES,                       \
#                           $(DIRECTORY_TO_SEARCH)                             \
#                       )                                                      \
#   Steps:
#
#   1.  Search the passed directory ($1) for the type file "f" with one of the
#       listed file extensions:
#        find $1                                                               
#           -type f                                                            
#           -name "*.mm"                                                       
#           -or                                                                
#           ...
#           -name "*.S"                                                      
#
#   2. Send errors to /dev/null
#
#       2> /dev/null
#
#   3. Exclude all hidden directories and files.
#
#       | grep -v "/\.[^\.]" )
#
################################################################################

define FUNC_RECURSIVE_FIND_SOURCES
    $(shell                                                                    \
        find $1                                                                \
            -type f                                                            \
            -name "*.mm"                                                       \
            -or                                                                \
            -name "*.m"                                                        \
            -or                                                                \
            -name "*.c"                                                        \
            -or                                                                \
            -name "*.cc"                                                       \
            -or                                                                \
            -name "*.cpp"                                                      \
            -or                                                                \
            -name "*.cxx"                                                      \
            -or                                                                \
            -name "*.S"                                                        \
            -not                                                               \
            -path "*.framework*"                                               \
            -not                                                               \
            -path "*/openFrameworksCompiled/*"                                 \
        2> /dev/null                                                           \
        | grep -v "/\.[^\.]"                                                   \
    )                                                                          \

endef

################################################################################
# FUNCTION FUNC_RECURSIVE_FIND_SOURCES
#   A function that will recursively search for source files beginning in a 
#   given directory.
#
#   Example Usage:
#   
#       THE_SOURCES_FOUND :=                                                   \
#                      $(call                                                  \
#                           FUNC_RECURSIVE_FIND_SOURCES,                       \
#                           $(DIRECTORY_TO_SEARCH)                             \
#                       )                                                      \
#   Steps:
#
#   1.  Search the passed directory ($1) for the type file "f" with one of the
#       listed file extensions:
#        find $1                                                               
#           -type f                                                            
#           -name "*.mm"                                                       
#           -or                                                                
#           ...
#           -name "*.S"                                                      
#
#   2. Send errors to /dev/null
#
#       2> /dev/null
#
#   3. Exclude all hidden directories and files.
#
#       | grep -v "/\.[^\.]" )
#
################################################################################

define FUNC_NON_RECURSIVE_FIND_SOURCES
    $(shell                                                                    \
        find $1                                                                \
            -maxdepth 1                                                        \
            -mindepth 1                                                        \
            -type f                                                            \
            -name "*.mm"                                                       \
            -or                                                                \
            -name "*.m"                                                        \
            -or                                                                \
            -name "*.c"                                                        \
            -or                                                                \
            -name "*.cc"                                                       \
            -or                                                                \
            -name "*.cpp"                                                      \
            -or                                                                \
            -name "*.cxx"                                                      \
            -or                                                                \
            -name "*.S"                                                        \
            -not                                                               \
            -path "*.framework*"                                               \
            -not                                                               \
            -path "*/openFrameworksCompiled/*"                                 \
        2> /dev/null                                                           \
        | grep -v "/\.[^\.]"                                                   \
    )                                                                          \

endef

################################################################################
# FUNCTION FUNC_RECURSIVE_FIND_SEARCH_PATHS
#   A function that will recursively search for header OR library search path 
#   directories while ignoring framework paths and hidden directories.
#
#   Will also ignore any framework or any file / folder inside the 
#   openFrameworks path.
#
################################################################################

define FUNC_RECURSIVE_FIND_SEARCH_PATHS
    $(shell                                                                    \
        find $1                                                                \
            -type d                                                            \
            -not                                                               \
            -path "*.framework*"                                               \
            -not                                                               \
            -path "*/openFrameworksCompiled/*"                                 \
        2> /dev/null                                                           \
        | grep -v "/\.[^\.]"                                                   \
    )                                                                          \

endef

################################################################################
# FUNCTION FUNC_RECURSIVE_FIND_LIBRARIES_WITH_TYPE_AND_NAME_PATTERN
#   A function that will recursively search for libraries and frameworks and
#   will ignore anything that is hidden. 
#   
#   Will also ignore any framework or any file / folder inside the 
#   openFrameworks path.
#
#   Arg $1 => the search directory
#   Arg #2 => the search type (f or d)
#   Arg #3 => the name pattern
#
#   See FUNC_PARSE_ADDON_TEMPLATE_HEADER_SEARCH_PATHS for an example.
################################################################################

define FUNC_RECURSIVE_FIND_LIBRARIES_WITH_TYPE_AND_NAME_PATTERN
    $(shell                                                                    \
        find $1                                                                \
            -type $2                                                           \
            -name $3                                                           \
            -not                                                               \
            -path "*/openFrameworksCompiled/*"                                 \
            -not                                                               \
            -path "*.framework*"                                               \
        2> /dev/null                                                           \
        | grep -v "/\.[^\.]"                                                   \
    )                                                                          \

endef
