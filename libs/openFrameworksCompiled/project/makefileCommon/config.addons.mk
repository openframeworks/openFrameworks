########################################################################
# PROCESS VALID ADDONS IF AVAILABLE
########################################################################


# parses addons includes, in PARSED_ADDON_INCLUDES receives full PATHS to addons
define parse_addons_includes
	$(eval ADDONS_INCLUDES_FILTER = $(addprefix $1/, $(ADDON_INCLUDES_EXCLUDE))) \
	$(eval PARSED_ADDONS_SOURCE_PATHS = $(addsuffix /src, $1)) \
	$(eval PARSED_ADDONS_SOURCE_INCLUDES = $(shell find $(PARSED_ADDONS_SOURCE_PATHS) -type d 2> /dev/null | grep -v "/\.[^\.]" )) \
	$(eval PARSED_ADDONS_FILTERED_INCLUDE_PATHS = $(filter-out $(ADDONS_INCLUDES_FILTER),$(PARSED_ADDONS_SOURCE_INCLUDES))) \
	$(eval PARSED_ADDONS_LIBS_SOURCE_PATHS = $(addsuffix /libs, $1)) \
	$(eval PARSED_ADDONS_LIBS_SOURCE_INCLUDES = $(shell find $(PARSED_ADDONS_LIBS_SOURCE_PATHS) -type d 2> /dev/null | grep -v "/\.[^\.]" )) \
	$(eval PARSED_ADDONS_FILTERED_LIBS_SOURCE_INCLUDE_PATHS = $(filter-out $(ADDONS_INCLUDES_FILTER),$(PARSED_ADDONS_LIBS_SOURCE_INCLUDES))) \
	$(eval PARSED_ADDONS_LIBS_INCLUDES_PATHS = $(addsuffix /libs/*/include, $1)) \
	$(eval PARSED_ADDONS_LIBS_INCLUDES = $(shell find $(PARSED_ADDONS_LIBS_INCLUDES_PATHS) -type d 2> /dev/null | grep -v "/\.[^\.]"  )) \
	$(eval PARSED_ADDONS_FILTERED_LIBS_INCLUDE_PATHS = $(filter-out $(ADDONS_INCLUDES_FILTER),$(PARSED_ADDONS_LIBS_INCLUDES))) \
	$(eval PARSED_ADDONS_INCLUDES = $(PARSED_ADDONS_FILTERED_INCLUDE_PATHS)) \
	$(eval PARSED_ADDONS_INCLUDES += $(PARSED_ADDONS_FILTERED_LIBS_SOURCE_INCLUDE_PATHS)) \
	$(eval PARSED_ADDONS_INCLUDES += $(PARSED_ADDONS_FILTERED_LIBS_INCLUDE_PATHS)) 
endef

# parses addons sources, in PARSED_ADDON_SOURCES receives full PATHS to addons
define parse_addons_sources
	$(eval ADDONS_SOURCES_FILTER = $(addprefix $1/, $(ADDON_SOURCES_EXCLUDE))) \
	$(eval PARSED_ADDONS_SOURCE_PATHS = $(addsuffix /src, $1)) \
	$(eval PARSED_ADDONS_OFX_SOURCES = $(shell find $(PARSED_ADDONS_SOURCE_PATHS) -type f \( -name "*.cpp" -or -name "*.c" -or -name "*.cc" -or -name "*.cxx" \) 2> /dev/null | grep -v "/\.[^\.]" )) \
	$(eval PARSED_ADDONS_FILTERED_SOURCE_PATHS = $(filter-out $(ADDONS_SOURCES_FILTER),$(PARSED_ADDONS_OFX_SOURCES))) \
	$(eval PARSED_ADDONS_LIBS_SOURCE_PATHS = $(addsuffix /libs, $1)) \
	$(eval PARSED_ADDONS_LIBS_SOURCES = $(shell find $(PARSED_ADDONS_LIBS_SOURCE_PATHS) -type f \( -name "*.cpp" -or -name "*.c" -or -name "*.cc" -or -name "*.cxx" \) 2> /dev/null | grep -v "/\.[^\.]"  )) \
	$(eval PARSED_ADDONS_FILTERED_LIBS_SOURCE_PATHS = $(filter-out $(ADDONS_SOURCES_FILTER),$(PARSED_ADDONS_LIBS_SOURCES))) \
	$(eval PARSED_ADDONS_SOURCE_FILES = $(PARSED_ADDONS_FILTERED_SOURCE_PATHS)) \
	$(eval PARSED_ADDONS_SOURCE_FILES += $(PARSED_ADDONS_FILTERED_LIBS_SOURCE_PATHS)) 
endef

# parses addons libraries, in PARSED_ADDON_LIBS receives full PATHS to addons and libs_exclude
define parse_addons_libraries
	$(eval PARSED_ADDONS_LIBS_PLATFORM_LIB_PATHS = $(filter-out $(ADDON_LIBS_EXCLUDE),$(addsuffix /libs/*/lib/$(ABI_LIB_SUBPATH), $1))) \
	$(eval PARSED_ALL_PLATFORM_LIBS = $(shell find $(PARSED_ADDONS_LIBS_PLATFORM_LIB_PATHS) -type d 2> /dev/null | grep -v "/\.[^\.]" )) \
	$(if $(PARSED_ALL_PLATFORM_LIBS), \
		$(eval PARSED_ADDONS_LIBS_PLATFORM_LIBS_STATICS = $(shell find $(PARSED_ALL_PLATFORM_LIBS) -name *.a 2> /dev/null | grep -v "/\.[^\.]" )) \
		$(eval PARSED_ADDONS_LIBS_PLATFORM_LIBS_SHARED = $(shell find $(PARSED_ALL_PLATFORM_LIBS) -name *.so 2> /dev/null | grep -v "/\.[^\.]" )) \
		$(eval PARSED_ADDONS_LIBS_PLATFORM_LIBS_SHARED += $(shell find $(PARSED_ALL_PLATFORM_LIBS) -name *.dylib 2> /dev/null | grep -v "/\.[^\.]" )) \
		$(eval PARSED_ADDONS_LIBS_PLATFORM_LIBS_SHARED += $(shell find $(PARSED_ALL_PLATFORM_LIBS) -name *.dll 2> /dev/null | grep -v "/\.[^\.]" )) \
		$(eval PARSED_ADDONS_LIBS = $(PARSED_ADDONS_LIBS_PLATFORM_LIBS_STATICS)) \
		$(eval PARSED_ADDONS_LIBS += $(PARSED_ADDONS_LIBS_PLATFORM_LIBS_SHARED)) \
	)
