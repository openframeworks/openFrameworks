################################################################################
# print debug information if desired

ifdef MAKEFILE_DEBUG
    $(info ===================makefile.compile.project=========================)
endif

# if APPNAME is not defined, set it to the project dir name
ifndef APPNAME
    APPNAME = $(shell basename `pwd`)
endif

# print the name of the project as a debug message if desired
ifdef MAKEFILE_DEBUG
    $(info APPNAME=$(APPNAME))
endif

# make sure we have a project root defined
ifndef PROJECT_ROOT
	PROJECT_ROOT=.
endif

ifdef PLATFORM_CXX
    CXX = $(PLATFORM_CXX)
endif

ifdef PROJECT_CXX
    CXX = $(PROJECT_CXX)
endif

ifdef PLATFORM_CC
    CC = $(PLATFORM_CC)
endif

ifdef PLATFORM_CC
    CC = $(PLATFORM_CC)
endif


# TODO: what is this for?
NODEPS = clean

################################################################################
# CFLAGS
################################################################################

# clean it
ALL_CFLAGS =

################################################################################
# CORE FLAGS ###################################################################
# add the base CFLAGS from the core (platform specific)
ALL_CFLAGS += $(OF_CORE_BASE_CFLAGS)
# add the base CFLAGS from the addons
ALL_CFLAGS += $(PROJECT_BASE_CFLAGS)

################################################################################
# DEFINE FLAGS #################################################################
# add the defines from the core (platform specific)
ALL_CFLAGS += $(OF_CORE_DEFINES_CFLAGS)
# add the defines from the project
ALL_CFLAGS += $(OF_PROJECT_DEFINES_CFLAGS)

################################################################################
# INCLUDE FLAGS ################################################################
# add the include CFLAGS from the core (platform specific)
ALL_CFLAGS += $(OF_CORE_INCLUDES_CFLAGS)
# add the include CFLAGS from the project
ALL_CFLAGS += $(OF_PROJECT_INCLUDES_CFLAGS)

# clean up all extra whitespaces in the CFLAGS
CFLAGS = $(strip $(ALL_CFLAGS))

################################################################################
# LDFLAGS
################################################################################

# clean it
ALL_LDFLAGS =

# add the include LDFLAGS from the project
ALL_LDFLAGS += $(OF_PROJECT_LDFLAGS)
# add the include LDFLAGS from the core (platform specific)
ALL_LDFLAGS += $(OF_CORE_LIBRARY_LDFLAGS)

# clean up all extra whitespaces in the LDFLAGS
LDFLAGS = $(strip $(ALL_LDFLAGS))

################################################################################
## stopped here ...

ifeq ($(findstring Debug,$(TARGET_NAME)),Debug)
    OPTIMIZATION_CFLAGS = $(PLATFORM_OPTIMIZATION_CFLAGS_DEBUG) $(PROJECT_OPTIMIZATION_CFLAGS_DEBUG)
    TARGET_LIBS = $(OF_CORE_LIB_PATH)/libopenFrameworksDebug.a
endif

ifeq ($(findstring Release,$(TARGET_NAME)),Release)
    OPTIMIZATION_CFLAGS = $(PLATFORM_OPTIMIZATION_CFLAGS_RELEASE) $(PROJECT_OPTIMIZATION_CFLAGS_RELEASE)
    TARGET_LIBS = $(OF_CORE_LIB_PATH)/libopenFrameworks.a
endif

### addons used to be done here ...

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
# OBJECT AND DEPENDENCY FILES DEFINITIONS
#	Object file paths are generated here (as opposed to with the rest of the 
#   flags) because we want to place them in target-specific folders. We
#   determine targets above. We –could– determine the target info earlier if we
#   wanted to.  It's here because that's approximately where it was in the 
#   legacy makefiles.
################################################################################

# define the subdirectory for our target name
OF_PLATFORM_OBJ_OUPUT_PATH = obj/$(TARGET_NAME)

OF_PROJECT_OBJ_FILES = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cxx,%.o,$(patsubst %.cc,%.o,$(OF_PROJECT_SOURCE_FILES)))))
OF_PROJECT_OBJS = $(subst $(PROJECT_ROOT)/,/,$(addprefix $(OF_PLATFORM_OBJ_OUPUT_PATH),$(OF_PROJECT_OBJ_FILES)))
OF_PROJECT_DEPS = $(patsubst %.o,%.d,$(OF_PROJECT_OBJS))

OF_PROJECT_ADDONS_OBJ_FILES = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cxx,%.o,$(patsubst %.cc,%.o,$(PROJECT_ADDONS_SOURCE_FILES)))))
OF_PROJECT_ADDONS_OBJS = $(subst $(OF_ROOT)/,/,$(addprefix $(OF_PLATFORM_OBJ_OUPUT_PATH),$(OF_PROJECT_ADDONS_OBJ_FILES)))
OF_PROJECT_ADDONS_DEPS = $(patsubst %.o,%.d,$(OF_PROJECT_ADDONS_OBJS))

# TODO: deal with shared libs?


.PHONY: all Debug Release after clean CleanDebug CleanRelease help

Release: $(TARGET) after

