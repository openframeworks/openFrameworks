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

.DEFAULT_GOAL=Release

################################################################################
# APPNAME (conditionally set)
#   The name of the executable.  If not set previously, set it to the name of
#   the current project directory.
################################################################################

ifndef APPNAME
    APPNAME:=$(shell basename `pwd`)
endif

################################################################################
# TARGET_NAME
#   This is the name of the make target that is used by the makefile 
#   system during compilation.
# TARGET
#   This is the name of the target file itself.  If available, the 
#   TARGET will be build from the PLATFORM_PROJECT_DEBUG_TARGET specified in 
#   the platform specific config file OR the PLATFORM_PROJECT_RELEASSE_TARGET
#   from the same file. 
# RUN_TARGET
#   This is the make target used when the user launches the final program
#   with a make target such as "make run". 
# BIN_NAME
#   This is the final executable binary name.  In the case of of osx, this is
#   the name of the final app bundle. If available, the BIN_NAME will be
#   built from the PLATFORM_PROJECT_DEBUG_BIN_NAME specified in the 
#   platform specific config file OR the PLATFORM_PROJECT_RELEASE_BIN_NAME
#   from the same file.
#################################################################################

# Name TARGET - Are we using a Debug target?
ifeq ($(findstring Debug,$(MAKECMDGOALS)),Debug)
    TARGET_NAME:=Debug

    ifndef RUN_TARGET
        RUN_TARGET:=RunDebug
    endif

    ifndef PLATFORM_PROJECT_DEBUG_TARGET
        TARGET:=bin/$(APPNAME)_debug
    else
        TARGET:=$(PLATFORM_PROJECT_DEBUG_TARGET)
    endif

    ifndef PLATFORM_PROJECT_DEBUG_BIN_NAME
        BIN_NAME:=$(APPNAME)_debug
    else
        BIN_NAME:=$(PLATFORM_PROJECT_DEBUG_BIN_NAME)
    endif
# Else, are we using a Release target?
else ifeq ($(findstring Release,$(MAKECMDGOALS)),Release)
    TARGET_NAME:=Release

    ifndef RUN_TARGET
        RUN_TARGET:=RunRelease
    endif

    ifndef PLATFORM_PROJECT_RELEASE_TARGET
        TARGET:=bin/$(APPNAME)
    else
        TARGET:=$(PLATFORM_PROJECT_RELEASE_TARGET)
    endif

    ifndef PLATFORM_PROJECT_RELEASE_BIN_NAME
        BIN_NAME:=$(APPNAME)
    else
        BIN_NAME:=$(PLATFORM_PROJECT_RELEASE_BIN_NAME)
    endif
# Else are we using a run target?
else ifeq ($(MAKECMDGOALS),run)
    TARGET_NAME:=Release
    ifndef PLATFORM_PROJECT_RELEASE_TARGET
        TARGET:=bin/$(APPNAME)
    else
        TARGET:=$(PLATFORM_PROJECT_RELEASE_TARGET)
    endif
    ifndef PLATFORM_PROJECT_RELEASE_BIN_NAME
        BIN_NAME:=$(APPNAME)
    else
        BIN_NAME:=$(PLATFORM_PROJECT_RELEASE_BIN_NAME)
    endif

# Else we default to Release and set defaults.
else ifeq ($(MAKECMDGOALS),)
    TARGET_NAME:=Release

    ifndef RUN_TARGET
        RUN_TARGET:=run
    endif

    ifndef PLATFORM_PROJECT_RELEASE_TARGET
        TARGET:=bin/$(APPNAME)
    else
        TARGET:=$(PLATFORM_PROJECT_RELEASE_TARGET)
    endif

    ifndef PLATFORM_PROJECT_RELEASE_BIN_NAME
        BIN_NAME:=$(APPNAME)
    else
        BIN_NAME:=$(PLATFORM_PROJECT_RELEASE_BIN_NAME)
    endif
endif

