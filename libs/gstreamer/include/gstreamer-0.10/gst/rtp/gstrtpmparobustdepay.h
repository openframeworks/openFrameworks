/* GStreamer
 * Copyright (C) <2010> Mark Nauwelaerts <mark.nauwelaerts@collabora.co.uk>
 * Copyright (C) <2010> Nokia Corporation
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

#ifndef __GST_RTP_MPA_ROBUST_DEPAY_H__
#define __GST_RTP_MPA_ROBUST_DEPAY_H__

#include <gst/gst.h>
#include <gst/rtp/gstbasertpdepayload.h>
#include <gst/base/gstadapter.h>
#include <gst/base/gstbytewriter.h>

G_BEGIN_DECLS

#define GST_TYPE_RTP_MPA_ROBUST_DEPAY \
  (gst_rtp_mpa_robust_depay_get_type())
#define GST_RTP_MPA_ROBUST_DEPAY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_RTP_MPA_ROBUST_DEPAY,GstRtpMPARobustDepay))
#define GST_RTP_MPA_ROBUST_DEPAY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_RTP_MPA_ROBUST_DEPAY,GstRtpMPARobustDepayClass))
#define GST_IS_RTP_MPA_ROBUST_DEPAY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_RTP_MPA_ROBUST_DEPAY))
#define GST_IS_RTP_MPA_ROBUST_DEPAY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_RTP_MPA_ROBUST_DEPAY))

typedef struct _GstRtpMPARobustDepay GstRtpMPARobustDepay;
typedef struct _GstRtpMPARobustDepayClass GstRtpMPARobustDepayClass;

struct _GstRtpMPARobustDepay
{
  GstBaseRTPDepayload depayload;

  GstAdapter *adapter;
  gboolean    has_descriptor;

  /* last interleave index */
  gint        last_ii;
  /* last interleave cycle count */
  gint        last_icc;
  /* buffers pending deinterleaving */
  GstBuffer   *deinter[256];

  /* ADU buffers pending MP3 transformation */
  GQueue      *adu_frames;
  GList       *cur_adu_frame;
  gint         offset;
  gint         size;
  GstByteWriter *mp3_frame;
};

struct _GstRtpMPARobustDepayClass
{
  GstBaseRTPDepayloadClass parent_class;
};

GType gst_rtp_mpa_robust_depay_get_type (void);

gboolean gst_rtp_mpa_robust_depay_plugin_init (GstPlugin * plugin);

G_END_DECLS

#endif /* __GST_RTP_MPA_ROBUST_DEPAY_H__ */
