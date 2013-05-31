################################################################################
# compile.core.mk
################################################################################
#
#   This file contains all of the core compilation rules.  It organizes CFLAGS
#   LDFLAGS, dependency files, etc.
#
################################################################################

################################################################################
# .DEFAULT_GOAL
#   From: 
#       http://www.gnu.org/software/make/manual/html_node/Special-Variables.html
#
#   "Sets the default goal to be used if no targets were specified on the 
#   command line (see Arguments to Specify the Goals). The .DEFAULT_GOAL
#   variable allows you to discover the current default goal, restart the 
#   default goal selection algorithm by clearing its value, or to explicitly 
#   set the default goal."
#
################################################################################

.DEFAULT_GOAL=all

################################################################################
# PLATFORM_CXX (conditionally set)
# PROJECT_CXX (conditionally set)
# CXX (conditionally set)
#   *CXX varibles contain link to the CXX compiler.  If this variable is 
#   not defined, it is taken from the compiling platform's environmental 
#   variables.  PLATFORM_CXX overrides CXX and PROJECT_CXX and PLATFORM_CXX. 
################################################################################

ifdef PLATFORM_CXX
    CXX:=$(PLATFORM_CXX)
endif

ifdef PROJECT_CXX
    CXX:=$(PROJECT_CXX)
endif

################################################################################
# PLATFORM_CC (conditionally set)
# PROJECT_CC (conditionally set)
# CC (conditionally set)
#   *CC varibles contain link to the CC compiler.  If this variable is 
#   not defined, it is taken from the compiling platform's environmental 
#   variables.  PLATFORM_CC overrides CC and PROJECT_CC and PLATFORM_CC. 
################################################################################

ifdef PLATFORM_CC
    CC:=$(PLATFORM_CC)
endif

ifdef PROJECT_CC
    CC:=$(PROJECT_CC)
endif

################################################################################
# PLATFORM_AR (conditionally set)
# PROJECT_AR (conditionally set)
# AR (conditionally set)
#   *AR varibles contain link to the archiver.  If this variable is 
#   not defined, it is taken from the compiling platform's environmental 
#   variables.  PLATFORM_AR overrides AR and PROJECT_AR and PLATFORM_AR. 
################################################################################

ifdef PLATFORM_AR
    AR:=$(PLATFORM_AR)
endif

ifdef PROJECT_AR
    AR:=$(PROJECT_AR)
endif

################################################################################
# COLLECT ALL CFLAGS
################################################################################
# Collect all CFLAGS needed to compile the core openFrameworks static libraries.
################################################################################

################################################################################
# ALL_CFLAGS (immediately assigned)
#   Create a list of all CFLAGS, including those generated in the 
#   config.shared.mk file and (by extension) any included in the platform
#   specific configuration files.
#
#   See config.shared.mk for variable descriptions.
################################################################################

ALL_CFLAGS :=
ALL_CFLAGS += $(CORE_CFLAGS)
ALL_CFLAGS += $(CORE_DEFINES_CFLAGS)
ALL_CFLAGS += $(CORE_HEADER_SEARCH_PATHS_CFLAGS)

################################################################################
# CFLAGS (immediately assigned)
#   Create a list of all CFLAGS, stripping out any extra spaces
################################################################################

CFLAGS := $(strip $(ALL_CFLAGS))

################################################################################
# COMPILER OPTIMIZATIONS AND TARGET GENERATION
################################################################################

################################################################################
# TARGET (immediately assigned)
#   TARGET is the name of the library that will be created.  If TARGET has not
#   already been defined in the platfor-specific make file, we use the system
#   below to generate our TARGET variable. 
# TARGET_NAME (immesiately assigned)
#   TARGET_NAME is the name of the target defined for the use of the 
#   CLEANTARGET target. Platform specific targets should
#   define the following and respect the "CLEANTARGET system below."
################################################################################