################################################################################
# ABIS_TO_COMPILE
#   Just as in the core, we sometimes need to simultaneously compile multiple
#   ABIs.  The ABIS_TO_COMPILE variable contains a list of the ABIs to compile.
#   This list is built from the platform specific build file.
################################################################################

ABIS_TO_COMPILE =

# If we are using a Release target ...
ifeq ($(findstring Release,$(TARGET_NAME)),Release)
    # ... and we have defined ABIS_TO_COMPILE_RELEASE in our platform-config 
    ifdef ABIS_TO_COMPILE_RELEASE
        # add it to our list.
        ABIS_TO_COMPILE += $(ABIS_TO_COMPILE_RELEASE)
    endif
endif

# If we are using a Debug target ...
ifeq ($(findstring Debug,$(TARGET_NAME)),Debug)
    # ... and we have defined ABIS_TO_COMPILE_DEBUG in our platform-config 
    ifdef ABIS_TO_COMPILE_DEBUG
        ifeq ($(findstring Release,$(TARGET_NAME)),Release)
            ifdef ABIS_TO_COMPILE_RELEASE
                ABIS_TO_COMPILE = $(filter-out $(ABIS_TO_COMPILE_DEBUG),$(ABIS_TO_COMPILE_RELEASE))
            endif
        endif
        ABIS_TO_COMPILE += $(ABIS_TO_COMPILE_DEBUG)
    endif
endif

# If we are doing a clean release ...
ifeq ($(MAKECMDGOALS),clean)
    TARGET = bin/$(APPNAME)_debug bin/$(APPNAME)
    TARGET_NAME = Release
endif

# we only get a CLEAN_TARGET if a TARGET_NAME has been defined
# Like TARGET, this must be defined above or in a platform file.
ifdef TARGET_NAME
    CLEANTARGET = $(addprefix Clean,$(TARGET_NAME))
endif

################################################################################
# CONFIGURE COMPILATION PARAMETERS
#   If we are executing an actual compilation target (indicated by the 
#   ABI string in the one or more of the $(MAKECMDGOALS), then include the
#   config.project.mk file.  See config.project.mk for documentation. 
################################################################################

ifeq ($(findstring ABI,$(MAKECMDGOALS)),ABI)
    include $(PATH_OF_SHARED_MAKEFILES)/config.project.mk
    -include $(OF_PROJECT_DEPENDENCY_FILES)
endif

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
		help \
		run

Release:
	@echo Compiling OF library for Release
	@$(MAKE) --no-print-directory -C $(PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED_PROJECT)/ Release PLATFORM_OS=$(PLATFORM_OS) ABIS_TO_COMPILE_RELEASE="$(ABIS_TO_COMPILE_RELEASE)"
	@echo
	@echo
	@echo Compiling $(APPNAME) for Release
ifndef ABIS_TO_COMPILE_RELEASE
	@$(MAKE) --no-print-directory ReleaseABI
else
	@$(foreach abi,$(ABIS_TO_COMPILE_RELEASE),$(MAKE) --no-print-directory ReleaseABI ABI=$(abi) &&) echo 
endif
ifneq ($(strip $(PROJECT_ADDONS_DATA)),)
	@$(MAKE) copyaddonsdata PROJECT_ADDONS_DATA=$(PROJECT_ADDONS_DATA)
endif
	@$(MAKE) --no-print-directory afterplatform BIN_NAME=$(BIN_NAME) ABIS_TO_COMPILE="$(ABIS_TO_COMPILE_RELEASE)" RUN_TARGET=$(RUN_TARGET) TARGET=$(TARGET)

Debug: 
	@echo Compiling OF library for Debug
	$(MAKE) --no-print-directory -C $(PATH_OF_LIBS_OPENFRAMEWORKS_COMPILED_PROJECT)/ Debug PLATFORM_OS=$(PLATFORM_OS) ABIS_TO_COMPILE_DEBUG="$(ABIS_TO_COMPILE_DEBUG)"
	@echo
	@echo
	@echo Compiling $(APPNAME) for Debug