Debug: $(TARGET) after

all:
	$(MAKE) Debug
	$(MAKE) Release

#This rule does the compilation
#$(OBJS): $(SOURCES)
$(OF_PLATFORM_OBJ_OUPUT_PATH)%.o: $(PROJECT_ROOT)/%.cpp
	@echo "compiling $(ARCH) object for: " $<
	mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF$(OF_PLATFORM_OBJ_OUPUT_PATH)$*.d -MT$(OF_PLATFORM_OBJ_OUPUT_PATH)$*.o -o$@ -c $<

$(OF_PLATFORM_OBJ_OUPUT_PATH)%.o: $(PROJECT_ROOT)/%.cxx
	@echo "compiling $(ARCH) object for: " $<
	mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF$(OF_PLATFORM_OBJ_OUPUT_PATH)$*.d -MT$(OF_PLATFORM_OBJ_OUPUT_PATH)$*.o -o$@ -c $<

$(OF_PLATFORM_OBJ_OUPUT_PATH)%.o: $(PROJECT_ROOT)/%.cc
	@echo "compiling $(ARCH) object for: " $<
	mkdir -p $(@D)
	$(CC) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF$(OF_PLATFORM_OBJ_OUPUT_PATH)$*.d -MT$(OF_PLATFORM_OBJ_OUPUT_PATH)$*.o -o$@ -c $<
	
$(OF_PLATFORM_OBJ_OUPUT_PATH)%.o: $(PROJECT_ROOT)/%.c
	@echo "compiling $(ARCH) object for: " $<
	mkdir -p $(@D)
	$(CC) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF$(OF_PLATFORM_OBJ_OUPUT_PATH)$*.d -MT$(OF_PLATFORM_OBJ_OUPUT_PATH)$*.o -o$@ -c $<

$(OF_PLATFORM_OBJ_OUPUT_PATH)%.o: $(OF_ROOT)/%.cpp
	@echo "compiling $(ARCH) object for: " $<
	mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF$(OF_PLATFORM_OBJ_OUPUT_PATH)$*.d -MT$(OF_PLATFORM_OBJ_OUPUT_PATH)$*.o -o$@ -c $<

$(OF_PLATFORM_OBJ_OUPUT_PATH)%.o: $(OF_ROOT)/%.cxx
	@echo "compiling $(ARCH) object for: " $<
	mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF$(OF_PLATFORM_OBJ_OUPUT_PATH)$*.d -MT$(OF_PLATFORM_OBJ_OUPUT_PATH)$*.o -o$@ -c $<

$(OF_PLATFORM_OBJ_OUPUT_PATH)%.o: $(OF_ROOT)/%.cc
	@echo "compiling $(ARCH) object for: " $<
	mkdir -p $(@D)
	$(CC) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF$(OF_PLATFORM_OBJ_OUPUT_PATH)$*.d -MT$(OF_PLATFORM_OBJ_OUPUT_PATH)$*.o -o$@ -c $<
	
$(OF_PLATFORM_OBJ_OUPUT_PATH)%.o: $(OF_ROOT)/%.c
	@echo "compiling $(ARCH) object for: " $<
	mkdir -p $(@D)
	$(CC) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF$(OF_PLATFORM_OBJ_OUPUT_PATH)$*.d -MT$(OF_PLATFORM_OBJ_OUPUT_PATH)$*.o -o$@ -c $<

$(TARGET): $(OF_PROJECT_OBJS) $(OF_PROJECT_ADDONS_OBJS)
	@echo 'linking $(TARGET) for $(PLATFORM_LIB_SUBPATH)'
	mkdir -p $(@D)
	$(CXX) -o $@ $(OF_PROJECT_OBJS) $(OF_PROJECT_ADDONS_OBJS) $(LDFLAGS) $(TARGET_LIBS) $(OF_CORE_LIBS) $(OF_PROJECT_LIBS) 
-include $(OF_PLATFORM_DEPENDENCY_FILES)

clean:
	rm -rf $(OF_PLATFORM_OBJ_OUPUT_PATH)
	rm -f $(TARGET)
	rm -rf bin/libs

$(CLEANTARGET):
	rm -rf $(OF_PLATFORM_OBJ_OUPUT_PATH)
	rm -f $(TARGET)
	rm -rf bin/libs

after: $(TARGET)
	cp -r $(OF_EXPORT_PATH)/$(PLATFORM_LIB_SUBPATH)/libs bin/
	@echo
	@echo "     compiling done"
	@echo "     to launch the application"
	@echo
	@echo "     cd bin"
	@echo "     ./$(BIN_NAME)"
	@echo

help:
	@echo
	@echo openFrameworks universal makefile
	@echo
	@echo "Targets:"
	@echo
	@echo "make Debug:		builds the library with debug symbols"
	@echo "make Release:		builds the library with optimizations"
	@echo "make:			= make Release"
	@echo "make all:		= make Debug + make Release"
	@echo "make CleanDebug:	cleans the Debug target"
	@echo "make CleanRelease:	cleans the Release target"
	@echo "make clean:		cleans everything"
	@echo "make help:		this help message"
	@echo
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