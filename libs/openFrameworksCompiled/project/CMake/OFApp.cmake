# openFrameworks apps
#
#   of_add_app(<name> [SOURCES <files>...] [ADDONS <addons>...] [NO_ADDONS_MAKE])
#
# Creates the app target from the project's src/ folder (or SOURCES), links the
# openFrameworks core, and adds ADDONS (or the project's addons.make unless
# NO_ADDONS_MAKE). The app is built into the project's bin/ folder like the
# makefiles and IDE projects: <name> for Release, <name>_debug for Debug.

include_guard(GLOBAL)

function(of_add_app name)
	cmake_parse_arguments(A "NO_ADDONS_MAKE" "" "SOURCES;ADDONS" ${ARGN})

	if(A_SOURCES)
		set(sources ${A_SOURCES})
	else()
		of_glob_sources(sources "${CMAKE_CURRENT_SOURCE_DIR}/src")
		file(GLOB_RECURSE headers CONFIGURE_DEPENDS
			"${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
		list(APPEND sources ${headers})
	endif()
	if(NOT sources)
		message(FATAL_ERROR "openFrameworks: no sources for ${name} (looked in ${CMAKE_CURRENT_SOURCE_DIR}/src)")
	endif()

	add_executable(${name} ${sources})
	source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})
	of_dir_and_subdirs(src_dirs "${CMAKE_CURRENT_SOURCE_DIR}/src")
	target_include_directories(${name} PRIVATE ${src_dirs})

	of_use_core()
	target_link_libraries(${name} PRIVATE openFrameworks::openFrameworks)

	set(bin_dir "${CMAKE_CURRENT_SOURCE_DIR}/bin")
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
		of_read_addons_make(addons "${CMAKE_CURRENT_SOURCE_DIR}/addons.make")
	endif()
	of_target_addons(${name} ${OF_PLATFORM_REQUIRED_ADDONS} ${addons})

	of_platform_setup_app(${name})
endfunction()
