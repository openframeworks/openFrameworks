# openFrameworks tests and examples in the core build
#
#   of_add_projects(<kind> <root> [TESTS] [EXCLUDE_FROM_ALL])
#
# Adds every <root>/<group>/<project> that has a src/ folder as an app (with
# of_add_app PROJECT_DIR, so the projects need no CMakeLists.txt), in the IDE
# folder <kind>/<group>, plus a build_all_<kind> target that builds them all.
# TESTS registers each one with CTest, run from the executable's folder like
# make Run (bin/, or <app>.app/Contents/MacOS on macOS). Groups for other
# platforms (android, ios, ...) are skipped.
#
# Based on the tests and examples setup in openframeworks/openFrameworks#7524.

include_guard(GLOBAL)

# groups that only build on their own platform
function(of_project_group_skipped out_var group)
	set(skip OFF)
	if(group STREQUAL "android")
		set(skip ON)
	elseif(group STREQUAL "ios" AND NOT OF_PLATFORM STREQUAL "ios")
		set(skip ON)
	elseif(group STREQUAL "tvOS" AND NOT OF_PLATFORM STREQUAL "tvos")
		set(skip ON)
	elseif(group STREQUAL "emscripten" AND NOT OF_PLATFORM STREQUAL "emscripten")
		set(skip ON)
	elseif(group STREQUAL "gles" AND NOT OF_PLATFORM MATCHES "^linux(aarch64|armv7l|armv6l)$")
		set(skip ON)
	elseif(OF_PLATFORM MATCHES "^(ios|tvos)$" AND NOT group MATCHES "^(ios|tvOS)$")
		set(skip ON)
	endif()
	set(${out_var} ${skip} PARENT_SCOPE)
endfunction()

function(of_add_projects kind root)
	cmake_parse_arguments(A "TESTS;EXCLUDE_FROM_ALL" "" "" ${ARGN})

	add_custom_target(build_all_${kind})
	set_target_properties(build_all_${kind} PROPERTIES FOLDER "${kind}")

	set(count 0)
	file(GLOB groups LIST_DIRECTORIES true "${root}/*")
	foreach(group_dir IN LISTS groups)
		get_filename_component(group "${group_dir}" NAME)
		if(NOT IS_DIRECTORY "${group_dir}")
			continue()
		endif()
		of_project_group_skipped(skip "${group}")
		if(skip)
			continue()
		endif()

		file(GLOB projects LIST_DIRECTORIES true "${group_dir}/*")
		foreach(project_dir IN LISTS projects)
			if(NOT IS_DIRECTORY "${project_dir}/src")
				continue()
			endif()
			get_filename_component(name "${project_dir}" NAME)
			# the same project name in two groups (gl/ and ios/glRenderSmokeTest)
			if(TARGET ${name})
				set(name "${group}_${name}")
			endif()

			of_add_app(${name} PROJECT_DIR "${project_dir}")
			set_target_properties(${name} PROPERTIES FOLDER "${kind}/${group}")
			if(A_EXCLUDE_FROM_ALL)
				set_target_properties(${name} PROPERTIES EXCLUDE_FROM_ALL ON)
			endif()
			add_dependencies(build_all_${kind} ${name})

			if(A_TESTS)
				add_test(NAME "${group}/${name}"
					COMMAND $<TARGET_FILE:${name}>
					WORKING_DIRECTORY $<TARGET_FILE_DIR:${name}>)
			endif()
			math(EXPR count "${count} + 1")
		endforeach()
	endforeach()
	message(STATUS "openFrameworks: ${count} ${kind} from ${root}")
endfunction()
