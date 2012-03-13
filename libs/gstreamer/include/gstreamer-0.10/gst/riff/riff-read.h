/* GStreamer RIFF I/O
 * Copyright (C) 2003 Ronald Bultje <rbultje@ronald.bitfreak.net>
 *
 * riff-read.h: function declarations for parsing a RIFF file
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

#ifndef __GST_RIFF_READ_H__
#define __GST_RIFF_READ_H__

#include <glib.h>
#include <gst/gst.h>

#include "riff-ids.h"

G_BEGIN_DECLS

/*
 * Operate using pull_range().
 */

GstFlowReturn gst_riff_read_chunk   (GstElement * element,
                                     GstPad     * pad,
                                     guint64    * offset,
                                     guint32    * tag,
                                     GstBuffer ** chunk_data);

/*
 * These functions operate on provided data (the caller is
 * supposed to strip the chunk headers). The buffer is
 * provided by the caller, the strf/strh/data are filled in
 * by the function.
 */

gboolean gst_riff_parse_chunk       (GstElement * element,
                                     GstBuffer  * buf,
                                     guint      * offset,
                                     guint32    * fourcc,
                                     GstBuffer ** chunk_data);

gboolean gst_riff_parse_file_header (GstElement * element,
                                     GstBuffer  * buf,
                                     guint32    * doctype);

gboolean gst_riff_parse_strh        (GstElement     * element,
                                     GstBuffer      * buf,
                                     gst_riff_strh ** strh);

gboolean gst_riff_parse_strf_vids   (GstElement          * element,
                                     GstBuffer           * buf,
                                     gst_riff_strf_vids ** strf,
                                     GstBuffer          ** data);
gboolean gst_riff_parse_strf_auds   (GstElement          * element,
                                     GstBuffer           * buf,
                                     gst_riff_strf_auds ** strf,
                                     GstBuffer          ** data);
gboolean gst_riff_parse_strf_iavs   (GstElement          * element,
                                     GstBuffer           * buf,
                                     gst_riff_strf_iavs ** strf,
                                     GstBuffer          ** data);

void gst_riff_parse_info            (GstElement  * element,
                                     GstBuffer   * buf,
                                     GstTagList ** taglist);

/*
 * Init.
 */
void gst_riff_init                  (void);

G_END_DECLS

#endif /* __GST_RIFF_READ_H__ */
