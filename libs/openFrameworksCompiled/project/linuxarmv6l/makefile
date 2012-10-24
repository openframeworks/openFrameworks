



##########################################################################################
# CORE SPACE SPACE
##########################################################################################

# setup the shell
SHELL =	 /bin/sh

ifeq ($(PLATFORM_ARCH),)
    # determine from the uname
    ARCH=$(shell uname -m)
else
    # obey the platform overrides
    ARCH=$(PLATFORM_ARCH)
endif

ifeq ($(PLATFORM_LIBS_PATH),)
    # determine from the arch
    LIBSPATH=$(ARCH)
else
    # obey the platform overrides
    LIBSPATH=$(PLATFORM_LIBS_PATH)
endif

# construct the relative path variables
OF_CORE_LIB_PATH=../../lib/$(LIBSPATH)
OF_LIBS_DIR=../../..
OF_SRC_LIB=$(OF_LIBS_DIR)/openFrameworks

# print the OF_SRC_LIB
#$(info $(OF_SRC_LIB))


# strip and collapse spaces, and add the % suffix to allow directory matching
FULLLY_QUALIFIED_CORE_SOURCE_EXCLUSIONS=$(addprefix $(OF_SRC_LIB),$(addsuffix %,$(strip $(CORE_SOURCE_EXCLUSIONS))))

# find all of the source directories, exclude all . (dot-hidden) files/folders
ALL_CORE_SOURCE_PATHS=$(shell find $(OF_SRC_LIB) -maxdepth 1 -mindepth 1 -type d \( ! -name ".*" \) )

#$(info --UNFILTERED_SOURCE_DIRS DIRS TO COMPILE-- )
#$(foreach v, $(UNFILTERED_SOURCE_DIRS),$(info $(v)))

# filter out all excluded files / folders that were defined above
# this list will be searched for cpp files below and will eventually
# be included as locations for header searches via 
FILTERED_CORE_SOURCE_PATHS=$(filter-out $(FULLLY_QUALIFIED_CORE_SOURCE_EXCLUSIONS),$(ALL_CORE_SOURCE_PATHS))

#$(info --SOURCE DIRS TO COMPILE-- )
#$(foreach v, $(SOURCE_DIRS),$(info $(v)))

# search the directories in the source folders for all .cpp files
ALL_CORE_SOURCES_FROM_FILTERED_SOURCE_PATHS=$(shell find $(FILTERED_CORE_SOURCE_PATHS) -name "*.cpp")

# filter out all excluded files / folders that were defined above
SOURCES=$(filter-out $(FULLLY_QUALIFIED_CORE_SOURCE_EXCLUSIONS),$(ALL_CORE_SOURCES_FROM_FILTERED_SOURCE_PATHS))

##########################################################################################
# OBJECT FILES
##########################################################################################

OBJ_OUTPUT_PATH = obj/$(TARGET_NAME)/
DEPFILES = $(addprefix $(OBJ_OUTPUT_PATH),$(patsubst $(OF_LIBS_DIR)/%.cpp,%.d,$(SOURCES)))
OBJS = $(addprefix $(OBJ_OUTPUT_PATH), $(OBJFILES))

$(info --SOURCES TO COMPILE-- )
$(foreach v, $(SOURCES),$(info $(v)))


#$(patsubst pattern,replacement,text)
#Finds whitespace-separated words in text that match pattern and replaces them with replacement
# this will remove the $(OF_LIBS_DIR) and produce
OBJFILES=$(patsubst $(OF_LIBS_DIR)/%.cpp,%.o,$(SOURCES))

$(info --OBJFILES-- )
$(foreach v, $(OBJFILES),$(info $(v)))

##########################################################################################
# COMPILER OPTIMIZATIONS
##########################################################################################



##########################################################################################
# DEFINES
##########################################################################################

# add any user defines to the core defines
CORE_DEFINES=
CORE_DEFINES+=$USER_DEFINES 

# bring all defines together, and add flags
DEFINES=$(addprefix -D,$(CORE_DEFINES));

$(info --DEFINES-- )
$(foreach v, $(DEFINES),$(info $(v)))

##########################################################################################
# INCLUDES
##########################################################################################

