# openFrameworks apps
#
#   of_add_app(<name> [SOURCES <files>...] [ADDONS <addons>...] [NO_ADDONS_MAKE]
#              [PROJECT_DIR <dir>])
#
# Creates the app target from the project's src/ folder (or SOURCES), links the
# openFrameworks core, and adds ADDONS (or the project's addons.make unless
# NO_ADDONS_MAKE). The app is built into the project's bin/ folder like the
# makefiles and IDE projects: <name> for Release, <name>_debug for Debug.
# PROJECT_DIR is the project folder (default: the current CMakeLists.txt folder),
# so a project without its own CMakeLists.txt (tests, examples) can be added.
# OF_PRECOMPILED_HEADER=ON precompiles ofMain.h for the app's src/ C++ files.

include_guard(GLOBAL)

function(of_add_app name)
	cmake_parse_arguments(A "NO_ADDONS_MAKE" "PROJECT_DIR" "SOURCES;ADDONS" ${ARGN})
	# the helpers called below read the project folder from OF_PROJECT_DIR
	if(A_PROJECT_DIR)
		get_filename_component(OF_PROJECT_DIR "${A_PROJECT_DIR}" ABSOLUTE)
	else()
		set(OF_PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
	endif()

	if(A_SOURCES)
		set(sources ${A_SOURCES})
	else()
		of_glob_sources(sources "${OF_PROJECT_DIR}/src")
		file(GLOB_RECURSE headers CONFIGURE_DEPENDS
			"${OF_PROJECT_DIR}/src/*.h" "${OF_PROJECT_DIR}/src/*.hpp")
		list(APPEND sources ${headers})
	endif()
	if(NOT sources)
		message(FATAL_ERROR "openFrameworks: no sources for ${name} (looked in ${OF_PROJECT_DIR}/src)")
	endif()

	add_executable(${name} ${sources})
	source_group(TREE "${OF_PROJECT_DIR}" FILES ${sources})
	of_dir_and_subdirs(src_dirs "${OF_PROJECT_DIR}/src")
	target_include_directories(${name} PRIVATE ${src_dirs})

	of_use_core()
	target_link_libraries(${name} PRIVATE openFrameworks::openFrameworks)

	set(bin_dir "${OF_PROJECT_DIR}/bin")
	set_target_properties(${name} PROPERTIES
		OUTPUT_NAME ${name}
		OUTPUT_NAME_DEBUG ${name}_debug
		RUNTIME_OUTPUT_DIRECTORY "${bin_dir}"
		VS_DEBUGGER_WORKING_DIRECTORY "${bin_dir}"
		XCODE_SCHEME_WORKING_DIRECTORY "${bin_dir}")
	foreach(config Debug Release RelWithDebInfo MinSizeRel)
		string(TOUPPER ${config} upper)
		set_target_properties(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${upper} "${bin_dir}")
	endforeach()

	set(addons ${A_ADDONS})
	if(NOT A_ADDONS AND NOT A_NO_ADDONS_MAKE)
		of_read_addons_make(addons "${OF_PROJECT_DIR}/addons.make")
	endif()
	of_target_addons(${name} ${OF_PLATFORM_REQUIRED_ADDONS} ${addons})

	# ofMain.h precompiled for the app's own C++ files; addon and library
	# sources compile without it, as they do in the other build systems
	if(OF_PRECOMPILED_HEADER)
		target_precompile_headers(${name} PRIVATE
			"$<$<COMPILE_LANGUAGE:CXX>:${OF_CORE_SRC_DIR}/ofMain.h>")
		get_target_property(app_sources ${name} SOURCES)
		foreach(source IN LISTS app_sources)
			get_filename_component(source "${source}" ABSOLUTE)
			string(FIND "${source}" "${OF_PROJECT_DIR}/src/" at)
			if(NOT at EQUAL 0)
				set_source_files_properties("${source}" PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
			endif()
		endforeach()
	endif()

	of_platform_setup_app(${name})
endfunction()
