/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 * Copyright (C) 2000,2005 Wim Taymans <wim@fluendo.com>
 * Copyright (C) 2006      Tim-Philipp Müller <tim centricular net>
 *
 * gsttypefindhelper.h:
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

#ifndef __GST_TYPEFINDHELPER_H__
#define __GST_TYPEFINDHELPER_H__

#include <gst/gst.h>

G_BEGIN_DECLS

GstCaps * gst_type_find_helper (GstPad *src, guint64 size);

GstCaps * gst_type_find_helper_for_buffer (GstObject              *obj,
                                           GstBuffer              *buf,
                                           GstTypeFindProbability *prob);

GstCaps * gst_type_find_helper_for_extension (GstObject * obj,
                                              const gchar * extension);

/**
 * GstTypeFindHelperGetRangeFunction:
 * @obj: a #GstObject that will handle the getrange request
 * @offset: the offset of the range
 * @length: the length of the range
 * @buffer: a memory location to hold the result buffer
 *
 * This function will be called by gst_type_find_helper_get_range() when
 * typefinding functions request to peek at the data of a stream at certain
 * offsets. If this function returns GST_FLOW_OK, the result buffer will be
 * stored in @buffer. The  contents of @buffer is invalid for any other
 * return value.
 *
 * This function is supposed to behave exactly like a #GstPadGetRangeFunction.
 *
 * Returns: GST_FLOW_OK for success
 */
typedef GstFlowReturn (*GstTypeFindHelperGetRangeFunction) (GstObject  *obj,
                                                            guint64     offset,
                                                            guint       length,
                                                            GstBuffer **buffer);

GstCaps * gst_type_find_helper_get_range (GstObject                        * obj,
                                          GstTypeFindHelperGetRangeFunction  func,
                                          guint64                            size,
                                          GstTypeFindProbability            *prob);

GstCaps * gst_type_find_helper_get_range_ext (GstObject                        * obj,
                                              GstTypeFindHelperGetRangeFunction  func,
                                              guint64                            size,
                                              const gchar                       *extension,
                                              GstTypeFindProbability            *prob);

G_END_DECLS

#endif /* __GST_TYPEFINDHELPER_H__ */
