################################################################################
# compile.project.mk
################################################################################
#
#   This file contains all of the project compilation rules.  
#   It organizes CFLAGS, LDFLAGS, dependency files, for the project and any
#   addons files.
#
################################################################################

################################################################################
# PATH_OF_ROOT (conditionally set)
#   The PATH_OF_ROOT is used throughout the script to locate the root folder 
#   of the openFrameworks installation directory.  This variable is often set
#   within custom project files such as $(PATH_OF_PROJECT)/config.make when
#   a project is in a non-standard location.
################################################################################

ifndef PATH_OF_ROOT
    PATH_OF_ROOT:=../../..
endif

################################################################################
# PATH_PROJECT_ROOT (conditionally set)
#   The PATH_PROJECT_ROOT is used throughout the script to locate the project
#   root folder.
################################################################################

ifndef PATH_PROJECT_ROOT
    PATH_PROJECT_ROOT:=.
endif

################################################################################
# ALL_CORE_THIRD_PARTY_LIBRARY_SEARCH_PATHS (immediately assigned)
#   The ALL_CORE_THIRD_PARTY_LIBRARY_SEARCH_PATHS variable is a list of search
#   paths for the third party library binaries.  In many cases, such as on
#   linux, there are very few static libraries included.  In other cases, such
#   as on OSX, we include many static libs that must be linked when building an
#   executable.
#
# Steps:
# 1. Generate a list of all third party library paths available for the 
#    for the current ABI_LIB_SUBPATH:
#
#       find $(PATH_OF_LIBS)/*/lib/$(ABI_LIB_SUBPATH) ...
#
# 2. Remove any paths that used for the core openFrameworks compilation
#
#       ... -type d -not -path "*/openFrameworksCompiled/*"
#
# 3. Remove any .framework paths.  These will be added later and should not 
#    be searched and included in the same way as static libraries:
#
#       ... -type d -not -path "*.framework*" \
#
# 4. Send any error messages to /dev/null
#
#       ... 2> /dev/null \
#
# 5. Remove any hidden directories:
#
#       ... | grep -v "/\.[^\.]"
#
################################################################################

