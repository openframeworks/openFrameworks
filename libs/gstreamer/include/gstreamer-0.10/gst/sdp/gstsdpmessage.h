/* GStreamer
 * Copyright (C) <2005,2006> Wim Taymans <wim@fluendo.com>
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
/*
 * Unless otherwise indicated, Source Code is licensed under MIT license.
 * See further explanation attached in License Statement (distributed in the file
 * LICENSE).
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __GST_SDP_MESSAGE_H__
#define __GST_SDP_MESSAGE_H__

#include <glib.h>

#include <gst/sdp/gstsdp.h>

G_BEGIN_DECLS

/**
 * GstSDPOrigin:
 * @username: the user's login on the originating host, or it is "-"
 *    if the originating host does not support the concept of user ids.
 * @sess_id: is a numeric string such that the tuple of @username, @sess_id,
 *    @nettype, @addrtype and @addr form a globally unique identifier for the
 *    session.
 * @sess_version: a version number for this announcement
 * @nettype: the type of network. "IN" is defined to have the meaning
 *    "Internet".
 * @addrtype: the type of @addr.
 * @addr: the globally unique address of the machine from which the session was
 *     created.
 *
 * The contents of the SDP "o=" field which gives the originator of the session
 * (their username and the address of the user's host) plus a session id and
 * session version number.
 */
typedef struct {
  gchar *username;
  gchar *sess_id;
  gchar *sess_version;
  gchar *nettype;
  gchar *addrtype;
  gchar *addr;
} GstSDPOrigin;

/**
 * GstSDPConnection:
 * @nettype: the type of network. "IN" is defined to have the meaning
 *    "Internet".
 * @addrtype: the type of @address.
 * @address: the address
 * @ttl: the time to live of the address
 * @addr_number: the number of layers
 *
 * The contents of the SDP "c=" field which contains connection data.
 */
typedef struct {
  gchar *nettype;
  gchar *addrtype;
  gchar *address;
  guint  ttl;
  guint  addr_number;
} GstSDPConnection;

/**
 * GST_SDP_BWTYPE_CT:
 *
 * The Conference Total bandwidth modifier.
 */
#define GST_SDP_BWTYPE_CT               "CT"
/**
 * GST_SDP_BWTYPE_AS:
 *
 * The Application-Specific Maximum bandwidth modifier.
 */
#define GST_SDP_BWTYPE_AS               "AS"
/**
 * GST_SDP_BWTYPE_EXT_PREFIX:
 *
 * The extension prefix bandwidth modifier.
 */
#define GST_SDP_BWTYPE_EXT_PREFIX       "X-"

/**
 * GST_SDP_BWTYPE_RS:
 *
 * RTCP bandwidth allocated to active data senders (RFC 3556).
 *
 * Since: 0.10.15
 */
#define GST_SDP_BWTYPE_RS               "RS"
/**
 * GST_SDP_BWTYPE_RR:
 *
 * RTCP bandwidth allocated to data receivers (RFC 3556).
 *
 * Since: 0.10.15
 */
#define GST_SDP_BWTYPE_RR               "RR"
/**
 * GST_SDP_BWTYPE_TIAS:
 *
 * Transport Independent Application Specific Maximum bandwidth (RFC 3890).
 *
 * Since: 0.10.30
 */
#define GST_SDP_BWTYPE_TIAS             "TIAS"


/**
 * GstSDPBandwidth:
 * @bwtype: the bandwidth modifier type
 * @bandwidth: the bandwidth in kilobits per second
 *
 * The contents of the SDP "b=" field which specifies the proposed bandwidth to
 * be used by the session or media.
 */
typedef struct {
  gchar *bwtype;
  guint  bandwidth;
} GstSDPBandwidth;

/**
 * GstSDPTime:
 * @start: start time for the conference. The value is the decimal
 *     representation of Network Time Protocol (NTP) time values in seconds
 * @stop: stop time for the conference. The value is the decimal
 *     representation of Network Time Protocol (NTP) time values in seconds
 * @repeat: repeat times for a session
 *
 * The contents of the SDP "t=" field which specify the start and stop times for
 * a conference session.
 */
