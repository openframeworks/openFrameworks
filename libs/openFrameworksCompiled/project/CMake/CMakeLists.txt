# openFrameworks core library
#
# Builds libs/openFrameworksCompiled/lib/<platform>[/<arch>]/<core library>.
# Apps include openFrameworks.cmake and link it with of_add_app(); they build
# this project themselves if the library isn't there yet.
#
#   cmake -S libs/openFrameworksCompiled/project/CMake -B build/core -DCMAKE_BUILD_TYPE=Release
#   cmake --build build/core
#
# -DOF_BUILD_TESTS=ON adds the unit tests in tests/ and registers them with CTest:
#   cmake --build build/core && ctest --test-dir build/core --output-on-failure

cmake_minimum_required(VERSION 3.22)
project(openFrameworks LANGUAGES C CXX)

include("${CMAKE_CURRENT_LIST_DIR}/openFrameworks.cmake")

option(OF_BUILD_TESTS "Build the unit tests in tests/ and register them with CTest" OFF)

of_add_core_library()

if(OF_BUILD_TESTS)
	if(OF_PLATFORM MATCHES "^(emscripten|ios|tvos)$")
		message(WARNING "openFrameworks: OF_BUILD_TESTS is for desktop platforms; skipping on ${OF_PLATFORM}")
	else()
		enable_testing()
		include("${CMAKE_CURRENT_LIST_DIR}/OFProjects.cmake")
		of_add_projects(tests "${OF_ROOT}/tests" TESTS)
	endif()
endif()
