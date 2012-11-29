################################################################################
#   This file will generate essential path variables including: 
#
#   $(OF_ADDONS_PATH)
#   $(OF_EXPORT_PATH)
#   $(OF_EXAMPLES_PATH)
#   $(OF_APPS_PATH)
#   $(OF_LIBS_PATH)
#
#   $(OF_LIBS_OPENFRAMEWORKS_PATH)
#   $(OF_LIBS_OF_COMPILED_PATH)
#   $(OF_LIBS_OF_COMPILED_PROJECT_PATH)
#   $(OF_MAKEFILE_INCLUDES_PATH)
#
#   $(OF_ROOT) must be defined previously.
################################################################################

################################################################################
# define the of root if it has not been defined in a higher level file
ifndef OF_ROOT
    OF_ROOT=../../..
endif

################################################################################
# create path definitions
ifndef OF_ADDONS_PATH
    OF_ADDONS_PATH=$(OF_ROOT)/addons
endif
ifndef OF_EXPORT_PATH
    OF_EXPORT_PATH=$(OF_ROOT)/export
endif
ifndef OF_EXAMPLES_PATH
    OF_EXAMPLES_PATH=$(OF_ROOT)/examples
endif
ifndef OF_APPS_PATH
    OF_APPS_PATH=$(OF_ROOT)/apps
endif
ifndef OF_LIBS_PATH
    OF_LIBS_PATH=$(OF_ROOT)/libs
endif

################################################################################
ifndef OF_LIBS_OPENFRAMEWORKS_PATH
    OF_LIBS_OPENFRAMEWORKS_PATH=$(OF_LIBS_PATH)/openFrameworks
endif
ifndef OF_LIBS_OF_COMPILED_PATH
    OF_LIBS_OF_COMPILED_PATH=$(OF_LIBS_OPENFRAMEWORKS_PATH)Compiled
endif
ifndef OF_LIBS_OF_COMPILED_PROJECT_PATH
    OF_LIBS_OF_COMPILED_PROJECT_PATH=$(OF_LIBS_OF_COMPILED_PATH)/project
endif
ifndef OF_SHARED_MAKEFILES_PATH
    OF_SHARED_MAKEFILES_PATH=$(OF_LIBS_OF_COMPILED_PROJECT_PATH)/makefileCommon
endif

################################################################################
# print debug information if needed
ifdef MAKEFILE_DEBUG
    $(info ===================configure.core.paths.make========================)
    $(info OF_ADDONS_PATH=$(OF_ADDONS_PATH))
    $(info OF_EXPORT_PATH=$(OF_EXPORT_PATH))
    $(info OF_EXAMPLES_PATH=$(OF_EXAMPLES_PATH))
    $(info OF_APPS_PATH=$(OF_APPS_PATH))
    $(info OF_LIBS_PATH=$(OF_LIBS_PATH))
    $(info OF_LIBS_OPENFRAMEWORKS_PATH=$(OF_LIBS_OPENFRAMEWORKS_PATH))
    $(info OF_LIBS_OF_COMPILED_PATH=$(OF_LIBS_OF_COMPILED_PATH))
    $(info OF_LIBS_OF_COMPILED_PROJECT_PATH=$(OF_LIBS_OF_COMPILED_PROJECT_PATH))
    $(info OF_SHARED_MAKEFILES_PATH=$(OF_SHARED_MAKEFILES_PATH))
endif