endef


space :=
space += 


# PARSE addon_config.mk FILES
#
# 1. read the addon_config.mk file for each adddon that has it
# 2. read each line of the addon by converting \n to \t since makefiles treat \n as spaces
#    also convert spaces to ? so foreach works for each line instead of each word
# 3. unscape ? to space inside the loop
# 4. if the line matches common: or platform: set the PROCESS_NEXT flag to true
# 5. if the line matches %: but it's not common or platform: set PROCESS_NEXT to false
# 6: if PROCESS_NEXT eval the line to put the variable in the makefile space
define parse_addon
	$(eval addon=$(addprefix $(OF_ADDONS_PATH)/, $1)) \
	$(eval ADDON_DEPENDENCIES= ) \
	$(eval ADDON_DATA= ) \
	$(eval ADDON_INCLUDES= ) \
	$(eval ADDON_CFLAGS= ) \
	$(eval ADDON_LDFLAGS= ) \
	$(eval ADDON_LIBS= ) \
	$(eval ADDON_PKG_CONFIG_LIBRARIES= ) \
	$(eval ADDON_FRAMEWORKS= ) \
	$(eval ADDON_SOURCES= ) \
	$(eval ADDON_LIBS_EXCLUDE= ) \
	$(eval ADDON_SOURCES_EXCLUDE= ) \
	$(eval PROCESS_NEXT=0) \
	$(if $(wildcard $(addon)/addon_config.mk), \
		$(foreach var_line, $(subst $(space),?,$(shell cat $(addon)/addon_config.mk | tr '\n' '\t')), \
			$(eval unscaped_var_line=$(strip $(subst ?, ,$(var_line)))) \
			$(if $(filter $(PROCESS_NEXT),1), $(eval $(unscaped_var_line))) \
			$(if $(filter %:,$(unscaped_var_line)), \
				$(if $(filter common:,$(unscaped_var_line)), \
					$(eval PROCESS_NEXT=1), \
					$(if $(filter $(ABI_LIB_SUBPATH):,$(unscaped_var_line)), \
						$(eval PROCESS_NEXT=1), \
						$(eval PROCESS_NEXT=0) \
					) \
				) \
			) \
		) \
	) \
	$(if $(strip $(ADDON_INCLUDES)), \
		$(eval PROJECT_ADDONS_INCLUDES += $(addprefix $(addon)/,$(ADDON_INCLUDES))), \
		$(call parse_addons_includes, $(addon)) \
		$(eval PROJECT_ADDONS_INCLUDES += $(PARSED_ADDONS_INCLUDES)) \
	) \
	$(eval PROJECT_ADDONS_CFLAGS += $(ADDON_CFLAGS)) \
	$(if $(strip $(ADDON_LIBS)), \
		$(eval PROJECT_ADDONS_LIBS += $(addprefix $(addon)/,$(ADDON_LIBS))), \
		$(call parse_addons_libraries, $(addon)) \
		$(eval PROJECT_ADDONS_LIBS += $(PARSED_ADDONS_LIBS)) \
	) \
	$(eval PROJECT_ADDONS_LDFLAGS += $(ADDON_LDFLAGS)) \
	$(eval PROJECT_ADDONS_PKG_CONFIG_LIBRARIES += $(ADDON_PKG_CONFIG_LIBRARIES)) \
	$(eval PROJECT_ADDONS_FRAMEWORKS += $(ADDON_FRAMEWORKS)) \
	$(if $(strip $(ADDON_SOURCES)), \
		$(eval PROJECT_ADDONS_SOURCE_FILES += $(addprefix $(addon)/,$(ADDON_SOURCES))), \
		$(call parse_addons_sources, $(addon)) \
		$(eval PROJECT_ADDONS_SOURCE_FILES += $(PARSED_ADDONS_SOURCE_FILES)) \
	) \
	$(if $(strip $(ADDON_DATA)), \
		$(eval PROJECT_ADDONS_DATA += $(addon)/$(ADDON_DATA)) \
	) \
	$(foreach addon_dep, $(strip $(ADDON_DEPENDENCIES)), \
		$(if $(filter $(addon_dep),$(PROJECT_ADDONS)), , \
			$(eval PROJECT_ADDONS += $(addon_dep)) \
			$(call parse_addon, $(addon_dep)) \
		) \
	) 
endef


$(foreach addon_to_parse, $(PROJECT_ADDONS), \
	$(call parse_addon, $(addon_to_parse)) \
)

########################################################################
#  DEBUGGING
########################################################################
# print debug information if so instructed
ifdef MAKEFILE_DEBUG
    $(info ---PROJECT_ADDONS_PATHS---)
    $(foreach v, $(PROJECT_ADDONS_PATHS),$(info $(v)))
    $(info ---PROJECT_ADDONS_WITH_CONFIG---)
    $(foreach v, $(PROJECT_ADDONS_WITH_CONFIG),$(info $(v)))
    $(info ---PROJECT_ADDONS_INCLUDES---)
    $(foreach v, $(PROJECT_ADDONS_INCLUDES),$(info $(v)))
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
endif