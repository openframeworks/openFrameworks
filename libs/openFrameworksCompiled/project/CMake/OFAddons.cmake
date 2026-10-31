# openFrameworks addons
#
#   of_target_addons(<target> <addon>...)
#       Adds addons the way the makefiles do: sources, include paths and libs
#       from the addon's folders, adjusted by its addon_config.mk (common: and
#       <platform>: sections), plus its ADDON_DEPENDENCIES. <addon> is a name in
#       OF_ROOT/addons, or a path (relative to the project) for local addons.
#
#   of_target_addon_files(<target> NAME <name>
#           [SOURCES ...] [INCLUDES ...] [LIBS ...] [DEFINES ...]
#           [CFLAGS ...] [LDFLAGS ...] [FRAMEWORKS ...] [PKG_CONFIG ...])
#       Adds an addon from explicit lists (absolute paths), e.g. as generated
#       by the Project Generator.

include_guard(GLOBAL)

# addon_config.mk section names that apply to this platform.
# Linux also reads the generic linux: section, then linux/64 and the flat linux64 alias.
set(OF_ADDON_CONFIG_SECTIONS common)
if(OF_PLATFORM MATCHES "^linux/")
	list(APPEND OF_ADDON_CONFIG_SECTIONS linux)
endif()
set(_of_addon_subpaths ${OF_LIB_SUBPATHS})
if(NOT _of_addon_subpaths)
	set(_of_addon_subpaths "${OF_PLATFORM}")
endif()
list(APPEND OF_ADDON_CONFIG_SECTIONS ${_of_addon_subpaths})
if(OF_PLATFORM STREQUAL "macos")
	list(APPEND OF_ADDON_CONFIG_SECTIONS osx)
endif()
if(OF_ARCH)
	foreach(subpath IN LISTS _of_addon_subpaths)
		list(APPEND OF_ADDON_CONFIG_SECTIONS "${subpath}/${OF_ARCH}")
	endforeach()
endif()

set(OF_ADDON_CONFIG_VARS
	ADDON_DEPENDENCIES ADDON_INCLUDES ADDON_INCLUDES_EXCLUDE ADDON_SOURCES ADDON_SOURCES_EXCLUDE
	ADDON_LIBS ADDON_LIBS_EXCLUDE ADDON_DEFINES ADDON_CFLAGS ADDON_CPPFLAGS ADDON_LDFLAGS
	ADDON_FRAMEWORKS ADDON_PKG_CONFIG_LIBRARIES ADDON_DATA)

# make's filter-out pattern (one % matches anything) -> regex
function(of_make_pattern_regex out_var pattern)
	string(REGEX REPLACE "([][+.*()^$?{}|\\\\])" "\\\\\\1" escaped "${pattern}")
	string(REPLACE "%" ".*" escaped "${escaped}")
	set(${out_var} "^${escaped}$" PARENT_SCOPE)
endfunction()

# removes items matching any of the make patterns (relative to <base>)
function(of_filter_make_patterns out_var base patterns)
	set(regexes)
	foreach(pattern IN LISTS patterns)
		if(NOT IS_ABSOLUTE "${pattern}")
			set(pattern "${base}/${pattern}")
		endif()
		of_make_pattern_regex(regex "${pattern}")
		list(APPEND regexes "${regex}")
	endforeach()
	of_filter_out(result "${regexes}" ${ARGN})
	set(${out_var} ${result} PARENT_SCOPE)
endfunction()

# expands addon paths relative to <base>; a % works like the makefiles' wildcard
function(of_expand_addon_paths out_var base)
	set(result)
	foreach(item IN LISTS ARGN)
		# Platform discovery can return configuration-dependent library expressions.
		# They already contain absolute paths and must reach target_link_libraries intact.
		if(item MATCHES "^\\$<")
			list(APPEND result "${item}")
			continue()
		endif()
		if(NOT IS_ABSOLUTE "${item}")
			set(item "${base}/${item}")
		endif()
		if(item MATCHES "%")
			string(REPLACE "%" "*" glob "${item}")
			file(GLOB matches LIST_DIRECTORIES true "${glob}")
			list(APPEND result ${matches})
		else()
			list(APPEND result "${item}")
		endif()
	endforeach()
	set(${out_var} ${result} PARENT_SCOPE)
endfunction()

