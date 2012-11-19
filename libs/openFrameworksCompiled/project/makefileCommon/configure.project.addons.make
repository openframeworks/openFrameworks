ifdef MAKEFILE_DEBUG
    $(info ===================makefile.configure.project.addons================)
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

ifeq ($(findstring addons.make,$(wildcard $(PROJECT_ROOT)/*.make)),addons.make)
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
    REQUESTED_PROJECT_ADDONS := $(shell cat $(PROJECT_ROOT)/addons.make | sed 's/[ ]*\#.*//g' | sed '/^$$/d' )

    # deal with platform specfic addons

    # remove any platform specific addons that were already added to the addons.make file
    REQUESTED_PROJECT_ADDONS := $(filter-out $(PLATFORM_REQUIRED_ADDONS),$(REQUESTED_PROJECT_ADDONS))

    remove-dupes = $(if $1,$(strip $(word 1,$1) \
                   $(call $0,$(filter-out $(word 1,$1),$1))))

    # remove all duplicates that might be in the addons.make file
    REQUESTED_PROJECT_ADDONS := = $(call remove-dupes,$(CC)))

    # add platform required addons from the platform configuration file (if needed)
    # add the platform required addons first, so that they are always linked first
    REQUESTED_PROJECT_ADDONS := $(PLATFORM_REQUIRED_ADDONS) $(REQUESTED_PROJECT_ADDONS)

    $(info --------------------------------------XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX)
    $(foreach v, $(INVALID_PROJECT_ADDONS),$(info $(v)))
    $(info --------------------------------------XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX)    

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
        PROJECT_ADDONS_LIBS_PLATFORM_LIBS_STATICS += $(foreach v,$(LIBSORDER_MAKE_FILES),$(foreach vv,$(shell cat $(v) | sed 's/[ ]*\#.*//g' | sed '/^$$/d'),$(addprefix $(subst libsorder.make,,$(v)),$(vv))))

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
