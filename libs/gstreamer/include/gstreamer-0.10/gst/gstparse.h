/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wtay@chello.be>
 *
 * gstparse.h: get a pipeline from a text pipeline description
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

#ifndef __GST_PARSE_H__
#define __GST_PARSE_H__

#include <gst/gstelement.h>

G_BEGIN_DECLS

GQuark gst_parse_error_quark (void);
/**
 * GST_PARSE_ERROR:
 *
 * Get access to the error quark of the parse subsystem.
 */
#define GST_PARSE_ERROR gst_parse_error_quark ()

/**
 * GstParseError:
 * @GST_PARSE_ERROR_SYNTAX: A syntax error occured.
 * @GST_PARSE_ERROR_NO_SUCH_ELEMENT: The description contained an unknown element
 * @GST_PARSE_ERROR_NO_SUCH_PROPERTY: An element did not have a specified property
 * @GST_PARSE_ERROR_LINK: There was an error linking two pads.
 * @GST_PARSE_ERROR_COULD_NOT_SET_PROPERTY: There was an error setting a property
 * @GST_PARSE_ERROR_EMPTY_BIN: An empty bin was specified.
 * @GST_PARSE_ERROR_EMPTY: An empty description was specified
 *
 * The different parsing errors that can occur.
 */
typedef enum
{
  GST_PARSE_ERROR_SYNTAX,
  GST_PARSE_ERROR_NO_SUCH_ELEMENT,
  GST_PARSE_ERROR_NO_SUCH_PROPERTY,
  GST_PARSE_ERROR_LINK,
  GST_PARSE_ERROR_COULD_NOT_SET_PROPERTY,
  GST_PARSE_ERROR_EMPTY_BIN,
  GST_PARSE_ERROR_EMPTY
} GstParseError;

/**
 * GstParseFlags:
 * @GST_PARSE_FLAG_NONE: Do not use any special parsing options.
 * @GST_PARSE_FLAG_FATAL_ERRORS: Always return NULL when an error occurs
 *     (default behaviour is to return partially constructed bins or elements
 *      in some cases)
 *
 * Parsing options.
 *
 * Since: 0.10.20
 */
typedef enum
{
  GST_PARSE_FLAG_NONE = 0,
  GST_PARSE_FLAG_FATAL_ERRORS = (1 << 0)
} GstParseFlags;

/**
 * GstParseContext:
 *
 * Opaque structure.
 *
 * Since: 0.10.20
 */
typedef struct _GstParseContext GstParseContext;

/* create, process and free a parse context */

GstParseContext * gst_parse_context_new (void);

gchar          ** gst_parse_context_get_missing_elements (GstParseContext * context);

void              gst_parse_context_free (GstParseContext * context);


/* parse functions */

GstElement      * gst_parse_launch       (const gchar      * pipeline_description,
                                          GError          ** error);

GstElement      * gst_parse_launchv      (const gchar     ** argv,
                                          GError          ** error);

GstElement      * gst_parse_launch_full  (const gchar      * pipeline_description,
                                          GstParseContext  * context,
                                          GstParseFlags      flags,
                                          GError          ** error);

GstElement      * gst_parse_launchv_full (const gchar     ** argv,
                                          GstParseContext  * context,
                                          GstParseFlags      flags,
                                          GError          ** error);

G_END_DECLS

#endif /* __GST_PARSE_H__ */
