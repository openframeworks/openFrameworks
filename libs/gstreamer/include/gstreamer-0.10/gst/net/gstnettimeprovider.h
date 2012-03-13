/* GStreamer
 * Copyright (C) 2005 Andy Wingo <wingo@pobox.com>
 *               2006 Joni Valtanen <joni.valtanen@movial.fi>
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


#ifndef __GST_NET_TIME_PROVIDER_H__
#define __GST_NET_TIME_PROVIDER_H__

/* to determinate os */
#include <glib.h>

#include <gst/gst.h>

G_BEGIN_DECLS

#include <errno.h>
#include <string.h>
#include <sys/types.h>

#ifdef G_OS_WIN32
#include <winsock2.h>
#else
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <fcntl.h>

#define GST_TYPE_NET_TIME_PROVIDER \
  (gst_net_time_provider_get_type())
#define GST_NET_TIME_PROVIDER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_NET_TIME_PROVIDER,GstNetTimeProvider))
#define GST_NET_TIME_PROVIDER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_NET_TIME_PROVIDER,GstNetTimeProviderClass))
#define GST_IS_NET_TIME_PROVIDER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_NET_TIME_PROVIDER))
#define GST_IS_NET_TIME_PROVIDER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_NET_TIME_PROVIDER))

typedef struct _GstNetTimeProvider GstNetTimeProvider;
typedef struct _GstNetTimeProviderClass GstNetTimeProviderClass;
typedef struct _GstNetTimeProviderPrivate GstNetTimeProviderPrivate;


/**
 * GstNetTimeProvider:
 *
 * Opaque #GstNetTimeProvider structure.
 */
struct _GstNetTimeProvider {
  GstObject parent;

  /*< private >*/
  gchar *address;
  int port;

  int sock;
  int control_sock[2];

  GThread *thread;

  GstClock *clock;

  union {
    gpointer _gst_reserved1;
    /* has to be a gint, we use atomic ops here */
    gint active;
  } active;

  /*< private >*/
  GstNetTimeProviderPrivate *priv;

  gpointer _gst_reserved[GST_PADDING - 2];
};

struct _GstNetTimeProviderClass {
  GstObjectClass parent_class;
};

GType			gst_net_time_provider_get_type	(void);
GstNetTimeProvider*	gst_net_time_provider_new 	(GstClock *clock,
                                                         const gchar *address,
                                                         gint port);

G_END_DECLS


#endif /* __GST_NET_TIME_PROVIDER_H__ */
