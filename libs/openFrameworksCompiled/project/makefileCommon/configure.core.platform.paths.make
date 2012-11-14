
# check to make sure the platform's OS is defined
ifndef PLATFORM_OS
    $(error PLATFORM_OS is not defined)
endif

# check to make sure the platform's architecture is defined
ifndef PLATFORM_ARCH
    $(error PLATFORM_ARCH is not defined)
endif

# check to make sure the platform's library subpath is defined
ifndef PLATFORM_LIB_SUBPATH
    $(error PLATFORM_LIB_SUBPATH is not defined)
endif

ifndef OF_LIBS_OF_COMPILED_PATH
    $(error OF_LIBS_OF_COMPILED_PATH is not defined)
endif

ifndef OF_LIBS_OF_COMPILED_PROJECT_PATH
    $(error OF_LIBS_OF_COMPILED_PROJECT_PATH is not defined)
endif

################################################################################
# make an entry like the shared makefiles
ifndef OF_PLATFORM_MAKEFILES
    OF_PLATFORM_MAKEFILES=$(OF_LIBS_OF_COMPILED_PROJECT_PATH)/$(PLATFORM_LIB_SUBPATH)
endif

ifndef OF_CORE_LIB_PATH
    OF_CORE_LIB_PATH=$(OF_LIBS_OF_COMPILED_PATH)/lib/$(PLATFORM_LIB_SUBPATH)
endif