# check to see if this is a "pure" clean target
ifeq ($(findstring clean,$(MAKECMDGOALS)),clean)
    TARGET :=

    ifdef PLATFORM_CORELIB_DEBUG_TARGET
        TARGET += $(PLATFORM_CORELIB_DEBUG_TARGET)
    else
        TARGET += $(PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED_LIB_PLATFORM_LIB_SUBPATH)/libopenFrameworksDebug.a
    endif
    
    ifdef PLATFORM_CORELIB_RELEASE_TARGET
        TARGET += $(PLATFORM_CORELIB_RELEASE_TARGET)
    else
        TARGET += $(PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED_LIB_PLATFORM_LIB_SUBPATH)/libopenFrameworks.a
    endif
    
    TARGET_NAME := 

# check to see if any part of our target includes the String "Debug"
# this will happen if we call Debug OR CleanDebug
else ifeq ($(findstring Debug,$(MAKECMDGOALS)),Debug)
    OPTIMIZATION_CFLAGS := $(PLATFORM_OPTIMIZATION_CFLAGS_DEBUG)
    TARGET_NAME := Debug
    ifdef PLATFORM_CORELIB_DEBUG_TARGET
        TARGET := $(PLATFORM_CORELIB_DEBUG_TARGET)
    else
        TARGET := $(PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED_LIB_PLATFORM_LIB_SUBPATH)/libopenFrameworksDebug.a
    endif

# check to see if any part of our target includes the String "Release"
# this will happen if we call Release OR CleanRelease
else ifeq ($(findstring Release,$(MAKECMDGOALS)),Release)
    OPTIMIZATION_CFLAGS = $(PLATFORM_OPTIMIZATION_CFLAGS_RELEASE)
    TARGET_NAME = Release
    ifdef PLATFORM_CORELIB_RELEASE_TARGET
        TARGET := $(PLATFORM_CORELIB_RELEASE_TARGET)
    else
        TARGET := $(PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED_LIB_PLATFORM_LIB_SUBPATH)/libopenFrameworks.a
    endif
    
else ifeq ($(MAKECMDGOALS),after)
    TARGET :=
    TARGET_NAME := 
    
else ## why doesn't make allow for easy logical operators?
    OPTIMIZATION_CFLAGS := $(PLATFORM_OPTIMIZATION_CFLAGS_RELEASE)
    TARGET_NAME := Release
    ifdef PLATFORM_CORELIB_RELEASE_TARGET
        TARGET += $(PLATFORM_CORELIB_RELEASE_TARGET)
    else
        TARGET += $(PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED_LIB_PLATFORM_LIB_SUBPATH)/libopenFrameworks.a
    endif
 endif

# we only get a CLEAN_TARGET if a TARGET_NAME has been defined
# Like TARGET, this must be defined above or in a platform file.
ifdef TARGET_NAME
    CLEANTARGET := $(addprefix Clean,$(TARGET_NAME))
endif

ABIS_TO_COMPILE :=

# look for the Release tag in the TARGET_NAME, and if we find it, add it to our
# ABIs to compile list.
ifeq ($(findstring Release,$(TARGET_NAME)),Release)
    ifdef ABIS_TO_COMPILE_RELEASE
        ABIS_TO_COMPILE += $(ABIS_TO_COMPILE_RELEASE)
    endif
endif

# look for the Debug tag in the TARGET_NAME, and if we find it, add it to our
# ABIs to compile list.
ifeq ($(findstring Debug,$(TARGET_NAME)),Debug)
    ifdef ABIS_TO_COMPILE_DEBUG
        ifeq ($(findstring Release,$(TARGET_NAME)),Release)
            ifdef ABIS_TO_COMPILE_RELEASE
                ABIS_TO_COMPILE = $(filter-out $(ABIS_TO_COMPILE_DEBUG),$(ABIS_TO_COMPILE_RELEASE))
            endif
        endif
        ABIS_TO_COMPILE += $(ABIS_TO_COMPILE_DEBUG)
    endif
