/* GStreamer base utils library missing plugins support
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

#ifndef __GST_PB_UTILS_MISSING_PLUGINS_H__
#define __GST_PB_UTILS_MISSING_PLUGINS_H__

#include <gst/gstmessage.h>
#include <gst/gstcaps.h>

G_BEGIN_DECLS

/*
 * functions to create missing-plugin messages, for use by plugins primarily
 */

GstMessage * gst_missing_uri_source_message_new (GstElement    * element,
                                                 const gchar   * protocol);

GstMessage * gst_missing_uri_sink_message_new   (GstElement    * element,
                                                 const gchar   * protocol);

GstMessage * gst_missing_element_message_new    (GstElement    * element,
                                                 const gchar   * factory_name);

GstMessage * gst_missing_decoder_message_new    (GstElement    * element,
                                                 const GstCaps * decode_caps);

GstMessage * gst_missing_encoder_message_new    (GstElement    * element,
                                                 const GstCaps * encode_caps);

/*
 * functions for use by applications when dealing with missing-plugin messages
 */

gchar       * gst_missing_plugin_message_get_installer_detail (GstMessage * msg);

gchar       * gst_missing_plugin_message_get_description (GstMessage * msg);

gboolean      gst_is_missing_plugin_message (GstMessage * msg);


/*
 * functions for use by applications that know exactly what plugins they are
 * missing and want to request them directly rather than just react to
 * missing-plugin messages posted by elements such as playbin or decodebin
 */

gchar * gst_missing_uri_source_installer_detail_new (const gchar * protocol);

gchar * gst_missing_uri_sink_installer_detail_new (const gchar * protocol);

gchar * gst_missing_element_installer_detail_new (const gchar * factory_name);

gchar * gst_missing_decoder_installer_detail_new (const GstCaps * decode_caps);

gchar * gst_missing_encoder_installer_detail_new (const GstCaps * encode_caps);

G_END_DECLS

#endif /* __GST_PB_UTILS_MISSING_PLUGINS_H__ */

