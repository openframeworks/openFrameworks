/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2000-2003, Ximian, Inc.
 */

#ifndef SOUP_SOCKET_H
#define SOUP_SOCKET_H 1

#include <libsoup/soup-types.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define SOUP_TYPE_SOCKET            (soup_socket_get_type ())
#define SOUP_SOCKET(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SOUP_TYPE_SOCKET, SoupSocket))
#define SOUP_SOCKET_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SOUP_TYPE_SOCKET, SoupSocketClass))
#define SOUP_IS_SOCKET(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SOUP_TYPE_SOCKET))
#define SOUP_IS_SOCKET_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((obj), SOUP_TYPE_SOCKET))
#define SOUP_SOCKET_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SOUP_TYPE_SOCKET, SoupSocketClass))

struct _SoupSocket {
	GObject parent;

};

typedef struct {
	GObjectClass parent_class;

	/* signals */
	void (*readable)       (SoupSocket *);
	void (*writable)       (SoupSocket *);
	void (*disconnected)   (SoupSocket *);

	void (*new_connection) (SoupSocket *, SoupSocket *);

	/* Padding for future expansion */
	void (*_libsoup_reserved1) (void);
	void (*_libsoup_reserved2) (void);
	void (*_libsoup_reserved3) (void);
	void (*_libsoup_reserved4) (void);
} SoupSocketClass;

#define SOUP_SOCKET_LOCAL_ADDRESS       "local-address"
#define SOUP_SOCKET_REMOTE_ADDRESS      "remote-address"
#define SOUP_SOCKET_FLAG_NONBLOCKING    "non-blocking"
#define SOUP_SOCKET_IS_SERVER           "is-server"
#define SOUP_SOCKET_SSL_CREDENTIALS     "ssl-creds"
#define SOUP_SOCKET_SSL_STRICT          "ssl-strict"
#define SOUP_SOCKET_TRUSTED_CERTIFICATE "trusted-certificate"
#define SOUP_SOCKET_ASYNC_CONTEXT       "async-context"
#define SOUP_SOCKET_TIMEOUT             "timeout"

typedef void (*SoupSocketCallback)            (SoupSocket         *sock,
					       guint               status,
					       gpointer            user_data);

GType soup_socket_get_type (void);

SoupSocket    *soup_socket_new                (const char         *optname1,
					       ...) G_GNUC_NULL_TERMINATED;

void           soup_socket_connect_async      (SoupSocket         *sock,
					       GCancellable       *cancellable,
					       SoupSocketCallback  callback,
					       gpointer            user_data);
guint          soup_socket_connect_sync       (SoupSocket         *sock,
					       GCancellable       *cancellable);
int            soup_socket_get_fd             (SoupSocket         *sock);

gboolean       soup_socket_listen             (SoupSocket         *sock);

gboolean       soup_socket_start_ssl          (SoupSocket         *sock,
					       GCancellable       *cancellable);
gboolean       soup_socket_start_proxy_ssl    (SoupSocket         *sock,
					       const char         *ssl_host,
					       GCancellable       *cancellable);
gboolean       soup_socket_is_ssl             (SoupSocket         *sock);

void           soup_socket_disconnect         (SoupSocket         *sock);
gboolean       soup_socket_is_connected       (SoupSocket         *sock);

SoupAddress   *soup_socket_get_local_address  (SoupSocket         *sock);
SoupAddress   *soup_socket_get_remote_address (SoupSocket         *sock);


typedef enum {
	SOUP_SOCKET_OK,
	SOUP_SOCKET_WOULD_BLOCK,
	SOUP_SOCKET_EOF,
	SOUP_SOCKET_ERROR
} SoupSocketIOStatus;

SoupSocketIOStatus  soup_socket_read       (SoupSocket         *sock,
					    gpointer            buffer,
					    gsize               len,
					    gsize              *nread,
					    GCancellable       *cancellable,
					    GError            **error);
SoupSocketIOStatus  soup_socket_read_until (SoupSocket         *sock,
					    gpointer            buffer,
					    gsize               len,
					    gconstpointer       boundary,
					    gsize               boundary_len,
					    gsize              *nread,
					    gboolean           *got_boundary,
					    GCancellable       *cancellable,
					    GError            **error);

SoupSocketIOStatus  soup_socket_write      (SoupSocket         *sock,
					    gconstpointer       buffer,
					    gsize               len,
					    gsize              *nwrote,
					    GCancellable       *cancellable,
					    GError            **error);

G_END_DECLS

#endif /* SOUP_SOCKET_H */
