########################################################################
# PROCESS VALID ADDONS IF AVAILABLE
########################################################################


# parses addons includes, in PARSED_ADDON_INCLUDES receives full PATHS to addons
define parse_addons_includes
	$(eval ADDONS_INCLUDES_FILTER = $(addprefix $1/, $(ADDON_INCLUDES_EXCLUDE))) \
	$(eval PARSED_ADDONS_SOURCE_PATHS = $(addsuffix /src, $1)) \
	$(eval PARSED_ADDONS_SOURCE_INCLUDES = $(shell $(FIND) $(PARSED_ADDONS_SOURCE_PATHS) -type d 2> /dev/null | grep -v "/\.[^\.]" )) \
	$(eval PARSED_ADDONS_FILTERED_INCLUDE_PATHS = $(filter-out $(ADDONS_INCLUDES_FILTER),$(PARSED_ADDONS_SOURCE_INCLUDES))) \
	$(eval PARSED_ADDONS_LIBS_SOURCE_PATHS = $(addsuffix /libs, $1)) \
	$(eval PARSED_ADDONS_LIBS_SOURCE_INCLUDES = $(shell $(FIND) $(PARSED_ADDONS_LIBS_SOURCE_PATHS) -type d 2> /dev/null | grep -v "/\.[^\.]" )) \
	$(eval PARSED_ADDONS_FILTERED_LIBS_SOURCE_INCLUDE_PATHS = $(filter-out $(ADDONS_INCLUDES_FILTER),$(PARSED_ADDONS_LIBS_SOURCE_INCLUDES))) \
	$(eval PARSED_ADDONS_LIBS_INCLUDES_PATHS = $(addsuffix /libs/*/include, $1)) \
	$(eval PARSED_ADDONS_LIBS_INCLUDES = $(shell $(FIND) $(PARSED_ADDONS_LIBS_INCLUDES_PATHS) -type d 2> /dev/null | grep -v "/\.[^\.]"  )) \
	$(eval PARSED_ADDONS_FILTERED_LIBS_INCLUDE_PATHS = $(filter-out $(ADDONS_INCLUDES_FILTER),$(PARSED_ADDONS_LIBS_INCLUDES))) \
	$(eval PARSED_ADDONS_INCLUDES = $(PARSED_ADDONS_FILTERED_INCLUDE_PATHS)) \
	$(eval PARSED_ADDONS_INCLUDES += $(PARSED_ADDONS_FILTERED_LIBS_SOURCE_INCLUDE_PATHS)) \
	$(eval PARSED_ADDONS_INCLUDES += $(PARSED_ADDONS_FILTERED_LIBS_INCLUDE_PATHS))
endef

# parses addons sources, in PARSED_ADDON_SOURCES receives full PATHS to addons
define parse_addons_sources
	$(eval ADDONS_SOURCES_FILTER = $(addprefix $1/, $(ADDON_SOURCES_EXCLUDE))) \
	$(eval PARSED_ADDONS_SOURCE_PATHS = $(addsuffix /src, $1)) \
	$(eval PARSED_ADDONS_OFX_SOURCES = $(shell $(FIND) $(PARSED_ADDONS_SOURCE_PATHS) -type f \( -name "*.cpp" -or -name "*.mm" -or -name "*.m" -or -name "*.c" -or -name "*.cc" -or -name "*.cxx" \) 2> /dev/null | grep -v "/\.[^\.]" )) \
	$(eval PARSED_ADDONS_FILTERED_SOURCE_PATHS = $(filter-out $(ADDONS_SOURCES_FILTER),$(PARSED_ADDONS_OFX_SOURCES))) \
	$(eval PARSED_ADDONS_LIBS_SOURCE_PATHS = $(addsuffix /libs, $1)) \
	$(eval PARSED_ADDONS_LIBS_SOURCES = $(shell $(FIND) $(PARSED_ADDONS_LIBS_SOURCE_PATHS) -type f \( -name "*.cpp" -or -name "*.mm" -or -name "*.m" -or -name "*.c" -or -name "*.cc" -or -name "*.cxx" \) 2> /dev/null | grep -v "/\.[^\.]"  )) \
	$(eval PARSED_ADDONS_FILTERED_LIBS_SOURCE_PATHS = $(filter-out $(ADDONS_SOURCES_FILTER),$(PARSED_ADDONS_LIBS_SOURCES))) \
	$(eval PARSED_ADDONS_SOURCE_FILES = $(PARSED_ADDONS_FILTERED_SOURCE_PATHS)) \
	$(eval PARSED_ADDONS_SOURCE_FILES += $(PARSED_ADDONS_FILTERED_LIBS_SOURCE_PATHS))
endef

# parses addons libraries, in PARSED_ADDON_LIBS receives full PATHS to addons and libs_exclude
define parse_addons_libraries
	$(eval PARSED_ADDONS_LIBS_PLATFORM_LIB_PATHS = $(filter-out $(ADDON_LIBS_EXCLUDE),$(addsuffix /libs/*/lib/$(ABI_LIB_SUBPATH), $1))) \
	$(eval PARSED_ALL_PLATFORM_LIBS = $(shell $(FIND) $(PARSED_ADDONS_LIBS_PLATFORM_LIB_PATHS) -type d 2> /dev/null | grep -v "/\.[^\.]" )) \
	$(if $(PARSED_ALL_PLATFORM_LIBS), \
		$(eval PARSED_ADDONS_LIBS_PLATFORM_LIBS_STATICS = $(shell $(FIND) $(PARSED_ALL_PLATFORM_LIBS) -name *.a 2> /dev/null | grep -v "/\.[^\.]" )) \
		$(eval PARSED_ADDONS_LIBS_PLATFORM_LIBS_SHARED = $(shell $(FIND) $(PARSED_ALL_PLATFORM_LIBS) -name *.so 2> /dev/null | grep -v "/\.[^\.]" )) \
		$(eval PARSED_ADDONS_LIBS_PLATFORM_LIBS_SHARED += $(shell $(FIND) $(PARSED_ALL_PLATFORM_LIBS) -name *.dylib 2> /dev/null | grep -v "/\.[^\.]" )) \
		$(eval PARSED_ADDONS_LIBS_PLATFORM_LIBS_SHARED += $(shell $(FIND) $(PARSED_ALL_PLATFORM_LIBS) -name *.dll 2> /dev/null | grep -v "/\.[^\.]" )) \
		$(eval PARSED_ADDONS_LIBS = $(PARSED_ADDONS_LIBS_PLATFORM_LIBS_STATICS)) \
		$(eval PARSED_ADDONS_LIBS += $(PARSED_ADDONS_LIBS_PLATFORM_LIBS_SHARED)) \
	)
