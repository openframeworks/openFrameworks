# openFrameworks universal makefile
#
# make help : shows this message
# make Debug:  makes the application with debug symbols
# make Release: makes the app with optimizations
# make: the same as make Release
# make CleanDebug: cleans the Debug target
# make CleanRelease: cleans the Release target
# make clean: cleans everything
#
# edit the following  vars to customize the makefile

USER_CFLAGS = 


SHELL =	 /bin/sh

ARCH = $(shell uname -m)

COMPILER_OPTIMIZATION = -march=armv6 -mfpu=vfp -mfloat-abi=hard

LIBSPATH=linuxarmv6l

OF_COMPILED_LIB=../../lib/$(LIBSPATH)

# where is the libraries directory?
OF_LIBS_DIR=../../..

# where are the openFrameworks sources?
OF_SRC_LIB=$(OF_LIBS_DIR)/openFrameworks

$(info $(OF_SRC_LIB))

# add folders or single files to exclude fromt he compiled lib
#SOURCE_EXCLUSIONS+=/3d 
#SOURCE_EXCLUSIONS+=$(OF_SRC_LIB)/app 
SOURCE_EXCLUSIONS+=/app/ofAppGlutWindow.cpp 
#SOURCE_EXCLUSIONS+=/communication 
#SOURCE_EXCLUSIONS+=/events 
#SOURCE_EXCLUSIONS+=/gl 
SOURCE_EXCLUSIONS+=/gl/ofFbo.cpp
#SOURCE_EXCLUSIONS+=/graphics 
#SOURCE_EXCLUSIONS+=/math 
#SOURCE_EXCLUSIONS+=/sound 
#SOURCE_EXCLUSIONS+=/text 
#SOURCE_EXCLUSIONS+=/types 
#SOURCE_EXCLUSIONS+=/utils 
#SOURCE_EXCLUSIONS+=/video 

# strip and collapse spaces, and add the % suffix to allow directory matching
CLEANED_SOURCE_EXCLUSIONS=$(addprefix $(OF_SRC_LIB),$(addsuffix %,$(strip $(SOURCE_EXCLUSIONS))))

# find all of the source directories
UNFILTERED_SOURCE_DIRS=$(shell find $(OF_SRC_LIB) -maxdepth 1 -mindepth 1 -type d \( ! -name ".*" \) )

# filter out all excluded files / folders that were defined above
SOURCE_DIRS=$(filter-out $(CLEANED_SOURCE_EXCLUSIONS),$(UNFILTERED_SOURCE_DIRS))

# search the directories in the source folders for all .cpp files
UNFILTERED_SOURCES=$(shell find $(SOURCE_DIRS) -name "*.cpp")

# filter out all excluded files / folders that were defined above
SOURCES=$(filter-out $(CLEANED_SOURCE_EXCLUSIONS),$(UNFILTERED_SOURCES))

$(info --SOURCES TO COMPILE-- )
$(foreach v, $(SOURCES),$(info $(v)))

OBJFILES=$(patsubst $(OF_LIBS_DIR)/%.cpp,%.o,$(SOURCES))

#$(info --OBJFILES-- )
#$(foreach v, $(OBJFILES),$(info $(v)))

################################################

CORE_INCLUDE_FLAGS=$(addprefix -I,$(SOURCE_DIRS))

#$(info --CORE_INCLUDE_FLAGS-- )
#$(foreach v, $(CORE_INCLUDE_FLAGS),$(info $(v)))

# where is the libraries directory?
OF_LIBS_DIR=../../..

# where are the openFrameworks sources?
OF_SRC_LIB=$(OF_LIBS_DIR)/openFrameworks

$(info $(OF_SRC_LIB))

# add folders or single files to exclude fromt he compiled lib
UNFILTERED_INCLUDES = $(shell find $(OF_LIBS_DIR)/*/include -type d \( ! -name ".*" \))

INCLUDE_EXCLUSIONS+=/poco/include/Poco 
INCLUDE_EXCLUSIONS+=/poco/include/Poco/CppUnit 
INCLUDE_EXCLUSIONS+=/quicktime

# strip and collapse spaces, and add the % suffix to allow directory matching
CLEANED_INCLUDE_EXCLUSIONS=$(addprefix $(OF_LIBS_DIR),$(addsuffix %,$(strip $(INCLUDE_EXCLUSIONS))))

# filter out all excluded files / folders that were defined above
INCLUDES=$(filter-out $(CLEANED_INCLUDE_EXCLUSIONS),$(UNFILTERED_INCLUDES))

INCLUDES_FLAGS = $(addprefix -I,$(INCLUDES))

$(info --INCLUDES_FLAGS-- )
$(foreach v, $(INCLUDES_FLAGS),$(info $(v)))

INCLUDES_FLAGS += $(shell pkg-config  gstreamer-0.10 gstreamer-video-0.10 gstreamer-base-0.10 libudev --cflags)


# ADD THE INCLUDES TO THE CFLAGS
CFLAGS += -DTARGET_OPENGLES -DTARGET_OPENGLES2 -DTARGET_RASPBERRY_PI -DTARGET_NO_X11

CFLAGS += -Wall -fexceptions
CFLAGS += -L/opt/vc/lib -lGLESv2 -lEGL -lm -lbcm_host
#CFLAGS += -lglut
CFLAGS += $(INCLUDES_FLAGS)
CFLAGS += $(CORE_INCLUDE_FLAGS)



################################################

ifeq ($(findstring Debug,$(MAKECMDGOALS)),Debug)
	TARGET_CFLAGS = -g3
	TARGET_NAME = $(MAKECMDGOALS)
	TARGET = $(OF_COMPILED_LIB)/libopenFrameworksDebug.a
endif

ifeq ($(findstring Release,$(MAKECMDGOALS)),Release)
	TARGET_CFLAGS = $(COMPILER_OPTIMIZATION)
	TARGET_NAME = $(MAKECMDGOALS)
	TARGET = $(OF_COMPILED_LIB)/libopenFrameworks.a
endif

ifeq ($(MAKECMDGOALS),)
	TARGET_CFLAGS = $(COMPILER_OPTIMIZATION)
	TARGET_NAME = Release
	TARGET = $(OF_COMPILED_LIB)/libopenFrameworks.a
endif

ifeq ($(MAKECMDGOALS),clean)
	TARGET = $(OF_COMPILED_LIB)/libopenFrameworks.a $(OF_COMPILED_LIB)/libopenFrameworksDebug.a
endif


CLEANTARGET = $(addprefix Clean,$(TARGET_NAME))
OBJ_OUTPUT = obj/$(TARGET_NAME)/
DEPFILES = $(addprefix $(OBJ_OUTPUT),$(patsubst $(OF_LIBS_DIR)/%.cpp,%.d,$(SOURCES)))
OBJS = $(addprefix $(OBJ_OUTPUT), $(OBJFILES))

.PHONY: all Debug Release after clean CleanDebug CleanRelease help

Release: $(TARGET) 

Debug: $(TARGET) 

all: 
	$(MAKE) Debug
	$(MAKE) Release
	
#This rule does the compilation
$(OBJ_OUTPUT)%.o: ../../../%.cpp 
	@echo "compiling object for " $<
	mkdir -p $(@D)
	$(CXX) $(TARGET_CFLAGS) $(CFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.o -o $@ -c $<

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
	rm -Rf -v $(OBJ_OUTPUT)
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


