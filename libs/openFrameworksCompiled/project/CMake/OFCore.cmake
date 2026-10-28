# openFrameworks core: sources, third-party libs, and the targets
#
#   openFrameworks::deps   INTERFACE target with everything needed to compile
#                          and link against the core (includes, defines, flags,
#                          third-party and system libs)
#   openFrameworks         the core library: prebuilt from
#                          libs/openFrameworksCompiled/lib/<platform>[/<arch>],
#                          or built in this build (of_use_core)
#
# Each platform/<family>.cmake sets:
#   OF_PLATFORM_CORE_EXCLUDES      regexes matched against core source paths
#   OF_PLATFORM_SOURCE_EXTENSIONS  source file extensions to compile
#   OF_PLATFORM_DEFINES            compile definitions
#   OF_PLATFORM_COMPILE_OPTIONS    compile options
#   OF_PLATFORM_LINK_OPTIONS       link options
#   OF_PLATFORM_LINK_LIBRARIES     system libraries / frameworks
#   OF_PLATFORM_THIRDPARTY_EXCLUDE libs/<name> folders not to use
#   OF_PLATFORM_INCLUDE_DIRS       extra include paths for the core
#   OF_PLATFORM_CORE_EXTRA_SOURCE_DIRS  extra folders compiled into the core (ofxiOS on iOS)
#   OF_PLATFORM_REQUIRED_ADDONS    addons every app on this platform needs
#   OF_PLATFORM_LIB_EXTENSIONS     static library extensions
#   OF_PLATFORM_LINK_GROUP         ON to wrap static libs in --start-group/--end-group
#   OF_PLATFORM_WHOLE_ARCHIVE_CORE ON to link every object in the core archive
#   OF_CORE_LIB_NAME / OF_CORE_LIB_NAME_DEBUG / OF_CORE_LIB_PREFIX / OF_CORE_LIB_SUFFIX
# and defines of_platform_lib_dirs(<lib folder> <out var>) and of_platform_setup_app(<target>).

include_guard(GLOBAL)

# filters a list of paths, removing entries that match any regex in the exclude list
function(of_filter_out out_var excludes)
	set(result)
	foreach(item IN LISTS ARGN)
		set(keep ON)
		foreach(pattern IN LISTS excludes)
			if(item MATCHES "${pattern}")
				set(keep OFF)
				break()
			endif()
		endforeach()
		if(keep)
			list(APPEND result "${item}")
		endif()
	endforeach()
	set(${out_var} ${result} PARENT_SCOPE)
endfunction()

# all source files under <dir> with the platform's source extensions
function(of_glob_sources out_var dir)
	set(patterns)
	foreach(ext IN LISTS OF_PLATFORM_SOURCE_EXTENSIONS)
		list(APPEND patterns "${dir}/*.${ext}")
	endforeach()
	file(GLOB_RECURSE files CONFIGURE_DEPENDS ${patterns})
	list(FILTER files EXCLUDE REGEX "/\\.[^/]*")
	list(SORT files)
	set(${out_var} ${files} PARENT_SCOPE)
endfunction()

# <dir> plus all of its subdirectories (like the makefiles' header search paths)
function(of_dir_and_subdirs out_var dir)
	set(result)
	if(IS_DIRECTORY "${dir}")
		list(APPEND result "${dir}")
		file(GLOB_RECURSE children LIST_DIRECTORIES true "${dir}/*")
		foreach(child IN LISTS children)
			if(IS_DIRECTORY "${child}" AND NOT child MATCHES "/\\.[^/]*")
				list(APPEND result "${child}")
			endif()
		endforeach()
	endif()
	set(${out_var} ${result} PARENT_SCOPE)
endfunction()

function(of_core_sources out_var)
	of_glob_sources(sources "${OF_CORE_SRC_DIR}")
	foreach(dir IN LISTS OF_PLATFORM_CORE_EXTRA_SOURCE_DIRS)
		of_glob_sources(extra "${dir}")
		list(APPEND sources ${extra})
	endforeach()
	of_filter_out(sources "${OF_PLATFORM_CORE_EXCLUDES}" ${sources})
	set(${out_var} ${sources} PARENT_SCOPE)
endfunction()

function(of_core_include_dirs out_var)
	set(dirs "${OF_CORE_SRC_DIR}")
	file(GLOB children LIST_DIRECTORIES true "${OF_CORE_SRC_DIR}/*")
	foreach(child IN LISTS children)
		if(IS_DIRECTORY "${child}")
			list(APPEND dirs "${child}")
		endif()
	endforeach()
	set(${out_var} ${dirs} PARENT_SCOPE)
endfunction()

