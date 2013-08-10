################################################################################
# config.shared.mk
################################################################################
#
#   This file detects the current platform and creates all shared variables
#   required for the compilation of the core libraries, addons and projects. 
#   This file is referenced by the core makefile:
#
#      $(OF_PATH)/libs/openFrameworksCompiled/project/Makefile
#
#   and also the project makefile:
#    
#      $(PROJECT_PATH)/Makefile
#
#   Many of the shared variables produced by this file can be set by the user
#   before this files is called.  See below for examples.
#
################################################################################

################################################################################
# COMPILATION FLAG GENERATION
################################################################################
# After calling the platform-specific makefile above, now we can begin creating 
# all of lists of files and flags for the compiler.
################################################################################

################################################################################
# CORE_EXCLUSIONS (immediately assigned)
#   This variable lists the fully qualitied paths to be excluded from the core
#   compilation.  These exclusions are made on a per-platform-basis.  Thus, we
#   simply strip out any extra whitespaces ($strip ...) set the 
#   CORE_EXCLUSIONS equal to the PLATFORM_EXCLUSIONS. 
################################################################################

CORE_EXCLUSIONS := $(strip $(PLATFORM_EXCLUSIONS))

################################################################################
# ALL_CORE_SOURCE_PATHS (immediately assigned)
#   The ALL_CORE_SOURCE_PATHS variable is a list of paths locating the core 
#   openFrameworks header (*.h) and implementation (*.c/cpp/m) files.  We
#   search the OF_LIBS_OPENFRAMEWORKS_PATH directory for these.
#
# Steps:
#
# 1. Find all of the openFrameworks course source folders.
#
#       $(shell find $(OF_LIBS_OPENFRAMEWORKS_PATH) \
#
# 2. Limit the search to directories at a depth of 1, since we keep all oF 
#    sources files in the base directory.
#
#       -maxdepth 1 -mindepth 1 -type d \
#
# 3. Exclude all hidden directories and files.
#
#       | grep -v "/\.[^\.]" )
#
################################################################################

#TODO: use function calls from config.common.mk
#TODO: combine ALL and filtered
ALL_CORE_SOURCE_PATHS :=                                                       \
    $(shell                                                                    \
        find $(OF_LIBS_OPENFRAMEWORKS_PATH)                                    \
        -maxdepth 1                                                            \
        -mindepth 1                                                            \
        -type d                                                                \
        | grep -v "/\.[^\.]"                                                   \
    )

################################################################################
# CORE_SOURCE_PATHS (immediately assigned)
#   The CORE_SOURCE_PATHS variable FILTERED list of paths locating the core
#   openFrameworks header (*.h) and implementation (*.c/cpp/m) files.  It is 
#   the result taking the ALL_CORE_SOURCE_PATHS variable (above) and removing
#   all files that are also included in the CORE_EXCLUSIONS list.
################################################################################

CORE_SOURCE_PATHS :=                                                           \
    $(filter-out                                                               \
        $(CORE_EXCLUSIONS),                                                    \
        $(ALL_CORE_SOURCE_PATHS)                                               \
    )

################################################################################
# CORE_HEADER_SEARCH_PATHS (immediately assigned)
#   The CORE_HEADER_SEARCH_PATHS is a list of header search paths that have
#   been filtered.  They will later be prepended with the compilers -I flag.
#   $(OF_LIBS_OPENFRAMEWORKS_PATH) is included so we don't miss ofMain.h, which
#   is located in the base folder.
################################################################################

CORE_HEADER_SEARCH_PATHS :=                                                    \
    $(OF_LIBS_OPENFRAMEWORKS_PATH)                                             \
    $(CORE_SOURCE_PATHS)

################################################################################
# ALL_CORE_THIRD_PARTY_HEADER_SEARCH_PATHS (immediately assigned)
#   The ALL_CORE_THIRD_PARTY_HEADER_SEARCH_PATHS is a list of header search 
#   that belong to various openFrameworks 3rd parth libraries (e.g. Cairo).
#
# Steps:
#   1. Search all paths recursively, looking for directories:
#       find $(OF_LIBS_PATH)/*/include -type d \
#   2. Exclude all .hidden directories:
#       grep -v "/\.[^\.]" will exclude all .hidden folders and files
#
################################################################################

