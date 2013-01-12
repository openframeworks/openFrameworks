################################################################################
ifdef MAKEFILE_DEBUG
    $(info ===================compile.project.make=============================)
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

ifdef PROJECT_CC
    CC = $(PROJECT_CC)
endif

# TODO: what is this for?
NODEPS = clean

################################################################################
# CFLAGS
################################################################################

# clean it
ALL_CFLAGS =
# add the CFLAGS from Makefiles.examples
ALL_CFLAGS += $(OF_PROJECT_CFLAGS)

# clean up all extra whitespaces in the CFLAGS
CFLAGS = $(strip $(ALL_CFLAGS))

################################################################################
# LDFLAGS
################################################################################

# clean it
ALL_LDFLAGS =

# add the include LDFLAGS from Makefiles.examples
ALL_LDFLAGS += $(OF_PROJECT_LDFLAGS)
ALL_LDFLAGS += $(PLATFORM_LDFLAGS)

# clean up all extra whitespaces in the LDFLAGS
LDFLAGS = $(strip $(ALL_LDFLAGS))

# Name TARGET
ifeq ($(findstring Debug,$(MAKECMDGOALS)),Debug)
	TARGET_NAME = Debug
	BIN_NAME = $(APPNAME)_debug
	TARGET = bin/$(BIN_NAME)
else ifeq ($(findstring Release,$(MAKECMDGOALS)),Release)
	TARGET_NAME = Release
	BIN_NAME = $(APPNAME)
	TARGET = bin/$(BIN_NAME)
else ifeq ($(MAKECMDGOALS),)
	TARGET_NAME = Release
	BIN_NAME = $(APPNAME)
	TARGET = bin/$(BIN_NAME)
endif


################################################################################
## stopped here ... TODO: what does this mean?

ifeq ($(findstring Debug,$(TARGET_NAME)),Debug)
	ifeq ($strip($(PROJECT_OPTIMIZATION_CFLAGS_DEBUG)),)
	    OPTIMIZATION_CFLAGS = $(PLATFORM_OPTIMIZATION_CFLAGS_DEBUG)
	else
		OPTIMIZATION_CFLAGS = $(PROJECT_OPTIMIZATION_CFLAGS_DEBUG)
	endif
    TARGET_LIBS = $(OF_CORE_LIB_PATH)/libopenFrameworksDebug.a
endif

ifeq ($(findstring Release,$(TARGET_NAME)),Release)
	ifeq ($strip($(PROJECT_OPTIMIZATION_CFLAGS_RELEASE)),)
	    OPTIMIZATION_CFLAGS = $(PLATFORM_OPTIMIZATION_CFLAGS_RELEASE)
	else
		OPTIMIZATION_CFLAGS = $(PROJECT_OPTIMIZATION_CFLAGS_RELEASE)
	endif
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

ifdef MAKEFILE_DEBUG
    $(info ---OF_PROJECT_SOURCE_FILES---)
    $(foreach v, $(OF_PROJECT_SOURCE_FILES),$(info $(v)))
endif
ifdef MAKEFILE_DEBUG
    $(info ---OF_PROJECT_DEPENDENCY_FILES---)
    $(foreach v, $(OF_PROJECT_DEPENDENCY_FILES),$(info $(v)))
endif


OF_PROJECT_OBJ_OUPUT_PATH = obj/$(PLATFORM_LIB_SUBPATH)/$(TARGET_NAME)

OF_PROJECT_OBJ_FILES = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cxx,%.o,$(patsubst %.cc,%.o,$(OF_PROJECT_SOURCE_FILES)))))
OF_PROJECT_OBJS = $(subst $(PROJECT_ROOT)/,,$(addprefix $(OF_PROJECT_OBJ_OUPUT_PATH)/,$(OF_PROJECT_OBJ_FILES)))
OF_PROJECT_DEPS = $(patsubst %.o,%.d,$(OF_PROJECT_OBJS))

OF_PROJECT_ADDONS_OBJ_FILES = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cxx,%.o,$(patsubst %.cc,%.o,$(PROJECT_ADDONS_SOURCE_FILES)))))
OF_PROJECT_ADDONS_OBJS = $(subst $(OF_ROOT)/,,$(addprefix $(OF_PROJECT_OBJ_OUPUT_PATH),$(OF_PROJECT_ADDONS_OBJ_FILES)))
OF_PROJECT_ADDONS_DEPS = $(patsubst %.o,%.d,$(OF_PROJECT_ADDONS_OBJS))

