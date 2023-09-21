# - macro find_gstreamer_library
#
# Copyright (c) 2010, Collabora Ltd.
#   @author George Kiagiadakis <george.kiagiadakis@collabora.co.uk>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

macro(find_gstreamer_library _name _header _abi_version _quiet)
    string(TOLOWER ${_name} _lower_name)
    string(TOUPPER ${_name} _upper_name)

    if (GSTREAMER_${_upper_name}_LIBRARY AND GSTREAMER_${_upper_name}_INCLUDE_DIR)
        set(_GSTREAMER_${_upper_name}_QUIET TRUE)
    else()
        set(_GSTREAMER_${_upper_name}_QUIET FALSE)
    endif()

    if (PKG_CONFIG_FOUND)
        pkg_check_modules(PKG_GSTREAMER_${_upper_name} QUIET gstreamer-${_lower_name}-${_abi_version})
    endif()

    find_library(GSTREAMER_${_upper_name}_LIBRARY
                 NAMES gst${_lower_name}-${_abi_version}
                 HINTS ${PKG_GSTREAMER_${_upper_name}_LIBRARY_DIRS}
                       ${PKG_GSTREAMER_${_upper_name}_LIBDIR}
    )

    find_path(GSTREAMER_${_upper_name}_INCLUDE_DIR
              gst/${_lower_name}/${_header}
              HINTS ${PKG_GSTREAMER_${_upper_name}_INCLUDE_DIRS}
                    ${PKG_GSTREAMER_${_upper_name}_INCLUDEDIR}
              PATH_SUFFIXES gstreamer-${_abi_version}
    )

    if (GSTREAMER_${_upper_name}_LIBRARY AND GSTREAMER_${_upper_name}_INCLUDE_DIR)
        set(GSTREAMER_${_upper_name}_LIBRARY_FOUND TRUE)
    else()
        set(GSTREAMER_${_upper_name}_LIBRARY_FOUND FALSE)
    endif()

    if (NOT _GSTREAMER_${_upper_name}_QUIET AND NOT _quiet)
        if (GSTREAMER_${_upper_name}_LIBRARY)
            message(STATUS "Found GSTREAMER_${_upper_name}_LIBRARY: ${GSTREAMER_${_upper_name}_LIBRARY}")
        else()
            message(STATUS "Could NOT find GSTREAMER_${_upper_name}_LIBRARY")
        endif()

        if (GSTREAMER_${_upper_name}_INCLUDE_DIR)
            message(STATUS "Found GSTREAMER_${_upper_name}_INCLUDE_DIR: ${GSTREAMER_${_upper_name}_INCLUDE_DIR}")
        else()
            message(STATUS "Could NOT find GSTREAMER_${_upper_name}_INCLUDE_DIR")
        endif()
    endif()

    mark_as_advanced(GSTREAMER_${_upper_name}_LIBRARY GSTREAMER_${_upper_name}_INCLUDE_DIR)
endmacro()