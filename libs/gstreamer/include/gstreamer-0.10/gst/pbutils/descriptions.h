/* GStreamer base utils library source/sink/codec description support
 * Copyright (C) 2006 Tim-Philipp Müller <tim centricular net>
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

#ifndef __GST_PB_UTILS_DESCRIPTIONS_H__
#define __GST_PB_UTILS_DESCRIPTIONS_H__

#include <gst/gsttaglist.h>
#include <gst/gstcaps.h>

G_BEGIN_DECLS

/*
 * functions for use by demuxers or decoders to add CODEC tags to tag lists
 * from caps
 */

gboolean   gst_pb_utils_add_codec_description_to_tag_list (GstTagList    * taglist,
                                                             const gchar   * codec_tag,
                                                             const GstCaps * caps);

gchar    * gst_pb_utils_get_codec_description (const GstCaps * caps);

/*
 * functions mainly used by the missing plugins message creation functions to
 * find descriptions of what exactly is missing
 */

gchar    * gst_pb_utils_get_source_description (const gchar * protocol);

gchar    * gst_pb_utils_get_sink_description (const gchar * protocol);

gchar    * gst_pb_utils_get_decoder_description (const GstCaps * caps);

gchar    * gst_pb_utils_get_encoder_description (const GstCaps * caps);

gchar    * gst_pb_utils_get_element_description (const gchar * factory_name);


G_END_DECLS

#endif /* __GST_PB_UTILS_DESCRIPTIONS_H__ */