# reads addon_config.mk into <prefix>_<VAR> for the sections that apply here.
# Variables already set (the defaults) are replaced by = and extended by +=.
macro(of_read_addon_config file prefix)
	file(STRINGS "${file}" _of_lines)
	set(_of_section "")
	set(_of_pending "")
	foreach(_of_line IN LISTS _of_lines)
		string(REGEX REPLACE "#.*$" "" _of_line "${_of_line}")
		if(_of_pending)
			string(PREPEND _of_line "${_of_pending} ")
			set(_of_pending "")
		endif()
		if(_of_line MATCHES "\\\\[ \t]*$")
			string(REGEX REPLACE "\\\\[ \t]*$" "" _of_pending "${_of_line}")
			continue()
		endif()
		string(STRIP "${_of_line}" _of_line)
		if(_of_line STREQUAL "")
			continue()
		endif()
		if(_of_line MATCHES "^([A-Za-z0-9_/.-]+)[ \t]*:$")
			set(_of_section "${CMAKE_MATCH_1}")
			continue()
		endif()
		if(NOT _of_section IN_LIST OF_ADDON_CONFIG_SECTIONS)
			continue()
		endif()
		if(_of_line MATCHES "^([A-Z_]+)[ \t]*(\\+=|:=|=)[ \t]*(.*)$")
			set(_of_var "${CMAKE_MATCH_1}")
			set(_of_op "${CMAKE_MATCH_2}")
			separate_arguments(_of_values UNIX_COMMAND "${CMAKE_MATCH_3}")
			if(_of_var IN_LIST OF_ADDON_CONFIG_VARS)
				if(_of_op STREQUAL "+=")
					list(APPEND ${prefix}_${_of_var} ${_of_values})
				else()
					set(${prefix}_${_of_var} ${_of_values})
				endif()
			endif()
		endif()
	endforeach()
endmacro()

function(of_find_addon_dir out_var addon)
	set(project_dir "${OF_PROJECT_DIR}")
	if(NOT project_dir)
		set(project_dir "${CMAKE_CURRENT_SOURCE_DIR}")
	endif()
	if(IS_ABSOLUTE "${addon}" AND IS_DIRECTORY "${addon}")
		set(dir "${addon}")
	elseif(IS_DIRECTORY "${project_dir}/${addon}" AND addon MATCHES "/")
		set(dir "${project_dir}/${addon}")
	elseif(IS_DIRECTORY "${OF_ADDONS_DIR}/${addon}")
		set(dir "${OF_ADDONS_DIR}/${addon}")
	elseif(IS_DIRECTORY "${project_dir}/${addon}")
		set(dir "${project_dir}/${addon}")
	else()
		message(FATAL_ERROR "openFrameworks: addon '${addon}' not found in ${OF_ADDONS_DIR} or the project")
	endif()
	get_filename_component(dir "${dir}" ABSOLUTE)
	set(${out_var} "${dir}" PARENT_SCOPE)
endfunction()

function(of_target_addon_files target)
	cmake_parse_arguments(A "" "NAME" "SOURCES;INCLUDES;LIBS;DEFINES;CFLAGS;LDFLAGS;FRAMEWORKS;PKG_CONFIG" ${ARGN})
	if(A_SOURCES)
		target_sources(${target} PRIVATE ${A_SOURCES})
		source_group(TREE "${OF_ROOT}" PREFIX "" FILES ${A_SOURCES})
	endif()
	target_include_directories(${target} PRIVATE ${A_INCLUDES})
	target_compile_definitions(${target} PRIVATE ${A_DEFINES})
	target_compile_options(${target} PRIVATE ${A_CFLAGS})
	target_link_options(${target} PRIVATE ${A_LDFLAGS})
	of_link_static_libs(${target} PRIVATE ${A_LIBS})
	foreach(framework IN LISTS A_FRAMEWORKS)
		target_link_libraries(${target} PRIVATE "-framework ${framework}")
	endforeach()
	if(A_PKG_CONFIG)
		find_package(PkgConfig REQUIRED)
		string(MAKE_C_IDENTIFIER "${A_NAME}" id)
		pkg_check_modules(OF_ADDON_PC_${id} REQUIRED IMPORTED_TARGET ${A_PKG_CONFIG})
		target_link_libraries(${target} PRIVATE PkgConfig::OF_ADDON_PC_${id})
	endif()
endfunction()

