/* GStreamer
 * Copyright (C) <2005> Philippe Khalaf <burger@speedy.org>
 *               <2005> Wim Taymans <wim@fluendo.com>
 *
 * gstrtpbuffer.h: various helper functions to manipulate buffers
 *     with RTP payload.
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

#ifndef __GST_RTPBUFFER_H__
#define __GST_RTPBUFFER_H__

#include <gst/gst.h>
#include <gst/rtp/gstrtppayloads.h>

G_BEGIN_DECLS

/**
 * GST_RTP_VERSION:
 *
 * The supported RTP version 2.
 */
#define GST_RTP_VERSION 2

/* creating buffers */
void            gst_rtp_buffer_allocate_data         (GstBuffer *buffer, guint payload_len, 
                                                      guint8 pad_len, guint8 csrc_count);

GstBuffer*      gst_rtp_buffer_new_take_data         (gpointer data, guint len);
GstBuffer*      gst_rtp_buffer_new_copy_data         (gpointer data, guint len);
GstBuffer*      gst_rtp_buffer_new_allocate          (guint payload_len, guint8 pad_len, guint8 csrc_count);
GstBuffer*      gst_rtp_buffer_new_allocate_len      (guint packet_len, guint8 pad_len, guint8 csrc_count);

GstBufferList*  gst_rtp_buffer_list_from_buffer      (GstBuffer * buffer);


guint           gst_rtp_buffer_calc_header_len       (guint8 csrc_count);
guint           gst_rtp_buffer_calc_packet_len       (guint payload_len, guint8 pad_len, guint8 csrc_count);
guint           gst_rtp_buffer_calc_payload_len      (guint packet_len, guint8 pad_len, guint8 csrc_count);

gboolean        gst_rtp_buffer_validate_data         (guint8 *data, guint len);
gboolean        gst_rtp_buffer_validate              (GstBuffer *buffer);
gboolean        gst_rtp_buffer_list_validate         (GstBufferList *list);

void            gst_rtp_buffer_set_packet_len        (GstBuffer *buffer, guint len);
guint           gst_rtp_buffer_get_packet_len        (GstBuffer *buffer);

guint           gst_rtp_buffer_get_header_len        (GstBuffer *buffer);

guint8          gst_rtp_buffer_get_version           (GstBuffer *buffer);
void            gst_rtp_buffer_set_version           (GstBuffer *buffer, guint8 version);

gboolean        gst_rtp_buffer_get_padding           (GstBuffer *buffer);
void            gst_rtp_buffer_set_padding           (GstBuffer *buffer, gboolean padding);
void            gst_rtp_buffer_pad_to                (GstBuffer *buffer, guint len);

gboolean        gst_rtp_buffer_get_extension         (GstBuffer *buffer);
void            gst_rtp_buffer_set_extension         (GstBuffer *buffer, gboolean extension);
gboolean        gst_rtp_buffer_get_extension_data    (GstBuffer *buffer, guint16 *bits,
                                                      gpointer *data, guint *wordlen);
gboolean        gst_rtp_buffer_set_extension_data    (GstBuffer *buffer, guint16 bits, guint16 length);

guint32         gst_rtp_buffer_get_ssrc              (GstBuffer *buffer);
guint32         gst_rtp_buffer_list_get_ssrc         (GstBufferList *list);
void            gst_rtp_buffer_set_ssrc              (GstBuffer *buffer, guint32 ssrc);
void            gst_rtp_buffer_list_set_ssrc         (GstBufferList *list, guint32 ssrc);

guint8          gst_rtp_buffer_get_csrc_count        (GstBuffer *buffer);
guint32         gst_rtp_buffer_get_csrc              (GstBuffer *buffer, guint8 idx);
void            gst_rtp_buffer_set_csrc              (GstBuffer *buffer, guint8 idx, guint32 csrc);

gboolean        gst_rtp_buffer_get_marker            (GstBuffer *buffer);
void            gst_rtp_buffer_set_marker            (GstBuffer *buffer, gboolean marker);

