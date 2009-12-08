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


# you shouldn't touch anything below this line


SHELL =  /bin/sh
CXX =  g++

NODEPS = clean
SOURCE_DIRS = $(shell find . -maxdepth 1 -mindepth 1 -type d | grep -v $(EXCLUDE_FROM_SOURCE) | sed s/.\\///)
SOURCES = $(shell find $(SOURCE_DIRS) -name "*.cpp")
OBJFILES = $(patsubst %.cpp,%.o,$(SOURCES))
APPNAME = $(shell basename `pwd`)

ARCH = $(shell uname -m)
ifeq ($(ARCH),x86_64)
	LIBSPATH=linux64
else
	LIBSPATH=linux
endif

CFLAGS = -Wall -fexceptions
CFLAGS += -I. -I../../../addons/ -I../../../libs/openFrameworks/ -I../../../libs/openFrameworks/events -I../../../libs/openFrameworks/app -I../../../libs/openFrameworks/communication -I../../../libs/openFrameworks/graphics -I../../../libs/openFrameworks/video -I../../../libs/openFrameworks/utils -I../../../libs/openFrameworks/sound -I../../../libs/rtAudio/include -I../../../libs/fmodex/inc -I../../../libs/glee/include -I../../../libs/glee/ -I../../../libs/FreeImage/include -I../../../libs/freetype/include -I../../../libs/freetype/include/freetype2 -I../../../libs/unicap/include -I../../../libs/gstappsink/include -I../../../addons -I../../../libs/poco/include
CFLAGS +=`pkg-config  gstreamer-0.10 gstreamer-video-0.10 gstreamer-base-0.10 libudev --cflags`

LDFLAGS = -L../../../libs/freetype/lib/$(LIBSPATH) -L../../../libs/FreeImage/lib/$(LIBSPATH) -L../../../libs/rtAudio/lib/$(LIBSPATH) -L../../../libs/glee/lib/$(LIBSPATH) -L../../../libs/gstappsink/lib/$(LIBSPATH) -L../../../libs/fmodex/lib/$(LIBSPATH) -L../../../libs/unicap/lib/$(LIBSPATH) -L../../../libs/poco/lib/$(LIBSPATH) -L../../../libs/openFrameworksCompiled/lib/$(LIBSPATH) -s

ifeq ($(findstring addons.make,$(wildcard *.make)),addons.make)
	ADDONSCFLAGS = -I../../../addons/ofx3DModelLoader/src/3DS/ -I../../../addons/ofx3DModelLoader/src/ -I../../../addons/ofxNetwork/src -I../../../addons/ofxOsc/src -I../../../addons/ofxOsc/libs/oscpack/include/ip -I../../../addons/ofxOsc/libs/oscpack/include/osc -I../../../addons/ofxVectorMath/src -I../../../addons/ofxVectorGraphics/src -I../../../addons/ofxVectorGraphics/libs -I../../../addons/ofxOpenCv/src -I../../../addons/ofxOpenCv/libs/opencv/include -I../../../addons/ofxThread/src -I../../../addons/ofxDirList/src -I../../../addons/ofxXmlSettings/src -I../../../addons/ofxXmlSettings/libs

	ADDONSLIBS = ../../../addons/ofxOsc/libs/oscpack/lib/$(LIBSPATH)/liboscpack.a ../../../addons/ofxOpenCv/libs/opencv/lib/$(LIBSPATH)/libcv.a ../../../addons/ofxOpenCv/libs/opencv/lib/$(LIBSPATH)/libcxcore.a

	ADDONS = $(shell cat addons.make)
	ADDONS_REL_DIRS = $(addsuffix /src, $(ADDONS))
	ADDONS_LIBS_REL_DIRS = $(addsuffix /libs, $(ADDONS))
	ADDONS_DIRS = $(addprefix ../../../addons/, $(ADDONS_REL_DIRS) )
	ADDONS_LIBS_DIRS = $(addprefix ../../../addons/, $(ADDONS_LIBS_REL_DIRS) )
	ADDONS_SOURCES = $(shell find $(ADDONS_DIRS) -name "*.cpp")
	ADDONS_SOURCES += $(shell find $(ADDONS_LIBS_DIRS) -name "*.cpp" 2>/dev/null)
	ADDONS_OBJFILES = $(subst ../../../, ,$(patsubst %.cpp,%.o,$(ADDONS_SOURCES)))
endif

LIBS = -lfmodex64-4.22.00 -lraw1394 -lglut -lGL -lGLU -lXxf86vm -lasound -lPocoFoundation -lunicap -lfreetype -lfreeimage -lRtAudio -lGLee -loFappsink
LIBS +=`pkg-config  gstreamer-0.10 gstreamer-video-0.10 gstreamer-base-0.10 libudev --libs`

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
	$(CXX) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) -MM -MT $@ `echo $@ | sed "s/$(SED_OUTPUT)//" | sed "s/\.d/\.cpp/"` > $@
	

#This rule does the compilation
$(OBJS): $(SOURCES) $(DEPFILES)
	echo "compiling objects"
	echo $@ | sed "s/$(SED_OUTPUT)//" | sed "s/\.o/\.cpp/"
	mkdir -p $(@D)
	$(CXX) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) -o $@ -c `echo $@ | sed "s/$(SED_OUTPUT)//" | sed "s/\.o/\.cpp/"`

$(ADDONS_OBJS): $(ADDONS_SOURCES)
	echo $@ | sed "s/$(SED_OUTPUT)//" | sed "s/\.o/\.cpp/"
	mkdir -p $(@D)
	$(CXX) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) -o $@ -c ../../../`echo $@ | sed "s/$(SED_OUTPUT)//" | sed "s/\.o/\.cpp/"`
	
$(TARGET) : $(OBJS) $(ADDONS_OBJS)
	echo "linking" $(TARGET)
	$(CXX) -o $@ $(OBJS) $(ADDONS_OBJS) $(TARGET_CFLAGS) $(CFLAGS) $(ADDONSCFLAGS) $(LDFLAGS) $(TARGET_LIBS) $(LIBS) $(ADDONSLIBS)

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