endif

################################################################################
# CORE OBJECT AND DEPENDENCY FILES DEFINITIONS
################################################################################
# Object file paths are generated here (as opposed to with the rest of the 
# flags) because we want to place them in target-specific folders. We
# determine targets above. We _could_ determine the target info earlier if we
# wanted to.  It's here because that's approximately where it was in the 
# legacy makefiles.
################################################################################

################################################################################
# PATH_CORE_OBJ_OUPUT
#   This is the path where the compiler-generated object files generated
#   from the core source will end up.  These files will later be used 
#   when linking the static core libraries.  The location depends on the 
#   platform and the number and type of ABIs being compiled simultaneously.
################################################################################

ifdef ABI
    PATH_CORE_OBJ_OUPUT = $(PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED_LIB_PLATFORM_LIB_SUBPATH)/obj/$(ABI)/$(TARGET_NAME)/
else
    PATH_CORE_OBJ_OUPUT = $(PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED_LIB_PLATFORM_LIB_SUBPATH)/obj/$(TARGET_NAME)/
endif

################################################################################
# OF_CORE_DEPENDENCY_FILES (set immediately)
#   Create a list of dependency (*.d) files for the compiler. These will be
#   stored in the PATH_CORE_OBJ_OUTPUT directory. 
#
# Steps:
#
# 1. Create a list of .d dependency files based on the current list of source
#    files by substituting their paths with the new path.
#     $(patsubst $(PATH_OF_ROOT)/%.cpp,%.d,\
#       $(patsubst $(PATH_OF_ROOT)/%.mm,%.d,\
#        $(patsubst $(PATH_OF_ROOT)/%.m,%.d,\
#           $(CORE_SOURCE_FILES)\
#         )\
#       )\
#     )\
# 2. Prefix everything with the PATH_CORE_OBJ_OUTPUT
#     $(addprefix $(PATH_CORE_OBJ_OUPUT),\ ...
#
################################################################################

OF_CORE_DEPENDENCY_FILES = $(addprefix $(PATH_CORE_OBJ_OUPUT),\
                                         $(patsubst $(PATH_OF_ROOT)/%.cpp,%.d,\
                                           $(patsubst $(PATH_OF_ROOT)/%.mm,%.d,\
                                             $(patsubst $(PATH_OF_ROOT)/%.m,%.d,\
                                               $(CORE_SOURCE_FILES)\
                                             )\
                                           )\
                                         )\
                                       )

################################################################################
# OF_CORE_OBJ_FILES (set immediately)
#   Create a list of object (*.o) files for the compiler. These will be
#   stored in the PATH_CORE_OBJ_OUTPUT directory. 
#
# Steps:
#
# 1. Create a list of .o object files based on the current list of source
#    files by substituting their paths with the new path.
#      $(patsubst $(PATH_OF_ROOT)/%.cpp,%.o,\
#       $(patsubst $(PATH_OF_ROOT)/%.mm,%.o,\
#         $(patsubst $(PATH_OF_ROOT)/%.m,%.o,\
#           $(CORE_SOURCE_FILES)\
#         )\
#       )\
#     )\
# 2. Prefix everything with the PATH_CORE_OBJ_OUTPUT
#     $(addprefix $(PATH_CORE_OBJ_OUPUT),\ ...
#
################################################################################

OF_CORE_OBJ_FILES = $(addprefix $(PATH_CORE_OBJ_OUPUT),\
                                  $(patsubst $(PATH_OF_ROOT)/%.cpp,%.o,\
                                     $(patsubst $(PATH_OF_ROOT)/%.mm,%.o,\
                                       $(patsubst $(PATH_OF_ROOT)/%.m,%.o,\
                                         $(CORE_SOURCE_FILES)\
                                       )\
                                     )\
                                  )\
                                )
    