# libs/<name> folders the core uses on this platform
function(of_thirdparty_roots out_var)
	file(GLOB roots LIST_DIRECTORIES true "${OF_LIBS_DIR}/*")
	set(result)
	foreach(root IN LISTS roots)
		get_filename_component(name "${root}" NAME)
		if(NOT IS_DIRECTORY "${root}" OR name MATCHES "^(openFrameworks|openFrameworksCompiled|download|\\..*)$")
			continue()
		endif()
		if(name IN_LIST OF_PLATFORM_THIRDPARTY_EXCLUDE)
			continue()
		endif()
		list(APPEND result "${root}")
	endforeach()
	set(${out_var} ${result} PARENT_SCOPE)
endfunction()

# libs/<name>/include roots, like the Xcode and Visual Studio projects. (The
# makefiles also add every subfolder, which lets e.g. glm/include/glm/simd shadow
# the SDK's <simd/...> headers.)
function(of_thirdparty_include_dirs out_var)
	of_thirdparty_roots(roots)
	set(result)
	foreach(root IN LISTS roots)
		if(IS_DIRECTORY "${root}/include")
			list(APPEND result "${root}/include")
		endif()
	endforeach()
	set(${out_var} ${result} PARENT_SCOPE)
endfunction()

# static libraries in <lib folder> (e.g. libs/curl/lib/macos), in the platform's
# layout (plain folder, arch subfolder, xcframework slice, ...)
function(of_static_libs_in out_var lib_folder)
	if(COMMAND of_platform_static_libs_in)
		of_platform_static_libs_in(result "${lib_folder}")
		set(${out_var} ${result} PARENT_SCOPE)
		return()
	endif()
	of_platform_lib_dirs("${lib_folder}" dirs)
	set(result)
	foreach(dir IN LISTS dirs)
		foreach(ext IN LISTS OF_PLATFORM_LIB_EXTENSIONS)
			file(GLOB libs "${dir}/*.${ext}")
			list(SORT libs)
			list(APPEND result ${libs})
		endforeach()
	endforeach()
	set(${out_var} ${result} PARENT_SCOPE)
endfunction()

# libs/<name>/lib/<subpath>. The canonical folder wins; an older flat folder is
# used when that is the one on disk (linux/64, then linux64).
function(of_platform_lib_folder out_var lib_root)
	# OFPlatform.cmake fills OF_LIB_SUBPATHS. Callers that set OF_PLATFORM only
	# (the MSVC addon fixture) still search libs/*/lib/${OF_PLATFORM}.
	set(subpaths ${OF_LIB_SUBPATHS})
	if(NOT subpaths)
		set(subpaths "${OF_PLATFORM}")
	endif()
	set(found "")
	foreach(subpath IN LISTS subpaths)
		if(IS_DIRECTORY "${lib_root}/lib/${subpath}")
			set(found "${lib_root}/lib/${subpath}")
			break()
		endif()
	endforeach()
	if(found STREQUAL "")
		list(GET subpaths 0 subpath)
		set(found "${lib_root}/lib/${subpath}")
	endif()
	set(${out_var} "${found}" PARENT_SCOPE)
endfunction()

function(of_thirdparty_libs out_var)
	of_thirdparty_roots(roots)
	set(result)
	foreach(root IN LISTS roots)
		of_platform_lib_folder(folder "${root}")
		of_static_libs_in(libs "${folder}")
		list(APPEND result ${libs})
	endforeach()
	set(${out_var} ${result} PARENT_SCOPE)
endfunction()

# links static libraries, grouped for single-pass linkers so their order doesn't matter
function(of_link_static_libs target scope)
	if(NOT ARGN)
		return()
	endif()
	if(OF_PLATFORM_LINK_GROUP)
		target_link_libraries(${target} ${scope} "-Wl,--start-group" ${ARGN} "-Wl,--end-group")
	else()
		target_link_libraries(${target} ${scope} ${ARGN})
	endif()
endfunction()

function(of_setup_deps)
	if(TARGET openFrameworks_deps)
		return()
	endif()
	add_library(openFrameworks_deps INTERFACE)
	add_library(openFrameworks::deps ALIAS openFrameworks_deps)

	of_core_include_dirs(core_dirs)
	of_thirdparty_include_dirs(thirdparty_dirs)
	target_include_directories(openFrameworks_deps INTERFACE ${core_dirs} ${thirdparty_dirs} ${OF_PLATFORM_INCLUDE_DIRS})

	target_compile_definitions(openFrameworks_deps INTERFACE
		GLM_FORCE_CTOR_INIT
		GLM_ENABLE_EXPERIMENTAL
		$<$<CONFIG:Debug>:DEBUG>
		$<$<NOT:$<CONFIG:Debug>>:NDEBUG>
		${OF_PLATFORM_DEFINES})
	target_compile_options(openFrameworks_deps INTERFACE ${OF_PLATFORM_COMPILE_OPTIONS})
	target_link_options(openFrameworks_deps INTERFACE ${OF_PLATFORM_LINK_OPTIONS})

	of_thirdparty_libs(thirdparty_libs)
	of_link_static_libs(openFrameworks_deps INTERFACE ${thirdparty_libs})
	target_link_libraries(openFrameworks_deps INTERFACE ${OF_PLATFORM_LINK_LIBRARIES})

	list(LENGTH thirdparty_libs n)
	message(STATUS "openFrameworks: ${n} third-party libraries from libs/*/lib/${OF_PLATFORM}")
