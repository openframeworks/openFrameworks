# define the location of the core path
#TODO: make sure all of the right checks are here.
ifndef PLATFORM_CORE_EXCLUSIONS
    $(error PLATFORM_CORE_EXCLUSIONS not defined)
endif

ifndef OF_LIBS_OPENFRAMEWORKS_PATH
    $(error OF_LIBS_OPENFRAMEWORKS_PATH not defined)
endif


################################################################################
# CLEAN CORE EXCLUSIONS 
################################################################################

# take from the platform core exclusions and strip and collapse spaces

CORE_EXCLUSIONS = $(strip $(PLATFORM_CORE_EXCLUSIONS))

################################################################################
# CORE SOURCE FILES
################################################################################

# find all of the source directories
# grep -v "/\.[^\.]" will exclude all .hidden folders and files
ALL_OF_CORE_SOURCE_PATHS=$(shell find $(OF_LIBS_OPENFRAMEWORKS_PATH) -maxdepth 1 -mindepth 1 -type d | grep -v "/\.[^\.]" )

# create a list of core source PATHS, filtering out any  items that have a match in the CORE_EXCLUSIONS list
OF_CORE_SOURCE_PATHS=$(filter-out $(CORE_EXCLUSIONS),$(ALL_OF_CORE_SOURCE_PATHS))

# search the directories in the source folders for all .cpp files
# filter out all excluded files / folders that were defined above
# grep -v "/\.[^\.]" will exclude all .hidden folders and files
OF_CORE_SOURCE_FILES=$(filter-out $(CORE_EXCLUSIONS),$(shell find $(OF_CORE_SOURCE_PATHS) -name "*.cpp" | grep -v "/\.[^\.]"))

################################################################################
# OF CORE HEADER INCLUDES (-I ...)
################################################################################

# create our core include paths from the source directory paths, 
# these have already been filtered and processed according to rules.
# plus the root so that we don't miss the ofMain.h.
OF_CORE_HEADER_PATHS = $(OF_LIBS_OPENFRAMEWORKS_PATH) $(OF_CORE_SOURCE_PATHS)

# add folders or single files to exclude fromt he compiled lib
# grep -v "/\.[^\.]" will exclude all .hidden folders and files
ALL_OF_CORE_THIRDPARTY_HEADER_PATHS = $(shell find $(OF_LIBS_PATH)/*/include -type d | grep -v "/\.[^\.]")

# filter out all excluded files / folders that were defined above
OF_CORE_THIRDPARTY_HEADER_PATHS = $(filter-out $(CORE_EXCLUSIONS),$(ALL_OF_CORE_THIRDPARTY_HEADER_PATHS))

# generate the list of core includes
# 1. Add the header search paths defined by the platform config files.
OF_CORE_INCLUDES_CFLAGS = $(addprefix -I,$(PLATFORM_HEADER_SEARCH_PATHS))
# 2. Add all of the system library search paths defined by the platform config files.
# Note,  2> /dev/null sends all STDERR message into the /dev/null abyss.
OF_CORE_INCLUDES_CFLAGS += $(shell pkg-config "$(PLATFORM_PKG_CONFIG_LIBRARIES)" --cflags 2> /dev/null)
# 3. Add all of the standard OF third party library headers (these have already been filtered above according to the platform config files)
OF_CORE_INCLUDES_CFLAGS += $(addprefix -I,$(OF_CORE_THIRDPARTY_HEADER_PATHS))
# 4. Add all of the core OF headers(these have already been filtered above according to the platform config files)
OF_CORE_INCLUDES_CFLAGS += $(addprefix -I,$(OF_CORE_HEADER_PATHS))

################################################################################
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
OF_CORE_LIBS_PLATFORM_LIBS_STATICS += $(foreach v,$(ALL_OF_CORE_LIBSORDER_MAKE_FILES),$(foreach vv,$(shell cat $(v) | sed 's/[ ]*\#.*//g' | sed '/^$$/d'),$(addprefix $(subst libsorder.make,,$(v)),$(vv))))

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
# OF CORE DEFINES
################################################################################

OF_CORE_DEFINES_CFLAGS=$(addprefix -D,$(PLATFORM_DEFINES))

################################################################################
# OF PLATFORM CFLAGS
################################################################################

# gather any platform CFLAGS
OF_CORE_BASE_CFLAGS=$(PLATFORM_CFLAGS)

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
    $(info ---OF_CORE_DEFINES_CFLAGS---)
    $(foreach v, $(OF_CORE_DEFINES_CFLAGS),$(info $(v)))

    $(info ---OF_CORE_SOURCE_FILES---)
    $(foreach v, $(OF_CORE_SOURCE_FILES),$(info $(v)))

    $(info ---OF_CORE_INCLUDES_CFLAGS---)
    $(foreach v, $(OF_CORE_INCLUDES_CFLAGS),$(info $(v)))
  
    $(info ---OF_CORE_FRAMEWORKS_CFLAGS---)
    $(foreach v, $(OF_CORE_FRAMEWORKS_CFLAGS),$(info $(v)))
   
    $(info ---OF_CORE_LIBS_LDFLAGS---)
    $(foreach v, $(OF_CORE_LIBS_LDFLAGS),$(info $(v)))
endif