################################################################################
# DEBUG ########################################################################
################################################################################
ifdef MAKEFILE_DEBUG
    $(info ========================= compile.core.make flags ==================)
    $(info PATH_CORE_OBJ_OUPUT=$(PATH_CORE_OBJ_OUPUT))
    
    $(info ---OF_CORE_DEPENDENCY_FILES---)
    $(foreach v, $(OF_CORE_DEPENDENCY_FILES),$(info $(v)))
     
    $(info ---OF_CORE_OBJ_FILES---)
    $(foreach v, $(OF_CORE_OBJ_FILES),$(info $(v)))

    $(info )
endif
################################################################################

################################################################################
# MAKE TARGETS
################################################################################

################################################################################
# .PHONY
#   While most MAKE targets respond to lists of filenames, .PHONY targets are 
#   targets that are "recipe" only -- that is recipes that respond to specific
#   requests, not filenames or lists of filenames.  .PNONY targets are used to 
#   avoid conflict with files of the same name and to improve performance.
################################################################################

.PHONY: Release \
        Debug \
        ReleaseABI \
        DebugABI \
        CleanDebug \
        CleanRelease \
        CleanDebugABI \
        CleanReleaseABI \
        all \
        after \
        clean \
        help

Release:
ifndef ABIS_TO_COMPILE_RELEASE
    # if we just have one ABI to compile, then we call the RealeaseABI target
	@$(MAKE) --no-print-directory ReleaseABI
else
    # if we just have more than one ABI to compile, 
    # then we call each item in our ABIS_TO_COMPILE_RELEASE list
	@$(foreach abi,$(ABIS_TO_COMPILE_RELEASE),$(MAKE) --no-print-directory ReleaseABI ABI=$(abi) &&) echo 
endif
    # call the after target (see below for more)
	@$(MAKE) --no-print-directory after 
	
Debug:
ifndef ABIS_TO_COMPILE_DEBUG
    # if we just have one ABI to compile, then we call the DebugABI target
	@$(MAKE) --no-print-directory DebugABI
else
    # if we just have more than one ABI to compile, 
    # then we call each item in our ABIS_TO_COMPILE_DEBUG list
	@$(foreach abi,$(ABIS_TO_COMPILE_DEBUG),$(MAKE) --no-print-directory DebugABI ABI=$(abi) &&) echo 
endif
    # call the after target (see below for more)
	@$(MAKE) --no-print-directory after 

# The ReleaseABI target (above) will pass the library name 
# (i.e. ... libopenFrameworks.a) down the the @(TARGET) target
ReleaseABI: $(TARGET)

# The DebugABI target (above) will pass the library name 
# (i.e. ... libopenFrameworksDebug.a) down the the @(TARGET) target
DebugABI: $(TARGET)
	
# all will first run the debug target, then the release target
all: 
	# will call Debug: and Release: above
	@$(MAKE) --no-print-directory Debug
	@$(MAKE) --no-print-directory Release

# This rule compiles any items that match *.cpp
# If the object directory doesn't exist, it is created.
$(PATH_CORE_OBJ_OUPUT)%.o: $(PATH_OF_ROOT)/%.cpp 
	@echo "Compiling *.cpp" $<
	mkdir -p $(@D)
	$(CXX) $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(PATH_CORE_OBJ_OUPUT)$*.d -MT$(PATH_CORE_OBJ_OUPUT)$*.o -o $@ -c $<

# This rule compiles any items that match *.mm
# If the object directory doesn't exist, it is created.
$(PATH_CORE_OBJ_OUPUT)%.o: $(PATH_OF_ROOT)/%.mm
	@echo "Compiling *.mm " $<
	mkdir -p $(@D)
	$(CXX) $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(PATH_CORE_OBJ_OUPUT)$*.d -MT$(PATH_CORE_OBJ_OUPUT)$*.o -o $@ -c $<