ifndef ABIS_TO_COMPILE_DEBUG
	@$(MAKE) --no-print-directory DebugABI
else
	@$(foreach abi,$(ABIS_TO_COMPILE_DEBUG),$(MAKE) --no-print-directory DebugABI ABI=$(abi) &&) echo 
endif
ifneq ($(strip $(PROJECT_ADDONS_DATA)),)
	@$(MAKE) copyaddonsdata PROJECT_ADDONS_DATA=$(PROJECT_ADDONS_DATA)
endif
	@$(MAKE) --no-print-directory afterplatform BIN_NAME=$(BIN_NAME) ABIS_TO_COMPILE="$(ABIS_TO_COMPILE_DEBUG)" RUN_TARGET=$(RUN_TARGET) TARGET=$(TARGET) OF_PROJECT_FRAMEWORKS_EXPORTS=$(OF_PROJECT_FRAMEWORKS_EXPORTS)

ReleaseABI: $(TARGET)

DebugABI: $(TARGET)

all:
	$(MAKE) Release
	
run:
ifeq ($(PLATFORM_RUN_COMMAND),)
	@bin/$(BIN_NAME)
else
	@$(PLATFORM_RUN_COMMAND) $(BIN_NAME)
endif

RunRelease:
ifeq ($(PLATFORM_RUN_COMMAND),)
	@bin/$(BIN_NAME)
else
	@$(PLATFORM_RUN_COMMAND) $(BIN_NAME)
endif

RunDebug:
ifeq ($(PLATFORM_RUN_COMMAND),)
	@bin/$(BIN_NAME)
else
	@$(PLATFORM_RUN_COMMAND) $(BIN_NAME)
endif

#This rule does the compilation
#$(OBJS): $(SOURCES)
$(OF_PROJECT_OBJ_OUPUT_PATH)%.o: $(PATH_PROJECT_ROOT)/%.m
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUPUT_PATH)$*.o -o $@ -c $<

$(OF_PROJECT_OBJ_OUPUT_PATH)%.o: $(PATH_PROJECT_ROOT)/%.mm
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUPUT_PATH)$*.o -o $@ -c $<

$(OF_PROJECT_OBJ_OUPUT_PATH)%.o: $(PATH_PROJECT_ROOT)/%.cpp
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUPUT_PATH)$*.o -o $@ -c $<

$(OF_PROJECT_OBJ_OUPUT_PATH)%.o: $(PATH_PROJECT_ROOT)/%.cxx
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUPUT_PATH)$*.o -o $@ -c $<

$(OF_PROJECT_OBJ_OUPUT_PATH)%.o: $(PATH_PROJECT_ROOT)/%.cc
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUPUT_PATH)$*.o -o $@ -c $<
	
$(OF_PROJECT_OBJ_OUPUT_PATH)%.o: $(PATH_PROJECT_ROOT)/%.c
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CC) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUPUT_PATH)$*.o -o $@ -c $<
	
$(OF_PROJECT_OBJ_OUPUT_PATH)%.o: $(PATH_PROJECT_ROOT)/%.S
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CC) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUPUT_PATH)$*.o -o $@ -c $<

$(OF_PROJECT_OBJ_OUPUT_PATH)%.o: $(PROJECT_EXTERNAL_SOURCE_PATHS)/%.cpp
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUPUT_PATH)$*.o -o $@ -c $<

$(OF_PROJECT_OBJ_OUPUT_PATH)%.o: $(PROJECT_EXTERNAL_SOURCE_PATHS)/%.m
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CC) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUPUT_PATH)$*.o -o $@ -c $<

$(OF_PROJECT_OBJ_OUPUT_PATH)%.o: $(PROJECT_EXTERNAL_SOURCE_PATHS)/%.mm
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUPUT_PATH)$*.o -o $@ -c $<

$(OF_PROJECT_OBJ_OUPUT_PATH)%.o: $(PROJECT_EXTERNAL_SOURCE_PATHS)/%.cxx
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUPUT_PATH)$*.o -o $@ -c $<