guint8          gst_rtp_buffer_get_payload_type      (GstBuffer *buffer);
guint8          gst_rtp_buffer_list_get_payload_type (GstBufferList *list);
void            gst_rtp_buffer_set_payload_type      (GstBuffer *buffer, guint8 payload_type);
void            gst_rtp_buffer_list_set_payload_type (GstBufferList *list, guint8 payload_type);

guint16         gst_rtp_buffer_get_seq               (GstBuffer *buffer);
guint16         gst_rtp_buffer_list_get_seq          (GstBufferList *list);
void            gst_rtp_buffer_set_seq               (GstBuffer *buffer, guint16 seq);
guint16         gst_rtp_buffer_list_set_seq          (GstBufferList *list, guint16 seq);

guint32         gst_rtp_buffer_get_timestamp         (GstBuffer *buffer);
guint32         gst_rtp_buffer_list_get_timestamp    (GstBufferList *list);
void            gst_rtp_buffer_set_timestamp         (GstBuffer *buffer, guint32 timestamp);
void            gst_rtp_buffer_list_set_timestamp    (GstBufferList *list, guint32 timestamp);

GstBuffer* 	gst_rtp_buffer_get_payload_buffer    (GstBuffer *buffer);
GstBuffer* 	gst_rtp_buffer_get_payload_subbuffer (GstBuffer *buffer, guint offset, guint len);

guint           gst_rtp_buffer_get_payload_len       (GstBuffer *buffer);
guint           gst_rtp_buffer_list_get_payload_len  (GstBufferList *list);
gpointer        gst_rtp_buffer_get_payload           (GstBuffer *buffer);

/* some helpers */
guint32         gst_rtp_buffer_default_clock_rate    (guint8 payload_type);
gint            gst_rtp_buffer_compare_seqnum        (guint16 seqnum1, guint16 seqnum2);
guint64         gst_rtp_buffer_ext_timestamp         (guint64 *exttimestamp, guint32 timestamp);

gboolean        gst_rtp_buffer_get_extension_onebyte_header (GstBuffer * buffer,
                                                             guint8 id,
                                                             guint nth,
                                                             gpointer * data,
                                                             guint * size);
gboolean        gst_rtp_buffer_get_extension_twobytes_header (GstBuffer * buffer,
                                                              guint8 * appbits,
                                                              guint8 id,
                                                              guint nth,
                                                              gpointer * data,
                                                              guint * size);

gboolean       gst_rtp_buffer_add_extension_onebyte_header (GstBuffer * buffer,
                                                            guint8 id,
                                                            gpointer data,
                                                            guint size);
gboolean       gst_rtp_buffer_add_extension_twobytes_header (GstBuffer * buffer,
                                                             guint8 appbits,
                                                             guint8 id,
                                                             gpointer data,
                                                             guint size);

gboolean       gst_rtp_buffer_list_get_extension_onebyte_header (GstBufferList * bufferlist,
                                                                 guint group_idx,
                                                                 guint8 id,
                                                                 guint nth,
                                                                 gpointer * data,
                                                                 guint * size);
gboolean       gst_rtp_buffer_list_get_extension_twobytes_header (GstBufferList * bufferlist,
                                                                  guint group_idx,
                                                                  guint8 * appbits,
                                                                  guint8 id,
                                                                  guint nth,
                                                                  gpointer * data,
                                                                  guint * size);

gboolean       gst_rtp_buffer_list_add_extension_onebyte_header (GstBufferListIterator * it,
                                                                  guint8 id,
                                                                  gpointer data,
                                                                  guint size);
gboolean       gst_rtp_buffer_list_add_extension_twobytes_header (GstBufferListIterator * it,
                                                                  guint8 appbits,
                                                                  guint8 id,
                                                                  gpointer data,
                                                                  guint size);


G_END_DECLS

#endif /* __GST_RTPBUFFER_H__ */