CORE_INCLUDE_PATHS=$(OF_SRC_LIB) $(SRC_DIRS) 

#$(info --CORE_INCLUDE_PATHS-- )
#$(foreach v, $(CORE_INCLUDE_PATHS),$(info $(v)))

# add folders or single files to exclude fromt he compiled lib
UNFILTERED_CORE_INCLUDES = $(shell find $(OF_LIBS_DIR)/*/include -type d \( ! -name ".*" \))

# strip and collapse spaces, and add the % suffix to allow directory matching
CLEANED_INCLUDE_EXCLUSIONS=$(addprefix $(OF_LIBS_DIR),$(addsuffix %,$(strip $(INCLUDE_EXCLUSIONS))))

# filter out all excluded files / folders that were defined above
INCLUDES=$(filter-out $(CLEANED_INCLUDE_EXCLUSIONS),$(UNFILTERED_INCLUDES))

INCLUDES_FLAGS = $(addprefix -I,$(INCLUDES))

$(info --INCLUDES_FLAGS-- )
$(foreach v, $(INCLUDES_FLAGS),$(info $(v)))

#INCLUDES_FLAGS += $(shell pkg-config  gstreamer-0.10 gstreamer-video-0.10 gstreamer-base-0.10 libudev --cflags)


# ADD THE INCLUDES TO THE CFLAGS

CFLAGS += -Wall -fexceptions
CFLAGS += $(INCLUDES_FLAGS)
CFLAGS += $(CORE_INCLUDE_FLAGS)
CFLAGS += $(USER_CFLAGS)


################################################

ifeq ($(findstring Debug,$(MAKECMDGOALS)),Debug)
	OPTIMIZATION_CFLAGS = -g3
	TARGET_NAME = $(MAKECMDGOALS)
	TARGET = $(OF_CORE_LIB_PATH)/libopenFrameworksDebug.a
endif

ifeq ($(findstring Release,$(MAKECMDGOALS)),Release)
	OPTIMIZATION_CFLAGS = $(COMPILER_OPTIMIZATION)
	TARGET_NAME = $(MAKECMDGOALS)
	TARGET = $(OF_CORE_LIB_PATH)/libopenFrameworks.a
endif

ifeq ($(MAKECMDGOALS),)
	OPTIMIZATION_CFLAGS = $(COMPILER_OPTIMIZATION)
	TARGET_NAME = Release
	TARGET = $(OF_CORE_LIB_PATH)/libopenFrameworks.a
endif

ifeq ($(MAKECMDGOALS),clean)
	TARGET = $(OF_CORE_LIB_PATH)/libopenFrameworks.a $(OF_CORE_LIB_PATH)/libopenFrameworksDebug.a
endif

CLEANTARGET = $(addprefix Clean,$(TARGET_NAME))

.PHONY: all Debug Release after clean CleanDebug CleanRelease help

Release: $(TARGET) 

Debug: $(TARGET) 

all: 
	$(MAKE) Debug
	$(MAKE) Release
	
#This rule does the compilation
$(OBJ_OUTPUT_PATH)%.o: ../../../%.cpp 
	@echo "compiling object for " $<
	mkdir -p $(@D)
	$(CXX) $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT_PATH)$*.d -MT$(OBJ_OUTPUT_PATH)$*.o -o $@ -c $<

$(TARGET) : $(OBJS) 
	echo "creating " $(TARGET)
	mkdir -p $(@D)
	$(AR) -r "$@" $(OBJS)

-include $(DEPFILES)

#.PHONY: clean CleanDebug CleanRelease
clean:
	rm -Rf obj
	rm -f -v $(TARGET)
	
$(CLEANTARGET):
	rm -Rf -v $(OBJ_OUTPUT_PATH)
	rm -f -v $(TARGET)
	
#.PHONY: help
help:
	@echo 
	@echo openFrameworks compiled library makefile
	@echo
	@echo targets:
	@echo "make Debug:		builds the library with debug symbols"
	@echo "make Release:		builds the library with optimizations"
	@echo "make:			= make Release"
	@echo "make all:		= make Debug + make Release"
	@echo "make CleanDebug: cleans the Debug target"
	@echo "make CleanRelease:	cleans the Release target"
	@echo "make clean:		cleans everything"
	@echo


