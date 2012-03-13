/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wtay@chello.be>
 *
 * gstversion.h: Version information for GStreamer
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
/**
 * SECTION:gstversion
 * @short_description: GStreamer version macros.
 *
 * Use the GST_VERSION_* macros e.g. when defining own plugins. The GStreamer
 * runtime checks if these plugin and core version match and refuses to use a
 * plugin compiled against a different version of GStreamer.
 * You can also use the macros to keep the GStreamer version information in
 * your application.
 *
 * Use the gst_version() function if you want to know which version of
 * GStreamer you are currently linked against.
 *
 * The version macros get defined by including "gst/gst.h".
 */

#ifndef __GST_VERSION_H__
#define __GST_VERSION_H__

G_BEGIN_DECLS

/**
 * GST_VERSION_MAJOR:
 *
 * The major version of GStreamer at compile time:
 */
#define GST_VERSION_MAJOR (0)
/**
 * GST_VERSION_MINOR:
 *
 * The minor version of GStreamer at compile time:
 */
#define GST_VERSION_MINOR (10)
/**
 * GST_VERSION_MICRO:
 *
 * The micro version of GStreamer at compile time:
 */
#define GST_VERSION_MICRO (31)
/**
 * GST_VERSION_NANO:
 *
 * The nano version of GStreamer at compile time:
 * Actual releases have 0, GIT versions have 1, prerelease versions have 2-...
 */
#define GST_VERSION_NANO (0)

/**
 * GST_CHECK_VERSION:
 * @major: a number indicating the major version
 * @minor: a number indicating the minor version
 * @micro: a number indicating the micro version
 *
 * Check whether a GStreamer version equal to or greater than
 * major.minor.micro is present.
 *
 * Since: 0.10.18
 */
#define	GST_CHECK_VERSION(major,minor,micro)	\
    (GST_VERSION_MAJOR > (major) || \
     (GST_VERSION_MAJOR == (major) && GST_VERSION_MINOR > (minor)) || \
     (GST_VERSION_MAJOR == (major) && GST_VERSION_MINOR == (minor) && \
      GST_VERSION_MICRO >= (micro)))

G_END_DECLS

#endif /* __GST_VERSION_H__ */