$(OF_PROJECT_OBJ_OUPUT_PATH)%.o: $(PROJECT_EXTERNAL_SOURCE_PATHS)/%.cc
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUPUT_PATH)$*.o -o $@ -c $<
	
$(OF_PROJECT_OBJ_OUPUT_PATH)%.o: $(PROJECT_EXTERNAL_SOURCE_PATHS)/%.c
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CC) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUPUT_PATH)$*.o -o $@ -c $<
	
$(OF_PROJECT_OBJ_OUPUT_PATH)%.o: $(PROJECT_EXTERNAL_SOURCE_PATHS)/%.S
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CC) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUPUT_PATH)$*.o -o $@ -c $<

$(PATH_OF_ROOT)/addons/$(OF_PROJECT_OBJ_OUPUT_PATH)/%.o: $(PATH_OF_ROOT)/addons/%.cpp
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(PATH_OF_ROOT)/addons/$(OF_PROJECT_OBJ_OUPUT_PATH)/$*.d -MT $(PATH_OF_ROOT)/addons/$(OF_PROJECT_OBJ_OUPUT_PATH)/$*.o -o $@ -c $<

$(PATH_OF_ROOT)/addons/$(OF_PROJECT_OBJ_OUPUT_PATH)/%.o: $(PATH_OF_ROOT)/addons/%.m
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CC) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(PATH_OF_ROOT)/addons/$(OF_PROJECT_OBJ_OUPUT_PATH)/$*.d -MT $(PATH_OF_ROOT)/addons/$(OF_PROJECT_OBJ_OUPUT_PATH)/$*.o -o $@ -c $<

$(PATH_OF_ROOT)/addons/$(OF_PROJECT_OBJ_OUPUT_PATH)/%.o: $(PATH_OF_ROOT)/addons/%.mm
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(PATH_OF_ROOT)/addons/$(OF_PROJECT_OBJ_OUPUT_PATH)/$*.d -MT $(PATH_OF_ROOT)/addons/$(OF_PROJECT_OBJ_OUPUT_PATH)/$*.o -o $@ -c $<

$(PATH_OF_ROOT)/addons/$(OF_PROJECT_OBJ_OUPUT_PATH)/%.o: $(PATH_OF_ROOT)/addons/%.cxx
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(PATH_OF_ROOT)/addons/$(OF_PROJECT_OBJ_OUPUT_PATH)/$*.d -MT $(PATH_OF_ROOT)/addons/$(OF_PROJECT_OBJ_OUPUT_PATH)/$*.o -o $@ -c $<

$(PATH_OF_ROOT)/addons/$(OF_PROJECT_OBJ_OUPUT_PATH)/%.o: $(PATH_OF_ROOT)/addons/%.cc
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(PATH_OF_ROOT)/addons/$(OF_PROJECT_OBJ_OUPUT_PATH)/$*.d -MT $(PATH_OF_ROOT)/addons/$(OF_PROJECT_OBJ_OUPUT_PATH)/$*.o -o $@ -c $<
	
$(PATH_OF_ROOT)/addons/$(OF_PROJECT_OBJ_OUPUT_PATH)/%.o: $(PATH_OF_ROOT)/addons/%.c
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CC) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(PATH_OF_ROOT)/addons/$(OF_PROJECT_OBJ_OUPUT_PATH)/$*.d -MT $(PATH_OF_ROOT)/addons/$(OF_PROJECT_OBJ_OUPUT_PATH)/$*.o -o $@ -c $<
	
$(PATH_OF_ROOT)/addons/$(OF_PROJECT_OBJ_OUPUT_PATH)/%.o: $(PATH_OF_ROOT)/addons/%.S
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CC) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(PATH_OF_ROOT)/addons/$(OF_PROJECT_OBJ_OUPUT_PATH)/$*.d -MT $(PATH_OF_ROOT)/addons/$(OF_PROJECT_OBJ_OUPUT_PATH)/$*.o -o $@ -c $<

