/* GStreamer
 * Copyright (C) <2005,2009> Wim Taymans <wim.taymans@gmail.com>
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

#ifndef __GST_RTSP_CONNECTION_H__
#define __GST_RTSP_CONNECTION_H__

#include <glib.h>

#include <gst/gstconfig.h>
#include <gst/rtsp/gstrtspdefs.h>
#include <gst/rtsp/gstrtspurl.h>
#include <gst/rtsp/gstrtspmessage.h>

G_BEGIN_DECLS

/**
 * GstRTSPConnection:
 *
 * Opaque RTSP connection object.
 */
typedef struct _GstRTSPConnection GstRTSPConnection;

/* opening/closing a connection */
GstRTSPResult      gst_rtsp_connection_create        (const GstRTSPUrl *url, GstRTSPConnection **conn);
GstRTSPResult      gst_rtsp_connection_create_from_fd (gint fd,
                                                       const gchar * ip,
                                                       guint16 port,
                                                       const gchar * initial_buffer,
                                                       GstRTSPConnection ** conn);
GstRTSPResult      gst_rtsp_connection_accept        (gint sock, GstRTSPConnection **conn);
GstRTSPResult      gst_rtsp_connection_connect       (GstRTSPConnection *conn, GTimeVal *timeout);
GstRTSPResult      gst_rtsp_connection_close         (GstRTSPConnection *conn);
GstRTSPResult      gst_rtsp_connection_free          (GstRTSPConnection *conn);


/* sending/receiving raw bytes */
GstRTSPResult      gst_rtsp_connection_read          (GstRTSPConnection * conn, guint8 * data,
                                                      guint size, GTimeVal * timeout);
GstRTSPResult      gst_rtsp_connection_write         (GstRTSPConnection * conn, const guint8 * data, 
		                                      guint size, GTimeVal * timeout);

/* sending/receiving messages */
GstRTSPResult      gst_rtsp_connection_send          (GstRTSPConnection *conn, GstRTSPMessage *message,
                                                      GTimeVal *timeout);
GstRTSPResult      gst_rtsp_connection_receive       (GstRTSPConnection *conn, GstRTSPMessage *message,
                                                      GTimeVal *timeout);

/* status management */
GstRTSPResult      gst_rtsp_connection_poll          (GstRTSPConnection *conn, GstRTSPEvent events,
                                                      GstRTSPEvent *revents, GTimeVal *timeout);

/* reset the timeout */
GstRTSPResult      gst_rtsp_connection_next_timeout  (GstRTSPConnection *conn, GTimeVal *timeout);
GstRTSPResult      gst_rtsp_connection_reset_timeout (GstRTSPConnection *conn);

/* flushing state */
GstRTSPResult      gst_rtsp_connection_flush         (GstRTSPConnection *conn, gboolean flush);

/* HTTP proxy support */
GstRTSPResult      gst_rtsp_connection_set_proxy     (GstRTSPConnection *conn,
                                                      const gchar *host, guint port);

/* configure authentication data */
GstRTSPResult      gst_rtsp_connection_set_auth      (GstRTSPConnection *conn, GstRTSPAuthMethod method,
                                                      const gchar *user, const gchar *pass);

void               gst_rtsp_connection_set_auth_param    (GstRTSPConnection *conn,
		                                          const gchar * param,
							  const gchar *value);
void               gst_rtsp_connection_clear_auth_params (GstRTSPConnection *conn);

/* configure DSCP */
GstRTSPResult      gst_rtsp_connection_set_qos_dscp  (GstRTSPConnection *conn,
                                                      guint qos_dscp);

/* accessors */
GstRTSPUrl *       gst_rtsp_connection_get_url       (const GstRTSPConnection *conn);
const gchar *      gst_rtsp_connection_get_ip        (const GstRTSPConnection *conn);
void               gst_rtsp_connection_set_ip        (GstRTSPConnection *conn, const gchar *ip);

gint               gst_rtsp_connection_get_readfd    (const GstRTSPConnection *conn);
gint               gst_rtsp_connection_get_writefd   (const GstRTSPConnection *conn);