typedef struct {
  gchar  *start;
  gchar  *stop;
  GArray *repeat;
} GstSDPTime;

/**
 * GstSDPZone:
 * @time: the NTP time that a time zone adjustment happens
 * @typed_time: the offset from the time when the session was first scheduled
 *
 * The contents of the SDP "z=" field which allows the sender to
 * specify a list of time zone adjustments and offsets from the base
 * time.
 */
typedef struct {
  gchar *time;
  gchar *typed_time;
} GstSDPZone;

/**
 * GstSDPKey:
 * @type: the encryption type
 * @data: the encryption data
 *
 * The contents of the SDP "k=" field which is used to convey encryption
 * keys.
 */
typedef struct {
  gchar *type;
  gchar *data;
} GstSDPKey;

/**
 * GstSDPAttribute:
 * @key: the attribute key
 * @value: the attribute value or NULL when it was a property attribute
 *
 * The contents of the SDP "a=" field which contains a key/value pair.
 */
typedef struct {
  gchar *key;
  gchar *value;
} GstSDPAttribute;

/**
 * GstSDPMedia:
 * @media: the media type
 * @port: the transport port to which the media stream will be sent
 * @num_ports: the number of ports or -1 if only one port was specified
 * @proto: the transport protocol
 * @fmts: an array of #gchar formats
 * @information: the media title
 * @connections: array of #GstSDPConnection with media connection information
 * @bandwidths: array of #GstSDPBandwidth with media bandwidth information
 * @key: the encryption key
 * @attributes: array of #GstSDPAttribute with the additional media attributes
 *
 * The contents of the SDP "m=" field with all related fields.
 */
typedef struct {
  gchar            *media;
  guint             port;
  guint             num_ports;
  gchar            *proto;
  GArray           *fmts;
  gchar            *information;
  GArray           *connections;
  GArray           *bandwidths;
  GstSDPKey         key;
  GArray           *attributes;
} GstSDPMedia;

/**
 * GstSDPMessage:
 * @version: the protocol version
 * @origin: owner/creator and session identifier
 * @session_name: session name
 * @information: session information
 * @uri: URI of description
 * @emails: array of #gchar with email addresses
 * @phones: array of #gchar with phone numbers
 * @connection: connection information for the session
 * @bandwidths: array of #GstSDPBandwidth with bandwidth information
 * @times: array of #GstSDPTime with time descriptions
 * @zones: array of #GstSDPZone with time zone adjustments
 * @key: encryption key
 * @attributes: array of #GstSDPAttribute with session attributes
 * @medias: array of #GstSDPMedia with media descriptions
 *
 * The contents of the SDP message.
 */
typedef struct {
  gchar            *version;
  GstSDPOrigin      origin;
  gchar            *session_name;
  gchar            *information;
  gchar            *uri;
  GArray           *emails;
  GArray           *phones;
  GstSDPConnection  connection;
  GArray           *bandwidths;
  GArray           *times;
  GArray           *zones;
  GstSDPKey         key;
  GArray           *attributes;
  GArray           *medias;
} GstSDPMessage;

/* Session descriptions */
GstSDPResult            gst_sdp_message_new                 (GstSDPMessage **msg);
GstSDPResult            gst_sdp_message_init                (GstSDPMessage *msg);
GstSDPResult            gst_sdp_message_uninit              (GstSDPMessage *msg);
GstSDPResult            gst_sdp_message_free                (GstSDPMessage *msg);

GstSDPResult            gst_sdp_message_parse_buffer        (const guint8 *data, guint size, GstSDPMessage *msg);
gchar*                  gst_sdp_message_as_text             (const GstSDPMessage *msg);

/* convert from/to uri */
GstSDPResult            gst_sdp_message_parse_uri           (const gchar *uri, GstSDPMessage *msg);
gchar*                  gst_sdp_message_as_uri              (const gchar *scheme, const GstSDPMessage *msg);

/* v=.. */
const gchar*            gst_sdp_message_get_version         (const GstSDPMessage *msg);
GstSDPResult            gst_sdp_message_set_version         (GstSDPMessage *msg, const gchar *version);

