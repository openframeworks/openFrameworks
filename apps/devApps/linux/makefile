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
#
# this should work with any OF app, just copy any example
# change the name of the folder and it should compile
# only .cpp support, don't use .c files
# it will look for files in any folder inside the application
# folder except that in the EXCLUDE_FROM_SOURCE variable
# it doesn't autodetect include paths yet
# add the include paths in the USER_CFLAGS variable
# using the gcc syntax: -Ipath
#
# to add addons to your application, edit the addons.make file
# in this directory and add the names of the addons you want to
# include
#
# edit the following  vars to customize the makefile


COMPILER_OPTIMIZATION = -march=native -mtune=native -O3
EXCLUDE_FROM_SOURCE="bin,.xcodeproj,obj"
USER_CFLAGS = 
USER_LD_FLAGS = 
USER_LIBS = 




# you shouldn't modify anything below this line


SHELL =  /bin/sh
CXX =  g++

ARCH = $(shell uname -m)
ifeq ($(ARCH),x86_64)
	LIBSPATH=linux64
else
	LIBSPATH=linux
endif

NODEPS = clean
SED_EXCLUDE_FROM_SRC = $(shell echo  $(EXCLUDE_FROM_SOURCE) | sed s/\,/\\\\\|/g)
SOURCE_DIRS = $(shell find . -maxdepth 1 -mindepth 1 -type d | grep -v $(SED_EXCLUDE_FROM_SRC) | sed s/.\\///)
SOURCES = $(shell find $(SOURCE_DIRS) -name "*.cpp" -or -name "*.c")
OBJFILES = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))
APPNAME = $(shell basename `pwd`)
CORE_INCLUDES = $(shell find ../../../libs/openFrameworks/ -type d)
CORE_INCLUDE_FLAGS = $(addprefix -I,$(CORE_INCLUDES))
INCLUDES = $(shell find ../../../libs/*/include -type d)
INCLUDES_FLAGS = $(addprefix -I,$(INCLUDES))
LIB_STATIC = $(shell ls ../../../libs/*/lib/$(LIBSPATH)/*.a | grep -v openFrameworksCompiled | sed "s/.*\\/lib\([^/]*\)\.a/-l\1/")
LIB_SHARED = $(shell ls ../../../libs/*/lib/$(LIBSPATH)/*.so | grep -v openFrameworksCompiled| sed "s/.*\\/lib\([^/]*\)\.so/-l\1/")

#LIB_PATHS_FLAGS = -L../../../libs/openFrameworksCompiled/lib/$(LIBSPATH)
LIB_PATHS_FLAGS = $(shell ls -d ../../../libs/*/lib/$(LIBSPATH) | sed "s/\(\.*\)/-L\1/")

CFLAGS = -Wall -fexceptions
CFLAGS += -I. 
CFLAGS += $(INCLUDES_FLAGS)
CFLAGS += $(CORE_INCLUDE_FLAGS)
CFLAGS +=`pkg-config  gstreamer-0.10 gstreamer-video-0.10 gstreamer-base-0.10 libudev --cflags`

LDFLAGS = $(LIB_PATHS_FLAGS) -Wl,-rpath=./libs

LIBS = $(LIB_SHARED)
LIBS += $(LIB_STATIC)
LIBS +=`pkg-config  gstreamer-0.10 gstreamer-video-0.10 gstreamer-base-0.10 libudev --libs`
LIBS += -lglut -lGL -lGLU -lasound

