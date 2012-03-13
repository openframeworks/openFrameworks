/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2000-2003, Ximian, Inc.
 */

#ifndef SOUP_SERVER_H
#define SOUP_SERVER_H 1

#include <libsoup/soup-types.h>
#include <libsoup/soup-uri.h>

G_BEGIN_DECLS

#define SOUP_TYPE_SERVER            (soup_server_get_type ())
#define SOUP_SERVER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SOUP_TYPE_SERVER, SoupServer))
#define SOUP_SERVER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SOUP_TYPE_SERVER, SoupServerClass))
#define SOUP_IS_SERVER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SOUP_TYPE_SERVER))
#define SOUP_IS_SERVER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((obj), SOUP_TYPE_SERVER))
#define SOUP_SERVER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SOUP_TYPE_SERVER, SoupServerClass))

typedef struct SoupClientContext SoupClientContext;
GType soup_client_context_get_type (void);
#define SOUP_TYPE_CLIENT_CONTEXT (soup_client_context_get_type ())

struct _SoupServer {
	GObject parent;

};

typedef struct {
	GObjectClass parent_class;

	/* signals */
	void (*request_started)  (SoupServer *server, SoupMessage *msg,
				  SoupClientContext *client);
	void (*request_read)     (SoupServer *server, SoupMessage *msg,
				  SoupClientContext *client);
	void (*request_finished) (SoupServer *server, SoupMessage *msg,
				  SoupClientContext *client);
	void (*request_aborted)  (SoupServer *server, SoupMessage *msg,
				  SoupClientContext *client);

	/* Padding for future expansion */
	void (*_libsoup_reserved1) (void);
	void (*_libsoup_reserved2) (void);
	void (*_libsoup_reserved3) (void);
	void (*_libsoup_reserved4) (void);
} SoupServerClass;

GType soup_server_get_type (void);

typedef void (*SoupServerCallback) (SoupServer        *server,
				    SoupMessage       *msg, 
				    const char        *path,
				    GHashTable        *query,
				    SoupClientContext *client,
				    gpointer           user_data);

#define SOUP_SERVER_PORT          "port"
#define SOUP_SERVER_INTERFACE     "interface"
#define SOUP_SERVER_SSL_CERT_FILE "ssl-cert-file"
#define SOUP_SERVER_SSL_KEY_FILE  "ssl-key-file"
#define SOUP_SERVER_ASYNC_CONTEXT "async-context"
#define SOUP_SERVER_RAW_PATHS     "raw-paths"
#define SOUP_SERVER_SERVER_HEADER "server-header"

SoupServer        *soup_server_new            (const char            *optname1,
					       ...) G_GNUC_NULL_TERMINATED;

gboolean           soup_server_is_https       (SoupServer            *server);
guint              soup_server_get_port       (SoupServer            *server);

SoupSocket        *soup_server_get_listener   (SoupServer            *server);

void               soup_server_run            (SoupServer            *server);
void               soup_server_run_async      (SoupServer            *server);
void               soup_server_quit           (SoupServer            *server);

GMainContext      *soup_server_get_async_context (SoupServer         *server);

/* Handlers and auth */

void               soup_server_add_handler    (SoupServer            *server,
					       const char            *path,
					       SoupServerCallback     callback,
					       gpointer               user_data,
					       GDestroyNotify         destroy);
void               soup_server_remove_handler (SoupServer            *server,
					       const char            *path);

void               soup_server_add_auth_domain    (SoupServer     *server,
						   SoupAuthDomain *auth_domain);
void               soup_server_remove_auth_domain (SoupServer     *server,
						   SoupAuthDomain *auth_domain);

/* I/O */

void               soup_server_pause_message   (SoupServer           *server,
						SoupMessage          *msg);
void               soup_server_unpause_message (SoupServer           *server,
						SoupMessage          *msg);

/* Client context */

SoupSocket     *soup_client_context_get_socket      (SoupClientContext *client);
SoupAddress    *soup_client_context_get_address     (SoupClientContext *client);
const char     *soup_client_context_get_host        (SoupClientContext *client);
SoupAuthDomain *soup_client_context_get_auth_domain (SoupClientContext *client);
const char     *soup_client_context_get_auth_user   (SoupClientContext *client);

G_END_DECLS

#endif /* SOUP_SERVER_H */
