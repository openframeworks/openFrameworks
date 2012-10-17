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


ifndef ARCH
    ARCH = $(shell uname -m)
endif

# set up host platform for android
ifeq ($(findstring Android,$(MAKECMDGOALS)),Android)
    include paths.make
    ARCH = android
    ifeq ($(shell uname),Darwin)
        HOST_PLATFORM = darwin-x86
    else
        HOST_PLATFORM = linux-x86
    endif
endif

ifeq ($(ARCH),android)
    COMPILER_OPTIMIZATION = -Os
    NDK_PLATFORM = android-8
    GCC_VERSION = 4.4.3
else ifeq ($(ARCH),armv7l)
    COMPILER_OPTIMIZATION = -march=armv7-a -mtune=cortex-a8 -finline-functions -funroll-all-loops  -O3 -funsafe-math-optimizations -mfpu=neon -ftree-vectorize -mfloat-abi=softfp
else ifeq ($(ARCH),armv7l)
    # From http://www.raspbian.org/RaspbianFAQ
    COMPILER_OPTIMIZATION = -march=armv6 -mfpu=vfp -mfloat-abi=hard
else
    COMPILER_OPTIMIZATION = -march=native -mtune=native -finline-functions -funroll-all-loops  -Os
endif

USER_CFLAGS = 





# you shouldn't modify anything below this line