/* o=<username> <sess-id> <sess-version> <nettype> <addrtype> <unicast-address> */
const GstSDPOrigin*     gst_sdp_message_get_origin          (const GstSDPMessage *msg);
GstSDPResult            gst_sdp_message_set_origin          (GstSDPMessage *msg, const gchar *username,
                                                             const gchar *sess_id, const gchar *sess_version,
                                                             const gchar *nettype, const gchar *addrtype,
                                                             const gchar *addr);

/* s=<session name> */
const gchar*            gst_sdp_message_get_session_name    (const GstSDPMessage *msg);
GstSDPResult            gst_sdp_message_set_session_name    (GstSDPMessage *msg, const gchar *session_name);

/* i=<session description> */
const gchar*            gst_sdp_message_get_information     (const GstSDPMessage *msg);
GstSDPResult            gst_sdp_message_set_information     (GstSDPMessage *msg, const gchar *information);

/* u=<uri> */
const gchar*            gst_sdp_message_get_uri             (const GstSDPMessage *msg);
GstSDPResult            gst_sdp_message_set_uri             (GstSDPMessage *msg, const gchar *uri);

/* e=<email-address> */
guint                   gst_sdp_message_emails_len          (const GstSDPMessage *msg);
const gchar*            gst_sdp_message_get_email           (const GstSDPMessage *msg, guint idx);
GstSDPResult            gst_sdp_message_add_email           (GstSDPMessage *msg, const gchar *email);

/* p=<phone-number> */
guint                   gst_sdp_message_phones_len          (const GstSDPMessage *msg);
const gchar*            gst_sdp_message_get_phone           (const GstSDPMessage *msg, guint idx);
GstSDPResult            gst_sdp_message_add_phone           (GstSDPMessage *msg, const gchar *phone);

/* c=<nettype> <addrtype> <connection-address>[/<ttl>][/<number of addresses>] */
const GstSDPConnection* gst_sdp_message_get_connection      (const GstSDPMessage *msg);
GstSDPResult            gst_sdp_message_set_connection      (GstSDPMessage *msg, const gchar *nettype,
                                                             const gchar *addrtype, const gchar *address,
                                                             guint ttl, guint addr_number);
/* b=<bwtype>:<bandwidth> */
guint                   gst_sdp_message_bandwidths_len      (const GstSDPMessage *msg);
const GstSDPBandwidth*  gst_sdp_message_get_bandwidth       (const GstSDPMessage *msg, guint idx);
GstSDPResult            gst_sdp_message_add_bandwidth       (GstSDPMessage *msg, const gchar *bwtype,
                                                             guint bandwidth);
/* t=<start-time> <stop-time> and
 * r=<repeat interval> <active duration> <offsets from start-time> */
guint                   gst_sdp_message_times_len           (const GstSDPMessage *msg);
const GstSDPTime*       gst_sdp_message_get_time            (const GstSDPMessage *msg, guint idx);
GstSDPResult            gst_sdp_message_add_time            (GstSDPMessage *msg, const gchar *start, const gchar *stop, const gchar **repeat);

/* z=<adjustment time> <offset> <adjustment time> <offset> .... */
guint                   gst_sdp_message_zones_len           (const GstSDPMessage *msg);
const GstSDPZone*       gst_sdp_message_get_zone            (const GstSDPMessage *msg, guint idx);
GstSDPResult            gst_sdp_message_add_zone            (GstSDPMessage *msg, const gchar *adj_time,
                                                             const gchar *typed_time);

/* k=<method>[:<encryption key>] */
const GstSDPKey*        gst_sdp_message_get_key             (const GstSDPMessage *msg);
GstSDPResult            gst_sdp_message_set_key             (GstSDPMessage *msg, const gchar *type,
                                                             const gchar *data);
/* a=... */
guint                   gst_sdp_message_attributes_len      (const GstSDPMessage *msg);
const GstSDPAttribute*  gst_sdp_message_get_attribute       (const GstSDPMessage *msg, guint idx);
const gchar*            gst_sdp_message_get_attribute_val   (const GstSDPMessage *msg, const gchar *key);
const gchar*            gst_sdp_message_get_attribute_val_n (const GstSDPMessage *msg, const gchar *key,
                                                             guint nth);