ifeq ($(findstring addons.make,$(wildcard *.make)),addons.make)
	ADDONS_INCLUDES = $(shell find ../../../addons/*/src/ -type d)
	ADDONS_INCLUDES += $(shell find ../../../addons/*/libs/ -type d)
	ADDONSCFLAGS = $(addprefix -I,$(ADDONS_INCLUDES))
	
	ADDONS_LIBS_STATICS = $(shell ls ../../../addons/*/libs/*/lib/$(LIBSPATH)/*.a)
	ADDONS_LIBS_SHARED = $(shell ls ../../../addons/*/libs/*/lib/$(LIBSPATH)/*.so)

	ADDONSLIBS = $(ADDONS_LIBS_STATICS)
	ADDONSLIBS += $(ADDONS_LIBS_SHARED)

	ADDONS = $(shell cat addons.make)
	ADDONS_REL_DIRS = $(addsuffix /src, $(ADDONS))
	ADDONS_LIBS_REL_DIRS = $(addsuffix /libs, $(ADDONS))
	ADDONS_DIRS = $(addprefix ../../../addons/, $(ADDONS_REL_DIRS) )
	ADDONS_LIBS_DIRS = $(addprefix ../../../addons/, $(ADDONS_LIBS_REL_DIRS) )
	ADDONS_SOURCES = $(shell find $(ADDONS_DIRS) -name "*.cpp" -or -name "*.c")
	ADDONS_SOURCES += $(shell find $(ADDONS_LIBS_DIRS) -name "*.cpp" -or -name "*.c" 2>/dev/null)
	ADDONS_OBJFILES = $(subst ../../../, ,$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(ADDONS_SOURCES))))
endif



ifeq ($(findstring Debug,$(MAKECMDGOALS)),Debug)
	TARGET_CFLAGS = -g
	TARGET_LIBS = -lopenFrameworksDebug
	TARGET_NAME = Debug
    BIN_NAME = $(APPNAME)_debug
	TARGET = bin/$(BIN_NAME)
endif

ifeq ($(findstring Release,$(MAKECMDGOALS)),Release)
	TARGET_CFLAGS = $(COMPILER_OPTIMIZATION)
	TARGET_LIBS = -lopenFrameworks
	TARGET_NAME = Release
    BIN_NAME = $(APPNAME)
	TARGET = bin/$(BIN_NAME)
endif

ifeq ($(MAKECMDGOALS),)
	TARGET_CFLAGS = $(COMPILER_OPTIMIZATION)
	TARGET_LIBS = -lopenFrameworks
	TARGET_NAME = Release
    BIN_NAME = $(APPNAME)
	TARGET = bin/$(BIN_NAME)
endif

ifeq ($(MAKECMDGOALS),clean)
	TARGET = bin/$(APPNAME)_debug bin/$(APPNAME)
endif

OBJ_OUTPUT = obj/$(TARGET_NAME)/
CLEANTARGET = Clean$(TARGET_NAME)
OBJS = $(addprefix $(OBJ_OUTPUT), $(OBJFILES))
DEPFILES = $(patsubst %.o,%.d,$(OBJS))
ifeq ($(findstring addons.make,$(wildcard *.make)),addons.make)
	ADDONS_OBJS = $(addprefix $(OBJ_OUTPUT), $(ADDONS_OBJFILES))
    DEPFILES += $(patsubst %.o,%.d,$(ADDONS_OBJS))
endif

.PHONY: Debug Release all after
	
Release: $(TARGET) after

Debug: $(TARGET) after

all: 
	make Release


#This rule does the compilation
#$(OBJS): $(SOURCES)
$(OBJ_OUTPUT)%.o: %.cpp
	@echo "compiling object for: " $<
	mkdir -p $(@D)
	$(CXX) -c $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.d -o$@ -c $<  

$(OBJ_OUTPUT)%.o: %.c
	@echo "compiling object for: " $<
	mkdir -p $(@D)
	$(CC) -c $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.d -o$@ -c $< 


$(OBJ_OUTPUT)%.o: ../../../%.cpp
	@echo "compiling addon object for" $<
	mkdir -p $(@D)
	$(CXX) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.d -o $@ -c $<

$(OBJ_OUTPUT)%.o: ../../../%.c
	@echo "compiling addon object for" $<
	mkdir -p $(@D)
	$(CC) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.d -o $@ -c $<
	
$(TARGET): $(OBJS) $(ADDONS_OBJS)
	@echo "linking" $(TARGET)
	$(CXX) -o $@ $(OBJS) $(ADDONS_OBJS) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) $(LDFLAGS) $(USER_LDFLAGS) $(TARGET_LIBS) $(LIBS) $(ADDONSLIBS) $(USER_LIBS)

-include $(DEPFILES)

.PHONY: clean CleanDebug CleanRelease
clean:
	rm -Rf obj
	rm -f -v $(TARGET)
	rm -Rf -v bin/libs
	
$(CLEANTARGET):
	rm -Rf -v $(OBJ_OUTPUT)
	rm -f -v $(TARGET)


after:
	cp -r ../../../export/$(LIBSPATH)/libs bin/
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
