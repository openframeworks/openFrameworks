
# check to make sure the platform's OS is defined
ifndef PLATFORM_OS
    $(error PLATFORM_OS is not defined)
endif

# check to make sure the platform's architecture is defined
ifndef PLATFORM_ARCH
    $(error PLATFORM_ARCH is not defined)
endif

# if not defined yet, construct the default PLATFORM_LIBS_PATH
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
endif

ifdef MAKEFILE_DEBUG
    $(info =============================config.libspath==================================)
    $(info PLATFORM_LIB_SUBPATH=$(PLATFORM_LIB_SUBPATH))
endif
