# - Try to find GStreamer
# Once done this will define
#
#  GSTREAMER_FOUND - system has GStreamer
#  GSTREAMER_INCLUDE_DIR - the GStreamer main include directory
#  GSTREAMER_INCLUDE_DIRS - the GStreamer include directories
#  GSTREAMER_LIBRARY - the main GStreamer library
#  GSTREAMER_PLUGIN_DIR - the GStreamer plugin directory
#
#  And for all the plugin libraries specified in the COMPONENTS
#  of find_package, this module will define:
#
#  GSTREAMER_<plugin_lib>_LIBRARY_FOUND - system has <plugin_lib>
#  GSTREAMER_<plugin_lib>_LIBRARY - the <plugin_lib> library
#  GSTREAMER_<plugin_lib>_INCLUDE_DIR - the <plugin_lib> include directory
#
# Copyright (c) 2010, Collabora Ltd.
#   @author George Kiagiadakis <george.kiagiadakis@collabora.co.uk>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if (GSTREAMER_INCLUDE_DIR AND GSTREAMER_LIBRARY)
    set(GStreamer_FIND_QUIETLY TRUE)
else()
    set(GStreamer_FIND_QUIETLY FALSE)
endif()

set(GSTREAMER_ABI_VERSION "1.0")


# Find the main library
find_package(PkgConfig)

if (PKG_CONFIG_FOUND)
    pkg_check_modules(PKG_GSTREAMER QUIET gstreamer-${GSTREAMER_ABI_VERSION})
    if(PKG_GSTREAMER_FOUND)
        exec_program(${PKG_CONFIG_EXECUTABLE}
                     ARGS --variable pluginsdir gstreamer-${GSTREAMER_ABI_VERSION}
                     OUTPUT_VARIABLE PKG_GSTREAMER_PLUGIN_DIR)
    endif()
    set(GSTREAMER_DEFINITIONS ${PKG_GSTREAMER_CFLAGS})
endif()

find_library(GSTREAMER_LIBRARY
             NAMES gstreamer-${GSTREAMER_ABI_VERSION}
             HINTS ${PKG_GSTREAMER_LIBRARY_DIRS} ${PKG_GSTREAMER_LIBDIR})

find_library(GSTREAMERBASE_LIBRARY
            NAMES gstbase-${GSTREAMER_ABI_VERSION}
            HINTS ${PKG_GSTREAMER_LIBRARY_DIRS} ${PKG_GSTREAMER_LIBDIR})

find_path(GSTREAMER_INCLUDE_DIR
          gst/gst.h
          HINTS ${PKG_GSTREAMER_INCLUDE_DIRS} ${PKG_GSTREAMER_INCLUDEDIR}
          PATH_SUFFIXES gstreamer-${GSTREAMER_ABI_VERSION})

find_path(GSTREAMER_gstconfig_INCLUDE_DIR
          gst/gstconfig.h
          HINTS ${PKG_GSTREAMER_INCLUDE_DIRS} ${PKG_GSTREAMER_INCLUDEDIR}
          PATH_SUFFIXES gstreamer-${GSTREAMER_ABI_VERSION})

set(GSTREAMER_INCLUDE_DIRS ${GSTREAMER_INCLUDE_DIR} ${GSTREAMER_gstconfig_INCLUDE_DIR})
list(REMOVE_DUPLICATES GSTREAMER_INCLUDE_DIRS)

if (PKG_GSTREAMER_PLUGIN_DIR)
    set(_GSTREAMER_PLUGIN_DIR ${PKG_GSTREAMER_PLUGIN_DIR})
else()
    get_filename_component(_GSTREAMER_LIB_DIR ${GSTREAMER_LIBRARY} PATH)
    set(_GSTREAMER_PLUGIN_DIR ${_GSTREAMER_LIB_DIR}/gstreamer-${GSTREAMER_ABI_VERSION})
endif()

set(GSTREAMER_PLUGIN_DIR ${_GSTREAMER_PLUGIN_DIR}
    CACHE PATH "The path to the gstreamer plugins installation directory")

mark_as_advanced(GSTREAMER_LIBRARY
                 GSTREAMER_INCLUDE_DIR
                 GSTREAMER_gstconfig_INCLUDE_DIR
                 GSTREAMER_PLUGIN_DIR)

# Version check
if (GStreamer_FIND_VERSION)
    if (PKG_GSTREAMER_FOUND)
        if("${PKG_GSTREAMER_VERSION}" VERSION_LESS "${GStreamer_FIND_VERSION}")
            if(NOT GStreamer_FIND_QUIETLY)
                message(STATUS "Found GStreamer version ${PKG_GSTREAMER_VERSION}, but at least version ${GStreamer_FIND_VERSION} is required")
            endif()
            set(GSTREAMER_VERSION_COMPATIBLE FALSE)
        else()
            set(GSTREAMER_VERSION_COMPATIBLE TRUE)
        endif()
    elseif(GSTREAMER_INCLUDE_DIR)
        include(CheckCXXSourceCompiles)

        set(CMAKE_REQUIRED_INCLUDES ${GSTREAMER_INCLUDE_DIR})
        string(REPLACE "." "," _comma_version ${GStreamer_FIND_VERSION})
        # Hack to invalidate the cached value
        set(GSTREAMER_VERSION_COMPATIBLE GSTREAMER_VERSION_COMPATIBLE)

        check_cxx_source_compiles("
#define G_BEGIN_DECLS
#define G_END_DECLS
#include <gst/gstversion.h>
#if GST_CHECK_VERSION(${_comma_version})
int main() { return 0; }
#else
# error \"GStreamer version incompatible\"
#endif
" GSTREAMER_VERSION_COMPATIBLE)

        if (NOT GSTREAMER_VERSION_COMPATIBLE AND NOT GStreamer_FIND_QUIETLY)
            message(STATUS "GStreamer ${GStreamer_FIND_VERSION} is required, but the version found is older")
        endif()
    else()
        # We didn't find gstreamer at all
        set(GSTREAMER_VERSION_COMPATIBLE FALSE)
    endif()
else()
    # No version constrain was specified, thus we consider the version compatible
    set(GSTREAMER_VERSION_COMPATIBLE TRUE)
endif()


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GStreamer DEFAULT_MSG
                                  GSTREAMER_LIBRARY GSTREAMER_INCLUDE_DIRS
                                  GSTREAMER_VERSION_COMPATIBLE ${_GSTREAMER_EXTRA_VARIABLES})