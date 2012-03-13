/* GStreamer
 * Copyright (C) 2005 Andy Wingo <wingo@pobox.com>
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


#ifndef __GST_NET_TIME_PACKET_H__
#define __GST_NET_TIME_PACKET_H__

#include <gst/gst.h>

G_BEGIN_DECLS

#include <errno.h>
#include <string.h>
#include <sys/types.h>

#ifdef G_OS_WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#ifndef socklen_t
#define socklen_t int
#endif
#else
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

/**
 * GST_NET_TIME_PACKET_SIZE:
 *
 * The size of the packets sent between network clocks.
 */
#define GST_NET_TIME_PACKET_SIZE 16

typedef struct _GstNetTimePacket GstNetTimePacket;

/**
 * GstNetTimePacket:
 * @local_time: the local time when this packet was sent
 * @remote_time: the remote time observation
 *
 * Content of a #GstNetTimePacket.
 */
struct _GstNetTimePacket {
  GstClockTime local_time;
  GstClockTime remote_time;
};

GstNetTimePacket*	gst_net_time_packet_new 	(const guint8 *buffer);
guint8*			gst_net_time_packet_serialize	(const GstNetTimePacket *packet);

GstNetTimePacket*	gst_net_time_packet_receive	(gint fd, struct sockaddr *addr,
                                                         socklen_t *len);
gint			gst_net_time_packet_send	(const GstNetTimePacket *packet,
                                                         gint fd, struct sockaddr *addr,
                                                         socklen_t len);

G_END_DECLS


#endif /* __GST_NET_TIME_PACKET_H__ */
