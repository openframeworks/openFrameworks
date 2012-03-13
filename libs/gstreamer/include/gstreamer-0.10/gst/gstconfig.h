/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *               2004,2005 Wim Taymans <wim@fluendo.com>
 *
 * gstconfig.h: GST_DISABLE_* macros for build configuration
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
 * SECTION:gstconfig
 * @short_description: Build configuration options
 *
 * This describes the configuration options for GStreamer. When building
 * GStreamer there are a lot of parts (known internally as "subsystems" ) that
 * can be disabled for various reasons. The most common reasons are speed and
 * size, which is important because GStreamer is designed to run on embedded
 * systems.
 *
 * If a subsystem is disabled, most of this changes are done in an API
 * compatible way, so you don't need to adapt your code in most cases. It is
 * never done in an ABI compatible way though. So if you want to disable a
 * suybsystem, you have to rebuild all programs depending on GStreamer, too.
 *
 * If a subsystem is disabled in GStreamer, a value is defined in
 * &lt;gst/gst.h&gt;. You can check this if you do subsystem-specific stuff.
 * <example>
 * <title>Doing subsystem specific things</title>
 * <programlisting>
 * &hash;ifndef GST_DISABLE_GST_DEBUG
 * // do stuff specific to the debugging subsystem
 * &hash;endif // GST_DISABLE_GST_DEBUG
 * </programlisting>
 * </example>
 */

#ifndef __GST_CONFIG_H__
#define __GST_CONFIG_H__

/*** trick gtk-doc into believing these symbols are defined (yes, it's ugly) */

#if 0
#define GST_DISABLE_LOADSAVE_REGISTRY 1
#define GST_DISABLE_GST_DEBUG 1
#define GST_DISABLE_LOADSAVE 1
#define GST_DISABLE_PARSE 1
#define GST_DISABLE_TRACE 1
#define GST_DISABLE_ALLOC_TRACE 1
#define GST_DISABLE_REGISTRY 1
#define GST_DISABLE_PLUGIN 1
#define GST_HAVE_GLIB_2_8 1
#endif

/***** default padding of structures *****/
#define GST_PADDING		4
#define GST_PADDING_INIT	{0}

/***** padding for very extensible base classes *****/
#define GST_PADDING_LARGE       20


/***** disabling of subsystems *****/

/* wether or not the debugging subsystem is enabled */
/* #undef GST_DISABLE_GST_DEBUG */

/* DOES NOT WORK */
/* #undef GST_DISABLE_LOADSAVE */

/* DOES NOT WORK */
/* #undef GST_DISABLE_PARSE */

/* DOES NOT WORK */
/* #undef GST_DISABLE_TRACE */

/* DOES NOT WORK */
/* #undef GST_DISABLE_ALLOC_TRACE */

/* DOES NOT WORK */
/* #undef GST_DISABLE_REGISTRY */

/* DOES NOT WORK */
/* #undef GST_DISABLE_PLUGIN */

/* printf extension format */
/**
 * GST_PTR_FORMAT:
 *
 * printf format type used to debug GStreamer types.
 * This can only be used on types whose size is >= sizeof(gpointer).
 */
#define GST_PTR_FORMAT "p"

/**
 * GST_SEGMENT_FORMAT:
 *
 * printf format type used to debug GStreamer segments.
 * This can only be used on pointers to GstSegment structures.
 *
 * Since: 0.10.10
 */
#define GST_SEGMENT_FORMAT "p"

/* whether or not GST_PTR_FORMAT or GST_SEGMENT_FORMAT are using
 * the printf extension mechanism. This is for internal use in our
 * header files so we know whether we can use G_GNUC_PRINTF or not */
#undef GST_USING_PRINTF_EXTENSION

/* whether or not the CPU supports unaligned access */
#define GST_HAVE_UNALIGNED_ACCESS 1

/* whether or not we are using glib 2.8 api, e.g. atomic gobject
   refcounting */
#define GST_HAVE_GLIB_2_8 1

/***** Deal with XML stuff, we have to handle both loadsave and registry *****/

#if (! (defined(GST_DISABLE_LOADSAVE) && defined(GST_DISABLE_REGISTRY)) )
# include <libxml/parser.h>
#else
# define GST_DISABLE_LOADSAVE_REGISTRY
#endif

/**
 * GST_EXPORT:
 *
 * Export the given variable from the built shared object.
 *
 * On Windows, this exports the variable from the DLL.
 * On other platforms, this gets defined to "extern".
 */
/**
 * GST_PLUGIN_EXPORT:
 *
 * Export the plugin's definition.
 *
 * On Windows, this exports the plugin definition from the DLL.
 * On other platforms, this gets defined as a no-op.
 */
#if defined(WIN32) && (!defined(__MINGW32__))
#define GST_PLUGIN_EXPORT __declspec(dllexport) extern
#ifdef GST_EXPORTS
#define GST_EXPORT __declspec(dllexport) extern
#else
#define GST_EXPORT __declspec(dllimport) extern
#endif
#else /* not WIN32 */
#define GST_PLUGIN_EXPORT
#define GST_EXPORT extern
#endif

#endif /* __GST_CONFIG_H__ */
