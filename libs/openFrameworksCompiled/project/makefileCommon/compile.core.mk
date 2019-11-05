.DEFAULT_GOAL=all

################################################################################
ifdef MAKEFILE_DEBUG
    $(info ===================compile.core.make================================)
endif

ifdef PROJECT_CXX
	CXX ?= $(PROJECT_CXX)
endif

ifdef PLATFORM_CXX
	CXX ?= $(PLATFORM_CXX)
endif

ifdef PROJECT_CC
	CC ?= $(PROJECT_CC)
endif

ifdef PLATFORM_CC
	CC ?= $(PLATFORM_CC)
endif

ifdef PROJECT_AR
	AR ?= $(PROJECT_AR)
endif

ifdef PLATFORM_AR
	AR ?= $(PLATFORM_AR)
endif

ifdef PLATFORM_ARFLAGS
	ARFLAGS = $(PLATFORM_ARFLAGS)
else
	ARFLAGS = -cr
endif

################################################################################
# CFLAGS
################################################################################

# clean it
ALL_CFLAGS =

# add the core flags (platform flags are aggregated in here)
ALL_CFLAGS += $(OF_CORE_BASE_CFLAGS)
# add the defines
ALL_CFLAGS += $(OF_CORE_DEFINES_CFLAGS)
# add the include cflags
ALL_CFLAGS += $(OF_CORE_INCLUDES_CFLAGS)
# clean up all extra whitespaces in the CFLAGS
CFLAGS += $(strip $(ALL_CFLAGS))
CXXFLAGS += $(OF_CORE_BASE_CXXFLAGS)

################################################################################
# COMPILER OPTIMIZATIONS and TARGET GENERATION
################################################################################
#
# $(TARGET_NAME) is the name of the target defined for the use of the
# CLEANTARGET target. $(TARGET) is the name of the library that will be created.
#
# If TARGET has not already been defined in a platform-based make file, we use
# this selection system to define our TARGET.  Platform specific targets should
# define the following and respect the "CLEANTARGET system below."
#
#  $(TARGET)
#  $(TARGET_NAME)
#  #(OPTIMIZATION_CFLAGS)
#

# check to see if this is a "pure" clean target
ifeq ($(findstring clean,$(MAKECMDGOALS)),clean)
	TARGET =
	ifdef PLATFORM_CORELIB_DEBUG_TARGET
		TARGET += $(PLATFORM_CORELIB_DEBUG_TARGET)
	else
		TARGET += $(OF_CORE_LIB_PATH)/libopenFrameworksDebug.a
	endif

	ifdef PLATFORM_CORELIB_RELEASE_TARGET
		TARGET += $(PLATFORM_CORELIB_RELEASE_TARGET)
	else
		TARGET += $(OF_CORE_LIB_PATH)/libopenFrameworks.a
	endif

	TARGET_NAME =

# check to see if any part of our target includes the String "Debug"
# this will happen if we call Debug OR CleanDebug
else ifeq ($(findstring Debug,$(MAKECMDGOALS)),Debug)
	OPTIMIZATION_CFLAGS = $(PLATFORM_OPTIMIZATION_CFLAGS_DEBUG) -DDEBUG
	TARGET_NAME = Debug
	ifdef PLATFORM_CORELIB_DEBUG_TARGET
		TARGET = $(PLATFORM_CORELIB_DEBUG_TARGET)
	else
		TARGET = $(OF_CORE_LIB_PATH)/libopenFrameworksDebug.a
	endif

# check to see if any part of our target includes the String "Release"
# this will happen if we call Release OR CleanRelease
else ifeq ($(findstring Release,$(MAKECMDGOALS)),Release)
	OPTIMIZATION_CFLAGS = $(PLATFORM_OPTIMIZATION_CFLAGS_RELEASE) -DNDEBUG
	TARGET_NAME = Release
	ifdef PLATFORM_CORELIB_RELEASE_TARGET
		TARGET = $(PLATFORM_CORELIB_RELEASE_TARGET)
	else
		TARGET = $(OF_CORE_LIB_PATH)/libopenFrameworks.a
	endif

else ifeq ($(MAKECMDGOALS),after)
	TARGET =
	TARGET_NAME =

else ## why doesn't make allow for easy logical operators?
	OPTIMIZATION_CFLAGS = $(PLATFORM_OPTIMIZATION_CFLAGS_RELEASE) -DNDEBUG
	TARGET_NAME = Release
	ifdef PLATFORM_CORELIB_RELEASE_TARGET
		TARGET += $(PLATFORM_CORELIB_RELEASE_TARGET)
	else
		TARGET += $(OF_CORE_LIB_PATH)/libopenFrameworks.a
	endif
 endif