ALL_CORE_THIRD_PARTY_LIBRARY_SEARCH_PATHS:=\
    $(shell \
        find $(PATH_OF_LIBS)/*/lib/$(ABI_LIB_SUBPATH) \
        -type d -not -path "*/openFrameworksCompiled/*" \
        -type d -not -path "*.framework*" \
        2> /dev/null \
        | grep -v "/\.[^\.]" \
    )

################################################################################
# CORE_THIRD_PARTY_LIBRARY_SEARCH_PATHS (immediately assigned)
#   The CORE_THIRD_PARTY_LIBRARY_SEARCH_PATHS variable is a filtered list of 
#   third party library (Poco, Cairo, etc) search paths created by removing all 
#   paths that are in the CORE_EXCLUSIONS list.  CORE_EXCLUSIONS is primarily 
#   defined in the config.shared.mk and the platform-specific configuration 
#   files.
################################################################################

CORE_THIRD_PARTY_LIBRARY_SEARCH_PATHS:=\
    $(filter-out \
        $(CORE_EXCLUSIONS),\
        $(ALL_CORE_THIRD_PARTY_LIBRARY_SEARCH_PATHS)\
    )

################################################################################
# ALL_CORE_THIRD_PARTY_STATIC_LIBRARIES (immediately assigned)
#   The ALL_CORE_THIRD_PARTY_STATIC_LIBRARIES variable is a list of all
#   party static library binaries.  In many cases, such as on linux, there are 
#   very few static libraries included.  In other cases, such as on OSX, we 
#   include many static libs that must be linked when building an executable.
#
# Steps:
# 1. Generate a list of all third party static libraries by searching for files 
#    ending with the PLATFORM_STATIC_LIBRARY_EXTENSION within the current
#    ABI_LIB_SUBPATH:
#
#        find $(PATH_OF_LIBS)/*/lib/$(ABI_LIB_SUBPATH)\
#                       /*.$(PLATFORM_STATIC_LIBRARY_EXTENSION) \ ...
#
# 2. Remove any paths that used for the core openFrameworks compilation
#
#       ... -type d -not -path "*/openFrameworksCompiled/*"
#
# 3. Remove any .framework paths.  These will be added later and should not 
#    be searched and included in the same way as static libraries:
#
#       ... -type d -not -path "*.framework*" \
#
# 4. Send any error messages to /dev/null
#
#       ... 2> /dev/null \
#
# 5. Remove any hidden directories:
#
#       ... | grep -v "/\.[^\.]"
#
################################################################################

ALL_CORE_THIRD_PARTY_STATIC_LIBRARIES:=\
    $(shell \
        find $(PATH_OF_LIBS)/*/lib/$(ABI_LIB_SUBPATH)/*.$(PLATFORM_STATIC_LIBRARY_EXTENSION) \
        -not -path "*/openFrameworksCompiled/*" \
        -not -path "*.framework*" \
        2> /dev/null \
        | grep -v "/\.[^\.]"\
    )

################################################################################
# CORE_THIRD_PARTY_STATIC_LIBRARIES (immediately assigned)
#   The CORE_THIRD_PARTY_STATIC_LIBRARIES variable created by filtering out all
#   items that have been included in the CORE_EXCLUSIONS variable.
################################################################################

CORE_THIRD_PARTY_STATIC_LIBRARIES:=\
    $(filter-out \
        $(CORE_EXCLUSIONS),\
        $(ALL_CORE_THIRD_PARTY_STATIC_LIBRARIES)\
    )

################################################################################
# ALL_CORE_THIRD_PARTY_SHARED_LIBRARIES (immediately assigned)
#   The ALL_CORE_THIRD_PARTY_SHARED_LIBRARIES variable is a list of all
#   party shared library binaries.  In many cases, such as on linux, there are 
#   very few static libraries included.  In other cases, such as on OSX, we 
#   include many static libs that must be linked when building an executable.
#
# Steps:
# 1. Generate a list of all third party static libraries by searching for files 
#    ending with the PLATFORM_SHARED_LIBRARY_EXTENSION within the current
#    ABI_LIB_SUBPATH:
#
#        find $(PATH_OF_LIBS)/*/lib/$(ABI_LIB_SUBPATH)\
#                       /*.$(PLATFORM_SHARED_LIBRARY_EXTENSION) \ ...
#
# 2. Remove any paths that used for the core openFrameworks compilation
#
#       ... -type d -not -path "*/openFrameworksCompiled/*"
#
# 3. Remove any .framework paths.  These will be added later and should not 
#    be searched and included in the same way as static libraries:
#
#       ... -type d -not -path "*.framework*" \
#
# 4. Send any error messages to /dev/null
#
#       ... 2> /dev/null \
#
# 5. Remove any hidden directories:
#
#       ... | grep -v "/\.[^\.]"
#
################################################################################

ALL_CORE_THIRD_PARTY_SHARED_LIBRARIES:=\
    $(shell \
        find $(PATH_OF_LIBS)/*/lib/$(ABI_LIB_SUBPATH)/*.$(PLATFORM_SHARED_LIBRARY_EXTENSION) \
        -not -path "*/openFrameworksCompiled/*" \
        -not -path "*.framework*" \
        2> /dev/null \
        | grep -v "/\.[^\.]"\
    )

################################################################################
# CORE_THIRD_PARTY_SHARED_LIBRARIES (immediately assigned)
#   The CORE_THIRD_PARTY_SHARED_LIBRARIES variable created by filtering out all
#   items that have been included in the CORE_EXCLUSIONS variable.
################################################################################

CORE_THIRD_PARTY_SHARED_LIBRARIES:=\
    $(filter-out \
        $(CORE_EXCLUSIONS),\
        $(ALL_CORE_THIRD_PARTY_SHARED_LIBRARIES)\
    )

################################################################################
# CORE_FRAMEWORK_SEARCH_PATHS (immediately assigned)
#   PLATFORM_FRAMEWORK_SEARCH_PATHS are defined in the platform-
#   specific configuration file.  Usually this is only used for OSX/iOS
#   based builds. 
################################################################################

CORE_FRAMEWORK_SEARCH_PATHS:=$(PLATFORM_FRAMEWORKS_SEARCH_PATHS)

################################################################################
# ADDONS #######################################################################
################################################################################

include $(PATH_OF_SHARED_MAKEFILES)/config.addons.mk


################################################################################
# FINAL COLLECTION OF VARIABLES ################################################
################################################################################






# generate the list of core libraries
# 2. Add all of the third party static libs defined by the platform config files.
OF_CORE_LIBS := $(OF_CORE_THIRD_PARTY_STATIC_LIBS)
# 2. Add all of the third party shared libs defined by the platform config files.
OF_CORE_LIBS += $(OF_CORE_THIRD_PARTY_SHARED_LIBS)
# 3. Add all of the core pkg-config OF libs defined by the platform config files.
CORE_PKG_CONFIG_LIBS += $(PROJECT_ADDONS_PKG_CONFIG_LIBS)

ifneq ($(strip $(CORE_PKG_CONFIG_LIBS)),)
	OF_CORE_LIBS += \
        $(shell \
            pkg-config "$(CORE_PKG_CONFIG_LIBS)" --libs\
        )
endif
# 4. Add the libraries defined in the platform config files.
OF_CORE_LIBS += $(addprefix -l,$(PLATFORM_LIBRARIES))

# add the list of addon includes
ifneq ($(strip $(PROJECT_ADDONS_PKG_CONFIG_LIBS)),)
	OF_CORE_INCLUDES_CFLAGS += \
        $(shell \
            pkg-config "$(PROJECT_ADDONS_PKG_CONFIG_LIBS)" --cflags\
        )
endif

################################################################################
# CLEAN PROJECT EXCLUSIONS 
################################################################################

OF_PROJECT_EXCLUSIONS := $(strip $(PROJECT_EXCLUSIONS))

# add defaults here TODO: should these always be 
OF_PROJECT_EXCLUSIONS += $(PATH_PROJECT_ROOT)/bin  # exactly this path
OF_PROJECT_EXCLUSIONS += $(PATH_PROJECT_ROOT)/bin% # any children of this path
OF_PROJECT_EXCLUSIONS += $(PATH_PROJECT_ROOT)/obj
OF_PROJECT_EXCLUSIONS += $(PATH_PROJECT_ROOT)/obj%
OF_PROJECT_EXCLUSIONS += $(PATH_PROJECT_ROOT)/.git
OF_PROJECT_EXCLUSIONS += $(PATH_PROJECT_ROOT)/.git%

# A pattern can contain only one TRAILING %. This is a "feature" in GNU Make.
# As a consequence of this "feature" $(filter ...) and $(filter-out ...) 
# cannot match substrings in our exclusions.  For example, the following will 
# not be matched as one might expect with normal wildcards / regex patterns:
#
# OF_PROJECT_EXCLUSIONS += $(PATH_PROJECT_ROOT)/%.xcodeproj 
# OF_PROJECT_EXCLUSIONS += $(PATH_PROJECT_ROOT)/%.xcodeproj/%
#
# GNU make does allow us to do something like this though:
# In this case, we look for all paths that have the pattern:
#     $(PATH_PROJECT_ROOT)/*.xcodeproj (here * works as expected)
# and then it appends a % (the make pattern matcher) ONLY to the end of each
# of the paths discovered by the $(wildcard ...) function.  The output of
# this function is now compatible with GNU Make $(filter ...) and $(filter-out).
OF_PROJECT_EXCLUSIONS += \
    $(addsuffix \
        %,\
        $(wildcard \
            $(PATH_PROJECT_ROOT)/*.xcodeproj\
        )\
    )

################################################################################
# PROJECT SOURCE FILES
################################################################################

# exclude all items from source according to the config.make file

# create a list of all dirs in the project root that might be valid project
# source directories 
# grep -v "/\.[^\.]" will exclude all .hidden folders and files
ALL_OF_PROJECT_SOURCE_PATHS:=\
    $(shell \
        find $(PATH_PROJECT_ROOT) \
        -mindepth 1 -type d \
        | grep -v "/\.[^\.]"\
    )

ifneq ($(PROJECT_EXTERNAL_SOURCE_PATHS),)
	ALL_OF_PROJECT_SOURCE_PATHS += $(PROJECT_EXTERNAL_SOURCE_PATHS)
	
    ALL_OF_PROJECT_SOURCE_PATHS += \
        $(shell \
            find $(PROJECT_EXTERNAL_SOURCE_PATHS) \
            -mindepth 1 -type d \
            | grep -v "/\.[^\.]"\
        )
endif

# be included as locations for header searches via 
OF_PROJECT_SOURCE_PATHS:=\
    $(filter-out \
        $(OF_PROJECT_EXCLUSIONS),\
        $(ALL_OF_PROJECT_SOURCE_PATHS)\
    )

ifdef MAKEFILE_DEBUG
    $(info ---OF_PROJECT_SOURCE_PATHS---)
    $(foreach v, $(OF_PROJECT_SOURCE_PATHS),$(info $(v)))

    $(info ---OF_PROJECT_EXCLUSIONS---)
    $(foreach v, $(OF_PROJECT_EXCLUSIONS),$(info $(v)))
endif

# find all sources inside the project's source directory (recursively)
# grep -v "/\.[^\.]" will exclude all .hidden folders and files
OF_PROJECT_SOURCE_FILES = $(shell find $(OF_PROJECT_SOURCE_PATHS) -maxdepth 1 -name "*.cpp" -or -name "*.c" -or -name "*.cc" -or -name "*.cxx" -or -name "*.S" | grep -v "/\.[^\.]")

################################################################################
# PROJECT HEADERS INCLUDES (-I ...)
################################################################################

OF_PROJECT_INCLUDES:=\
    $(filter-out \
        $(PROJECT_INCLUDE_EXCLUSIONS),\
        $(OF_PROJECT_SOURCE_PATHS)\
    )

OF_PROJECT_INCLUDES += $(PROJECT_ADDONS_INCLUDES) # already filtered against ADDONS_INCLUDES_EXCLUDE

OF_PROJECT_INCLUDES_CFLAGS := \
    $(addprefix \
        -I,\
        $(OF_PROJECT_INCLUDES)\
    )

################################################################################
# OF PLATFORM LDFLAGS
################################################################################

OF_CORE_LIBRARY_LDFLAGS :=\
    $(addprefix \
        -L,\
        $(CORE_THIRD_PARTY_LIBRARY_SEARCH_PATHS)\
    )

OF_CORE_LIBRARY_LDFLAGS+= \
    $(addprefix \
        -L,\
        $(PLATFORM_LIBRARY_SEARCH_PATHS)\
    )


ifdef MAKEFILE_DEBUG
    $(info ---OF_PROJECT_INCLUDES_CFLAGS---)
    $(foreach v, $(OF_PROJECT_INCLUDES_CFLAGS),$(info $(v)))
endif

################################################################################
# PROJECT LIBRARIES (-l ...) (not used during core compilation, but vars are 
#    used during project compilation)
################################################################################

# TODO - do we look for search paths or search ... or ?
# TODO - can libs be included in the template?
# TODO - + frameworks?
OF_PROJECT_LIBS := $(PROJECT_ADDONS_LIBS)
#OF_PROJECT_LIBS_LDFLAGS = $(addprefix -l,$(OF_PROJECT_LIBS))

################################################################################
# PROJECT DEFINES
################################################################################

OF_PROJECT_DEFINES := $(PROJECT_DEFINES)
OF_PROJECT_DEFINES += $(PROJECT_ADDONS_DEFINES)

OF_PROJECT_DEFINES_CFLAGS = $(addprefix -D,$(OF_PROJECT_DEFINES))

################################################################################
# PROJECT CFLAGS
################################################################################

# gather any project CFLAGS
OF_PROJECT_CFLAGS := $(PROJECT_CFLAGS)
OF_PROJECT_CFLAGS += $(PROJECT_ADDONS_CFLAGS)
OF_PROJECT_CFLAGS += $(USER_CFLAGS) # legacy
OF_PROJECT_CFLAGS += $(OF_PROJECT_DEFINES_CFLAGS)
OF_PROJECT_CFLAGS += $(OF_PROJECT_INCLUDES_CFLAGS)
OF_PROJECT_CFLAGS += $(addprefix -F,$(PROJECT_ADDONS_FRAMEWORKS_SEARCH_PATHS))
OF_PROJECT_CFLAGS += $(OF_CORE_BASE_CFLAGS)
OF_PROJECT_CFLAGS += $(OF_CORE_DEFINES_CFLAGS)
OF_PROJECT_CFLAGS += $(OF_CORE_INCLUDES_CFLAGS)

################################################################################
# PROJECT LDFLAGS
################################################################################

OF_PROJECT_LDFLAGS := $(PROJECT_LDFLAGS)
OF_PROJECT_LDFLAGS += $(PROJECT_ADDONS_LDFLAGS)
OF_PROJECT_LDFLAGS += $(OF_CORE_LIBRARY_LDFLAGS)
OF_PROJECT_LDFLAGS += $(USER_LDFLAGS) # legacy
OF_PROJECT_LDFLAGS += $(USER_LIBS)   # legacy
OF_PROJECT_LDFLAGS += $(OF_PROJECT_LIBS_LDFLAGS)
OF_PROJECT_LDFLAGS += $(addprefix -framework ,$(PROJECT_FRAMEWORKS))
OF_PROJECT_LDFLAGS += $(addprefix -framework ,$(PLATFORM_FRAMEWORKS))
OF_PROJECT_LDFLAGS += $(addprefix -framework ,$(PROJECT_ADDONS_FRAMEWORKS))
OF_PROJECT_LDFLAGS += $(addprefix -F,$(PROJECT_ADDONS_FRAMEWORKS_SEARCH_PATHS))

################################################################################
ifdef MAKEFILE_DEBUG
    $(info ===================compile.project.make=============================)
endif

ifdef PLATFORM_CXX
    CXX = $(PLATFORM_CXX)
endif

ifdef PROJECT_CXX
    CXX = $(PROJECT_CXX)
endif

ifdef PLATFORM_CC
    CC = $(PLATFORM_CC)
endif

ifdef PROJECT_CC
    CC = $(PROJECT_CC)
endif

# TODO: what is this for?
NODEPS = clean

################################################################################
# CFLAGS
################################################################################

# clean it
ALL_CFLAGS =
# add the CFLAGS from Makefiles.examples
ALL_CFLAGS += $(OF_PROJECT_CFLAGS)

# clean up all extra whitespaces in the CFLAGS
CFLAGS = $(strip $(ALL_CFLAGS))

################################################################################
# LDFLAGS
################################################################################

# clean it
ALL_LDFLAGS =

# add the include LDFLAGS from Makefiles.examples
ALL_LDFLAGS += $(OF_PROJECT_LDFLAGS)
ALL_LDFLAGS += $(PLATFORM_LDFLAGS)

# clean up all extra whitespaces in the LDFLAGS
LDFLAGS = $(strip $(ALL_LDFLAGS))

# Optimization flags

PROJECT_OPTIMIZATION_CFLAGS_DEBUG = 
PROJECT_OPTIMIZATION_CFLAGS_DEBUG += 

ifeq ($(findstring Debug,$(TARGET_NAME)),Debug)
	ifeq ($(strip $(PROJECT_OPTIMIZATION_CFLAGS_DEBUG)),)
		OPTIMIZATION_CFLAGS = $(PLATFORM_OPTIMIZATION_CFLAGS_DEBUG)
	else
		OPTIMIZATION_CFLAGS = $(PROJECT_OPTIMIZATION_CFLAGS_DEBUG)
	endif
	
    ifdef PLATFORM_CORELIB_DEBUG_TARGET
    	TARGET_LIBS += $(PLATFORM_CORELIB_DEBUG_TARGET)
    else
    	TARGET_LIBS += $(PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED_LIB_PLATFORM_LIB_SUBPATH)/libopenFrameworksDebug.a
    endif
endif

ifeq ($(findstring Release,$(TARGET_NAME)),Release)
	ifeq ($(strip $(PROJECT_OPTIMIZATION_CFLAGS_RELEASE)),)
	    OPTIMIZATION_CFLAGS = $(PLATFORM_OPTIMIZATION_CFLAGS_RELEASE)
	else
		OPTIMIZATION_CFLAGS = $(PROJECT_OPTIMIZATION_CFLAGS_RELEASE)
	endif
	
    ifdef PLATFORM_CORELIB_RELEASE_TARGET
    	TARGET_LIBS += $(PLATFORM_CORELIB_RELEASE_TARGET)
    else
    	TARGET_LIBS += $(PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED_LIB_PLATFORM_LIB_SUBPATH)/libopenFrameworks.a
    endif
endif

################################################################################
# OBJECT AND DEPENDENCY FILES DEFINITIONS
#	Object file paths are generated here (as opposed to with the rest of the 
#   flags) because we want to place them in target-specific folders. We
#   determine targets above. We –could– determine the target info earlier if we
#   wanted to.  It's here because that's approximately where it was in the 
#   legacy makefiles.
################################################################################

# define the subdirectory for our target name

ifdef MAKEFILE_DEBUG
    $(info ---OF_PROJECT_SOURCE_FILES---)
    $(foreach v, $(OF_PROJECT_SOURCE_FILES),$(info $(v)))
endif
ifdef MAKEFILE_DEBUG
    $(info ---OF_PROJECT_DEPENDENCY_FILES---)
    $(foreach v, $(OF_PROJECT_DEPENDENCY_FILES),$(info $(v)))
endif

ifdef ABI
	OF_PROJECT_OBJ_OUPUT_PATH = obj/$(PLATFORM_LIB_SUBPATH)/$(ABI)/$(TARGET_NAME)/
else
	OF_PROJECT_OBJ_OUPUT_PATH = obj/$(PLATFORM_LIB_SUBPATH)/$(TARGET_NAME)/
endif

OF_PROJECT_OBJ_FILES = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cxx,%.o,$(patsubst %.cc,%.o,$(patsubst %.S,%.o,$(OF_PROJECT_SOURCE_FILES))))))
OF_PROJECT_OBJS = $(subst $(PROJECT_ROOT)/,,$(subst $(PROJECT_EXTERNAL_SOURCE_PATHS),,$(addprefix $(OF_PROJECT_OBJ_OUPUT_PATH),$(OF_PROJECT_OBJ_FILES))))
OF_PROJECT_DEPS = $(patsubst %.o,%.d,$(OF_PROJECT_OBJS))

# lots of indentation so we can see what's going on (remember, in Make
# \t are treated as new lines, while \n are treated as spaces)
OF_PROJECT_ADDONS_OBJ_FILES = $(patsubst %.c,%.o,\
                                $(patsubst %.cpp,%.o,\
                                  $(patsubst %.cxx,%.o,\
                                    $(patsubst %.cc,%.o,\
                                      $(patsubst %.S,%.o,\
                                        $(patsubst %.mm,%.o,\
                                          $(patsubst %.m,%.o,\
                                            $(PROJECT_ADDONS_SOURCE_FILES)\
                                           )\
                                         )\
                                       )\
                                     )\
                                   )\
                                 )\
                               )

OF_PROJECT_ADDONS_OBJS = 
$(foreach addon_obj, $(OF_PROJECT_ADDONS_OBJ_FILES), \
     $(eval OF_PROJECT_ADDONS_OBJS+= $(patsubst $(OF_ROOT)/addons/$(word 1, $(subst /, ,$(subst $(OF_ROOT)/addons/,,$(addon_obj))))/%, \
                                          $(OF_ROOT)/addons/$(OF_PROJECT_OBJ_OUPUT_PATH)$(word 1, $(subst /, ,$(subst $(OF_ROOT)/addons/,,$(addon_obj))))/%, \
                                          $(addon_obj))) \
)

OF_PROJECT_ADDONS_DEPS = $(patsubst %.o,%.d,$(OF_PROJECT_ADDONS_OBJS))

OF_PROJECT_DEPENDENCY_FILES = $(OF_PROJECT_DEPS) $(OF_PROJECT_ADDONS_DEPS)

# TODO: deal with shared libs?

ifdef MAKEFILE_DEBUG
    $(info ---OF_PROJECT_DEPENDENCY_FILES---)
    $(foreach v, $(OF_PROJECT_DEPENDENCY_FILES),$(info $(v)))
endif
