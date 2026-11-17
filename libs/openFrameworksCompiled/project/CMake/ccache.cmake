# Run ccache in front of the C and C++ compilers when it is installed.
# A launcher already set by the user or by CI is left alone.
# Turn this off with -DOF_USE_CCACHE=OFF.

include_guard(GLOBAL)

option(OF_USE_CCACHE "Prefix the C and C++ compilers with ccache when ccache is installed" ON)

if(OF_USE_CCACHE AND NOT CMAKE_C_COMPILER_LAUNCHER AND NOT CMAKE_CXX_COMPILER_LAUNCHER)
	find_program(OF_CCACHE_PROGRAM ccache)
	if(OF_CCACHE_PROGRAM)
		set(CMAKE_C_COMPILER_LAUNCHER "${OF_CCACHE_PROGRAM}" CACHE STRING "C compiler launcher")
		set(CMAKE_CXX_COMPILER_LAUNCHER "${OF_CCACHE_PROGRAM}" CACHE STRING "C++ compiler launcher")
		message(STATUS "openFrameworks: ccache ${OF_CCACHE_PROGRAM}")
	endif()
endif()
