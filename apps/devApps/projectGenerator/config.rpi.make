# add custom variables to this file

# OF_ROOT allows to move projects outside apps/* just set this variable to the
# absoulte path to the OF root folder

OF_ROOT = ../../..


# USER_CFLAGS allows to pass custom flags to the compiler
# for example search paths like:
# USER_CFLAGS = -I src/objects

USER_CFLAGS = $(addprefix -I, $(shell find src -type d)) 


# USER_LDFLAGS allows to pass custom flags to the linker
# for example libraries like:
# USER_LDFLAGS = libs/libawesomelib.a

USER_LDFLAGS =


EXCLUDE_FROM_SOURCE="bin,.xcodeproj,obj"

# change this to add different compiler optimizations to your project

USER_COMPILER_OPTIMIZATION = 

# uncomment this for custom application name (if the folder name is different than the application name)
#APPNAME = folderName

LINUX_ARM6VL_COMPILER_OPTIMIZATIONS = -march=armv6 -mfpu=vfp -mfloat-abi=hard


