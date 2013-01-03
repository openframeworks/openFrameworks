# define the OF_SHARED_MAKEFILES location
OF_SHARED_MAKEFILES_PATH=$(OF_ROOT)/libs/openFrameworksCompiled/project/makefileCommon

# do all of the core cofiguration work.
# configuration work basically means creating lots of
# lists of source files, search paths, libraries, etc.
#
ifndef SHELL
    SHELL := /bin/sh
endif

ifndef OF_ROOT
    OF_ROOT= ../../..
endif

ifndef PROJECT_ROOT
    PROJECT_ROOT= .
endif

# if APPNAME is not defined, set it to the project dir name
ifndef APPNAME
    APPNAME = $(shell basename `pwd`)
endif

# if the user has not specified a special variant, then use the default variant
ifndef PLATFORM_VARIANT
    PLATFORM_VARIANT = default
endif

# if not defined
ifndef PROJECT_LDFLAGS
	PROJECT_LDFLAGS = -Wl,-rpath=./libs
endif


include $(OF_SHARED_MAKEFILES_PATH)/config.shared.make

############################## FLAGS ###########################################
# OF CORE LIBRARIES SEARCH PATHS (-L ...) (not used during core compilation, but 
#    variables are used during project compilation)
################################################################################

# generate a list of all third party libs, excluding the compiled openFrameworkslibrary.
# grep -v "/\.[^\.]" will exclude all .hidden folders and files
ALL_OF_CORE_THIRDPARTY_LIB_SEARCH_PATHS = $(shell find $(OF_LIBS_PATH)/*/lib/$(PLATFORM_LIB_SUBPATH) -type d -not -path "*/openFrameworksCompiled/*" | grep -v "/\.[^\.]")

# filter out all excluded paths defined in the platform config files.
OF_CORE_THIRDPARTY_LIBS_SEARCH_PATHS = $(filter-out $(CORE_EXCLUSIONS),$(ALL_OF_CORE_THIRDPARTY_LIB_SEARCH_PATHS))


################################################################################
# OF CORE LIBRARIES (-l ...) (not used during core compilation, but variables are 
#    used during project compilation)
################################################################################

# construct the full paths of the core's platform specific static libs 
ALL_OF_CORE_LIBS_PLATFORM_LIB_PATHS = $(OF_LIBS_PATH)/*/lib/$(PLATFORM_LIB_SUBPATH)

# create a list of all core platform libraries
# grep -v "/\.[^\.]" will exclude all .hidden folders and files
ALL_OF_CORE_LIBS_PATHS = $(shell find $(ALL_OF_CORE_LIBS_PLATFORM_LIB_PATHS) -type d -not -path "*/openFrameworksCompiled/*" 2> /dev/null | grep -v "/\.[^\.]" )

# create a list of all core lib directories that have libsorder.make
# grep -v "/\.[^\.]" will exclude all .hidden folders and files
ALL_OF_CORE_LIBSORDER_MAKE_FILES = $(shell find $(ALL_OF_CORE_LIBS_PLATFORM_LIB_PATHS) -name libsorder.make -not -path "*/openFrameworksCompiled/*" 2> /dev/null | grep -v "/\.[^\.]" )

# create a list of all of the core libs that require ordering
OF_CORE_LIBS_THAT_NEED_ORDER = $(subst /lib/$(PLATFORM_LIB_SUBPATH)/libsorder.make,,$(ALL_OF_CORE_LIBSORDER_MAKE_FILES))

# create a list of all of the platform libs that DO NOT require ordering
# by removing those that do from the list of all platform libraries
OF_CORE_LIBS_THAT_DONT_NEED_ORDER = $(filter-out $(OF_CORE_LIBS_THAT_NEED_ORDER),$(subst /lib/$(PLATFORM_LIB_SUBPATH),,$(ALL_OF_CORE_LIBS_PATHS)))

# create a list of all static libs in the core lib dir, using only 
# the static libs that don't need order
# 2> /dev/null consumes file not found errors from find searches
# grep -v "/\.[^\.]" will exclude all .hidden folders and files
# TODO: create a varaible for core specific static lib suffix
OF_CORE_LIBS_PLATFORM_LIBS_STATICS = $(shell find $(addsuffix /lib/$(PLATFORM_LIB_SUBPATH),$(OF_CORE_LIBS_THAT_DONT_NEED_ORDER)) -name *.a 2> /dev/null | grep -v "/\.[^\.]" )
# create a list of all static lib files for the libs that need order
# NOTE. this is the most unintuitive line of make script magic in here
# How does it work?
# 1. Explode each libsorder.make file using cat.  
# 2. For each lib listed in the libsorder.make file, look back at the
#    _original_ path for the libsorder.make file itself, and construct 
#    a path for each of the ordered static libs.  Thus, each exploded 
#    static lib will get its path from the path of the libsorder.make 
#    in which it was listed.
# 3. Add all of them to the ordered files.
#    sed 's/[ ]*#.*//g' strips all comments beginning with #
#    (to escape # in make, you must use \#)
#    sed '/^$/d' removes all empty lines
#    (to escape $ in make, you must use $$)
OF_CORE_LIBS_PLATFORM_LIBS_STATICS += $(foreach v,$(ALL_OF_CORE_LIBSORDER_MAKE_FILES),$(foreach vv,$(shell cat $(v) 2> /dev/null | sed 's/[ ]*\#.*//g' | sed '/^$$/d'),$(addprefix $(subst libsorder.make,,$(v)),$(vv))))

# grep -v "/\.[^\.]" will exclude all .hidden folders and files
ALL_OF_CORE_THIRDPARTY_SHARED_LIBS := $(shell find $(OF_LIBS_PATH)/*/lib/$(PLATFORM_LIB_SUBPATH)/*.so -not -path "*/openFrameworksCompiled/*" 2> /dev/null | grep -v "/\.[^\.]")

# add in any libraries that were explicitly listed in the platform config files.
OF_CORE_THIRDPARTY_STATIC_LIBS := $(PLATFORM_STATIC_LIBRARIES)
OF_CORE_THIRDPARTY_STATIC_LIBS += $(filter-out $(CORE_EXCLUSIONS),$(OF_CORE_LIBS_PLATFORM_LIBS_STATICS))

# add in any libraries that were explicitly listed in the platform config files.
OF_CORE_THIRDPARTY_SHARED_LIBS := $(PLATFORM_SHARED_LIBRARIES)

#TODO what to do with shared libs?
#OF_CORE_THIRDPARTY_SHARED_LIBS += $(filter-out $(CORE_EXCLUSIONS),$(ALL_OF_CORE_THIRDPARTY_SHARED_LIBS))

# generate the list of core includes
# 1. Add the libraries defined in the platform config files.
OF_CORE_LIBS := $(addprefix -l,$(PLATFORM_LIBRARIES))
# 2. Add all of the third party static libs defined by the platform config files.
OF_CORE_LIBS += $(OF_CORE_THIRDPARTY_STATIC_LIBS)
# 2. Add all of the third party shared libs defined by the platform config files.
OF_CORE_LIBS += $(OF_CORE_THIRDPARTY_SHARED_LIBS)
# 3. Add all of the core pkg-config OF libs defined by the platform config files.
# Note,  2> /dev/null sends all STDERR message into the /dev/null abyss.
OF_CORE_LIBS += $(shell pkg-config "$(PLATFORM_PKG_CONFIG_LIBRARIES)" --libs 2> /dev/null)

################################################################################
# OF PLATFORM LDFLAGS
################################################################################

OF_CORE_LIBRARY_LDFLAGS = $(addprefix -L,$(OF_CORE_THIRDPARTY_LIBS_SEARCH_PATHS))
OF_CORE_LIBRARY_LDFLAGS += $(addprefix -L,$(PLATFORM_LIBRARY_SEARCH_PATHS))


################################################################################
# DEBUG INFO
################################################################################
ifdef MAKEFILE_DEBUG
    $(info =============================configure.core.flags.make========================)   
    $(info ---OF_CORE_LIBS_LDFLAGS---)
    $(foreach v, $(OF_CORE_LIBS_LDFLAGS),$(info $(v)))
    
    $(info ---OF_CORE_LIBS---)
    $(foreach v, $(OF_CORE_LIBS),$(info $(v)))
endif

################################# ADDONS ######################################

ifdef MAKEFILE_DEBUG
    $(info ===================ADDONS================)
endif

# check to make sure OF_ROOT is defined
ifndef OF_ROOT
    $(error OF_ROOT is not defined)
endif

ifndef OF_ADDONS_PATH
    $(error OF_ADDONS_PATH is not defined)
endif

# check to make sure PLATFORM_LIB_SUBPATH is defined
ifndef PLATFORM_LIB_SUBPATH
    $(error PLATFORM_LIB_SUBPATH is not defined)
endif

################################################################################
# for reference, please see the following:
# https://github.com/benben/ofxAddonTemplate

# we will process addons if there is an 
# addons.make file OR PLATFORM_REQUIRED_ADDONS is defined
# we do it this way because gnu make can't do logical ORs 
B_PROCESS_ADDONS = 
ifdef PLATFORM_REQUIRED_ADDONS
    B_PROCESS_ADDONS = yes
endif
ifeq ($(findstring addons.make,$(wildcard $(PROJECT_ROOT)/*.make)),addons.make)
    B_PROCESS_ADDONS = yes
endif

# you can't do LOGICAL ORs in make.  So we do this ...
ifdef B_PROCESS_ADDONS
    ############################################################################
    # VALIDATE REQUESTED ADDONS
    ############################################################################

    # create a list of every addon installed in the addons directory
    ALL_INSTALLED_ADDONS = $(subst $(OF_ADDONS_PATH)/,,$(wildcard $(OF_ADDONS_PATH)/*))

    # get a list of all addons listed in addons.make file
    # sed 's/[ ]*#.*//g' strips all comments beginning with #
    # (to escape # in make, you must use \#)
    # sed '/^$/d' removes all empty lines
    # (to escape $ in make, you must use $$)
    REQUESTED_PROJECT_ADDONS := $(shell cat $(PROJECT_ROOT)/addons.make 2> /dev/null | sed 's/[ ]*\#.*//g' | sed '/^$$/d' )

    # deal with platform specfic addons
    # remove any platform specific addons that were already added to the addons.make file
    REQUESTED_PROJECT_ADDONS := $(filter-out $(PLATFORM_REQUIRED_ADDONS),$(REQUESTED_PROJECT_ADDONS))

    # define a function to remove duplicates without using sort, because sort 
    # will place the list in lexicographic order, and we want to respect the 
    # user's addons.make order.  
    remove-dupes-func = $(if $1,$(strip $(word 1,$1) \
                        $(call $0,$(filter-out $(word 1,$1),$1))))

    # remove all duplicates that might be in the addons.make file
    REQUESTED_PROJECT_ADDONS := $(call remove-dupes-func,$(REQUESTED_PROJECT_ADDONS))

    # add platform required addons from the platform configuration file (if needed)
    # add the platform required addons first, so that they are always linked first
    REQUESTED_PROJECT_ADDONS := $(PLATFORM_REQUIRED_ADDONS) $(REQUESTED_PROJECT_ADDONS)

    # create a list requested addons that are actually installed on the system
    VALID_PROJECT_ADDONS = $(filter $(REQUESTED_PROJECT_ADDONS),$(ALL_INSTALLED_ADDONS))

    # create a list of the invalid addons
    INVALID_PROJECT_ADDONS = $(filter-out $(VALID_PROJECT_ADDONS),$(REQUESTED_PROJECT_ADDONS))

    # if any invalid addons are found, throw a warning, but don't cause an error
    ifneq ($(INVALID_PROJECT_ADDONS),)
        $(warning The following unknown addons will be ignored:)
        $(foreach v, $(INVALID_PROJECT_ADDONS),$(info $(v)))
        # TODO: download and launch requested addons from ofxaddons?
    endif

    # create a list of addons, excluding invalid and platform-specific addons
    PROJECT_ADDONS = $(filter-out $(INVALID_PROJECT_ADDONS),$(REQUESTED_PROJECT_ADDONS))

    ifdef MAKEFILE_DEBUG
        $(info ---PROJECT_ADDONS---)
        $(foreach v, $(PROJECT_ADDONS),$(info $(v)))
        $(info --------------------)
    endif

    ############################################################################
    # PROCESS PROJECT ADDONS IF AVAILABLE
    ############################################################################

    # if the addons list is NOT empty ...
    ifneq ($(PROJECT_ADDONS),)

        ########################################################################
        # PROCESS VALID ADDONS IF AVAILABLE
        ########################################################################

        ########################################################################
        # ADDON SOURCE FILES ###################################################
        # construct the full paths of the addon source directories 
        # (e.g. https://github.com/benben/ofxAddonTemplate/tree/master/src)
        PROJECT_ADDONS_SOURCE_PATHS = $(addprefix $(OF_ADDONS_PATH)/, $(addsuffix /src, $(PROJECT_ADDONS)) )

        # create a list of addon source files
        # 2> /dev/null consumes file not found errors from find searches
        # grep -v "/\.[^\.]" will exclude all .hidden folders and files
        PROJECT_ADDONS_OFX_SOURCES = $(shell find $(PROJECT_ADDONS_SOURCE_PATHS) -type f \( -name "*.cpp" -or -name "*.c" -or -name "*.cc" -or -name "*.cxx" \) 2> /dev/null | grep -v "/\.[^\.]" )

        ########################################################################
        # ADDON'S LIBS' SOURCE FILES ###########################################
        # construct the full paths of the addon's libs' source directories 
        # (e.g. https://github.com/benben/ofxAddonTemplate/tree/master/libs/necessaryLib/src)
        PROJECT_ADDONS_LIBS_SOURCE_PATHS = $(addprefix $(OF_ADDONS_PATH)/, $(addsuffix /libs/*/src, $(PROJECT_ADDONS)) )

        # construct the full paths of the addon's libs' source directories to be included
        # grep -v "/\.[^\.]" will exclude all .hidden folders and files
        PROJECT_ADDONS_LIBS_SOURCE_INCLUDES = $(shell find $(PROJECT_ADDONS_LIBS_SOURCE_PATHS) -type d 2> /dev/null | grep -v "/\.[^\.]" )

        # create a list of addon's libs' source files
        # 2> /dev/null consumes file not found errors from find searches
        # grep -v "/\.[^\.]" will exclude all .hidden folders and files
        PROJECT_ADDONS_LIBS_SOURCES = $(shell find $(PROJECT_ADDONS_LIBS_SOURCE_PATHS) -type f \( -name "*.cpp" -or -name "*.c" -or -name "*.cc" -or -name "*.cxx" \) 2> /dev/null | grep -v "/\.[^\.]"  )

        ########################################################################
        # ADDON'S LIBS' INCLUDES ###############################################
        # construct the full paths of the addon's include directories 
        # TODO: do we need to prevent recursive searching for includes?
        # (e.g. https://github.com/benben/ofxAddonTemplate/tree/master/libs/necessaryLib/include)
        PROJECT_ADDONS_LIBS_INCLUDES_PATHS = $(addprefix $(OF_ADDONS_PATH)/, $(addsuffix /libs/*/include, $(PROJECT_ADDONS)) )
        
        # search through all of the include's sub folders
        # 2> /dev/null consumes file not found errors from find searches
        # grep -v "/\.[^\.]" will exclude all .hidden folders and files
        PROJECT_ADDONS_LIBS_INCLUDES = $(shell find $(PROJECT_ADDONS_LIBS_INCLUDES_PATHS) -type d 2> /dev/null | grep -v "/\.[^\.]"  )

        ########################################################################
        # ADDON'S PLATFORM SPECIFIC STATIC LIBS ################################
        # construct the full paths of the addon's platform specific static libs 
        # (e.g. https://github.com/benben/ofxAddonTemplate/tree/master/libs/necessaryLib/lib/linux)
        PROJECT_ADDONS_LIBS_PLATFORM_LIB_PATHS = $(addprefix $(OF_ADDONS_PATH)/, $(addsuffix /libs/*/lib/$(PLATFORM_LIB_SUBPATH), $(PROJECT_ADDONS)) )
 
        # create a list of all addon platform libraries
        # grep -v "/\.[^\.]" will exclude all .hidden folders and files
        ALL_PLATFORM_LIBS = $(shell find $(PROJECT_ADDONS_LIBS_PLATFORM_LIB_PATHS) -type d 2> /dev/null | grep -v "/\.[^\.]" )

        # create a list of all platform lib directories that have libsorder.make
        # grep -v "/\.[^\.]" will exclude all .hidden folders and files
        LIBSORDER_MAKE_FILES = $(shell find $(PROJECT_ADDONS_LIBS_PLATFORM_LIB_PATHS) -name libsorder.make 2> /dev/null | grep -v "/\.[^\.]" )

        # create a list of all of the platform libs that require ordering
        PLATFORM_LIBS_THAT_NEED_ORDER = $(subst /lib/$(PLATFORM_LIB_SUBPATH)/libsorder.make,,$(LIBSORDER_MAKE_FILES))

        # create a list of all of the platform libs that DO NOT require ordering
        # by removing those that do from the list of all platform libraries
        PLATFORM_LIBS_THAT_DONT_NEED_ORDER := $(filter-out $(PLATFORM_LIBS_THAT_NEED_ORDER),$(subst /lib/$(PLATFORM_LIB_SUBPATH),,$(ALL_PLATFORM_LIBS)))

        # create a list of all static libs in the platform lib dir, using only 
        # the static libs that don't need order
        # 2> /dev/null consumes file not found errors from find searches
        # grep -v "/\.[^\.]" will exclude all .hidden folders and files
        # TODO: create a varaible for platform specific static lib suffix
        PROJECT_ADDONS_LIBS_PLATFORM_LIBS_STATICS = $(shell find $(addsuffix /lib/$(PLATFORM_LIB_SUBPATH),$(PLATFORM_LIBS_THAT_DONT_NEED_ORDER)) -name *.a 2> /dev/null | grep -v "/\.[^\.]" )

        # create a list of all static lib files for the libs that need order
        # NOTE. this is the most unintuitive line of make script magic in here
        # How does it work?
        # 1. Explode each libsorder.make file using cat.  
        # 2. For each lib listed in the libsorder.make file, look back at the
        #    _original_ path for the libsorder.make file itself, and construct 
        #    a path for each of the ordered static libs.  Thus, each exploded 
        #    static lib will get its path from the path of the libsorder.make 
        #    in which it was listed.
        # 3. Add all of them to the ordered files.
        #    sed 's/[ ]*#.*//g' strips all comments beginning with #
        #    (to escape # in make, you must use \#)
        #    sed '/^$/d' removes all empty lines
        #    (to escape $ in make, you must use $$)
        PROJECT_ADDONS_LIBS_PLATFORM_LIBS_STATICS += $(foreach v,$(LIBSORDER_MAKE_FILES),$(foreach vv,$(shell cat $(v) 2> /dev/null | sed 's/[ ]*\#.*//g' | sed '/^$$/d'),$(addprefix $(subst libsorder.make,,$(v)),$(vv))))

        ########################################################################
        # ADDON'S PLATFORM SPECIFIC SHARED LIBS ################################
        # 2> /dev/null consumes file not found errors from find searches
        # grep -v "/\.[^\.]" will exclude all .hidden folders and files
        # TODO: create a varaible for platform specific shared lib suffix
        # TODO: do we need to create an equiv to libsorder.make for shared libs?
        #PROJECT_ADDONS_LIBS_PLATFORM_LIBS_SHARED = $(shell find $(PROJECT_ADDONS_LIBS_PLATFORM_LIB_PATHS) -name *.so 2> /dev/null | grep -v "/\.[^\.]")
        #TODO: what to do with shared libs?

################################################################################
# GATHER ALL LISTS #############################################################
################################################################################

        ########################################################################
        # ADDON'S SOURCES ######################################################
        # create a master list of both static and shared libraries
        # Note: add libs sources first
        PROJECT_ADDONS_SOURCE_FILES := $(PROJECT_ADDONS_LIBS_SOURCES)
        PROJECT_ADDONS_SOURCE_FILES += $(PROJECT_ADDONS_OFX_SOURCES)

        ########################################################################
        # ADDON'S INCLUDES #####################################################
        # create a master list of both static and shared libraries
        # Note: add libs sources first

        PROJECT_ADDONS_INCLUDES := $(PROJECT_ADDONS_LIBS_SOURCE_INCLUDES)
        PROJECT_ADDONS_INCLUDES += $(PROJECT_ADDONS_LIBS_INCLUDES)
        PROJECT_ADDONS_INCLUDES += $(PROJECT_ADDONS_SOURCE_PATHS)

        ########################################################################
        # ADDON'S PLATFORM SPECIFIC LIBS #######################################
        # create a master list of both static and shared libraries
        PROJECT_ADDONS_LIBS := $(PROJECT_ADDONS_LIBS_PLATFORM_LIBS_STATICS)
        #PROJECT_ADDONS_LIBS += $(PROJECT_ADDONS_LIBS_PLATFORM_LIBS_SHARED)
        #TODO: what to do with shared libs?

        ########################################################################
        #  DEBUGGING
        ########################################################################
        # print debug information if so instructed
        ifdef MAKEFILE_DEBUG
            $(info ---PROJECT_ADDONS_INCLUDES---)
            $(foreach v, $(PROJECT_ADDONS_INCLUDES),$(info $(v)))
            $(info ---PROJECT_ADDONS_SOURCES---)
            $(foreach v, $(PROJECT_ADDONS_SOURCES),$(info $(v)))
            $(info ---PROJECT_ADDONS_LIBS---)
            $(foreach v, $(PROJECT_ADDONS_LIBS),$(info $(v)))
            $(info ---PROJECT_ADDONS_OBJFILES---)
            $(foreach v, $(PROJECT_ADDONS_OBJFILES),$(info $(v)))
            $(info ---PROJECT_ADDONS_BASE_CFLAGS---)
            $(foreach v, $(PROJECT_ADDONS_BASE_CFLAGS),$(info $(v)))
            $(info ---PROJECT_ADDONS_DEFINES_CFLAGS---)
            $(foreach v, $(PROJECT_ADDONS_DEFINES_CFLAGS),$(info $(v)))
            $(info ---PROJECT_ADDONS_INCLUDES_CFLAGS---)
            $(foreach v, $(PROJECT_ADDONS_INCLUDES_CFLAGS),$(info $(v)))
            $(info ---PROJECT_ADDONS_LDFLAGS---)
            $(foreach v, $(PROJECT_ADDONS_LDFLAGS),$(info $(v)))
        endif
    endif
endif

################################################################################
# CLEAN PROJECT EXCLUSIONS 
################################################################################

OF_PROJECT_EXCLUSIONS := $(strip $(PROJECT_EXCLUSIONS))

# add defaults here TODO: should these always be 
OF_PROJECT_EXCLUSIONS += $(PROJECT_ROOT)/bin%
OF_PROJECT_EXCLUSIONS += $(PROJECT_ROOT)/obj%
OF_PROJECT_EXCLUSIONS += $(PROJECT_ROOT)/%.xcodeproj


################################################################################
# PROJECT SOURCE FILES
################################################################################

# exclude all items from source according to the config.make file

# create a list of all dirs in the project root that might be valid project
# source directories 
# grep -v "/\.[^\.]" will exclude all .hidden folders and files
ALL_OF_PROJECT_SOURCE_PATHS = $(shell find $(PROJECT_ROOT) -mindepth 1 -type d | grep -v "/\.[^\.]")

# be included as locations for header searches via 
TMP_SOURCE_PATHS = $(filter-out $(OF_PROJECT_EXCLUSIONS),$(ALL_OF_PROJECT_SOURCE_PATHS))

# legacy exclusion
ifdef EXCLUDE_FROM_SOURCE
    SED_EXCLUDE_FROM_SRC = $(shell echo  $(EXCLUDE_FROM_SOURCE) | sed 's/\,/\\|/g')\|^\..*\|\/\..**
    OF_PROJECT_SOURCE_PATHS = $(shell echo $(TMP_SOURCE_PATHS) | sed s/.\\/// | grep -v "$(SED_EXCLUDE_FROM_SRC)")
else
    OF_PROJECT_SOURCE_PATHS = $(TMP_SOURCE_PATHS)
endif

ifdef MAKEFILE_DEBUG
	$(info ---OF_PROJECT_SOURCE_PATHS---)
	$(foreach v, $(OF_PROJECT_SOURCE_PATHS),$(info $(v)))

	$(info ---OF_PROJECT_EXCLUSIONS---)
	$(foreach v, $(OF_PROJECT_EXCLUSIONS),$(info $(v)))
endif

# find all sources inside the project's source directory (recursively)
# grep -v "/\.[^\.]" will exclude all .hidden folders and files
OF_PROJECT_SOURCE_FILES = $(shell find $(OF_PROJECT_SOURCE_PATHS) -name "*.cpp" -or -name "*.c" -or -name "*.cc" -or -name "*.cxx" | grep -v "/\.[^\.]")

################################################################################
# PROJECT HEADER INCLUDES (-I ...)
################################################################################

OF_PROJECT_INCLUDES := $(OF_PROJECT_SOURCE_PATHS)
OF_PROJECT_INCLUDES += $(PROJECT_ADDONS_INCLUDES)

OF_PROJECT_INCLUDES_CFLAGS = $(addprefix -I,$(OF_PROJECT_INCLUDES))

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
OF_PROJECT_LIBS := 
OF_PROJECT_LIBS += $(PROJECT_ADDONS_LIBS)

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
OF_PROJECT_CFLAGS += $(USER_CFLAGS) # legacy
OF_PROJECT_CFLAGS += $(OF_PROJECT_DEFINES_CFLAGS)
OF_PROJECT_CFLAGS += $(OF_PROJECT_INCLUDES_CFLAGS)
OF_PROJECT_CFLAGS += $(OF_CORE_BASE_CFLAGS)
OF_PROJECT_CFLAGS += $(OF_CORE_DEFINES_CFLAGS)
OF_PROJECT_CFLAGS += $(OF_CORE_INCLUDES_CFLAGS)


################################################################################
# PROJECT LDLAGS
################################################################################

OF_PROJECT_LDFLAGS := $(PROJECT_LDFLAGS)
OF_PROJECT_LDFLAGS += $(USER_LDLAGS) # legacy
OF_PROJECT_LDFLAGS += $(USER_LIBS)   # legacy
OF_PROJECT_LDFLAGS += $(OF_PROJECT_LIBS_LDFLAGS)
OF_PROJECT_LDFLAGS += $(OF_CORE_LIBRARY_LDFLAGS)


# compile core openFrameworks library
# check for platform specific compile rules
# if there's none use common ones
ifeq ($(wildcard $(OF_PLATFORM_MAKEFILES)/compile.project.make),) 
    include $(OF_SHARED_MAKEFILES_PATH)/compile.project.make
else 
    include $(OF_PLATFORM_MAKEFILES)/compile.project.make
endif
