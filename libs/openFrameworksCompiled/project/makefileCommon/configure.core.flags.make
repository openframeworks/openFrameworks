# define the location of the core path
#TODO: make sure all of the right checks are here.
ifndef PLATFORM_CORE_EXCLUSIONS
    $(error PLATFORM_CORE_EXCLUSIONS not defined)
endif

ifndef OF_LIBS_OPENFRAMEWORKS_PATH
    $(error OF_LIBS_OPENFRAMEWORKS_PATH not defined)
endif


##########################################################################################
# CLEAN CORE EXCLUSIONS AND ADD WILDCARD MATCHER FOR USE WITH FILTER-OUT / FILTER
##########################################################################################

# take from the platform core exclusions
# strip and collapse spaces, and add the % suffix to allow directory wildcard matching
# 1. Strip all extra whitespace : $(strip $(PLATFORM_CORE_EXCLUSIONS))
# 2. Add the makefile % wildcard to the end each exclusion : $(addsuffix %,...) to the end of the path 

CORE_EXCLUSIONS = $(strip $(PLATFORM_CORE_EXCLUSIONS))

##########################################################################################
# OF CORE SOURCE FILES
##########################################################################################

# find all of the source directories, exclude all . (dot-hidden) files/folders
ALL_CORE_SOURCE_PATHS=$(shell find $(OF_LIBS_OPENFRAMEWORKS_PATH) -maxdepth 1 -mindepth 1 -type d -not -name ".*" )

# create a list of core source PATHS, filtering out any  items that have a match in the CORE_EXCLUSIONS list
CORE_SOURCE_PATHS=$(filter-out $(CORE_EXCLUSIONS),$(ALL_CORE_SOURCE_PATHS))

# search the directories in the source folders for all .cpp files
# filter out all excluded files / folders that were defined above
OF_CORE_SOURCE_FILES=$(filter-out $(CORE_EXCLUSIONS),$(shell find $(CORE_SOURCE_PATHS) -name "*.cpp"))

##########################################################################################
# OF CORE HEADER INCLUDES (-I ...)
##########################################################################################

# create our core include paths from the source directory paths, 
# these have already been filtered and processed according to rules.
# plus the root so that we don't miss the ofMain.h.
OF_CORE_HEADER_PATHS = $(OF_LIBS_CORE_PATH) $(CORE_SOURCE_PATHS)

# add folders or single files to exclude fromt he compiled lib
ALL_OF_CORE_THIRDPARTY_HEADER_PATHS = $(shell find $(OF_LIBS_PATH)/*/include -type d)

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

##########################################################################################
# OF CORE LIBRARIES SEARCH PATHS (-L ...) (not used during core compilation, but 
#    variables are used during project compilation)
##########################################################################################

# generate a list of all third party libs, excluding the compiled openFrameworkslibrary.
ALL_OF_CORE_THIRDPARTY_LIB_SEARCH_PATHS = $(shell find $(OF_LIBS_PATH)/*/lib/$(PLATFORM_LIB_SUBPATH) -type d -not -path "*/openFrameworksCompiled/*")

# filter out all excluded paths defined in the platform config files.
OF_CORE_THIRDPARTY_LIBS_SEARCH_PATHS = $(filter-out $(CORE_EXCLUSIONS),$(ALL_OF_CORE_THIRDPARTY_LIB_SEARCH_PATHS))

#TODO: we might not need these search paths, because we explicityly add these libs as a full paths below

##########################################################################################
# OF CORE LIBRARIES (-l ...) (not used during core compilation, but variables are 
#    used during project compilation)
##########################################################################################

# generate a list of all static libraries
ALL_OF_CORE_THIRDPARTY_STATIC_LIBS = $(shell find $(OF_LIBS_PATH)/*/lib/$(PLATFORM_LIB_SUBPATH)/*.a -not -path "*/openFrameworksCompiled/*")
# generate a list of all shared libraries
ALL_OF_CORE_THIRDPARTY_SHARED_LIBS = $(shell find $(OF_LIBS_PATH)/*/lib/$(PLATFORM_LIB_SUBPATH)/*.so -not -path "*/openFrameworksCompiled/*")

# filter out all excluded paths defined in the platform config files.
OF_CORE_THIRDPARTY_STATIC_LIBS = $(filter-out $(CORE_EXCLUSIONS),$(ALL_OF_CORE_THIRDPARTY_STATIC_LIBS))

# filter out all excluded paths defined in the platform config files.
OF_CORE_THIRDPARTY_SHARED_LIBS = $(filter-out $(CORE_EXCLUSIONS),$(ALL_OF_CORE_THIRDPARTY_SHARED_LIBS))

# add in any libraries that were explicitly listed in the platform config files
OF_CORE_THIRDPARTY_STATIC_LIBS += $(PLATFORM_STATIC_LIBRARIES)
OF_CORE_THIRDPARTY_SHARED_LIBS += $(PLATFORM_SHARED_LIBRARIES)

# generate the list of core includes
# 1. Add the libraries defined in the platform config files.
OF_CORE_LIBRARY_CFLAGS = $(addprefix -l,$(PLATFORM_LIBRARIES))
# 2. Add all of the third party static libs defined by the platform config files.
OF_CORE_LIBRARY_CFLAGS += $(addprefix -l,$(OF_CORE_THIRDPARTY_STATIC_LIBS))
# 2. Add all of the third party shared libs defined by the platform config files.
OF_CORE_LIBRARY_CFLAGS += $(addprefix -l,$(OF_CORE_THIRDPARTY_SHARED_LIBS))
# 3. Add all of the core pkg-config OF libs defined by the platform config files.
# Note,  2> /dev/null sends all STDERR message into the /dev/null abyss.
OF_CORE_LIBRARY_CFLAGS += $(shell pkg-config "$(PLATFORM_PKG_CONFIG_LIBRARIES)" --libs 2> /dev/null)

##########################################################################################
# OF CORE DEFINES
##########################################################################################

OF_CORE_DEFINES_CFLAGS=$(addprefix -D,$(PLATFORM_DEFINES))

##########################################################################################
# OF PLATFORM CFLAGS
##########################################################################################

# gather any platform CFLAGS
OF_CORE_BASE_CFLAGS=$(PLATFORM_CFLAGS)

##########################################################################################
# DEBUG INFO
##########################################################################################
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
   
    $(info ---OF_CORE_LIBRARY_CFLAGS---)
    $(foreach v, $(OF_CORE_LIBRARY_CFLAGS),$(info $(v)))
endif