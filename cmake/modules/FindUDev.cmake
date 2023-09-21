# - try to find the udev library
#
# Cache Variables: (probably not for direct use in your scripts)
#  UDEV_INCLUDE_DIR
#  UDEV_SOURCE_DIR
#  UDEV_LIBRARY
#
# Non-cache variables you might use in your CMakeLists.txt:
#  UDEV_FOUND
#  UDEV_INCLUDE_DIRS
#  UDEV_LIBRARIES
#
# Requires these CMake modules:
#  FindPackageHandleStandardArgs (known included with CMake >=2.6.2)
#
# Original Authors:
# 2014, Kevin M. Godby <kevin@godby.org>
# 2021, Ryan Pavlik <ryan.pavlik@collabora.com> <abiryan@ryand.net>
#
# Copyright 2014, Kevin M. Godby <kevin@godby.org>
# Copyright 2021, Collabora, Ltd.
#
# SPDX-License-Identifier: BSL-1.0
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

set(UDEV_ROOT_DIR
    "${UDEV_ROOT_DIR}"
	CACHE
	PATH
    "Directory to search for udev")

if(NOT ANDROID)
	find_package(PkgConfig QUIET)
	if(PKG_CONFIG_FOUND)
		pkg_check_modules(PC_LIBUDEV QUIET libudev)
	endif()
endif()

find_library(UDEV_LIBRARY
	NAMES
	udev
	PATHS
	${PC_LIBUDEV_LIBRARY_DIRS}
	${PC_LIBUDEV_LIBDIR}
	HINTS
	"${UDEV_ROOT_DIR}"
	PATH_SUFFIXES
	lib
	)

get_filename_component(_libdir "${UDEV_LIBRARY}" PATH)

find_path(UDEV_INCLUDE_DIR
	NAMES
	libudev.h
	PATHS
	${PC_LIBUDEV_INCLUDE_DIRS}
	${PC_LIBUDEV_INCLUDEDIR}
	HINTS
	"${_libdir}"
	"${_libdir}/.."
	"${UDEV_ROOT_DIR}"
	PATH_SUFFIXES
	include
	)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(UDev
	DEFAULT_MSG
	UDEV_LIBRARY
	UDEV_INCLUDE_DIR
	)

if(UDEV_FOUND)
	list(APPEND UDEV_LIBRARIES ${UDEV_LIBRARY})
	list(APPEND UDEV_INCLUDE_DIRS ${UDEV_INCLUDE_DIR})
	mark_as_advanced(UDEV_ROOT_DIR)
endif()

mark_as_advanced(UDEV_INCLUDE_DIR
	UDEV_LIBRARY)

