/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2000-2003, Ximian, Inc.
 */

#ifndef SOUP_SESSION_H
#define SOUP_SESSION_H 1

#include <libsoup/soup-types.h>
#include <libsoup/soup-message.h>

G_BEGIN_DECLS

#define SOUP_TYPE_SESSION            (soup_session_get_type ())
#define SOUP_SESSION(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SOUP_TYPE_SESSION, SoupSession))
#define SOUP_SESSION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SOUP_TYPE_SESSION, SoupSessionClass))
#define SOUP_IS_SESSION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SOUP_TYPE_SESSION))
#define SOUP_IS_SESSION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((obj), SOUP_TYPE_SESSION))
#define SOUP_SESSION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SOUP_TYPE_SESSION, SoupSessionClass))

typedef void (*SoupSessionCallback) (SoupSession           *session,
				     SoupMessage           *msg,
				     gpointer               user_data);

struct _SoupSession {
	GObject parent;

};

typedef struct {
	GObjectClass parent_class;

	/* signals */
	void (*request_started) (SoupSession *session, SoupMessage *msg,
				 SoupSocket *socket);
	void (*authenticate)    (SoupSession *session, SoupMessage *msg,
				 SoupAuth *auth, gboolean retrying);

	/* methods */
	void  (*queue_message)   (SoupSession *session, SoupMessage *msg,
				  SoupSessionCallback callback,
				  gpointer user_data);
	void  (*requeue_message) (SoupSession *session, SoupMessage *msg);
	guint (*send_message)    (SoupSession *session, SoupMessage *msg);

	void  (*cancel_message)  (SoupSession *session, SoupMessage *msg,
				  guint status_code);

	void  (*auth_required)   (SoupSession *session, SoupMessage *msg,
				  SoupAuth *auth, gboolean retrying);


	/* Padding for future expansion */
	void (*_libsoup_reserved2) (void);
	void (*_libsoup_reserved3) (void);
	void (*_libsoup_reserved4) (void);
} SoupSessionClass;

GType soup_session_get_type (void);

#define SOUP_SESSION_PROXY_URI              "proxy-uri"
#define SOUP_SESSION_MAX_CONNS              "max-conns"
#define SOUP_SESSION_MAX_CONNS_PER_HOST     "max-conns-per-host"
#define SOUP_SESSION_USE_NTLM               "use-ntlm"
#define SOUP_SESSION_SSL_CA_FILE            "ssl-ca-file"
#define SOUP_SESSION_SSL_STRICT             "ssl-strict"
#define SOUP_SESSION_ASYNC_CONTEXT          "async-context"
#define SOUP_SESSION_TIMEOUT                "timeout"
#define SOUP_SESSION_USER_AGENT             "user-agent"
#define SOUP_SESSION_ACCEPT_LANGUAGE        "accept-language"
#define SOUP_SESSION_ACCEPT_LANGUAGE_AUTO   "accept-language-auto"
#define SOUP_SESSION_IDLE_TIMEOUT           "idle-timeout"
#define SOUP_SESSION_ADD_FEATURE            "add-feature"
#define SOUP_SESSION_ADD_FEATURE_BY_TYPE    "add-feature-by-type"
#define SOUP_SESSION_REMOVE_FEATURE_BY_TYPE "remove-feature-by-type"

GMainContext   *soup_session_get_async_context(SoupSession           *session);

void            soup_session_queue_message    (SoupSession           *session,
					       SoupMessage           *msg,
					       SoupSessionCallback    callback,
					       gpointer               user_data);
void            soup_session_requeue_message  (SoupSession           *session,
					       SoupMessage           *msg);

guint           soup_session_send_message     (SoupSession           *session,
					       SoupMessage           *msg);

void            soup_session_pause_message    (SoupSession           *session,
					       SoupMessage           *msg);
void            soup_session_unpause_message  (SoupSession           *session,
					       SoupMessage           *msg);

void            soup_session_cancel_message   (SoupSession           *session,
					       SoupMessage           *msg,
					       guint                  status_code);
void            soup_session_abort            (SoupSession           *session);

void            soup_session_prepare_for_uri  (SoupSession           *session,
					       SoupURI               *uri);

void                soup_session_add_feature            (SoupSession        *session,
							 SoupSessionFeature *feature);
void                soup_session_add_feature_by_type    (SoupSession        *session,
							 GType               feature_type);
void                soup_session_remove_feature         (SoupSession        *session,
							 SoupSessionFeature *feature);
void                soup_session_remove_feature_by_type (SoupSession        *session,
							 GType               feature_type);
GSList             *soup_session_get_features           (SoupSession        *session,
							 GType               feature_type);
SoupSessionFeature *soup_session_get_feature            (SoupSession        *session,
							 GType               feature_type);
SoupSessionFeature *soup_session_get_feature_for_message(SoupSession        *session,
							 GType               feature_type,
							 SoupMessage        *msg);

G_END_DECLS

#endif /* SOUP_SESSION_H */