endfunction()

function(of_core_lib_path out_var config)
	if(config STREQUAL "Debug")
		set(name "${OF_CORE_LIB_NAME_DEBUG}")
	else()
		set(name "${OF_CORE_LIB_NAME}")
	endif()
	set(${out_var} "${OF_CORE_LIB_DIR}/${OF_CORE_LIB_PREFIX}${name}${OF_CORE_LIB_SUFFIX}" PARENT_SCOPE)
endfunction()

# openFrameworks::openFrameworks is what apps link. With
# OF_PLATFORM_WHOLE_ARCHIVE_CORE the archive is wrapped in --whole-archive,
# which is what the Emscripten makefile gets from cc -r: every core object
# is in the link, including ones reached only from a static constructor.
function(of_expose_core)
	if(TARGET openFrameworks::openFrameworks)
		return()
	endif()
	if(OF_PLATFORM_WHOLE_ARCHIVE_CORE)
		add_library(openFrameworks_link INTERFACE)
		add_library(openFrameworks::openFrameworks ALIAS openFrameworks_link)
		# LINKER: is only expanded in link options, not in link libraries
		target_link_options(openFrameworks_link INTERFACE
			"LINKER:--whole-archive,$<TARGET_FILE:openFrameworks>,--no-whole-archive")
		set_property(TARGET openFrameworks_link APPEND PROPERTY
			INTERFACE_LINK_DEPENDS "$<TARGET_FILE:openFrameworks>")
		target_link_libraries(openFrameworks_link INTERFACE openFrameworks::deps)
		add_dependencies(openFrameworks_link openFrameworks)
	else()
		add_library(openFrameworks::openFrameworks ALIAS openFrameworks)
	endif()
endfunction()

# defines the openFrameworks core library target, built from source
function(of_add_core_library)
	of_setup_deps()
	of_core_sources(sources)
	add_library(openFrameworks STATIC ${sources})
	target_link_libraries(openFrameworks PUBLIC openFrameworks::deps)
	set_target_properties(openFrameworks PROPERTIES
		OUTPUT_NAME "${OF_CORE_LIB_NAME}"
		OUTPUT_NAME_DEBUG "${OF_CORE_LIB_NAME_DEBUG}"
		PREFIX "${OF_CORE_LIB_PREFIX}"
		ARCHIVE_OUTPUT_DIRECTORY "${OF_CORE_LIB_DIR}")
	foreach(config Debug Release RelWithDebInfo MinSizeRel)
		string(TOUPPER ${config} upper)
		set_target_properties(openFrameworks PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_${upper} "${OF_CORE_LIB_DIR}")
	endforeach()
	if(COMMAND of_platform_setup_core)
		of_platform_setup_core(openFrameworks)
	endif()
	of_expose_core()
	list(LENGTH sources n)
	message(STATUS "openFrameworks: building the core (${n} sources) into ${OF_CORE_LIB_DIR}")
endfunction()

# makes the openFrameworks core available to this build: the prebuilt library
# if it exists for the configurations being built, otherwise builds it here
function(of_use_core)
	if(TARGET openFrameworks)
		return()
	endif()
	of_setup_deps()

	if(CMAKE_CONFIGURATION_TYPES)
		set(configs Debug Release)
	else()
		set(configs ${CMAKE_BUILD_TYPE})
	endif()
	set(prebuilt ON)
	foreach(config IN LISTS configs)
		of_core_lib_path(path ${config})
		if(NOT EXISTS "${path}")
			set(prebuilt OFF)
		endif()
	endforeach()

	if(prebuilt AND NOT OF_BUILD_CORE)
		add_library(openFrameworks STATIC IMPORTED GLOBAL)
		of_core_lib_path(release Release)
		of_core_lib_path(debug Debug)
		set_target_properties(openFrameworks PROPERTIES
			IMPORTED_LOCATION "${release}"
			IMPORTED_LOCATION_RELEASE "${release}"
			IMPORTED_LOCATION_RELWITHDEBINFO "${release}"
			IMPORTED_LOCATION_MINSIZEREL "${release}"
			IMPORTED_LOCATION_DEBUG "${debug}"
			INTERFACE_LINK_LIBRARIES openFrameworks::deps)
		of_expose_core()
		message(STATUS "openFrameworks: using prebuilt core from ${OF_CORE_LIB_DIR}")
	else()
		add_subdirectory("${OF_CMAKE_DIR}" "${CMAKE_BINARY_DIR}/openFrameworks")
	endif()
endfunction()