GstSDPResult            gst_sdp_message_add_attribute       (GstSDPMessage *msg, const gchar *key,
                                                             const gchar *value);

/* m=.. sections */
guint                   gst_sdp_message_medias_len          (const GstSDPMessage *msg);
const GstSDPMedia*      gst_sdp_message_get_media           (const GstSDPMessage *msg, guint idx);
GstSDPResult            gst_sdp_message_add_media           (GstSDPMessage *msg, GstSDPMedia *media);

GstSDPResult            gst_sdp_message_dump                (const GstSDPMessage *msg);

/* Media descriptions */
GstSDPResult            gst_sdp_media_new                   (GstSDPMedia **media);
GstSDPResult            gst_sdp_media_init                  (GstSDPMedia *media);
GstSDPResult            gst_sdp_media_uninit                (GstSDPMedia *media);
GstSDPResult            gst_sdp_media_free                  (GstSDPMedia *media);

gchar*                  gst_sdp_media_as_text               (const GstSDPMedia *media);

/* m=<media> <port>/<number of ports> <proto> <fmt> ... */
const gchar*            gst_sdp_media_get_media             (const GstSDPMedia *media);
GstSDPResult            gst_sdp_media_set_media             (GstSDPMedia *media, const gchar *med);

guint                   gst_sdp_media_get_port              (const GstSDPMedia *media);
guint                   gst_sdp_media_get_num_ports         (const GstSDPMedia *media);
GstSDPResult            gst_sdp_media_set_port_info         (GstSDPMedia *media, guint port,
                                                             guint num_ports);

const gchar*            gst_sdp_media_get_proto             (const GstSDPMedia *media);
GstSDPResult            gst_sdp_media_set_proto             (GstSDPMedia *media, const gchar *proto);

guint                   gst_sdp_media_formats_len           (const GstSDPMedia *media);
const gchar*            gst_sdp_media_get_format            (const GstSDPMedia *media, guint idx);
GstSDPResult            gst_sdp_media_add_format            (GstSDPMedia *media, const gchar *format);

/* i=<session description> */
const gchar*            gst_sdp_media_get_information       (const GstSDPMedia *media);
GstSDPResult            gst_sdp_media_set_information       (GstSDPMedia *media, const gchar *information);

/* c=<nettype> <addrtype> <connection-address>[/<ttl>][/<number of addresses>] */
guint                   gst_sdp_media_connections_len       (const GstSDPMedia *media);
const GstSDPConnection* gst_sdp_media_get_connection        (const GstSDPMedia *media, guint idx);
GstSDPResult            gst_sdp_media_add_connection        (GstSDPMedia *media, const gchar *nettype,
                                                             const gchar *addrtype, const gchar *address,
                                                             guint ttl, guint addr_number);

/* b=<bwtype>:<bandwidth> */
guint                   gst_sdp_media_bandwidths_len        (const GstSDPMedia *media);
const GstSDPBandwidth*  gst_sdp_media_get_bandwidth         (const GstSDPMedia *media, guint idx);
GstSDPResult            gst_sdp_media_add_bandwidth         (GstSDPMedia *media, const gchar *bwtype,
                                                             guint bandwidth);

/* k=<method>:<encryption key> */
const GstSDPKey*        gst_sdp_media_get_key               (const GstSDPMedia *media);
GstSDPResult            gst_sdp_media_set_key               (GstSDPMedia *media, const gchar *type,
                                                             const gchar *data);
/* a=... */
guint                   gst_sdp_media_attributes_len        (const GstSDPMedia *media);
const GstSDPAttribute * gst_sdp_media_get_attribute         (const GstSDPMedia *media, guint idx);
const gchar*            gst_sdp_media_get_attribute_val     (const GstSDPMedia *media, const gchar *key);
const gchar*            gst_sdp_media_get_attribute_val_n   (const GstSDPMedia *media, const gchar *key,
                                                             guint nth);
GstSDPResult            gst_sdp_media_add_attribute         (GstSDPMedia *media, const gchar *key,
                                                             const gchar *value);

G_END_DECLS

#endif /* __GST_SDP_MESSAGE_H__ */
