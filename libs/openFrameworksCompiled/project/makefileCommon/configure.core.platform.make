##########################################################################################
#   If they haven't already been defined, this file will generate the following variables 
#
#   $(PLATFORM_OS) (e.g. Linux, Darwin, etc.).
#   $(PLATFORM_ARCH) (e.g. armv6l, x86_64, i386, etc.).
#   $(PLATFORM_LIB_SUBPATH) (e.g. linuxarmv6l, osx, linux64, linux, etc)
##########################################################################################

# if it is not already defined, determine this platform's operating system via uname -s
ifndef PLATFORM_OS 
    # determine from the uname if not defined manually
    PLATFORM_OS=$(shell uname -s)
endif

# if it is not already defined, determine this platform's architecture via uname -m
ifndef PLATFORM_ARCH
    # determine from the uname
    PLATFORM_ARCH=$(shell uname -m)
endif

# if not defined yet, construct the default PLATFORM_LIB_SUBPATH
ifndef PLATFORM_LIB_SUBPATH
    # determine from the arch
    ifeq ($(PLATFORM_OS),Linux)
        ifeq ($(PLATFORM_ARCH),x86_64)
            PLATFORM_LIB_SUBPATH=linux64
        else ifeq ($(PLATFORM_ARCH),armv6l)
            PLATFORM_LIB_SUBPATH=linuxarmv6l
        else ifeq ($(PLATFORM_ARCH),i386)
            PLATFORM_LIB_SUBPATH=linux
        else
            $(error This makefile does not support your architecture)
        endif
    else ifeq ($(PLATFORM_OS),android)
        PLATFORM_LIB_SUBPATH=android
    else ifeq ($(PLATFORM_OS),Darwin)
        PLATFORM_LIB_SUBPATH=osx
    else
        $(error This makefile does not support your operating system)
    endif

    ifdef OF_LIBS_OF_COMPILED_PROJECT_PATH
        OF_PLATFORM_MAKEFILES=$(OF_LIBS_OF_COMPILED_PROJECT_PATH)/$(PLATFORM_LIB_SUBPATH)
    else 
        $(error OF_LIBS_OF_COMPILED_PATH is not defined)
    endif
endif

# TODO: add appropriate list of platform suffixes
# these variables will actually be used during compilation
# http://en.wikipedia.org/wiki/Library_(computing)#File_naming 
#ifndef PLATFORM_SHARED_LIB_SUFFIXES
# i.e. if osx check for .dylib AND .so
# if linux, just look for so
#endif
#ifndef PLATFORM_STATIC_LIB_SUFFIXES
#endif

# if desired, print the variables
ifdef MAKEFILE_DEBUG
    $(info =============================configure.platform.make==========================)
    $(info PLATFORM_ARCH=$(PLATFORM_ARCH))
    $(info PLATFORM_OS=$(PLATFORM_OS))
    $(info PLATFORM_LIB_SUBPATH=$(PLATFORM_LIB_SUBPATH))
    $(info OF_PLATFORM_MAKEFILES=$(OF_PLATFORM_MAKEFILES))
endif