########################################################################
# PROCESS VALID ADDONS IF AVAILABLE
########################################################################

# parses addons includes, in PARSED_ADDON_INCLUDES receives full PATHS to addons
define FUNC_PARSE_ADDON_TEMPLATE_HEADER_SEARCH_PATHS
    $(eval ADDON_TEMPLATE_SRC_PATH := $(addsuffix /src, $1)) \
    \
    $(eval HEADER_SEARCH_PATHS_FROM_SRC := \
        $(shell \
            find $(ADDON_TEMPLATE_SRC_PATH) \
                -not -path "*.framework*" \
                -type d 2> /dev/null \
                | grep -v "/\.[^\.]" \
            )\
    ) \
    \
    $(eval ADDON_TEMPLATE_LIBS_PATH := $(addsuffix /libs, $1)) \
    \
    \
    $(eval ADDON_TEMPLATE_LIBS_SRC_PATHS := \
        $(addsuffix /libs/*/src, $1)) \
    \
    $(eval HEADER_SEARCH_PATHS_FROM_ALL_LIBS_SRC := \
        $(shell \
            find $(ADDON_TEMPLATE_LIBS_SRC_PATHS) \
                -not -path "*.framework*" \
                -type d 2> /dev/null \
                | grep -v "/\.[^\.]" \
        )\
    ) \
    \
    $(eval PARSED_TEMPLATE_LIBS_INCLUDE_PATHS := \
        $(addsuffix /libs/*/include, $1)) \
    \
    \
    $(eval HEADER_SEARCH_PATHS_FROM_ALL_LIBS_INCLUDE := \
        $(shell \
            find $(PARSED_TEMPLATE_LIBS_INCLUDE_PATHS) \
                -not -path "*.framework*" \
                -type d 2> /dev/null \
                | grep -v "/\.[^\.]" \
        )\
    ) \
    \
    $(eval PARSED_ADDON_HEADER_SEARCH_PATHS := $(HEADER_SEARCH_PATHS_FROM_ALL_LIBS_SRC)) \
    $(eval PARSED_ADDON_HEADER_SEARCH_PATHS += $(HEADER_SEARCH_PATHS_FROM_ALL_LIBS_INCLUDE)) \
    $(eval PARSED_ADDON_HEADER_SEARCH_PATHS += $(HEADER_SEARCH_PATHS_FROM_SRC)) 
endef

# parses addons sources, in PARSED_ADDON_SOURCES receives full PATHS to addons
define FUNC_PARSE_ADDON_SOURCES
	$(eval PARSED_ADDONS_SOURCE_PATHS = $(addsuffix /src, $1)) \
	$(eval PARSED_ADDONS_OFX_SOURCES = $(shell find $(PARSED_ADDONS_SOURCE_PATHS) -type f \( -name "*.mm" -or -name "*.m" -or -name "*.cpp" -or -name "*.c" -or -name "*.cc" -or -name "*.cxx" \) 2> /dev/null | grep -v "/\.[^\.]" )) \
	$(eval PARSED_ADDONS_LIBS_SOURCE_PATHS = $(addsuffix /libs, $1)) \
	$(eval PARSED_ADDONS_LIBS_SOURCES = $(shell find $(PARSED_ADDONS_LIBS_SOURCE_PATHS) -type f \( -name "*.mm" -or -name "*.m" -or  -name "*.cpp" -or -name "*.c" -or -name "*.cc" -or -name "*.cxx" \) 2> /dev/null | grep -v "/\.[^\.]"  )) \
	$(eval PARSED_ADDONS_SOURCE_FILES = $(PARSED_ADDONS_LIBS_SOURCES)) \
	$(eval PARSED_ADDONS_SOURCE_FILES += $(PARSED_ADDONS_OFX_SOURCES)) 
endef