endef


space :=
space +=

define src_to_obj
	$(addsuffix .o,$(basename $(filter %.c %.cpp %.mm %.m %.cc %.cxx %.cc %.s %.S, $(addprefix $3,$(addprefix $2,$1)))))
endef

define rwildcard
	$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)$(filter $(subst *,%,$2),$d))
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
define parse_addon
	$(if $(wildcard $(PROJECT_ROOT)/$1), \
		$(eval addon=$(realpath $(addprefix $(PROJECT_ROOT)/, $1))) \
		$(eval addon_obj_path=$(PROJECT_ROOT)) \
		$(eval ADDON_PATHS+= $(dir $(addon))) \
		$(eval obj_prefix=$(OF_PROJECT_OBJ_OUTPUT_PATH)addons/) \
	, \
		$(eval addon=$(realpath $(addprefix $(OF_ADDONS_PATH)/, $1))) \
		$(eval addon_obj_path=$(OF_ADDONS_PATH)) \
		$(eval obj_prefix=$(OF_PROJECT_OBJ_OUTPUT_PATH)) \
	) \
	$(eval ADDON_DEPENDENCIES= ) \
	$(eval ADDON_DATA= ) \
	$(eval ADDON_CFLAGS= ) \
	$(eval ADDON_CPPFLAGS= ) \
	$(eval ADDON_LDFLAGS= ) \
	$(eval ADDON_PKG_CONFIG_LIBRARIES= ) \
	$(eval ADDON_FRAMEWORKS= ) \
	$(eval ADDON_LIBS_EXCLUDE= ) \
	$(eval ADDON_SOURCES_EXCLUDE= ) \
	$(call parse_addons_includes, $(addon)) \
	$(eval ADDON_INCLUDES=$(PARSED_ADDONS_INCLUDES)) \
	$(call parse_addons_libraries, $(addon)) \
	$(eval ADDON_LIBS=$(PARSED_ADDONS_LIBS)) \
	$(call parse_addons_sources, $(addon)) \
	$(eval ADDON_SOURCES=$(PARSED_ADDONS_SOURCE_FILES)) \
	$(eval PROCESS_NEXT=0) \
	$(if $(wildcard $(addon)/addon_config.mk), \
		$(foreach var_line, $(shell cat $(addon)/addon_config.mk | tr '\n ' '\t?'), \
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
		$(eval ADDON_INCLUDES_FILTERED = $(filter-out $(addprefix $(addon)/,$(ADDON_INCLUDES_EXCLUDE)),$(ADDON_INCLUDES))) \
		$(foreach addon_include, $(strip $(ADDON_INCLUDES_FILTERED)), \
			$(if $(wildcard $(addon)/$(addon_include)), \
				$(eval TMP_PROJECT_ADDONS_INCLUDES += $(addon)/$(addon_include)) \
			) \
			$(if $(wildcard $(addon_include)), \
				$(eval TMP_PROJECT_ADDONS_INCLUDES += $(addon_include)) \
			) \
		) \
	) \
	$(eval TMP_PROJECT_ADDONS_CFLAGS += $(ADDON_CFLAGS)) \
	$(eval TMP_PROJECT_ADDONS_CFLAGS += $(ADDON_CPPFLAGS)) \
	$(if $(strip $(ADDON_LIBS)), \
		$(foreach addon_lib, $(strip $(ADDON_LIBS)), \
			$(if $(wildcard $(addon)/$(addon_lib)), \
				$(eval TMP_PROJECT_ADDONS_LIBS += $(addon)/$(addon_lib)) \
			) \
			$(if $(wildcard $(addon_lib)), \
				$(eval TMP_PROJECT_ADDONS_LIBS += $(addon_lib)) \
			) \
		) \
	) \
	$(eval TMP_PROJECT_ADDONS_LDFLAGS += $(ADDON_LDFLAGS)) \
	$(eval TMP_PROJECT_ADDONS_PKG_CONFIG_LIBRARIES += $(ADDON_PKG_CONFIG_LIBRARIES)) \
	$(eval TMP_PROJECT_ADDONS_FRAMEWORKS += $(ADDON_FRAMEWORKS)) \
	$(eval PROJECT_AFTER += $(ADDON_AFTER)) \
	$(if $(strip $(ADDON_SOURCES)), \
		$(eval ADDON_SOURCES_FILTERED = $(filter-out $(addprefix $(addon)/,$(ADDON_SOURCES_EXCLUDE)),$(ADDON_SOURCES))) \
		$(foreach addon_src, $(strip $(ADDON_SOURCES_FILTERED)), \
			$(if $(filter $(addon)%, $(addon_src)), \
				$(eval addon_path=$(subst %,*,$(addon_src))) \
				$(if $(findstring *,$(addon_path)), \
					$(eval addon_dir=$(dir $(addon_path))) \
					$(eval addon_rest=$(notdir $(addon_path))) \
					$(eval addon_files=$(strip $(call rwildcard,$(addon_dir),$(addon_rest)))) \
					$(foreach expanded_addon_src, $(addon_files), \
						$(eval TMP_PROJECT_ADDONS_SOURCE_FILES += $(expanded_addon_src)) \
						$(eval SRC_OBJ_FILE=$(addprefix $(addon_obj_path)/,$(strip $(call src_to_obj, $(expanded_addon_src:$(addon)/%=%),$(notdir $1)/,$(obj_prefix))))) \
						$(eval TMP_PROJECT_ADDONS_OBJ_FILES += $(SRC_OBJ_FILE)) \
					) \
				, \
					$(eval TMP_PROJECT_ADDONS_SOURCE_FILES += $(addon_src)) \
					$(eval SRC_OBJ_FILE=$(addprefix $(addon_obj_path)/,$(strip $(call src_to_obj, $(addon_src:$(addon)/%=%),$(notdir $1)/,$(obj_prefix))))) \
					$(eval TMP_PROJECT_ADDONS_OBJ_FILES += $(SRC_OBJ_FILE)) \
				) \
			, \
				$(if $(filter $(OF_ROOT)%, $(addon_src)), \
					$(eval addon_path=$(subst %,*,$(addon_src))) \
					$(if $(findstring *,$(addon_path)), \
						$(eval addon_dir=$(dir $(addon_src))) \
						$(eval addon_rest=$(notdir $(addon_src))) \
						$(eval addon_files=$(strip $(call rwildcard,$(addon_dir),$(addon_rest)))) \
						$(foreach expanded_addon_src, $(addon_files), \
							$(eval TMP_PROJECT_ADDONS_SOURCE_FILES += $(expanded_addon_src)) \
							$(eval SRC_OBJ_FILE=$(strip $(call src_to_obj, $(expanded_addon_src:$(OF_ROOT)/%=%),,$(obj_prefix)))) \
							$(eval TMP_PROJECT_ADDONS_OBJ_FILES += $(SRC_OBJ_FILE)) \
						) \
					, \
						$(eval TMP_PROJECT_ADDONS_SOURCE_FILES += $(addon_src)) \
						$(eval SRC_OBJ_FILE=$(strip $(call src_to_obj, $(addon_src:$(OF_ROOT)/%=%),,$(obj_prefix)))) \
						$(eval TMP_PROJECT_ADDONS_OBJ_FILES += $(SRC_OBJ_FILE)) \
					) \
				,$(if $(filter-out /%, $(addon_src)), \
					$(eval addon_path=$(addon)/$(subst %,*,$(addon_src))) \
					$(if $(findstring *,$(addon_path)), \
						$(eval addon_dir=$(dir $(addon_path))) \
						$(eval addon_rest=$(notdir $(addon_path))) \
						$(eval addon_files=$(strip $(call rwildcard,$(addon_dir),$(addon_rest)))) \
						$(foreach expanded_addon_src, $(addon_files), \
							$(eval TMP_PROJECT_ADDONS_SOURCE_FILES += $(expanded_addon_src)) \
							$(eval SRC_OBJ_FILE=$(addprefix $(addon_obj_path)/,$(strip $(call src_to_obj, $(expanded_addon_src:$(addon)/%=%),$(notdir $1)/,$(obj_prefix))))) \
							$(eval TMP_PROJECT_ADDONS_OBJ_FILES += $(SRC_OBJ_FILE)) \
						) \
					, \
						$(eval TMP_PROJECT_ADDONS_SOURCE_FILES += $(addon_path)) \
						$(eval SRC_OBJ_FILE=$(addprefix $(addon_obj_path)/,$(strip $(call src_to_obj, $(addon_path:$(addon)/%=%),$(notdir $1)/,$(obj_prefix))))) \
						$(eval TMP_PROJECT_ADDONS_OBJ_FILES += $(SRC_OBJ_FILE)) \
					) \
					,$(error cannot find addon source file $(addon_src))) \
				) \
			) \
		) \
	) \
	$(if $(strip $(ADDON_DATA)), \
		$(eval TMP_PROJECT_ADDONS_DATA += $(addprefix $(addon)/,$(ADDON_DATA))) \
	) \
	$(foreach addon_dep, $(strip $(ADDON_DEPENDENCIES)), \
		$(if $(filter %$(addon_dep), $(PROJECT_ADDONS)), \
		, \
			$(eval PROJECT_ADDONS += $(addon_dep)) \
			$(call parse_addon,$(addon_dep)) \
		) \
	)
endef


$(foreach addon_to_parse, $(PROJECT_ADDONS), \
	$(call parse_addon,$(addon_to_parse)) \
)

#define uniq =
#  $(eval seen :=)
#  $(foreach _,$1,$(if $(filter $_,${seen}),,$(eval seen += $_)))
#  ${seen}
#endef


uniq = $(if $1,$(firstword $1) $(call uniq,$(filter-out $(firstword $1),$1)))


PROJECT_ADDONS_CFLAGS = $(call uniq,$(TMP_PROJECT_ADDONS_CFLAGS))
PROJECT_ADDONS_INCLUDES = $(call uniq,$(TMP_PROJECT_ADDONS_INCLUDES))
PROJECT_ADDONS_LIBS = $(call uniq,$(TMP_PROJECT_ADDONS_LIBS))
PROJECT_ADDONS_LDFLAGS = $(call uniq,$(TMP_PROJECT_ADDONS_LDFLAGS))
PROJECT_ADDONS_PKG_CONFIG_LIBRARIES = $(call uniq,$(TMP_PROJECT_ADDONS_PKG_CONFIG_LIBRARIES))
PROJECT_ADDONS_FRAMEWORKS = $(call uniq,$(TMP_PROJECT_ADDONS_FRAMEWORKS))
PROJECT_ADDONS_SOURCE_FILES = $(call uniq,$(TMP_PROJECT_ADDONS_SOURCE_FILES))
PROJECT_ADDONS_OBJ_FILES = $(call uniq,$(TMP_PROJECT_ADDONS_OBJ_FILES))
PROJECT_ADDONS_DATA = $(call uniq,$(TMP_PROJECT_ADDONS_DATA))
VPATH += $(call uniq, $(ADDON_PATHS))


OF_PROJECT_ADDONS_OBJS = $(PROJECT_ADDONS_OBJ_FILES)
OF_PROJECT_ADDONS_DEPS = $(patsubst %.o,%.d,$(PROJECT_ADDONS_OBJ_FILES))

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
    $(info ---PROJECT_ADDONS_DATA---)
    $(foreach v, $(PROJECT_ADDONS_DATA),$(info $(v)))
endif