# This rule compiles any items that match *.m
# If the object directory doesn't exist, it is created.
$(PATH_CORE_OBJ_OUPUT)%.o: $(PATH_OF_ROOT)/%.m
	@echo "Compiling *.m  " $<
	mkdir -p $(@D)
	$(CC) $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(PATH_CORE_OBJ_OUPUT)$*.d -MT$(PATH_CORE_OBJ_OUPUT)$*.o -o $@ -c $<

# This target does the linking of the library
# $(TARGET) : $(OF_CORE_OBJ_FILES) means that each of the items in the 
# $(OF_CORE_OBJ_FILES) must be processed first.
$(TARGET) : $(OF_CORE_OBJ_FILES) 
	@echo "Creating library " $(TARGET)
	mkdir -p $(@D)
	$(AR) -cr "$@" $(OF_CORE_OBJ_FILES)

-include $(OF_CORE_DEPENDENCY_FILES)

# Calls the $(CLEANTARGET) target below.
clean:
	$(MAKE) CleanRelease
	$(MAKE) CleanDebug

$(CLEANTARGET)ABI:
	@echo "Removing object files in " $(PATH_CORE_OBJ_OUPUT)
	rm -Rf $(PATH_CORE_OBJ_OUPUT)
	@echo "Removing " $(TARGET)
	rm -f $(TARGET)
	
# Receives the CleanTarget and passes it to $(CLEANTARGET)ABI above
$(CLEANTARGET):
ifndef ABIS_TO_COMPILE
	$(MAKE) --no-print-directory $(CLEANTARGET)ABI
else
ifeq ($(TARGET_NAME),Debug)
	@$(foreach abi,$(ABIS_TO_COMPILE_DEBUG),$(MAKE) --no-print-directory $(CLEANTARGET)ABI ABI=$(abi) &&) echo done
else
	@$(foreach abi,$(ABIS_TO_COMPILE_RELEASE),$(MAKE) --no-print-directory $(CLEANTARGET)ABI ABI=$(abi) &&) echo done
endif
endif

# The after target, called after normal compilation.
after: $(TARGET)
	@echo "Done!"

# Print help target
help:
# TODO: update this help file
	@echo 
	@echo openFrameworks compiled library makefile
	@echo
	@echo "Targets:"
	@echo
	@echo "make Debug:		builds the library with debug symbols"
	@echo "make Release:		builds the library with optimizations"
	@echo "make:			= make all"
	@echo "make all:		= make Debug + make Release"
	@echo "make CleanDebug:		cleans the Debug target"
	@echo "make CleanRelease:	cleans the Release target"
	@echo "make clean:		cleans everything"
	@echo "make help:		this help message"
	@echo 
	@echo "Platform OS"
	@echo 
	@echo "Currently the only platform that needs to specify the OS is"
	@echo "android. This is already setup in the eclipse porjects but"
	@echo "the correct parameter is:"
	@echo 
	@echo "e.g."
	@echo
	@echo "make Release PLATFORM_OS=Android"
	@echo
	@echo "Platform Variants:"
	@echo
	@echo "openFrameworks makefiles can be customized for a generic platform"
	@echo "(i.e. Linux or Darwin) and can be further customized with a"
	@echo "platform variant make files. To use a platform variant makefile"
	@echo "set the PLATFORM_VARIANT variable to a valid variant."
	@echo 
	@echo "e.g."
	@echo 
	@echo "make Release PLATFORM_VARIANT=raspberrypi"
	@echo
	@echo "In this case, the makefile will automatically determine architecture"
	@echo "architecture and operating system, and will use the raspberrypi"
	@echo "makefile variant rather than the \"default\" platform variant."
	@echo
	@echo "Debugging:"
	@echo
	@echo "openFrameworks makefiles offer a lot of debugging information"
	@echo "for the curious developer.  To see debugging output define the"
	@echo "MAKEFILE_DEBUG variable."
	@echo
	@echo "e.g."
	@echo
	@echo "make Release MAKEFILE_DEBUG=true"
	@echo
	@echo
