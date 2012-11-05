# define the location of the core path

ifndef PLATFORM_CORE_SOURCE_EXCLUSIONS
    $(error PLATFORM_CORE_SOURCE_EXCLUSIONS not defined)
endif

ifndef OF_LIBS_OF_PATH
    $(error OF_LIBS_OF_PATH not defined)
endif


##########################################################################################
# SOURCE FILES
##########################################################################################

# take from the platform core exclusions
# strip and collapse spaces, and add the % suffix to allow directory wildcard matching
CORE_SOURCE_EXCLUSIONS = $(addprefix $(OF_LIBS_OF_PATH),$(addsuffix %,$(strip $(PLATFORM_CORE_SOURCE_EXCLUSIONS))))

# find all of the source directories, exclude all . (dot-hidden) files/folders
ALL_CORE_SOURCE_PATHS=$(shell find $(OF_LIBS_OF_PATH) -maxdepth 1 -mindepth 1 -type d \( ! -name ".*" \) )

# filter out all excluded files / folders that were defined above
# this list will be searched for cpp files below and will eventually
# be included as locations for header searches via 
CORE_SOURCE_PATHS=$(filter-out $(CORE_SOURCE_EXCLUSIONS),$(ALL_CORE_SOURCE_PATHS))

# search the directories in the source folders for all .cpp files
# filter out all excluded files / folders that were defined above
OF_CORE_SOURCES=$(filter-out $(CORE_SOURCE_EXCLUSIONS),$(shell find $(CORE_SOURCE_PATHS) -name "*.cpp"))

##TODO: add platform inclusions here

##########################################################################################
# INCLUDES
##########################################################################################

# create our core include paths from the source directory paths, 
# these have already been filtered and processed according to rules.
# plus the root so that we don't miss the ofMain.h.
OF_CORE_HEADER_PATHS = $(OF_LIBS_CORE_PATH) $(CORE_SOURCE_PATHS)

# add folders or single files to exclude fromt he compiled lib
UNFILTERED_OF_CORE_THIRDPARTY_HEADER_PATHS = $(shell find $(OF_LIBS_PATH)/*/include -type d \( ! -name ".*" \))

THIRDPARTY_HEADER_EXCLUSIONS = $(PLATFORM_THIRDPARTY_HEADER_EXCLUSIONS)
# add more here?

# strip and collapse spaces, and add the % suffix to allow directory matching
CLEANED_OF_CORE_THIRDPARTY_HEADER_EXCLUSIONS = $(addprefix $(OF_LIBS_PATH),$(addsuffix %,$(strip $(THIRDPARTY_HEADER_EXCLUSIONS))))

# filter out all excluded files / folders that were defined above
OF_CORE_THIRDPARTY_HEADER_PATHS = $(filter-out $(CLEANED_OF_CORE_THIRDPARTY_HEADER_EXCLUSIONS),$(UNFILTERED_OF_CORE_THIRDPARTY_HEADER_PATHS))

# ALL OF THE HEADER FILES REQUIRED BY THE OF CORE
ALL_OF_CORE_HEADER_PATHS = $(PLATFORM_HEADER_SEARCH_PATHS) $(OF_CORE_THIRDPARTY_HEADER_PATHS) $(OF_CORE_HEADER_PATHS)

##########################################################################################
# LIBRARIES
##########################################################################################

UNFILTERED_OF_CORE_THIRDPARTY_STATIC_LIBS = $(shell ls $(OF_LIBS_PATH)/*/lib/$(PLATFORM_LIB_SUBPATH)/*.a  2> /dev/null | grep -v openFrameworksCompiled)
UNFILTERED_OF_CORE_THIRDPARTY_SHARED_LIBS = $(shell ls $(OF_LIBS_PATH)/*/lib/$(PLATFORM_LIB_SUBPATH)/*.so 2> /dev/null | grep -v openFrameworksCompiled)

# filter out all excluded files / folders that were defined above
OF_CORE_THIRDPARTY_STATIC_LIBS = $(filter-out $(CLEANED_OF_CORE_THIRDPARTY_HEADER_EXCLUSIONS),$(UNFILTERED_OF_CORE_THIRDPARTY_STATIC_LIBS))
OF_CORE_THIRDPARTY_SHARED_LIBS = $(filter-out $(CLEANED_OF_CORE_THIRDPARTY_HEADER_EXCLUSIONS),$(UNFILTERED_OF_CORE_THIRDPARTY_SHARED_LIBS))

# add in any libraries that were explicitly listed in the platform config
OF_CORE_THIRDPARTY_STATIC_LIBS += $(PLATFORM_STATIC_LIBRARIES)
OF_CORE_THIRDPARTY_SHARED_LIBS += $(PLATFORM_SHARED_LIBRARIES)

##########################################################################################
# FRAMEWORKS, OSX
##########################################################################################

#OF_CORE_FRAMEWORKS = $(addprefix -l,$(OF_CORE_))


##########################################################################################
# CFLAGS CONSTRUCTION
##########################################################################################

OF_CORE_INCLUDES_CFLAGS = $(addprefix -I,$(ALL_OF_CORE_HEADER_PATHS))

OF_CORE_STATIC_LIB_CFLAGS = $(addprefix -l,$(OF_CORE_THIRDPARTY_STATIC_LIBS))
OF_CORE_SHARED_LIB_CFLAGS = $(addprefix -l,$(OF_CORE_THIRDPARTY_SHARED_LIBS))
OF_CORE_PKG_CONFIG_LIB_CFLAGS = $(shell pkg-config "$(PLATFORM_PKG_CONFIG_LIBRARIES)" --libs)

#OF_CORE_FRAMEWORKS_CFLAGS = $(addprefix -framework,$(OF_CORE_FRAMEWORKS))

#$(info ---hhh---)

##########################################################################################
# DEBUG INFO
##########################################################################################
ifdef MAKEFILE_DEBUG
    $(info =============================configure.core.flags.make========================)
    $(info ---OF_CORE_INCLUDES_CFLAGS---)
    $(foreach v, $(OF_CORE_INCLUDES_CFLAGS),$(info $(v)))
#    $(info ---OF_CORE_STATIC_LIB_CFLAGS---)
#    $(foreach v, $(OF_CORE_STATIC_LIB_CFLAGS),$(info $(v)))
#    $(info ---OF_CORE_SHARED_LIB_CFLAGS---)
#    $(foreach v, $(OF_CORE_SHARED_LIB_CFLAGS),$(info $(v)))
#    $(info ---OF_CORE_PKG_CONFIG_LIB_CFLAGS---)
#    $(foreach v, $(OF_CORE_PKG_CONFIG_LIB_CFLAGS),$(info $(v)))
#    $(info ---OF_CORE_FRAMEWORKS_CFLAGS---)
#    $(foreach v, $(OF_CORE_FRAMEWORKS_CFLAGS),$(info $(v)))
endif

$(info hereherherere)