# parses addons libraries, in PARSED_ADDON_LIBS receives full PATHS to addons and libs_exclude
define FUNC_PARSE_ADDON_LIBS
	$(eval $(info -----------------------------------ADDON_EXCLUSIONS-----------------------------------))\
    $(eval $(foreach v,$(ADDON_EXCLUSIONS),$(info $(v))))\
    \
	$(eval PARSED_ADDONS_LIBS_PLATFORM_LIB_PATHS = $(filter-out $(ADDON_LIBS_EXCLUDE),$(addsuffix /libs/*/lib/$(ABI_LIB_SUBPATH), $1))) \
	\
	$(eval $(info -----------------------------------PARSED_ADDONS_LIBS_PLATFORM_LIB_PATHS---))\
    $(eval $(foreach v, $(PARSED_ADDONS_LIBS_PLATFORM_LIB_PATHS),$(info $(v))))\
	\
	$(eval PARSED_ALL_PLATFORM_LIBS = $(shell find $(PARSED_ADDONS_LIBS_PLATFORM_LIB_PATHS) -type d 2> /dev/null | grep -v "/\.[^\.]" )) \
	\
	$(eval $(info -----------------------------------PARSED_ALL_PLATFORM_LIBS---))\
    $(eval $(foreach v, $(PARSED_ALL_PLATFORM_LIBS),$(info $(v))))\
	\
	$(eval PARSED_ALL_PLATFORM_FRAMEWORKS = $(shell find $(PARSED_ADDONS_LIBS_PLATFORM_LIB_PATHS) -name *.framework -type d 2> /dev/null | grep -v "/\.[^\.]" )) \
	$(eval $(info -----------------------------------PARSED_ALL_PLATFORM_FRAMEWORKS---))\
    $(eval $(foreach v, $(PARSED_ALL_PLATFORM_FRAMEWORKS),$(info $(v))))\
	\
	\
	$(if $(PARSED_ALL_PLATFORM_LIBS), \
		$(eval PARSED_ADDONS_LIBS_PLATFORM_LIBS_STATICS = $(shell find $(PARSED_ALL_PLATFORM_LIBS) -name *.a 2> /dev/null | grep -v "/\.[^\.]" )) \
		\
		$(eval PARSED_ADDONS_LIBS_PLATFORM_LIBS_SHARED = $(shell find $(PARSED_ALL_PLATFORM_LIBS) -name *.so 2> /dev/null | grep -v "/\.[^\.]" )) \
		$(eval PARSED_ADDONS_LIBS_PLATFORM_LIBS_SHARED += $(shell find $(PARSED_ALL_PLATFORM_LIBS) -name *.dylib 2> /dev/null | grep -v "/\.[^\.]" )) \
		$(eval PARSED_ADDONS_LIBS_PLATFORM_LIBS_SHARED += $(shell find $(PARSED_ALL_PLATFORM_LIBS) -name *.dll 2> /dev/null | grep -v "/\.[^\.]" )) \
		$(eval $(info -----------------------------------PARSED_ADDONS_LIBS_PLATFORM_LIBS_SHARED---))\
    	$(eval $(foreach v, $(PARSED_ADDONS_LIBS_PLATFORM_LIBS_SHARED),$(info $(v))))\
		\
		$(eval PARSED_ADDONS_LIBS = $(PARSED_ADDONS_LIBS_PLATFORM_LIBS_STATICS)) \
		$(eval PARSED_ADDONS_LIBS += $(PARSED_ADDONS_LIBS_PLATFORM_LIBS_SHARED)) \
	)
endef

EMPTY_SPACE :=
EMPTY_SPACE += 

ESCAPED_SPACE := <?--SPACE--?>
ESCAPED_NEW_LINE := <?--NEW_LINE--?>
ESCAPED_DELIMITER := <?--DELIMITER--?>

# TODO:
# is it a good idea to shift addons to the top if the are required by other addons?
#             $(eval PROJECT_ADDON_DEPENDENCIES := $(filter-out $(ADDON_DEPENDENCY),$(PROJECT_ADDON_DEPENDENCIES)))\
#            $(eval PROJECT_ADDON_DEPENDENCIES := $(ADDON_DEPENDENCY) $(PROJECT_ADDON_DEPENDENCIES)) \
# the potential problem is that that addon that gets shifted might then get out of order
# a btter solution is that a given addon can explicitly set the dependency order
#
#
    # $(foreach ADDON_DEPENDENCY,$(strip $(ADDON_DEPENDENCIES)), \
    #     $(if \
    #         $(filter $(ADDON_DEPENDENCY),$(PROJECT_ADDON_DEPENDENCIES))\
    #         ,\
    #         $(eval PROJECT_ADDON_DEPENDENCIES := $(filter-out $(ADDON_DEPENDENCY),$(PROJECT_ADDON_DEPENDENCIES)))\
    #         $(eval PROJECT_ADDON_DEPENDENCIES := $(ADDON_DEPENDENCY) $(PROJECT_ADDON_DEPENDENCIES)) \
    #         ,\
    #         $(eval PROJECT_ADDON_DEPENDENCIES := $(ADDON_DEPENDENCY) $(PROJECT_ADDON_DEPENDENCIES)) \
    #         $(call FUNC_BUILD_ADDON_DEPENDENCY_LIST, $(ADDON_DEPENDENCY)) \
    #     ) \
    # )

define FUNC_INSTALL_ADDON
    $(eval THIS_ADDON:=$(strip $1)) \
    $(error $(THIS_ADDON) is required for this project. You must install $(THIS_ADDON) in your $(PATH_OF_ADDONS) directory to continue)
endef

define FUNC_BUILD_ADDON_DEPENDENCY_LIST
    $(eval THIS_ADDON:=$(strip $1)) \
    $(if $(filter $(THIS_ADDON),$(ALL_INSTALLED_ADDONS)),\
        ,\
        $(call FUNC_INSTALL_ADDON, $(THIS_ADDON))\
        $(eval $(ALL_INSTALLED_ADDONS)+=$(THIS_ADDON))\
    )\
    $(eval PATH_OF_ADDON:=$(addprefix $(PATH_OF_ADDONS)/,$(THIS_ADDON))) \
    $(eval ADDON_DEPENDENCIES:=) \
    $(eval ADDON_DEPENDENCIES_ESCAPED_STRING:=) \
    $(eval PROCESS_NEXT:=0) \
    $(if $(wildcard $(PATH_OF_ADDON)/addon_config.mk), \
        $(foreach VAR_LINE, \
            $(subst $(EMPTY_SPACE),$(ESCAPED_DELIMITER),$(shell cat $(PATH_OF_ADDON)/addon_config.mk | tr '\n' '\t'))\
            ,\
            $(eval UNESCAPED_VAR_LINE:=\
                $(strip \
                    $(subst \
                        $(ESCAPED_DELIMITER),\
                        $(EMPTY_SPACE),\
                        $(VAR_LINE))\
                )\
            )\
            $(if $(filter \
                $(PROCESS_NEXT),1)\
                ,\
                $(eval $(UNESCAPED_VAR_LINE))\
            ) \
            $(if $(filter \
                %:,$(UNESCAPED_VAR_LINE)),\
                $(if $(filter \
                    common:,\
                    $(UNESCAPED_VAR_LINE)),\
                    $(eval PROCESS_NEXT=1),\
                    $(if $(filter \ 
                        $(ABI_LIB_SUBPATH):,\
                        $(UNESCAPED_VAR_LINE)),\
                        $(eval PROCESS_NEXT:=1),\
                        $(eval PROCESS_NEXT:=0)\
                    ) \
                ) \
            ) \
        ) \
    ) \
    \
    \
    $(if $(filter $(THIS_ADDON),$(ADDON_DEPENDENCIES)),\
        $(error Whhhhoa. $(THIS_ADDON) depends on itself!  Please check its addon_config.make file)\
    )\
    \
    $(if $(ADDON_DEPENDENCIES),\
        $(eval ADDON_DEPENDENCIES_ESCAPED_STRING:=\
            $(subst $(EMPTY_SPACE),$(ESCAPED_SPACE),$(THIS_ADDON) $(ADDON_DEPENDENCIES))\
        )\
        $(eval ADDON_DEPENDENCIES_ESCAPED_STRING:=\
            $(addsuffix $(ESCAPED_NEW_LINE),$(ADDON_DEPENDENCIES_ESCAPED_STRING))\
        )\
        $(if $(PROJECT_ADDON_DEPENDENCIES_ESCAPED_STRINGS),\
            $(eval PROJECT_ADDON_DEPENDENCIES_ESCAPED_STRINGS:=\
                $(addsuffix $(ADDON_DEPENDENCIES_ESCAPED_STRING),$(PROJECT_ADDON_DEPENDENCIES_ESCAPED_STRINGS))\
            )\
            ,\
            $(eval PROJECT_ADDON_DEPENDENCIES_ESCAPED_STRINGS:=\
                $(ADDON_DEPENDENCIES_ESCAPED_STRING)\
            )\
        )\
    )\
    \
    $(foreach ADDON_DEPENDENCY,$(strip $(ADDON_DEPENDENCIES)), \
        $(if \
            $(filter $(ADDON_DEPENDENCY),$(PROJECT_ADDON_DEPENDENCIES))\
            ,\
            $(eval PROJECT_ADDON_DEPENDENCIES := $(filter-out $(ADDON_DEPENDENCY),$(PROJECT_ADDON_DEPENDENCIES)))\
            $(eval PROJECT_ADDON_DEPENDENCIES := $(ADDON_DEPENDENCY) $(PROJECT_ADDON_DEPENDENCIES)) \
            ,\
            $(eval PROJECT_ADDON_DEPENDENCIES := $(ADDON_DEPENDENCY) $(PROJECT_ADDON_DEPENDENCIES)) \
            $(call FUNC_BUILD_ADDON_DEPENDENCY_LIST, $(ADDON_DEPENDENCY)) \
        ) \
        \
        \
    )\

endef


# PARSE addon_config.mk FILES
#
# 1. read the addon_config.mk file for each adddon that has it
# 2. read each line of the addon by converting \n to \t since makefiles treat \n as spaces
#    also convert spaces to ? so foreach works for each line instead of each word
# 3. unscape ? to space inside the loop
# 4. if the line matches common: or platform: set the PROCESS_NEXT flag to true
# 5. if the line matches %: but it's not common or platform: set PROCESS_NEXT to false
# 6: if PROCESS_NEXT eval the line to put the variable in the makefile space
define FUNC_PARSE_ADDON 
    $(eval PATH_OF_ADDON:=$(addprefix $(PATH_OF_ADDONS)/, $1)) \
    $(eval ADDON_DEPENDENCIES:= ) \
    $(eval ADDON_DATA:= ) \
    $(eval ADDON_INCLUDES:= ) \
    $(eval ADDON_CFLAGS:= ) \
    $(eval ADDON_LDFLAGS:= ) \
    $(eval ADDON_LIBS:= ) \
    $(eval ADDON_PKG_CONFIG_LIBS:= ) \
    $(eval ADDON_FRAMEWORKS:= ) \
    $(eval ADDON_SOURCES:= ) \
    $(eval ADDON_EXCLUSIONS:= ) \
    $(eval PROCESS_NEXT:=0) \
    $(if $(wildcard $(PATH_OF_ADDON)/addon_config.mk), \
        $(foreach VAR_LINE, $(subst $(EMPTY_SPACE),?,$(shell cat $(PATH_OF_ADDON)/addon_config.mk | tr '\n' '\t')), \
            $(eval UNESCAPED_VAR_LINE:=$(strip $(subst ?, ,$(VAR_LINE)))) \
            $(if $(filter $(PROCESS_NEXT),1), $(eval $(UNESCAPED_VAR_LINE))) \
            $(if $(filter %:,$(UNESCAPED_VAR_LINE)), \
                $(if $(filter common:,$(UNESCAPED_VAR_LINE)), \
                    $(eval PROCESS_NEXT=1), \
                    $(if $(filter $(ABI_LIB_SUBPATH):,$(UNESCAPED_VAR_LINE)), \
                        $(eval PROCESS_NEXT:=1), \
                        $(eval PROCESS_NEXT:=0) \
                    ) \
                ) \
            ) \
        ) \
    ) \
    $(foreach ADDON_DEPENDENCY,$(strip $(ADDON_DEPENDENCIES)), \
        $(if $(filter $(ADDON_DEPENDENCY),$(PROJECT_ADDONS)), , \
            $(eval PROJECT_ADDONS += $(ADDON_DEPENDENCY)) \
            $(call FUNC_PARSE_ADDON, $(ADDON_DEPENDENCY)) \
        ) \
    ) \
    $(if $(strip $(ADDON_EXCLUSIONS)), \
        $(eval PROJECT_ADDONS_EXCLUSIONS += $(ADDON_EXCLUSIONS)), \
    ) \
    $(if $(strip $(ADDON_HEADER_SEARCH_PATHS)), \
        $(eval PROJECT_ADDONS_HEADER_SEARCH_PATHS += $(ADDON_HEADER_SEARCH_PATHS)) \
        $(call FUNC_PARSE_ADDON_TEMPLATE_HEADER_SEARCH_PATHS, $(PATH_OF_ADDON)) \
        $(eval PROJECT_ADDONS_HEADER_SEARCH_PATHS += \
            $(filter-out $(PROJECT_ADDONS_EXCLUSIONS),$(PARSED_ADDON_HEADER_SEARCH_PATHS)))\
        )\
    \
    \
    $(info ---PROJECT_ADDONS_HEADER_SEARCH_PATHS---) \
    $(foreach v, $(PROJECT_ADDONS_HEADER_SEARCH_PATHS),$(info $(v))) \
    \
    \
    $(eval PROJECT_ADDONS_CFLAGS += $(ADDON_CFLAGS)) \
    \
    $(if $(strip $(ADDON_LIBS)), \
        $(eval PROJECT_ADDONS_LIBS += $(addprefix $(PATH_OF_ADDON)/,$(ADDON_LIBS))), \
        $(call PARSE_ADDON_LIBS, $(PATH_OF_ADDON)) \
        $(eval PROJECT_ADDONS_LIBS += $(PARSED_ADDONS_LIBS)) \
    ) \
    \
    $(eval PROJECT_ADDONS_LDFLAGS += $(ADDON_LDFLAGS)) \
    $(eval PROJECT_ADDONS_PKG_CONFIG_LIBS += $(ADDON_PKG_CONFIG_LIBS)) \
    \
    $(eval PROJECT_ADDONS_FRAMEWORKS += $(ADDON_FRAMEWORKS)) \
    \
    $(if $(strip $(ADDON_SOURCES)), \
        $(eval PROJECT_ADDONS_SOURCE_FILES += $(addprefix $(PATH_OF_ADDON)/,$(ADDON_SOURCES))), \
        $(call parse_addons_sources, $(PATH_OF_ADDON)) \
        $(eval PROJECT_ADDONS_SOURCE_FILES += $(PARSED_ADDONS_SOURCE_FILES)) \
    ) \
    $(if $(strip $(ADDON_DATA)), \
        $(eval PROJECT_ADDONS_DATA += $(PATH_OF_ADDON)/$(ADDON_DATA)) \
    )
endef


################################################################################
# ADDON CONFIGURATION
################################################################################
# In this section, we check and see what addons are requested and make an 
# attempt to parse them and prepare them for compilation.  Addons should follow
# the template located here:
# 
#   https://github.com/openFrameworks/ofxAddonTemplate
#
# Files that are included in this standardized structure will be included 
# automatically with this makefile.  Files, assets, etc that do not fit into 
# the standard template should define an addon_config.make file (see the 
# ofxAddonTemplate repository above for more information) to further define
# addon-specific variables, etc.  Further documentation is in the example
# addon_config.make file.
#
# Addons will be compiled for a given project IF an addons.make file listing 
# the requested addons is defined OR the platform-specific configuration file
# has set the PLATFORM_REQUIRED_ADDONS variable.  In either case, the requested
# addons will be validated against the addons located in the PATH_OF_ADDONS 
# folder.
#
################################################################################

# We are unable to do logical ORs in Make, so we set up if/else statements
B_PROCESS_ADDONS = FALSE

ifdef PLATFORM_REQUIRED_ADDONS
    B_PROCESS_ADDONS = TRUE
endif

ifeq ($(findstring addons.make,$(wildcard $(PATH_PROJECT_ROOT)/*.make)),addons.make)
    B_PROCESS_ADDONS = TRUE
endif

ifeq ($(B_PROCESS_ADDONS),TRUE)

################################################################################
# VALIDATE REQUESTED ADDONS ####################################################
################################################################################

################################################################################
# ALL_INSTALLED_ADDONS
#   Create a list of every addon installed in the PATH_OF_ADDONS directory.
#   Remove all paths to leave us with a list of addon names.
################################################################################

    ALL_INSTALLED_ADDONS := \
        $(subst \
            $(PATH_OF_ADDONS)/,\
            ,\
            $(wildcard $(PATH_OF_ADDONS)/*)\
        )

################################################################################
# ALL_REQUESTED_PROJECT_ADDONS (immediately assigned)
#   Create a list of all addons requested in the addons.make file.
#
# Steps:
#
#   1. Use cat to dump the contents of the addons.make file
#
#       cat $(PATH_PROJECT_ROOT)/addons.make 2> /dev/null \ ...
#
#   2. Use sed to strip out all comments beginning with #
#       (NOTE: to escape $ in make, you must use \#)
#
#      | sed 's/[ ]*\#.*//g' \
#
#   3. Use sed to remove any empty lines 
#       (NOTE: to escape $ in make you must use $$)
#
#       | sed '/^$$/d' \
#
################################################################################

    ALL_REQUESTED_PROJECT_ADDONS:=\
        $(shell \
            cat $(PATH_PROJECT_ROOT)/addons.make 2> /dev/null \
            | sed 's/[ ]*\#.*//g' \
            | sed '/^$$/d' \
        )

################################################################################
# ALL_REQUESTED_PROJECT_ADDONS_WITHOUT_PLATFORM_REQUIRED (immediately assigned)
#   Remove any PLATFORM_REQUIRED_ADDONS from ALL_REQUESTED_PROJECT_ADDONS
################################################################################

    ALL_REQUESTED_PROJECT_ADDONS_WITHOUT_PLATFORM_REQUIRED:=\
        $(filter-out \
            $(PLATFORM_REQUIRED_ADDONS),\
            $(ALL_REQUESTED_PROJECT_ADDONS)\
        )

################################################################################
# FUNCTION FUNC_REMOVE_DUPLICATE_ADDONS
#   Define a function to remove duplicates without using $(sort ..), 
#   because ($sort ...) will place the list in lexicographic order, and we 
#   need to respect the user's addons.make order.
#   
#   This function is not for the faint of heart.
#
#   For more information about $(strip ...), $(word, ...), see:
#
#       http://www.gnu.org/software/make/manual/html_node/Text-Functions.html
#
#   For more information about $(call ...) see:
#
#       http://www.gnu.org/software/make/manual/html_node/Call-Function.html
##
################################################################################

    FUNC_REMOVE_DUPLICATE_ADDONS = $(if $1,\
                                        $(strip \
                                            $(word 1,$1) \
                                            $(call $0,$(filter-out \
                                                $(word 1,$1),\
                                                $1))\
                                            )\
                                        )


################################################################################
# ALL_REQUESTED_PROJECT_ADDONS_WITHOUT_PLATFORM_REQUIRED_CLEANED 
#       (immediately assigned)
#   Remove duplicates
################################################################################

    ALL_REQUESTED_PROJECT_ADDONS_WITHOUT_PLATFORM_REQUIRED_CLEANED := \
        $(call FUNC_REMOVE_DUPLICATE_ADDONS,\
            $(ALL_REQUESTED_PROJECT_ADDONS_WITHOUT_PLATFORM_REQUIRED)\
        )

################################################################################
# REQUESTED_PROJECT_ADDONS (immediately assigned)
#   Add platform required addons from the platform-specific configuration file 
#   (if needed) FIRST, so that they are always linked first.
################################################################################

    REQUESTED_PROJECT_ADDONS:=\
        $(PLATFORM_REQUIRED_ADDONS) \
        $(ALL_REQUESTED_PROJECT_ADDONS_WITHOUT_PLATFORM_REQUIRED_CLEANED)

################################################################################
# VALID_REQUESTED_PROJECT_ADDONS (immediately assigned)
#   Compare the list of addons that we have requested to the those that are
#   located in the PATH_OF_ADDONS folder listed in ALL_INSTALLED_ADDONS
################################################################################

    VALID_REQUESTED_PROJECT_ADDONS:=\
        $(filter \
            $(REQUESTED_PROJECT_ADDONS),\
            $(ALL_INSTALLED_ADDONS)\
        )

################################################################################
# INVALID_REQUESTED_PROJECT_ADDONS (immediately assigned)
#   Compare the list of addons that we have requested to the those that are
#   located in the PATH_OF_ADDONS folder listed in ALL_INSTALLED_ADDONS
#
#   If any invalid addons are found, we list them.
#
#   We list the invalid files using $(warning ...) because using $(error ...)
#   immediate stops the make file (which means we won't get a full list of 
#   missing addons).  After listing the files, we conclude with an $(error ...) 
#   causing a full stop.
#
################################################################################

    INVALID_REQUESTED_PROJECT_ADDONS:=\
        $(filter-out \
            $(VALID_REQUESTED_PROJECT_ADDONS),\
            $(REQUESTED_PROJECT_ADDONS)\
        )

    ifneq ($(INVALID_REQUESTED_PROJECT_ADDONS),)
        $(warning The following unknown addons will be ignored:)
        $(foreach v, $(INVALID_REQUESTED_PROJECT_ADDONS),$(warning $(v)))

        $(error Invalid addons listed in addons.make file)
    endif


################################################################################
# PROJECT_ADDONS (immediately assigned)
#   PROJECT_ADDONS is a list of the addons that will be compiled for this
#   project.  Theses addon directories will be parsed and compiled.
################################################################################

    PROJECT_ADDONS:=$(REQUESTED_PROJECT_ADDONS)

    ifdef MAKEFILE_DEBUG
        $(info ---PROJECT_ADDONS---)
        $(foreach v, $(PROJECT_ADDONS),$(info $(v)))
        $(info --------------------)
    endif

    ############################################################################
    # PROCESS PROJECT ADDONS IF THERE ARE ANY
    ############################################################################
    ifneq ($(PROJECT_ADDONS),)
        # $(foreach ADDON_TO_PARSE,\
        #     $(PROJECT_ADDONS),\
        #     $(call FUNC_PARSE_ADDON,$(ADDON_TO_PARSE))\
        # )

        # a list of all dependencies, unordered, listed once    
        PROJECT_ADDON_DEPENDENCIES:=

        # an escaped string list for tsort that needs to be collapsed into new lines
        PROJECT_ADDON_DEPENDENCIES_ESCAPED_STRINGS:=

        PROJECT_ADDON_DEPENDENCIES_ORDERED:=

        $(foreach ADDON_TO_PARSE,\
            $(PROJECT_ADDONS),\
            $(call FUNC_BUILD_ADDON_DEPENDENCY_LIST,$(ADDON_TO_PARSE))\
        )

        PROJECT_ADDON_DEPENDENCIES_ORDERED := \
            $(shell \
                echo "$(PROJECT_ADDON_DEPENDENCIES_ESCAPED_STRINGS)" \
                | sed 's/$(ESCAPED_NEW_LINE)/\ \
                /g' \
                | sed 's/$(ESCAPED_SPACE)/ /g' \
                | tsort \
                | tail -r \
                | tr '\n' ' ' \
            )

        ADDITIONAL_DEPENDENCIES := \
            $(filter-out $(PROJECT_ADDON_DEPENDENCIES_ORDERED),$(PROJECT_ADDON_DEPENDENCIES))

        $(info --------------------ADDITIONAL_DEPENDENCIES)
        $(foreach v, $(ADDITIONAL_DEPENDENCIES),$(info $(v)))


        $(info --------------------PROJECT_ADDON_DEPENDENCIES_ORDERED)
        $(foreach v, $(PROJECT_ADDON_DEPENDENCIES_ORDERED),$(info $(v)))

        
    endif
endif

########################################################################
#  DEBUGGING
########################################################################
# print debug information if so instructed
#ifdef MAKEFILE_DEBUG
    $(info ---PROJECT_ADDONS_INCLUDES---)
    $(foreach v, $(PROJECT_ADDONS_INCLUDES),$(info $(v)))
    $(info ---PROJECT_ADDONS_EXCLUSIONS---)
    $(foreach v, $(PROJECT_ADDONS_EXCLUSIONS),$(info $(v)))
    $(info ---PROJECT_ADDONS_FRAMEWORKS---)
    $(foreach v, $(PROJECT_ADDONS_FRAMEWORKS),$(info $(v)))
    $(info ---PROJECT_ADDONS_SOURCE_FILES---)
    $(foreach v, $(PROJECT_ADDONS_SOURCE_FILES),$(info $(v)))
    $(info ---PROJECT_ADDONS_LIBS---)
    $(foreach v, $(PROJECT_ADDONS_LIBS),$(info $(v)))
    $(info ---PROJECT_ADDONS_OBJFILES---)
    $(foreach v, $(PROJECT_ADDONS_OBJFILES),$(info $(v)))
    $(info ---PROJECT_ADDONS_BASE_CFLAGS---)
    $(foreach v, $(PROJECT_ADDONS_BASE_CFLAGS),$(info $(v)))
    $(info ---PROJECT_ADDONS_DEFINES_CFLAGS---)
    $(foreach v, $(PROJECT_ADDONS_DEFINES_CFLAGS),$(info $(v)))
    $(info ---PROJECT_ADDONS_INCLUDES_CFLAGS---)
    $(foreach v, $(PROJECT_ADDONS_INCLUDES_CFLAGS),$(info $(v)))
    $(info ---PROJECT_ADDONS_LDFLAGS---)
    $(foreach v, $(PROJECT_ADDONS_LDFLAGS),$(info $(v)))
#endif