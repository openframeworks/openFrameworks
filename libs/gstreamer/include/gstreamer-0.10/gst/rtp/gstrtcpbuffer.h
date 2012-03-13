/* GStreamer
 * Copyright (C) <2007> Wim Taymans <wim@fluendo.com>
 *
 * gstrtcpbuffer.h: various helper functions to manipulate buffers
 *     with RTCP payload.
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

#ifndef __GST_RTCPBUFFER_H__
#define __GST_RTCPBUFFER_H__

#include <gst/gst.h>

G_BEGIN_DECLS

/**
 * GST_RTCP_VERSION:
 *
 * The supported RTCP version 2.
 */
#define GST_RTCP_VERSION 2

/** 
 * GstRTCPType:
 * @GST_RTCP_TYPE_INVALID: Invalid type
 * @GST_RTCP_TYPE_SR: Sender report
 * @GST_RTCP_TYPE_RR: Receiver report
 * @GST_RTCP_TYPE_SDES: Source description
 * @GST_RTCP_TYPE_BYE: Goodbye
 * @GST_RTCP_TYPE_APP: Application defined
 * @GST_RTCP_TYPE_RTPFB: Transport layer feedback. Since: 0.10.23
 * @GST_RTCP_TYPE_PSFB: Payload-specific feedback. Since: 0.10.23
 *
 * Different RTCP packet types.
 */
typedef enum
{
  GST_RTCP_TYPE_INVALID = 0,
  GST_RTCP_TYPE_SR      = 200,
  GST_RTCP_TYPE_RR      = 201,
  GST_RTCP_TYPE_SDES    = 202,
  GST_RTCP_TYPE_BYE     = 203,
  GST_RTCP_TYPE_APP     = 204,
  GST_RTCP_TYPE_RTPFB   = 205,
  GST_RTCP_TYPE_PSFB    = 206
} GstRTCPType;

/**
 * GstRTCPFBType:
 * @GST_RTCP_FB_TYPE_INVALID: Invalid type
 * @GST_RTCP_RTPFB_TYPE_NACK: Generic NACK
 * @GST_RTCP_PSFB_TYPE_PLI: Picture Loss Indication
 * @GST_RTCP_PSFB_TYPE_SLI: Slice Loss Indication
 * @GST_RTCP_PSFB_TYPE_RPSI: Reference Picture Selection Indication
 * @GST_RTCP_PSFB_TYPE_AFB: Application layer Feedback
 *
 * Different types of feedback messages.
 *
 * Since: 0.10.23
 */
typedef enum
{
  /* generic */
  GST_RTCP_FB_TYPE_INVALID    = 0,
  /* RTPFB types */
  GST_RTCP_RTPFB_TYPE_NACK    = 1,
  /* PSFB types */
  GST_RTCP_PSFB_TYPE_PLI      = 1,
  GST_RTCP_PSFB_TYPE_SLI      = 2,
  GST_RTCP_PSFB_TYPE_RPSI     = 3,
  GST_RTCP_PSFB_TYPE_AFB      = 15
} GstRTCPFBType;

/** 
 * GstRTCPSDESType:
 * @GST_RTCP_SDES_INVALID: Invalid SDES entry
 * @GST_RTCP_SDES_END: End of SDES list
 * @GST_RTCP_SDES_CNAME: Canonical name
 * @GST_RTCP_SDES_NAME: User name
 * @GST_RTCP_SDES_EMAIL: User's electronic mail address
 * @GST_RTCP_SDES_PHONE: User's phone number
 * @GST_RTCP_SDES_LOC: Geographic user location
 * @GST_RTCP_SDES_TOOL: Name of application or tool
 * @GST_RTCP_SDES_NOTE: Notice about the source
 * @GST_RTCP_SDES_PRIV: Private extensions
 *
 * Different types of SDES content.
 */
typedef enum 
{
  GST_RTCP_SDES_INVALID  = -1,
  GST_RTCP_SDES_END      = 0,
  GST_RTCP_SDES_CNAME    = 1,
  GST_RTCP_SDES_NAME     = 2,
  GST_RTCP_SDES_EMAIL    = 3,
  GST_RTCP_SDES_PHONE    = 4,
  GST_RTCP_SDES_LOC      = 5,
  GST_RTCP_SDES_TOOL     = 6,
  GST_RTCP_SDES_NOTE     = 7,
  GST_RTCP_SDES_PRIV     = 8
} GstRTCPSDESType;

/**
 * GST_RTCP_MAX_SDES:
 *
 * The maximum text length for an SDES item.
 */
