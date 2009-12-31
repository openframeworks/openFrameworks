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
# but you'll need to add the include paths in USER_CFLAGS
#
# to add addons to your application, edit the addons.make file
# in this directory and add the names of the addons you want to
# include
#
# edit the following  vars to customize the makefile


COMPILER_OPTIMIZATION = -march=core2 -O3
EXCLUDE_FROM_SOURCE="bin\|.xcodeproj\|obj"
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
SOURCE_DIRS = $(shell find . -maxdepth 1 -mindepth 1 -type d | grep -v $(EXCLUDE_FROM_SOURCE) | sed s/.\\///)
SOURCES = $(shell find $(SOURCE_DIRS) -name "*.cpp")
OBJFILES = $(patsubst %.cpp,%.o,$(SOURCES))
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

LDFLAGS = $(LIB_PATHS_FLAGS) -s

LIBS = $(LIB_SHARED)
LIBS += $(LIB_STATIC)
LIBS +=`pkg-config  gstreamer-0.10 gstreamer-video-0.10 gstreamer-base-0.10 libudev --libs`
LIBS += -lglut -lGL -lasound

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
	ADDONS_SOURCES = $(shell find $(ADDONS_DIRS) -name "*.cpp")
	ADDONS_SOURCES += $(shell find $(ADDONS_LIBS_DIRS) -name "*.cpp" 2>/dev/null)
	ADDONS_OBJFILES = $(subst ../../../, ,$(patsubst %.cpp,%.o,$(ADDONS_SOURCES)))
endif



ifeq ($(findstring Debug,$(MAKECMDGOALS)),Debug)
	TARGET_CFLAGS = -g
	TARGET_LIBS = -lopenFrameworksDebug
	OBJ_OUTPUT = obj/Debug/
	SED_OUTPUT = obj\\/Debug\\/
	TARGET_NAME = Debug
	TARGET = bin/$(APPNAME)_debug
	CLEANTARGET = CleanDebug
endif

ifeq ($(findstring Release,$(MAKECMDGOALS)),Release)
	TARGET_CFLAGS = $(COMPILER_OPTIMIZATION)
	TARGET_LIBS = -lopenFrameworks
	OBJ_OUTPUT = obj/Release/
	SED_OUTPUT = obj\\/Release\\/
	TARGET_NAME = Release
	TARGET = bin/$(APPNAME)
	CLEANTARGET = CleanRelease
endif

ifeq ($(MAKECMDGOALS),)
	TARGET_CFLAGS = $(COMPILER_OPTIMIZATION)
	TARGET_LIBS = -lopenFrameworks
	OBJ_OUTPUT = obj/Release/
	SED_OUTPUT = obj\\/Release\\/
	TARGET_NAME = Release
	TARGET = bin/$(APPNAME)
endif

ifeq ($(MAKECMDGOALS),clean)
	TARGET = bin/$(APPNAME)_debug bin/$(APPNAME)
endif

DEPFILES = $(addprefix $(OBJ_OUTPUT),$(patsubst %.cpp,%.d,$(SOURCES)))
OBJS = $(addprefix $(OBJ_OUTPUT), $(OBJFILES))
ifeq ($(findstring addons.make,$(wildcard *.make)),addons.make)
	ADDONS_OBJS = $(addprefix $(OBJ_OUTPUT), $(ADDONS_OBJFILES))
endif

.PHONY: Debug Release after
	
Release: $(TARGET) after

Debug: $(TARGET) after



#ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
#	-include $(DEPFILES)
#endif

#This is the rule for creating the dependency files
$(DEPFILES): $(SOURCES)
	echo "creating dependencies"
	mkdir -p $(@D)
	$(CXX) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -MM -MT $@ `echo $@ | sed "s/$(SED_OUTPUT)//" | sed "s/\.d/\.cpp/"` > $@
	

#This rule does the compilation
$(OBJS): $(SOURCES) $(DEPFILES)
	echo "compiling objects"
	echo $@ | sed "s/$(SED_OUTPUT)//" | sed "s/\.o/\.cpp/"
	mkdir -p $(@D)
	$(CXX) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -o $@ -c `echo $@ | sed "s/$(SED_OUTPUT)//" | sed "s/\.o/\.cpp/"`

$(ADDONS_OBJS): $(ADDONS_SOURCES)
	echo $@ | sed "s/$(SED_OUTPUT)//" | sed "s/\.o/\.cpp/"
	mkdir -p $(@D)
	$(CXX) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) -o $@ -c ../../../`echo $@ | sed "s/$(SED_OUTPUT)//" | sed "s/\.o/\.cpp/"`
	
$(TARGET) : $(OBJS) $(ADDONS_OBJS)
	echo "linking" $(TARGET)
	$(CXX) -o $@ $(OBJS) $(ADDONS_OBJS) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(USER_CFLAGS) $(LDFLAGS) $(USER_LDFLAGS) $(TARGET_LIBS) $(LIBS) $(ADDONSLIBS) $(USER_LIBS)

.PHONY: clean CleanDebug CleanRelease
clean:
	rm -Rf obj
	rm -f -v $(TARGET)
	rm -Rf -v bin/libs
	rm -f -v bin/clickToLaunchApp*
	
$(CLEANTARGET):
	rm -Rf -v $(OBJ_OUTPUT)
	rm -f -v $(TARGET)
	rm -f -v bin/clickToLaunchApp_$(TARGET_NAME).sh


after:
	cp -r ../../../export/$(LIBSPATH)/libs bin/
	cp ../../../export/$(LIBSPATH)/clickToLaunchApp.sh bin/clickToLaunchApp_$(TARGET_NAME).sh
	sed -i s/applicationName/$(APPNAME)/g  bin/clickToLaunchApp_$(TARGET_NAME).sh
	#
	#      compiling done
	#      to launch the app	
	#      cd bin
	#      ./clickToLaunchApp_$(TARGET_NAME).sh
	#
    

.PHONY: help
help:
 
	# openFrameworks universal makefile
	#
	# targets:
	# make Debug:  makes the application with debug symbols
	# make Release: makes the app with optimizations
	# make: the same as make Release
	# make CleanDebug: cleans the Debug target
	# make CleanRelease: cleans the Release target
	# make clean: cleans everything
	#
	# this should work with any OF app, just copy any example
	# change the name of the folder and it should compile
	# only .cpp support, don't use .c files
	# it will look for files in any folder inside the application
	# folder except that in the EXCLUDE_FROM_SOURCE variable
	# but you'll need to add the include paths in USER_CFLAGS
	#
	# to add addons to your application, edit the addons.make file
	# in this directory and add the names of the addons you want to
	# include
	#
