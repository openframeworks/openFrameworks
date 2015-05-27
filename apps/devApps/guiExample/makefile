# import all of the project configurations
include config.make
#MAKEFILE_DEBUG=1

ifndef OF_ROOT
	OF_ROOT=../../..
endif

OF_SHARED_MAKEFILES_PATH=$(OF_ROOT)/libs/openFrameworksCompiled/project/makefileCommon

# get the configuration
include $(OF_SHARED_MAKEFILES_PATH)/configure.core.make


include $(OF_SHARED_MAKEFILES_PATH)/configure.project.flags.make


# # print debug information if needed
# ifdef MAKEFILE_DEBUG
#    $(info ---OF_CORE_INCLUDES_CFLAGS--- )
#     $(foreach v, $(OF_CORE_INCLUDES_CFLAGS),$(info $(v)))
#     $(info ---OF_CORE_STATIC_LIB_CFLAGS--- )
#     $(foreach v, $(OF_CORE_STATIC_LIB_CFLAGS),$(info $(v)))
#     $(info ---OF_CORE_SHARED_LIB_CFLAGS--- )
#     $(foreach v, $(OF_CORE_SHARED_LIB_CFLAGS),$(info $(v)))
#     $(info ---OF_CORE_PKG_CONFIG_LIB_CFLAGS--- )
#     $(foreach v, $(OF_CORE_PKG_CONFIG_LIB_CFLAGS),$(info $(v)))
# endif



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


 include $(OF_SHARED_MAKEFILES_PATH)/compile.project.make









