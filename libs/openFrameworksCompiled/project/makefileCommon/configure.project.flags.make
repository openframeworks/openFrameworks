# gather all of the addons flags
include $(OF_SHARED_MAKEFILES_PATH)/configure.project.addons.make

################################################################################
# CLEAN PROJECT EXCLUSIONS 
################################################################################

OF_PROJECT_EXCLUSIONS := $(strip $(PROJECT_EXCLUSIONS))

################################################################################
# PROJECT SOURCE FILES
################################################################################

# exclude all items from source according to the config.make file

# create a list of all dirs in the project root that might be valid project
# source directories 
# grep -v "/\.[^\.]" will exclude all .hidden folders and files
ALL_OF_PROJECT_SOURCE_PATHS = $(shell find $(PROJECT_ROOT) -maxdepth 1 -mindepth 1 -type d | grep -v "/\.[^\.]")

# add all external source paths
#ALL_OF_OF_PROJECT_SOURCE_PATHS += $(PROJECT_EXTERNAL_SOURCE_PATHS)

# filter out all excluded files / folders that were defined above
# this list will be searched for cpp files below and will eventually
# be included as locations for header searches via 
OF_PROJECT_SOURCE_PATHS = $(filter-out $(OF_PROJECT_EXCLUSIONS),$(ALL_OF_PROJECT_SOURCE_PATHS))

# find all sources inside the project's source directory (recursively)
# grep -v "/\.[^\.]" will exclude all .hidden folders and files
OF_PROJECT_SOURCE_FILES = $(shell find $(OF_PROJECT_SOURCE_PATHS) -name "*.cpp" -or -name "*.c" -or -name "*.cc" -or -name "*.cxx" | grep -v "/\.[^\.]")

################################################################################
# PROJECT HEADER INCLUDES (-I ...)
################################################################################

OF_PROJECT_INCLUDES := $(OF_PROJECT_SOURCE_PATHS)
OF_PROJECT_INCLUDES += $(PROJECT_ADDONS_INCLUDES)

OF_PROJECT_INCLUDES_CFLAGS = $(addprefix -I,$(OF_PROJECT_INCLUDES))

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
OF_PROJECT_CFLAGS += $(OF_PROJECT_DEFINES_CFLAGS)
OF_PROJECT_CFLAGS += $(OF_PROJECT_INCLUDE_CFLAGS)

################################################################################
# PROJECT LDLAGS
################################################################################

OF_PROJECT_LDFLAGS := $(PROJECT_LDFLAGS)
OF_PROJECT_LDFLAGS += $(OF_PROJECT_LIBS_LDFLAGS)
