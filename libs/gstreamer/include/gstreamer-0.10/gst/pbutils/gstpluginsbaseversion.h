/* GStreamer base plugins libraries version information
 * Copyright (C) 2010 Tim-Philipp Müller <tim centricular net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GST_PLUGINS_BASE_VERSION_H__
#define __GST_PLUGINS_BASE_VERSION_H__

#include <glib.h>

G_BEGIN_DECLS

/**
 * GST_PLUGINS_BASE_VERSION_MAJOR:
 *
 * The major version of GStreamer's gst-plugins-base libraries at compile time.
 *
 * Since: 0.10.31
 */
#define GST_PLUGINS_BASE_VERSION_MAJOR (0)
/**
 * GST_PLUGINS_BASE_VERSION_MINOR:
 *
 * The minor version of GStreamer's gst-plugins-base libraries at compile time.
 *
 * Since: 0.10.31
 */
#define GST_PLUGINS_BASE_VERSION_MINOR (10)
/**
 * GST_PLUGINS_BASE_VERSION_MICRO:
 *
 * The micro version of GStreamer's gst-plugins-base libraries at compile time.
 *
 * Since: 0.10.31
 */
#define GST_PLUGINS_BASE_VERSION_MICRO (31)
/**
 * GST_PLUGINS_BASE_VERSION_NANO:
 *
 * The nano version of GStreamer's gst-plugins-base libraries at compile time.
 * Actual releases have 0, GIT versions have 1, prerelease versions have 2-...
 *
 * Since: 0.10.31
 */
#define GST_PLUGINS_BASE_VERSION_NANO (0)

/**
 * GST_CHECK_PLUGIN_BASE_VERSION:
 * @major: a number indicating the major version
 * @minor: a number indicating the minor version
 * @micro: a number indicating the micro version
 *
 * Check whether a GStreamer's gst-plugins-base libraries' version equal to
 * or greater than major.minor.micro is present.
 *
 * Since: 0.10.31
 */
#define	GST_CHECK_PLUGINS_BASE_VERSION(major,minor,micro)	\
    (GST_PLUGINS_BASE_VERSION_MAJOR > (major) || \
     (GST_PLUGINS_BASE_VERSION_MAJOR == (major) && GST_PLUGINS_BASE_VERSION_MINOR > (minor)) || \
     (GST_PLUGINS_BASE_VERSION_MAJOR == (major) && GST_PLUGINS_BASE_VERSION_MINOR == (minor) && \
      GST_PLUGINS_BASE_VERSION_MICRO >= (micro)))

void     gst_plugins_base_version (guint *major, guint *minor, guint *micro, guint *nano);

gchar *  gst_plugins_base_version_string (void);

G_END_DECLS

#endif /* __GST_PLUGINS_BASE_VERSION_H__ */