void               gst_rtsp_connection_set_http_mode (GstRTSPConnection *conn,
                                                      gboolean enable);

/* tunneling */
void               gst_rtsp_connection_set_tunneled  (GstRTSPConnection *conn, gboolean tunneled);
gboolean           gst_rtsp_connection_is_tunneled   (const GstRTSPConnection *conn);

const gchar *      gst_rtsp_connection_get_tunnelid  (const GstRTSPConnection *conn);
GstRTSPResult      gst_rtsp_connection_do_tunnel     (GstRTSPConnection *conn, GstRTSPConnection *conn2);

/* async IO */

/**
 * GstRTSPWatch:
 *
 * Opaque RTSP watch object that can be used for asynchronous RTSP
 * operations.
 */
typedef struct _GstRTSPWatch GstRTSPWatch;

/**
 * GstRTSPWatchFuncs:
 * @message_received: callback when a message was received
 * @message_sent: callback when a message was sent
 * @closed: callback when the connection is closed
 * @error: callback when an error occured
 * @tunnel_start: a client started a tunneled connection. The tunnelid of the
 *   connection must be saved.
 * @tunnel_complete: a client finished a tunneled connection. In this callback
 *   you usually pair the tunnelid of this connection with the saved one using
 *   gst_rtsp_connection_do_tunnel().
 * @error_full: callback when an error occured with more information than
 *   the @error callback. Since 0.10.25
 * @tunnel_lost: callback when the post connection of a tunnel is closed.
 *   Since 0.10.29
 *
 * Callback functions from a #GstRTSPWatch.
 *
 * Since: 0.10.23
 */
typedef struct {
  GstRTSPResult     (*message_received) (GstRTSPWatch *watch, GstRTSPMessage *message,
                                         gpointer user_data);
  GstRTSPResult     (*message_sent)     (GstRTSPWatch *watch, guint id,
                                         gpointer user_data);
  GstRTSPResult     (*closed)           (GstRTSPWatch *watch, gpointer user_data);
  GstRTSPResult     (*error)            (GstRTSPWatch *watch, GstRTSPResult result,
                                         gpointer user_data);
  GstRTSPStatusCode (*tunnel_start)     (GstRTSPWatch *watch, gpointer user_data);
  GstRTSPResult     (*tunnel_complete)  (GstRTSPWatch *watch, gpointer user_data);
  GstRTSPResult     (*error_full)       (GstRTSPWatch *watch, GstRTSPResult result,
                                         GstRTSPMessage *message, guint id,
                                         gpointer user_data);
  GstRTSPResult     (*tunnel_lost)      (GstRTSPWatch *watch, gpointer user_data);

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING - 2];
} GstRTSPWatchFuncs;

GstRTSPWatch *     gst_rtsp_watch_new                (GstRTSPConnection *conn,
                                                      GstRTSPWatchFuncs *funcs,
						      gpointer user_data,
						      GDestroyNotify notify);
void               gst_rtsp_watch_reset              (GstRTSPWatch *watch);
void               gst_rtsp_watch_unref              (GstRTSPWatch *watch);

guint              gst_rtsp_watch_attach             (GstRTSPWatch *watch,
                                                      GMainContext *context);

GstRTSPResult      gst_rtsp_watch_write_data         (GstRTSPWatch *watch,
                                                      const guint8 *data,
                                                      guint size, guint *id);
GstRTSPResult      gst_rtsp_watch_send_message       (GstRTSPWatch *watch,
                                                      GstRTSPMessage *message,
                                                      guint *id);

#ifndef GST_DISABLE_DEPRECATED
guint              gst_rtsp_watch_queue_data         (GstRTSPWatch * watch,
                                                      const guint8 * data,
                                                      guint size);
guint              gst_rtsp_watch_queue_message      (GstRTSPWatch *watch,
                                                      GstRTSPMessage *message);
#endif

G_END_DECLS

#endif /* __GST_RTSP_CONNECTION_H__ */
