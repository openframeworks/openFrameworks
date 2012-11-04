# setup the shell variable
SHELL = /bin/sh

# print debug information if desired
ifdef MAKEFILE_DEBUG
    $(info =============================makefile.compile.project=========================)
endif

# if the APPNAME is not already defined, set it to the present working directory of the proejct
ifndef APPNAME
    APPNAME = $(shell basename `pwd`)
endif

# print the name of the project as a debug message if desired
ifdef MAKEFILE_DEBUG
    $(info APPNAME=$(APPNAME))
endif

# TODO: what is this for?
NODEPS = clean

##################
# exclude all items from source according to the config.make file

# make a list of source file and folder exclusions relative to the project directory
FULLY_QUALIFIED_PROJECT_SOURCE_EXCLUSIONS=$(addprefix ./,$(addsuffix %,$(strip $(PROJECT_SOURCE_EXCLUSIONS))))

# make a list of all directories that could be valid project source directories
ALL_PROJECT_SOURCE_DIRS = $(shell find . -maxdepth 1 -mindepth 1 -type d)

# filter out all excluded files / folders that were defined above
# this list will be searched for cpp files below and will eventually
# be included as locations for header searches via 
FILTERED_PROJECT_SOURCE_DIRS=$(filter-out $(FULLY_QUALIFIED_PROJECT_SOURCE_EXCLUSIONS),$(ALL_PROJECT_SOURCE_DIRS))

# find all sources inside the project's source directory (recursively)
PROJECT_SOURCES = $(shell find $(FILTERED_PROJECT_SOURCE_DIRS) -name "*.cpp" -or -name "*.c" -or -name "*.cc" -or -name "*.cxx")

# produce object files for all project sourcees
PROJECT_OBJFILES = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cxx,%.o,$(patsubst %.cc,%.o,$(PROJECT_SOURCES)))))

ifdef PROJECT_EXTERNAL_SOURCE_PATHS
    PROJECT_EXTERNAL_SOURCES = $(shell find $(PROJECT_EXTERNAL_SOURCE_PATHS) -name "*.cpp" -or -name "*.c" -or -name "*.cc" -or -name "*.cxx")
    PROJECT_EXTERNAL_OBJFILES = $(subst $(PROJECT_EXTERNAL_SOURCE_PATHS)/, ,$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cxx,%.o,$(patsubst %.cc,%.o,$(PROJECT_EXTERNAL_SOURCES))))))
endif


PROJECT_INCLUDES = $(FILTERED_PROJECT_SOURCE_DIRS) $(PROJECT_EXTERNAL_SOURCE_PATHS)

# construct all project-specific cflags
PROJECT_INCLUDES_CFLAGS = $(addprefix -I,$(PROJECT_INCLUDES))

LDFLAGS = -Wl,-rpath=./libs

##########################################################################################
# construct all includes cflags
ALL_INCLUDES_CFLAGS += $(OF_CORE_INCLUDES_CFLAGS)
ALL_INCLUDES_CFLAGS += $(ADDONS_INCLUDES_CFLAGS)
ALL_INCLUDES_CFLAGS += -I.
ALL_INCLUDES_CFLAGS += $(PROJECT_INCLUDES_CFLAGS)

################################
## stopped here ...

ifeq ($(findstring Debug,$(TARGET_NAME)),Debug)
    CFLAGS += -g3
    TARGET_LIBS = $(OF_ROOT)/libs/openFrameworksCompiled/lib/$(LIBSPATH)/libopenFrameworksDebug.a
endif

ifeq ($(findstring Release,$(TARGET_NAME)),Release)
    CFLAGS += $(COMPILER_OPTIMIZATION)
    TARGET_LIBS = $(OF_ROOT)/libs/openFrameworksCompiled/lib/$(LIBSPATH)/libopenFrameworks.a
endif

### addons used to be done here ...

ifeq ($(MAKECMDGOALS),clean)
    TARGET = bin/$(APPNAME)_debug bin/$(APPNAME)
    TARGET_NAME = Release
endif


OBJ_OUTPUT = obj/$(ARCH)$(TARGET_NAME)/
CLEANTARGET = clean$(TARGET_NAME)

OBJS = $(addprefix $(OBJ_OUTPUT), $(OBJFILES))
DEPFILES = $(patsubst %.o,%.d,$(OBJS))

USER_OBJS = $(addprefix $(OBJ_OUTPUT), $(USER_OBJFILES))
DEPFILES += $(patsubst %.o,%.d,$(USER_OBJS))

ifeq ($(findstring addons.make,$(wildcard *.make)),addons.make)
    ADDONS_OBJS = $(addprefix $(OBJ_OUTPUT), $(ADDONS_OBJFILES))
    DEPFILES += $(patsubst %.o,%.d,$(ADDONS_OBJS))
endif

ifeq ($(TARGET_LIBS),)
    TARGET_LIBS = $(OF_ROOT)/libs/openFrameworksCompiled/lib/$(LIBSPATH)/libopenFrameworks.a
endif

.PHONY: Debug Release all after

Release: $(TARGET) after

Debug: $(TARGET) after

all:
	$(MAKE) Release


#This rule does the compilation
#$(OBJS): $(SOURCES)
$(OBJ_OUTPUT)%.o: %.cpp
	@echo "compiling $(ARCH) object for: " $<
	mkdir -p $(@D)
	$(CXX) -c $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.o -o$@ -c $<
	