# we only get a CLEAN_TARGET if a TARGET_NAME has been defined
# Like TARGET, this must be defined above or in a platform file.
ifdef TARGET_NAME
	CLEANTARGET = $(addprefix Clean,$(TARGET_NAME))
endif

ABIS_TO_COMPILE =

ifeq ($(findstring Release,$(TARGET_NAME)),Release)
	ifdef ABIS_TO_COMPILE_RELEASE
		ABIS_TO_COMPILE += $(ABIS_TO_COMPILE_RELEASE)
	endif
endif

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
#	Object file paths are generated here (as opposed to with the rest of the
#   flags) because we want to place them in target-specific folders. We
#   determine targets above. We –could– determine the target info earlier if we
#   wanted to.  It's here because that's approximately where it was in the
#   legacy makefiles.
################################################################################

# define the subdirectory for our target name
ifdef ABI
	OF_CORE_OBJ_OUTPUT_PATH = $(OF_CORE_LIB_PATH)/obj/$(ABI)/$(TARGET_NAME)/
else
	OF_CORE_OBJ_OUTPUT_PATH = $(OF_CORE_LIB_PATH)/obj/$(TARGET_NAME)/
endif

# create a named list of dependency files
# 1. create a list of .d dependency files based on the current list of
#  OF_CORE_SOURCE_FILES $(patsubst $(OF_ROOT)/%.cpp,%.d,$(OF_CORE_SOURCE_FILES))
# 2. Add the OF_CORE_OBJ_OUTPUT_PATH as a prefix
#  $(addprefix $(OF_CORE_OBJ_OUTPUT_PATH), ...)
OF_CORE_DEPENDENCY_FILES = $(addprefix $(OF_CORE_OBJ_OUTPUT_PATH),$(patsubst $(OF_ROOT)/%.cpp,%.d,$(patsubst $(OF_ROOT)/%.mm,%.d,$(patsubst $(OF_ROOT)/%.m,%.d,$(OF_CORE_SOURCE_FILES)))))

# create a named list of object files
# 1. create a list of object files based on the current list of
#   OF_CORE_SOURCE_FILES $(patsubst $(OF_ROOT)/%.cpp,%.o,$(OF_CORE_SOURCE_FILES)
# 2. Add the OF_CORE_OBJ_OUTPUT_PATH as a prefix
#	$(addprefix $(OF_CORE_OBJ_OUTPUT_PATH), ...)
OF_CORE_OBJ_FILES = $(addprefix $(OF_CORE_OBJ_OUTPUT_PATH),$(patsubst $(OF_ROOT)/%.cpp,%.o,$(patsubst $(OF_ROOT)/%.mm,%.o,$(patsubst $(OF_ROOT)/%.m,%.o,$(OF_CORE_SOURCE_FILES)))))


################################################################################
# DEBUG INFO
################################################################################
ifdef MAKEFILE_DEBUG
    $(info ========================= compile.core.make flags ========================)
    $(info OF_CORE_OBJ_OUTPUT_PATH=$(OF_CORE_OBJ_OUTPUT_PATH))

    $(info ---OF_CORE_DEPENDENCY_FILES---)
    $(foreach v, $(OF_CORE_DEPENDENCY_FILES),$(info $(v)))

    $(info ---OF_CORE_OBJ_FILES---)
    $(foreach v, $(OF_CORE_OBJ_FILES),$(info $(v)))
endif


################################################################################
# While most MAKE targets respond to lists of filenames, .PHONY targets are
# targets that are "recipe" only -- that is recipes that respond to specific
# requests, not filenames or lists of filenames.  .PNONY targets are used to
# avoid conflict with files of the same name and to improve performance.
.PHONY: all Debug Release after clean CleanDebug CleanRelease help force

Release:
ifndef ABIS_TO_COMPILE_RELEASE
	@$(MAKE) --no-print-directory ReleaseABI
else
	@$(foreach abi,$(ABIS_TO_COMPILE_RELEASE),$(MAKE) --no-print-directory ReleaseABI ABI=$(abi) &&) echo
endif
	@$(MAKE) --no-print-directory after

Debug:
ifndef ABIS_TO_COMPILE_DEBUG
	@$(MAKE) --no-print-directory DebugABI
else
	@$(foreach abi,$(ABIS_TO_COMPILE_DEBUG),$(MAKE) --no-print-directory DebugABI ABI=$(abi) &&) echo
endif
	@$(MAKE) --no-print-directory after

# Release will pass the library name (i.e. ... libopenFrameworks.a)
# down the the @(TARGET) target
ReleaseABI: $(TARGET)