function(of_target_addon target addon)
	of_find_addon_dir(dir "${addon}")
	get_filename_component(name "${dir}" NAME)
	get_target_property(done ${target} OF_ADDONS)
	if(done AND name IN_LIST done)
		return()
	endif()
	set_property(TARGET ${target} APPEND PROPERTY OF_ADDONS ${name})

	# defaults, as the makefiles build them before reading addon_config.mk
	of_glob_sources(src_sources "${dir}/src")
	of_glob_sources(lib_sources "${dir}/libs")
	set(cfg_ADDON_SOURCES ${src_sources} ${lib_sources})
	of_dir_and_subdirs(src_dirs "${dir}/src")
	of_dir_and_subdirs(lib_dirs "${dir}/libs")
	set(cfg_ADDON_INCLUDES ${src_dirs} ${lib_dirs})
	set(cfg_ADDON_LIBS)
	file(GLOB addon_lib_roots LIST_DIRECTORIES true "${dir}/libs/*")
	foreach(lib_root IN LISTS addon_lib_roots)
		of_platform_lib_folder(folder "${lib_root}")
		of_static_libs_in(libs "${folder}")
		list(APPEND cfg_ADDON_LIBS ${libs})
	endforeach()

	if(EXISTS "${dir}/addon_config.mk")
		of_read_addon_config("${dir}/addon_config.mk" cfg)
	endif()

	foreach(dependency IN LISTS cfg_ADDON_DEPENDENCIES)
		of_target_addon(${target} "${dependency}")
	endforeach()

	of_expand_addon_paths(sources "${dir}" ${cfg_ADDON_SOURCES})
	of_filter_make_patterns(sources "${dir}" "${cfg_ADDON_SOURCES_EXCLUDE}" ${sources})
	of_filter_out(sources "${OF_PLATFORM_CORE_EXCLUDES}" ${sources})
	of_expand_addon_paths(includes "${dir}" ${cfg_ADDON_INCLUDES})
	of_filter_make_patterns(includes "${dir}" "${cfg_ADDON_INCLUDES_EXCLUDE}" ${includes})
	of_expand_addon_paths(libs "${dir}" ${cfg_ADDON_LIBS})
	of_filter_make_patterns(libs "${dir}" "${cfg_ADDON_LIBS_EXCLUDE}" ${libs})
	# Release/Debug libs are $<$<CONFIG:...>:path.lib>. That trailing > must stay,
	# or ofxAssimpModelLoader and ofxOpenCv link with no libraries on Visual Studio.
	list(FILTER libs INCLUDE REGEX "\\.(a|lib|so|dylib|tbd|xcframework)(>)?$|^-")

	of_target_addon_files(${target} NAME ${name}
		SOURCES ${sources}
		INCLUDES ${includes}
		LIBS ${libs}
		DEFINES ${cfg_ADDON_DEFINES}
		CFLAGS ${cfg_ADDON_CFLAGS} ${cfg_ADDON_CPPFLAGS}
		LDFLAGS ${cfg_ADDON_LDFLAGS}
		FRAMEWORKS ${cfg_ADDON_FRAMEWORKS}
		PKG_CONFIG ${cfg_ADDON_PKG_CONFIG_LIBRARIES})

	list(LENGTH sources n_sources)
	list(LENGTH libs n_libs)
	message(STATUS "openFrameworks: addon ${name} (${n_sources} sources, ${n_libs} libs)")
endfunction()

function(of_target_addons target)
	foreach(addon IN LISTS ARGN)
		of_target_addon(${target} "${addon}")
	endforeach()
endfunction()

# space separated list from a project's config.make, NAME = a b / NAME += c
function(of_read_config_make_list out_var file name)
	set(result)
	if(EXISTS "${file}")
		file(STRINGS "${file}" lines REGEX "^[ \t]*${name}[ \t]*[+:?]?=")
		foreach(line IN LISTS lines)
			string(REGEX MATCH "^[ \t]*${name}[ \t]*([+:?]?)=(.*)$" _ "${line}")
			set(op "${CMAKE_MATCH_1}")
			string(STRIP "${CMAKE_MATCH_2}" values)
			separate_arguments(values UNIX_COMMAND "${values}")
			if(NOT op STREQUAL "+")
				set(result)
			endif()
			list(APPEND result ${values})
		endforeach()
	endif()
	set(${out_var} ${result} PARENT_SCOPE)
endfunction()

# addon names from a project's addons.make (one per line, # comments)
function(of_read_addons_make out_var file)
	set(result)
	if(EXISTS "${file}")
		file(STRINGS "${file}" lines)
		foreach(line IN LISTS lines)
			string(REGEX REPLACE "#.*$" "" line "${line}")
			string(STRIP "${line}" line)
			if(line)
				list(APPEND result "${line}")
			endif()
		endforeach()
	endif()
	set(${out_var} ${result} PARENT_SCOPE)
endfunction()
