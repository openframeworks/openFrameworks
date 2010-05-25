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


COMPILER_OPTIMIZATION = -march=native -mtune=native -finline-functions -funroll-all-loops  -O3
USER_CFLAGS = 





# you shouldn't modify anything below this line




SHELL =  /bin/sh
CXX =  g++

ARCH = $(shell uname -m)
ifeq ($(ARCH),x86_64)
	LIBSPATH=linux64
else
	LIBSPATH=linux
endif

SOURCE_DIRS = $(shell find ../../../openFrameworks -maxdepth 1 -mindepth 1 -type d )
SOURCES = $(shell find $(SOURCE_DIRS) -name "*.cpp")
OBJFILES = $(patsubst ../../../%.cpp,%.o,$(SOURCES))
CORE_INCLUDES = $(shell find ../../../openFrameworks/ -type d)
CORE_INCLUDE_FLAGS = $(addprefix -I,$(CORE_INCLUDES))
INCLUDES = $(shell find ../../../*/include -type d)
INCLUDES_FLAGS = $(addprefix -I,$(INCLUDES))

CFLAGS = -Wall -fexceptions
CFLAGS += $(INCLUDES_FLAGS)
CFLAGS += $(CORE_INCLUDE_FLAGS)
CFLAGS +=`pkg-config  gstreamer-0.10 gstreamer-video-0.10 gstreamer-base-0.10 libudev libavcodec libavformat libavutil --cflags`

ifeq ($(findstring Debug,$(MAKECMDGOALS)),Debug)
	TARGET_CFLAGS = -g3
	TARGET_NAME = Debug
	TARGET = ../../lib/$(LIBSPATH)/libopenFrameworksDebug.a
endif

ifeq ($(findstring Release,$(MAKECMDGOALS)),Release)
	TARGET_CFLAGS = $(COMPILER_OPTIMIZATION)
	TARGET_NAME = Release
	TARGET = ../../lib/$(LIBSPATH)/libopenFrameworks.a
endif

ifeq ($(MAKECMDGOALS),)
	TARGET_CFLAGS = $(COMPILER_OPTIMIZATION)
	TARGET_NAME = Release
	TARGET = ../../lib/$(LIBSPATH)/libopenFrameworks.a
endif

ifeq ($(MAKECMDGOALS),clean)
	TARGET = ../../lib/$(LIBSPATH)/libopenFrameworks.a ../../lib/$(LIBSPATH)/libopenFrameworksDebug.a
endif

CLEANTARGET = $(addprefix Clean,$(TARGET_NAME))
OBJ_OUTPUT = obj/$(TARGET_NAME)/
DEPFILES = $(addprefix $(OBJ_OUTPUT),$(patsubst ../../../%.cpp,%.d,$(SOURCES)))
OBJS = $(addprefix $(OBJ_OUTPUT), $(OBJFILES))

.PHONY: all Debug Release after clean CleanDebug CleanRelease help

Release: $(TARGET) 

Debug: $(TARGET) 

all: 
	make Debug
	make Release
	

#This rule does the compilation
$(OBJ_OUTPUT)%.o: ../../../%.cpp 
	@echo "compiling object for " $<
	mkdir -p $(@D)
	$(CXX) $(TARGET_CFLAGS) $(CFLAGS) $(USER_CFLAGS) -MMD -MP -MF$(OBJ_OUTPUT)$*.d -MT$(OBJ_OUTPUT)$*.d -o $@ -c $<

$(TARGET) : $(OBJS) 
	echo "creating " $(TARGET)
	ar cr $@ $(OBJS)

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
	@echo "make CleanDebug:	cleans the Debug target"
	@echo "make CleanRelease:	cleans the Release target"
	@echo "make clean:		cleans everything"
	@echo