# Debug will pass the library name (i.e. ... libopenFrameworksDebug.a)
# down the the @(TARGET) target
DebugABI: $(TARGET)

# all will first run the debug target, then the release target
all:
	@$(MAKE) --no-print-directory Debug
	@$(MAKE) --no-print-directory Release

$(OF_CORE_OBJ_OUTPUT_PATH).compiler_flags: force
	@mkdir -p $(OF_CORE_OBJ_OUTPUT_PATH)
	@if [ "$(strip $(OPTIMIZATION_CFLAGS) $(CFLAGS) $(CXXFLAGS))" != "$(strip $$(cat $@ 2>/dev/null))" ]; then echo "Compiler flags have changed, recompiling"; echo "Old: $(strip $$(cat $@ 2>/dev/null))"; echo "New $(strip $(OPTIMIZATION_CFLAGS) $(CFLAGS) $(CXXFLAGS))"; echo $(strip $(OPTIMIZATION_CFLAGS) $(CFLAGS) $(CXXFLAGS))> $@; fi


#This rule does the compilation
$(OF_CORE_OBJ_OUTPUT_PATH)%.o: $(OF_ROOT)/%.cpp $(OF_CORE_OBJ_OUTPUT_PATH).compiler_flags
	@echo "Compiling" $<
	@mkdir -p $(@D)
	$(CXX) $(OPTIMIZATION_CFLAGS) $(CFLAGS) $(CXXFLAGS) -MMD -MP -MF $(OF_CORE_OBJ_OUTPUT_PATH)$*.d -MT$(OF_CORE_OBJ_OUTPUT_PATH)$*.o -o $@ -c $<

$(OF_CORE_OBJ_OUTPUT_PATH)%.o: $(OF_ROOT)/%.mm $(OF_CORE_OBJ_OUTPUT_PATH).compiler_flags
	@echo "Compiling" $<
	@mkdir -p $(@D)
	$(CXX) $(OPTIMIZATION_CFLAGS) $(CFLAGS) $(CXXFLAGS) -MMD -MP -MF $(OF_CORE_OBJ_OUTPUT_PATH)$*.d -MT$(OF_CORE_OBJ_OUTPUT_PATH)$*.o -o $@ -c $<

$(OF_CORE_OBJ_OUTPUT_PATH)%.o: $(OF_ROOT)/%.m $(OF_CORE_OBJ_OUTPUT_PATH).compiler_flags
	@echo "Compiling" $<
	@mkdir -p $(@D)
	$(CC) $(OPTIMIZATION_CFLAGS) $(CFLAGS) $(CXXFLAGS) -MMD -MP -MF $(OF_CORE_OBJ_OUTPUT_PATH)$*.d -MT$(OF_CORE_OBJ_OUTPUT_PATH)$*.o -o $@ -c $<

# this target does the linking of the library
# $(TARGET) : $(OF_CORE_OBJ_FILES) means that each of the items in the
# $(OF_CORE_OBJ_FILES) must be processed first
ifeq ($(SHAREDCORE),1)
$(TARGET) : $(OF_CORE_OBJ_FILES) $(OF_CORE_OBJ_OUTPUT_PATH).compiler_flags
	@echo "Creating library " $(TARGET)
	@mkdir -p $(@D)
	$(CC) -shared $(OF_CORE_OBJ_FILES) -o $@
else ifeq ($(BYTECODECORE),1)
$(TARGET) : $(OF_CORE_OBJ_FILES) $(OF_CORE_OBJ_OUTPUT_PATH).compiler_flags
	@echo "Creating library " $(TARGET)
	@mkdir -p $(@D)
	$(CC) $(OF_CORE_OBJ_FILES) -o $@
else
$(TARGET) : $(OF_CORE_OBJ_FILES) $(OF_CORE_OBJ_OUTPUT_PATH).compiler_flags
	@echo "Creating library " $(TARGET)
	@mkdir -p $(@D)
	$(AR) ${ARFLAGS} "$@" $(OF_CORE_OBJ_FILES)
endif
-include $(OF_CORE_DEPENDENCY_FILES)

#.PHONY: clean CleanDebug CleanRelease


clean:
	$(MAKE) CleanRelease
	$(MAKE) CleanDebug

$(CLEANTARGET)ABI:
	@echo "Removing object files in " $(OF_CORE_OBJ_OUTPUT_PATH)
	rm -Rf $(OF_CORE_OBJ_OUTPUT_PATH)
	@echo "Removing " $(TARGET)
	rm -f $(TARGET)

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

after: $(TARGET)
	@echo "Done!"


help:
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