OF_PROJECT_DEPENDENCY_FILES = $(OF_PROJECT_DEPS) $(OF_PROJECT_ADDONS_DEPS)

# TODO: deal with shared libs?



ifdef MAKEFILE_DEBUG
    $(info ---OF_PROJECT_DEPENDENCY_FILES---)
    $(foreach v, $(OF_PROJECT_DEPENDENCY_FILES),$(info $(v)))
endif

.PHONY: all Debug Release after clean CleanDebug CleanRelease help

	
Release: $(TARGET_LIBS) $(TARGET) afterplatform

Debug: $(TARGET_LIBS) $(TARGET) afterplatform

all:
	$(MAKE) Debug
	$(MAKE) Release
	

#This rule does the compilation
#$(OBJS): $(SOURCES)
$(OF_PROJECT_OBJ_OUPUT_PATH)%.o: $(PROJECT_ROOT)/%.cpp
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUPUT_PATH)$*.o -o $@ -c $<

$(OF_PROJECT_OBJ_OUPUT_PATH)%.o: $(PROJECT_ROOT)/%.cxx
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUPUT_PATH)$*.o -o $@ -c $<

$(OF_PROJECT_OBJ_OUPUT_PATH)%.o: $(PROJECT_ROOT)/%.cc
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CC) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUPUT_PATH)$*.o -o $@ -c $<
	
$(OF_PROJECT_OBJ_OUPUT_PATH)%.o: $(PROJECT_ROOT)/%.c
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CC) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUPUT_PATH)$*.o -o $@ -c $<

$(OF_PROJECT_OBJ_OUPUT_PATH)%.o: $(OF_ROOT)/%.cpp
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUPUT_PATH)$*.o -o $@ -c $<

$(OF_PROJECT_OBJ_OUPUT_PATH)%.o: $(OF_ROOT)/%.cxx
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUPUT_PATH)$*.o -o $@ -c $<

$(OF_PROJECT_OBJ_OUPUT_PATH)%.o: $(OF_ROOT)/%.cc
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CC) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUPUT_PATH)$*.o -o $@ -c $<
	
$(OF_PROJECT_OBJ_OUPUT_PATH)%.o: $(OF_ROOT)/%.c
	@echo "Compiling" $<
	mkdir -p $(@D)
	$(CC) -c $(OPTIMIZATION_CFLAGS) $(CFLAGS) -MMD -MP -MF $(OF_PROJECT_OBJ_OUPUT_PATH)$*.d -MT $(OF_PROJECT_OBJ_OUPUT_PATH)$*.o -o $@ -c $<

$(TARGET): $(OF_PROJECT_OBJS) $(OF_PROJECT_ADDONS_OBJS) $(OF_PROJECT_LIBS)
	@echo 'Linking $(TARGET) for $(PLATFORM_LIB_SUBPATH)'
	mkdir -p $(@D)
	$(CXX) -o $@ $(OF_PROJECT_OBJS) $(OF_PROJECT_ADDONS_OBJS) $(LDFLAGS) $(OF_PROJECT_LIBS) $(OF_CORE_LIBS) $(TARGET_LIBS);	
	
# This rule adds a dependency for projects to the OF library 
# so if any OF file gets modified the OF library will be compiled
# before compiling the project
$(TARGET_LIBS): $(OF_CORE_OBJ_FILES)
	$(MAKE) -C $(OF_ROOT)/libs/openFrameworksCompiled/project/ $(TARGET_NAME)
	
	
-include $(OF_CORE_DEPENDENCY_FILES)
-include $(OF_PROJECT_DEPENDENCY_FILES)

clean:
	rm -rf $(OF_PROJECT_OBJ_OUPUT_PATH)
	rm -f $(TARGET)
	rm -rf bin/libs

$(CLEANTARGET):
	rm -rf $(OF_PROJECT_OBJ_OUPUT_PATH)
	rm -f $(TARGET)
	rm -rf bin/libs

after: $(TARGET)
	cp -r $(OF_EXPORT_PATH)/$(PLATFORM_LIB_SUBPATH) bin/
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