$(TARGET): $(OF_PROJECT_OBJS) $(OF_PROJECT_ADDONS_OBJS) $(OF_PROJECT_LIBS) $(TARGET_LIBS)
	@echo 'Linking $(TARGET) for $(ABI_LIB_SUBPATH)'
	mkdir -p $(@D)
	$(CXX) -o $@ $(OF_PROJECT_OBJS) $(OF_PROJECT_ADDONS_OBJS) $(LDFLAGS) $(TARGET_LIBS) $(OF_PROJECT_LIBS) $(OF_CORE_LIBS)

clean:
	@$(MAKE) --no-print-directory CleanDebug
	@$(MAKE) --no-print-directory CleanRelease

$(CLEANTARGET)ABI:
ifneq ($(OF_PROJECT_ADDONS_OBJS),)
	rm -f $(OF_PROJECT_ADDONS_OBJS)
endif
	rm -rf $(OF_PROJECT_OBJ_OUPUT_PATH)
	rm -f $(TARGET)
	rm -rf $(BIN_NAME)
	
$(CLEANTARGET):
ifndef ABIS_TO_COMPILE
	@$(MAKE) --no-print-directory $(CLEANTARGET)ABI
else
ifeq ($(TARGET_NAME),Debug)
	@$(foreach abi,$(ABIS_TO_COMPILE_DEBUG),$(MAKE) --no-print-directory $(CLEANTARGET)ABI ABI=$(abi) &&) echo done
else
	@$(foreach abi,$(ABIS_TO_COMPILE_RELEASE),$(MAKE) --no-print-directory $(CLEANTARGET)ABI ABI=$(abi) &&) echo done
endif
endif
	@rm -rf bin/libs

after: $(TARGET_NAME)
	cp -r $(OF_EXPORT_PATH)/$(ABI_LIB_SUBPATH)/* bin/
	@echo
	@echo "     compiling done"
	@echo "     to launch the application"
	@echo
	@echo "     cd bin"
	@echo "     ./$(BIN_NAME)"
	@echo "     "
	@echo "     - or -"
	@echo "     "
	@echo "     $(MAKE) $(RUN_TARGET)"
	@echo
	
copyaddonsdata:
	@mkdir -p bin/data
	@cp -rf $(PROJECT_ADDONS_DATA) bin/data/

# TODO: fix this help to make it more accurate.
# TODO: tell user to use project generator (which needs to be updated)
# or copy the Makefile and config.make / addons.make from the scripts
# template directory
help:
	@echo
	@echo openFrameworks universal makefile
	@echo
	@echo "Targets:"
	@echo
	@echo "make Debug:		builds the library with debug symbols"
	@echo "make Release:		builds the library with optimizations"
	@echo "make:			= make Release"
	@echo "make all:		= make Release"
	@echo "make CleanDebug:	cleans the Debug target"
	@echo "make CleanRelease:	cleans the Release target"
	@echo "make clean:		cleans everything"
	@echo "make help:		this help message"
	@echo
	@echo
	@echo This should work with any OF app. Just copy any example,
	@echo change the name of the folder and it should compile
	@echo "only .cpp support, don't use .c files"
	@echo It will look for files in any folder inside the application
	@echo folder except that in the EXCLUDE_FROM_SOURCE variable.
	@echo "it doesn't autodetect include paths yet"
	@echo "add the include paths editing the var USER_CFLAGS"
	@echo at the beginning of the makefile using the gcc syntax:
	@echo -Ipath
	@echo
	@echo to add addons to your application, edit the addons.make file
	@echo in this directory and add the names of the addons you want to
	@echo include
	@echo

# Legacy targets
AndroidRelease:
	$(MAKE) Release PLATFORM_OS=Android
	
AndroidDebug:
	$(MAKE) Debug PLATFORM_OS=Android
	
CleanAndroid:
	$(MAKE) clean PLATFORM_OS=Android
