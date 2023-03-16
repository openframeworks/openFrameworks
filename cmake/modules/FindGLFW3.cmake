#
# Find GLFW
#
# Try to find GLFW library.
# This module defines the following variables:
# - GLFW_INCLUDE_DIRS
# - GLFW_LIBRARIES
# - GLFW_FOUND
#
# The following variables can be set as arguments for the module.
# - GLFW_ROOT_DIR : Root library directory of GLFW
# - GLFW_USE_STATIC_LIBS : Specifies to use static version of GLFW library (Windows only)
#
# References:
# - https://github.com/progschj/OpenGL-Examples/blob/master/cmake_modules/FindGLFW.cmake
# - https://bitbucket.org/Ident8/cegui-mk2-opengl3-renderer/src/befd47200265/cmake/FindGLFW.cmake
#

# Additional modules
include(FindPackageHandleStandardArgs)

if (WIN32)
	# Find include files
	find_path(
		GLFW3_INCLUDE_DIR
		NAMES GLFW/glfw3.h
		PATHS
		$ENV{PROGRAMFILES}/include
		${GLFW3_ROOT_DIR}/include
		DOC "The directory where GLFW/glfw3.h resides")

	# Use glfw3.lib for static library
	if (GLFW3_USE_STATIC_LIBS)
		set(GLFW3_LIBRARY_NAME glfw3)
	else()
		set(GLFW3_LIBRARY_NAME glfw3dll)
	endif()

	# Find library files
	find_library(
		GLFW3_LIBRARY
		NAMES ${GLFW3_LIBRARY_NAME}
		PATHS
		$ENV{PROGRAMFILES}/lib
		${GLFW3_ROOT_DIR}/lib)

	unset(GLFW3_LIBRARY_NAME)
else()
	# Find include files
	find_path(
		GLFW3_INCLUDE_DIR
		NAMES GLFW/glfw3.h
		PATHS
		/usr/include
		/usr/local/include
		/sw/include
		/opt/local/include
		DOC "The directory where GL/glfw3.h resides")

	# Find library files
	# Try to use static libraries
	find_library(
		GLFW3_LIBRARY
		NAMES glfw
		PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
		${GLFW_ROOT_DIR}/lib
		DOC "The GLFW library")
endif()

# Handle REQUIRED argument, define *_FOUND variable
find_package_handle_standard_args(GLFW3 DEFAULT_MSG GLFW3_INCLUDE_DIR GLFW3_LIBRARY)

# Define GLFW3_LIBRARIES and GLFW3_INCLUDE_DIRS
if (GLFW3_FOUND)
	set(GLFW3_LIBRARIES ${OPENGL_LIBRARIES} ${GLFW3_LIBRARY})
	set(GLFW3_INCLUDE_DIRS ${GLFW3_INCLUDE_DIR})
endif()

# Hide some variables
mark_as_advanced(GLFW3_INCLUDE_DIR GLFW3_LIBRARY)