#define GST_RTCP_MAX_SDES 255

/**
 * GST_RTCP_MAX_RB_COUNT:
 *
 * The maximum amount of Receiver report blocks in RR and SR messages.
 */
#define GST_RTCP_MAX_RB_COUNT   31

/**
 * GST_RTCP_MAX_SDES_ITEM_COUNT:
 *
 * The maximum amount of SDES items.
 */
#define GST_RTCP_MAX_SDES_ITEM_COUNT   31

/**
 * GST_RTCP_MAX_BYE_SSRC_COUNT:
 *
 * The maximum amount of SSRCs in a BYE packet.
 */
#define GST_RTCP_MAX_BYE_SSRC_COUNT   31

/**
 * GST_RTCP_VALID_MASK:
 *
 * Mask for version, padding bit and packet type pair
 */
#define GST_RTCP_VALID_MASK (0xc000 | 0x2000 | 0xfe)
/**
 * GST_RTCP_VALID_VALUE:
 *
 * Valid value for the first two bytes of an RTCP packet after applying
 * #GST_RTCP_VALID_MASK to them.
 */
#define GST_RTCP_VALID_VALUE ((GST_RTCP_VERSION << 14) | GST_RTCP_TYPE_SR)

typedef struct _GstRTCPPacket GstRTCPPacket;

/**
 * GstRTCPPacket:
 * @buffer: pointer to RTCP buffer
 * @offset: offset of packet in buffer data
 *
 * Data structure that points to a packet at @offset in @buffer. 
 * The size of the structure is made public to allow stack allocations.
 */
struct _GstRTCPPacket
{ 
  GstBuffer   *buffer;
  guint        offset;
  
  /*< private >*/
  gboolean     padding;      /* padding field of current packet */
  guint8       count;        /* count field of current packet */
  GstRTCPType  type;         /* type of current packet */
  guint16      length;       /* length of current packet in 32-bits words */

  guint        item_offset;  /* current item offset for navigating SDES */
  guint        item_count;   /* current item count */
  guint        entry_offset; /* current entry offset for navigating SDES items */
};

/* creating buffers */
GstBuffer*      gst_rtcp_buffer_new_take_data     (gpointer data, guint len);
GstBuffer*      gst_rtcp_buffer_new_copy_data     (gpointer data, guint len);

gboolean        gst_rtcp_buffer_validate_data     (guint8 *data, guint len);
gboolean        gst_rtcp_buffer_validate          (GstBuffer *buffer);

GstBuffer*      gst_rtcp_buffer_new               (guint mtu);
void            gst_rtcp_buffer_end               (GstBuffer *buffer);

/* adding/retrieving packets */
guint           gst_rtcp_buffer_get_packet_count  (GstBuffer *buffer);
gboolean        gst_rtcp_buffer_get_first_packet  (GstBuffer *buffer, GstRTCPPacket *packet);
gboolean        gst_rtcp_packet_move_to_next      (GstRTCPPacket *packet);

gboolean        gst_rtcp_buffer_add_packet        (GstBuffer *buffer, GstRTCPType type,
		                                   GstRTCPPacket *packet);
gboolean        gst_rtcp_packet_remove            (GstRTCPPacket *packet);

/* working with packets */
gboolean        gst_rtcp_packet_get_padding       (GstRTCPPacket *packet);
guint8          gst_rtcp_packet_get_count         (GstRTCPPacket *packet);
GstRTCPType     gst_rtcp_packet_get_type          (GstRTCPPacket *packet);
guint16         gst_rtcp_packet_get_length        (GstRTCPPacket *packet);


/* sender reports */ 
void            gst_rtcp_packet_sr_get_sender_info    (GstRTCPPacket *packet, guint32 *ssrc, 
                                                       guint64 *ntptime, guint32 *rtptime, 
						       guint32 *packet_count, guint32 *octet_count);
void            gst_rtcp_packet_sr_set_sender_info    (GstRTCPPacket *packet, guint32 ssrc, 
                                                       guint64 ntptime, guint32 rtptime, 
						       guint32 packet_count, guint32 octet_count);
/* receiver reports */ 
guint32         gst_rtcp_packet_rr_get_ssrc           (GstRTCPPacket *packet);
void            gst_rtcp_packet_rr_set_ssrc           (GstRTCPPacket *packet, guint32 ssrc);