$(OBJ_OUTPUT)%.o: %.cxx
	@echo "compiling $(ARCH) object for: " $<
	mkdir -p $(@D)
	$(CXX) -c $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.o -o$@ -c $<

$(OBJ_OUTPUT)%.o: %.c
	@echo "compiling $(ARCH) object for: " $<
	mkdir -p $(@D)
	$(CC) -c $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.o -o$@ -c $<

$(OBJ_OUTPUT)%.o: %.cc
	@echo "compiling $(ARCH) object for: " $<
	mkdir -p $(@D)
	$(CC) -c $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.o -o$@ -c $<

$(OBJ_OUTPUT)%.o: $(OF_ROOT)/%.cpp
	@echo "compiling addon $(ARCH) object for" $<
	mkdir -p $(@D)
	$(CXX) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.o -o $@ -c $<

$(OBJ_OUTPUT)%.o: $(OF_ROOT)/%.cxx
	@echo "compiling addon $(ARCH) object for" $<
	mkdir -p $(@D)
	$(CXX) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.o -o $@ -c $<

$(OBJ_OUTPUT)%.o: $(OF_ROOT)/%.c
	@echo "compiling addon $(ARCH) object for" $<
	mkdir -p $(@D)
	$(CC) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.o -o $@ -c $<

$(OBJ_OUTPUT)%.o: $(OF_ROOT)/%.cc
	@echo "compiling addon $(ARCH) object for" $<
	mkdir -p $(@D)
	$(CC) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.o -o $@ -c $<

$(OBJ_OUTPUT)%.o: $(USER_SOURCE_DIR)/%.c
	@echo "compiling $(ARCH) object for: " $<
	mkdir -p $(@D)
	$(CC) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.o -o$@ -c $<

$(OBJ_OUTPUT)%.o: $(USER_SOURCE_DIR)/%.cc
	@echo "compiling $(ARCH) object for: " $<
	mkdir -p $(@D)
	$(CC) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.o -o$@ -c $<

$(OBJ_OUTPUT)%.o: $(USER_SOURCE_DIR)/%.cpp
	@echo "compiling $(ARCH) object for: " $<
	mkdir -p $(@D)
	$(CXX) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.o -o$@ -c $<

$(OBJ_OUTPUT)%.o: $(USER_SOURCE_DIR)/%.cxx
	@echo "compiling $(ARCH) object for: " $<
	mkdir -p $(@D)
	$(CXX) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.o -o$@ -c $<

$(TARGET): $(OBJS) $(ADDONS_OBJS) $(USER_OBJS) $(TARGET_LIBS) $(LIB_STATIC)
	@echo 'linking $(ARCH) $(TARGET) $(LIBSPATH)'
	mkdir -p $(@D)
	$(CXX) -o $@ $(OBJS) $(ADDONS_OBJS) $(USER_OBJS) $(LDFLAGS) $(USER_LDFLAGS) $(TARGET_LIBS) $(ADDONSLIBS) $(USER_LIBS) $(LIB_STATIC) $(LIB_PATHS_FLAGS) $(LIB_SHARED) $(SYSTEMLIBS)

-include $(DEPFILES)

.PHONY: clean cleanDebug cleanRelease

clean:
	rm -rf $(OBJ_OUTPUT)
	rm -f $(TARGET)
	rm -rf bin/libs

ifneq ($(CLEANTARGET),clean)
	$(CLEANTARGET):
		rm -rf $(OBJ_OUTPUT)
		rm -f $(TARGET)
		rm -rf bin/libs
endif


after:$(TARGET)
	cp -r $(OF_ROOT)/export/$(LIBSPATH)/libs bin/
	@echo
	@echo "     compiling done"
	@echo "     to launch the application"
	@echo
	@echo "     cd bin"
	@echo "     ./$(BIN_NAME)"
	@echo


.PHONY: help
help:
	@echo
	@echo openFrameworks universal makefile
	@echo
	@echo targets:
	@echo "make Debug:		builds the application with debug symbols"
	@echo "make Release:		builds the app with optimizations"
	@echo "make:			= make Release"
	@echo "make all:		= make Release"
	@echo "make CleanDebug:	cleans the Debug target"
	@echo "make CleanRelease:	cleans the Release target"
	@echo "make clean:		cleans everything"
	@echo
	@echo this should work with any OF app, just copy any example
	@echo change the name of the folder and it should compile
	@echo "only .cpp support, don't use .c files"
	@echo it will look for files in any folder inside the application
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




















# print debug information if needed
ifdef MAKEFILE_DEBUG
##    $(info ---FULLY_QUALIFIED_PROJECT_SOURCE_EXCLUSIONS--- )
#    $(foreach v, $(FULLY_QUALIFIED_PROJECT_SOURCE_EXCLUSIONS),$(info $(v)))
#    $(info ---ALL_PROJECT_SOURCE_DIRS--- )
#    $(foreach v, $(ALL_PROJECT_SOURCE_DIRS),$(info $(v)))
##    $(info ---FILTERED_PROJECT_SOURCE_DIRS--- )
 #   $(foreach v, $(FILTERED_PROJECT_SOURCE_DIRS),$(info $(v)))
 #   $(info ---PROJECT_SOURCES--- )
 #   $(foreach v, $(PROJECT_SOURCES),$(info $(v)))
  # $(info ---PROJECT_OBJFILES--- )
 # $(foreach v, $(PROJECT_OBJFILES),$(info $(v)))
endif