#TODO: use function calls from config.common.mk
#TODO: combine ALL and filtered
ALL_CORE_THIRD_PARTY_HEADER_SEARCH_PATHS :=                                    \
    $(shell                                                                    \
        find $(OF_LIBS_PATH)/*/include                                         \
        -type d                                                                \
        | grep -v "/\.[^\.]"                                                   \
    )

################################################################################
# CORE_THIRD_PARTY_HEADER_SEARCH_PATHS (immediately assigned)
#   The CORE_THIRD_PARTY_HEADER_SEARCH_PATHS is a filtered version of the 
#   CORE_THIRD_PARTY_HEADER_SEARCH_PATHS list, removing all paths that are in
#   the CORE_EXCLUSIONS list.
################################################################################

CORE_THIRD_PARTY_HEADER_SEARCH_PATHS :=                                        \
    $(filter-out                                                               \
        $(CORE_EXCLUSIONS),                                                    \
        $(ALL_CORE_THIRD_PARTY_HEADER_SEARCH_PATHS)                            \
    )

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
#       find $(OF_LIBS_PATH)/*/lib/$(ABI_LIB_SUBPATH) ...
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

#TODO: use function calls from config.common.mk
#TODO: combine ALL and filtered
ALL_CORE_THIRD_PARTY_LIBRARY_SEARCH_PATHS :=                                   \
    $(shell                                                                    \
        find $(OF_LIBS_PATH)/*/lib/$(ABI_LIB_SUBPATH)                          \
        -type d                                                                \
        -not                                                                   \
        -path "*/openFrameworksCompiled/*"                                     \
        -type d                                                                \
        -not                                                                   \
        -path "*.framework*"                                                   \
        2> /dev/null                                                           \
        | grep -v "/\.[^\.]"                                                   \
    )

################################################################################
# CORE_THIRD_PARTY_LIBRARY_SEARCH_PATHS (immediately assigned)
#   The CORE_THIRD_PARTY_LIBRARY_SEARCH_PATHS variable is a filtered list of 
#   third party library (Poco, Cairo, etc) search paths created by removing all 
#   paths that are in the CORE_EXCLUSIONS list.  CORE_EXCLUSIONS is primarily 
#   defined in the config.shared.mk and the platform-specific configuration 
#   files.
################################################################################

CORE_THIRD_PARTY_LIBRARY_SEARCH_PATHS :=                                       \
    $(filter-out                                                               \
        $(CORE_EXCLUSIONS),                                                    \
        $(ALL_CORE_THIRD_PARTY_LIBRARY_SEARCH_PATHS)                           \
    )

################################################################################
# ALL_CORE_THIRD_PARTY_STATIC_LIBRARIES (immediately assigned)
#   The ALL_CORE_THIRD_PARTY_STATIC_LIBRARIES variable is a list of all
#   party static library binaries.  In many cases, such as on linux, there are 
#   very few static libraries included.  In other cases, such as on OSX, we 
#   include many static libs that must be linked when building an executable.
#
################################################################################

# TODO: strict platform lib prefixes :
# replace *.$(PLATFORM_STATIC_LIBRARY_EXTENSION),  \
# with: $(PLATFORM_LIBRARY_PREFIX)*.$(PLATFORM_STATIC_LIBRARY_EXTENSION),  \

ALL_CORE_THIRD_PARTY_STATIC_LIBRARIES :=                                       \
        $(call                                                                 \
            FUNC_RECURSIVE_FIND_LIBRARIES_WITH_TYPE_AND_NAME_PATTERN,          \
            $(OF_LIBS_PATH)/*/lib/$(ABI_LIB_SUBPATH),                          \
            $(FIND_TYPE_FILE),                                                 \
            *.$(PLATFORM_STATIC_LIBRARY_EXTENSION),                            \
        )                                                                      \


################################################################################
# CORE_THIRD_PARTY_STATIC_LIBRARIES (immediately assigned)
#   The CORE_THIRD_PARTY_STATIC_LIBRARIES variable created by filtering out all
#   items that have been included in the CORE_EXCLUSIONS variable.
################################################################################

CORE_THIRD_PARTY_STATIC_LIBRARIES :=                                           \
    $(filter-out                                                               \
        $(CORE_EXCLUSIONS),                                                    \
        $(ALL_CORE_THIRD_PARTY_STATIC_LIBRARIES)                               \
    )

################################################################################
# ALL_CORE_THIRD_PARTY_SHARED_LIBRARIES (immediately assigned)
#   The ALL_CORE_THIRD_PARTY_SHARED_LIBRARIES variable is a list of all
#   party shared library binaries.  In many cases, such as on linux, there are 
#   very few static libraries included.  In other cases, such as on OSX, we 
#   include many static libs that must be linked when building an executable.
#
################################################################################

# TODO: strict platform lib prefixes :
# replace *.$(PLATFORM_STATIC_LIBRARY_EXTENSION),  \
# with: $(PLATFORM_LIBRARY_PREFIX)*.$(PLATFORM_STATIC_LIBRARY_EXTENSION),  \

ALL_CORE_THIRD_PARTY_SHARED_LIBRARIES :=                                       \
        $(call                                                                 \
            FUNC_RECURSIVE_FIND_LIBRARIES_WITH_TYPE_AND_NAME_PATTERN,          \
            $(OF_LIBS_PATH)/*/lib/$(ABI_LIB_SUBPATH),                          \
            $(FIND_TYPE_FILE),                                                 \
            *.$(PLATFORM_SHARED_LIBRARY_EXTENSION),                            \
        )                                                                      \

################################################################################
# CORE_THIRD_PARTY_SHARED_LIBRARIES (immediately assigned)
#   The CORE_THIRD_PARTY_SHARED_LIBRARIES variable created by filtering out all
#   items that have been included in the CORE_EXCLUSIONS variable.
################################################################################

CORE_THIRD_PARTY_SHARED_LIBRARIES :=                                           \
    $(filter-out                                                               \
        $(CORE_EXCLUSIONS),                                                    \
        $(ALL_CORE_THIRD_PARTY_SHARED_LIBRARIES)                               \
    )

################################################################################
# CORE_FRAMEWORK_SEARCH_PATHS (immediately assigned)
#   PLATFORM_FRAMEWORK_SEARCH_PATHS are defined in the platform-
#   specific configuration file.  Usually this is only used for OSX/iOS
#   based builds. 
################################################################################

CORE_FRAMEWORK_SEARCH_PATHS :=                                                 \
    $(PLATFORM_FRAMEWORKS_SEARCH_PATHS)

################################################################################
# CORE CFLAGS
################################################################################
# In this section, we begin building the list of core CFLAGS.
################################################################################

################################################################################
# CORE_HEADER_SEARCH_PATHS_CFLAGS (immediately assigned)
#   The CORE_HEADER_SEARCH_PATHS_CFLAGS are generated by prepending -I to each
#   of the PLATFORM_HEADER_SEARCH_PATHS. 
################################################################################

CORE_HEADER_SEARCH_PATHS_CFLAGS:=                                              \
    $(addprefix                                                                \
        -I,                                                                    \
        $(PLATFORM_HEADER_SEARCH_PATHS)                                        \
    )

################################################################################
# CORE_PKG_CONFIG_LIBRARIES (immediately assigned)
#   The CORE_PKG_CONFIG_LIBRARIES are simply set from the 
#   PLATFORM_PKG_CONFIG_LIBRARIES variable.
################################################################################

CORE_PKG_CONFIG_LIBRARIES:=                                                    \
    $(PLATFORM_PKG_CONFIG_LIBRARIES)

# TODO: do we need to include this above?
#   $(PROJECT_PKG_CONFIG_LIBS)

################################################################################
# CORE_HEADER_SEARCH_PATHS_CFLAGS (continued)
#   Acquire relevant CFLAGS from the pkg-config utility for all 
#   CORE_PKG_CONFIG_LIBRARIES.  Add them to the #
#   CORE_HEADER_SEARCH_PATHS_CFLAGS.
################################################################################

ifneq ($(strip $(CORE_PKG_CONFIG_LIBRARIES)),)
    CORE_HEADER_SEARCH_PATHS_CFLAGS +=                                         \
        $(shell                                                                \
            pkg-config "$(CORE_PKG_CONFIG_LIBRARIES)" --cflags                 \
        )
endif

################################################################################
# CORE_HEADER_SEARCH_PATHS_CFLAGS (continued)
#   Prepend all CORE_THIRD_PARTY_HEADER_SEARCH_PATHS with the -I flag.
#   Add them to the CORE_HEADER_SEARCH_PATHS_CFLAGS.
################################################################################

CORE_HEADER_SEARCH_PATHS_CFLAGS +=                                             \
    $(addprefix                                                                \
        -I,                                                                    \
        $(CORE_THIRD_PARTY_HEADER_SEARCH_PATHS)                                \
    )

################################################################################
# CORE_HEADER_SEARCH_PATHS_CFLAGS (continued)
#   Prepend all CORE_HEADER_SEARCH_PATHS with the -I flag.
#   Add them to the CORE_HEADER_SEARCH_PATHS_CFLAGS.
################################################################################

CORE_HEADER_SEARCH_PATHS_CFLAGS +=                                             \
    $(addprefix                                                                \
        -I,                                                                    \
        $(CORE_HEADER_SEARCH_PATHS)                                            \
    )

################################################################################
# CORE_FRAMEWORK_SEARCH_PATHS_CFLAGS (immediately assigned)
#   The CORE_FRAMEWORK_SEARCH_PATHS_CFLAGS are generated by prepending -F to 
#   each of the PLATFORM_FRAMEWORK_SEARCH_PATHS defined in the platform-
#   specific configuration file.  Usually this is only used for OSX/iOS
#   based builds. 
################################################################################

CORE_FRAMEWORK_SEARCH_PATHS_CFLAGS:=                                           \
    $(addprefix                                                                \
        -F,                                                                    \
        $(PLATFORM_FRAMEWORK_SEARCH_PATHS)                                     \
    )

################################################################################
# CORE DEFINES CFLAGS
################################################################################
# In this section, we gather all of the DEFINES and prepend them with -D
################################################################################

################################################################################
# CORE_DEFINES_CFLAGS (immediately assigned)
#   Set CORE_DEFINES_CFLAGS to PLATFORM_DEFINES while prepending -D to each.
################################################################################

CORE_DEFINES_CFLAGS :=                                                         \
    $(addprefix                                                                \
        -D,                                                                    \
        $(PLATFORM_DEFINES)                                                    \
    )

################################################################################
# CORE_CFLAGS (immediately assigned)
#   Set raw CORE_CFLAGS to raw PLATFORM_CFLAGS.  PLATFORM_CFLAGS include all 
#   fully qualified CFLAGS. 
################################################################################

CORE_CFLAGS :=                                                                 \
    $(PLATFORM_CFLAGS)

################################################################################
# GENERATE A LIST OF ALL CORE SOURCE FILES
################################################################################
# In this section, we generate our list of all core source files by 
# recursively searching the approprate directories.
################################################################################

################################################################################
# ALL_CORE_SOURCE_FILES (immediately assigned)
#   Create a list of all source files found in the CORE_SOURCE_PATHS (defined
#   above).
################################################################################

ALL_CORE_SOURCE_FILES :=                                                       \
             $(call                                                            \
                  FUNC_RECURSIVE_FIND_SOURCES,                                 \
                  $(CORE_SOURCE_PATHS)                                         \
              )                                                                \

################################################################################
# CORE_SOURCE_FILES (immediately assigned)
#   CORE_SOURCE_FILES is a filtered list of source files by removing all files
#   that are included in the CORE_EXCLUSIONS list.
################################################################################

CORE_SOURCE_FILES :=                                                           \
    $(filter-out                                                               \
        $(CORE_EXCLUSIONS),                                                    \
        $(ALL_CORE_SOURCE_FILES)                                               \
    )

################################################################################
# DEBUG ########################################################################
################################################################################
ifdef MAKEFILE_DEBUG
    $(info ========================= config.shared.mk flags ========================)
  
    $(info ---CORE_EXCLUSIONS---)
    $(foreach v, $(CORE_EXCLUSIONS),$(info $(v)))

    $(info ---ALL_CORE_SOURCE_PATHS---)
    $(foreach v, $(ALL_CORE_SOURCE_PATHS),$(info $(v)))

    $(info ---CORE_SOURCE_PATHS---)
    $(foreach v, $(CORE_SOURCE_PATHS),$(info $(v)))

    $(info ---CORE_HEADER_SEARCH_PATHS---)
    $(foreach v, $(CORE_HEADER_SEARCH_PATHS),$(info $(v)))

    $(info ---ALL_CORE_THIRD_PARTY_HEADER_SEARCH_PATHS---)
    $(foreach v, $(ALL_CORE_THIRD_PARTY_HEADER_SEARCH_PATHS),$(info $(v)))

    $(info ---CORE_HEADER_SEARCH_PATHS_CFLAGS---)
    $(foreach v, $(CORE_HEADER_SEARCH_PATHS_CFLAGS),$(info $(v)))

    $(info ---CORE_PKG_CONFIG_LIBRARIES---)
    $(foreach v, $(CORE_PKG_CONFIG_LIBRARIES),$(info $(v)))

    $(info ---ALL_CORE_THIRD_PARTY_STATIC_LIBRARIES---)
    $(foreach v, $(ALL_CORE_THIRD_PARTY_STATIC_LIBRARIES),$(info $(v)))

    $(info ---CORE_THIRD_PARTY_STATIC_LIBRARIES---)
    $(foreach v, $(CORE_THIRD_PARTY_STATIC_LIBRARIES),$(info $(v)))

    $(info ---ALL_CORE_THIRD_PARTY_SHARED_LIBRARIES---)
    $(foreach v, $(ALL_CORE_THIRD_PARTY_SHARED_LIBRARIES),$(info $(v)))

    $(info ---CORE_THIRD_PARTY_SHARED_LIBRARIES---)
    $(foreach v, $(CORE_THIRD_PARTY_SHARED_LIBRARIES),$(info $(v)))

    $(info ---CORE_DEFINES_CFLAGS---)
    $(foreach v, $(CORE_DEFINES_CFLAGS),$(info $(v)))

    $(info ---CORE_CFLAGS---)
    $(foreach v, $(CORE_CFLAGS),$(info $(v)))

    $(info ---ALL_CORE_SOURCE_FILES---)
    $(foreach v, $(ALL_CORE_SOURCE_FILES),$(info $(v)))

    $(info ---CORE_SOURCE_FILES---)
    $(foreach v, $(CORE_SOURCE_FILES),$(info $(v)))

endif
################################################################################