/* report blocks for SR and RR */
guint           gst_rtcp_packet_get_rb_count          (GstRTCPPacket *packet);
void            gst_rtcp_packet_get_rb                (GstRTCPPacket *packet, guint nth, guint32 *ssrc,
		                                       guint8 *fractionlost, gint32 *packetslost,
						       guint32 *exthighestseq, guint32 *jitter,
						       guint32 *lsr, guint32 *dlsr);
gboolean        gst_rtcp_packet_add_rb                (GstRTCPPacket *packet, guint32 ssrc,
		                                       guint8 fractionlost, gint32 packetslost,
						       guint32 exthighestseq, guint32 jitter,
						       guint32 lsr, guint32 dlsr);
void            gst_rtcp_packet_set_rb                (GstRTCPPacket *packet, guint nth, guint32 ssrc,
		                                       guint8 fractionlost, gint32 packetslost,
						       guint32 exthighestseq, guint32 jitter,
						       guint32 lsr, guint32 dlsr);

/* source description packet */
guint           gst_rtcp_packet_sdes_get_item_count   (GstRTCPPacket *packet);
gboolean        gst_rtcp_packet_sdes_first_item       (GstRTCPPacket *packet);
gboolean        gst_rtcp_packet_sdes_next_item        (GstRTCPPacket *packet);
guint32         gst_rtcp_packet_sdes_get_ssrc         (GstRTCPPacket *packet); 
gboolean        gst_rtcp_packet_sdes_first_entry      (GstRTCPPacket *packet);
gboolean        gst_rtcp_packet_sdes_next_entry       (GstRTCPPacket *packet);
gboolean        gst_rtcp_packet_sdes_get_entry        (GstRTCPPacket *packet, 
                                                       GstRTCPSDESType *type, guint8 *len,
						       guint8 **data);
gboolean        gst_rtcp_packet_sdes_copy_entry       (GstRTCPPacket *packet, 
                                                       GstRTCPSDESType *type, guint8 *len,
						       guint8 **data);

gboolean        gst_rtcp_packet_sdes_add_item         (GstRTCPPacket *packet, guint32 ssrc);
gboolean        gst_rtcp_packet_sdes_add_entry        (GstRTCPPacket *packet, GstRTCPSDESType type, 
                                                       guint8 len, const guint8 *data);

/* bye packet */
guint           gst_rtcp_packet_bye_get_ssrc_count    (GstRTCPPacket *packet);
guint32         gst_rtcp_packet_bye_get_nth_ssrc      (GstRTCPPacket *packet, guint nth);
gboolean        gst_rtcp_packet_bye_add_ssrc          (GstRTCPPacket *packet, guint32 ssrc);
gboolean        gst_rtcp_packet_bye_add_ssrcs         (GstRTCPPacket *packet, guint32 *ssrc, guint len);
guint8          gst_rtcp_packet_bye_get_reason_len    (GstRTCPPacket *packet);
gchar*          gst_rtcp_packet_bye_get_reason        (GstRTCPPacket *packet);
gboolean        gst_rtcp_packet_bye_set_reason        (GstRTCPPacket *packet, const gchar *reason);

/* feedback packets */
guint32         gst_rtcp_packet_fb_get_sender_ssrc    (GstRTCPPacket *packet);
void            gst_rtcp_packet_fb_set_sender_ssrc    (GstRTCPPacket *packet, guint32 ssrc);
guint32         gst_rtcp_packet_fb_get_media_ssrc     (GstRTCPPacket *packet);
void            gst_rtcp_packet_fb_set_media_ssrc     (GstRTCPPacket *packet, guint32 ssrc);
GstRTCPFBType   gst_rtcp_packet_fb_get_type           (GstRTCPPacket *packet);
void            gst_rtcp_packet_fb_set_type           (GstRTCPPacket *packet, GstRTCPFBType type);
guint16         gst_rtcp_packet_fb_get_fci_length     (GstRTCPPacket *packet);
gboolean        gst_rtcp_packet_fb_set_fci_length     (GstRTCPPacket *packet, guint16 wordlen);
guint8 *        gst_rtcp_packet_fb_get_fci            (GstRTCPPacket *packet);

/* helper functions */
guint64         gst_rtcp_ntp_to_unix                  (guint64 ntptime);
guint64         gst_rtcp_unix_to_ntp                  (guint64 unixtime);

const gchar *   gst_rtcp_sdes_type_to_name            (GstRTCPSDESType type);
GstRTCPSDESType gst_rtcp_sdes_name_to_type            (const gchar *name);

G_END_DECLS

#endif /* __GST_RTCPBUFFER_H__ */

