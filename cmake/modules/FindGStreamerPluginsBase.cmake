# - Try to find gst-plugins-base
# Once done this will define
#
#  GSTREAMER_PLUGINS_BASE_FOUND - system has gst-plugins-base
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

set(GSTREAMER_ABI_VERSION "1.0")


# Find the pkg-config file for doing the version check
find_package(PkgConfig)

if (PKG_CONFIG_FOUND)
    pkg_check_modules(PKG_GSTREAMER_PLUGINS_BASE QUIET gstreamer-plugins-base-${GSTREAMER_ABI_VERSION})
endif()


# Find the plugin libraries
include(MacroFindGStreamerLibrary)

if (NOT DEFINED GStreamerPluginsBase_FIND_QUIETLY)
  set(GStreamerPluginsBase_FIND_QUIETLY 0)
endif()

macro(_find_gst_plugins_base_component _name _header)
    find_gstreamer_library(${_name} ${_header} ${GSTREAMER_ABI_VERSION} ${GStreamerPluginsBase_FIND_QUIETLY})
    set(_GSTREAMER_PLUGINS_BASE_EXTRA_VARIABLES ${_GSTREAMER_PLUGINS_BASE_EXTRA_VARIABLES}
                                        GSTREAMER_${_name}_LIBRARY GSTREAMER_${_name}_INCLUDE_DIR)
endmacro()

foreach(_component ${GStreamerPluginsBase_FIND_COMPONENTS})
    if (${_component} STREQUAL "app")
        _find_gst_plugins_base_component(APP gstappsrc.h)
    elseif (${_component} STREQUAL "audio")
        _find_gst_plugins_base_component(AUDIO audio.h)
    elseif (${_component} STREQUAL "fft")
        _find_gst_plugins_base_component(FFT gstfft.h)
    elseif (${_component} STREQUAL "riff")
        _find_gst_plugins_base_component(RIFF riff-ids.h)
    elseif (${_component} STREQUAL "rtp")
        _find_gst_plugins_base_component(RTP gstrtpbuffer.h)
    elseif (${_component} STREQUAL "rtsp")
        _find_gst_plugins_base_component(RTSP gstrtspdefs.h)
    elseif (${_component} STREQUAL "sdp")
        _find_gst_plugins_base_component(SDP gstsdp.h)
    elseif (${_component} STREQUAL "tag")
        _find_gst_plugins_base_component(TAG tag.h)
    elseif (${_component} STREQUAL "pbutils")
        _find_gst_plugins_base_component(PBUTILS pbutils.h)
    elseif (${_component} STREQUAL "video")
        _find_gst_plugins_base_component(VIDEO video.h)
    else()
        message (AUTHOR_WARNING "FindGStreamer.cmake: Invalid component \"${_component}\" was specified")
    endif()
endforeach()


# Version check
if (GStreamerPluginsBase_FIND_VERSION)
    if (PKG_GSTREAMER_PLUGINS_BASE_FOUND)
        if("${PKG_GSTREAMER_PLUGINS_BASE_VERSION}" VERSION_LESS "${GStreamerPluginsBase_FIND_VERSION}")
            if (NOT GStreamerPluginsBase_FIND_QUIETLY)
                message(STATUS "Found gst-plugins-base version ${PKG_GSTREAMER_PLUGINS_BASE_VERSION}, but at least version ${GStreamerPluginsBase_FIND_VERSION} is required")
            endif()
            set(GSTREAMER_PLUGINS_BASE_VERSION_COMPATIBLE FALSE)
        else()
            set(GSTREAMER_PLUGINS_BASE_VERSION_COMPATIBLE TRUE)
        endif()
    else()
        # We can't make any version checks without pkg-config, just assume version is compatible and hope...
        set(GSTREAMER_PLUGINS_BASE_VERSION_COMPATIBLE TRUE)
    endif()
else()
    # No version constrain was specified, thus we consider the version compatible
    set(GSTREAMER_PLUGINS_BASE_VERSION_COMPATIBLE TRUE)
endif()


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GStreamerPluginsBase DEFAULT_MSG
                                  GSTREAMER_PLUGINS_BASE_VERSION_COMPATIBLE
                                  ${_GSTREAMER_PLUGINS_BASE_EXTRA_VARIABLES})