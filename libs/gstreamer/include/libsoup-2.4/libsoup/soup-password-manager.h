/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2008 Red Hat, Inc.
 */

#ifndef SOUP_PASSWORD_MANAGER_H
#define SOUP_PASSWORD_MANAGER_H 1

#ifdef LIBSOUP_I_HAVE_READ_BUG_594377_AND_KNOW_SOUP_PASSWORD_MANAGER_MIGHT_GO_AWAY

#include <libsoup/soup-types.h>
#include <gio/gio.h>

#define SOUP_TYPE_PASSWORD_MANAGER            (soup_password_manager_get_type ())
#define SOUP_PASSWORD_MANAGER(object)         (G_TYPE_CHECK_INSTANCE_CAST ((object), SOUP_TYPE_PASSWORD_MANAGER, SoupPasswordManager))
#define SOUP_PASSWORD_MANAGER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SOUP_TYPE_PASSWORD_MANAGER, SoupPasswordManagerInterface))
#define SOUP_IS_PASSWORD_MANAGER(object)      (G_TYPE_CHECK_INSTANCE_TYPE ((object), SOUP_TYPE_PASSWORD_MANAGER))
#define SOUP_IS_PASSWORD_MANAGER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SOUP_TYPE_PASSWORD_MANAGER))
#define SOUP_PASSWORD_MANAGER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_INTERFACE ((obj), SOUP_TYPE_PASSWORD_MANAGER, SoupPasswordManagerInterface))

typedef struct _SoupPasswordManager SoupPasswordManager;

typedef void (*SoupPasswordManagerCallback) (SoupPasswordManager *,
					     SoupMessage *, SoupAuth *,
					     gboolean retrying,
					     gpointer user_data);

typedef struct {
	GTypeInterface base;

	/* virtual methods */
	void (*get_passwords_async) (SoupPasswordManager *, SoupMessage *,
				     SoupAuth *, gboolean,
				     GMainContext *, GCancellable *,
				     SoupPasswordManagerCallback, gpointer);
	void (*get_passwords_sync)  (SoupPasswordManager *, SoupMessage *,
				     SoupAuth *, GCancellable *);

} SoupPasswordManagerInterface;

GType soup_password_manager_get_type (void);

void  soup_password_manager_get_passwords_async (SoupPasswordManager  *password_manager,
						 SoupMessage          *msg,
						 SoupAuth             *auth,
						 gboolean              retrying,
						 GMainContext         *async_context,
						 GCancellable         *cancellable,
						 SoupPasswordManagerCallback callback,
						 gpointer              user_data);

void  soup_password_manager_get_passwords_sync  (SoupPasswordManager  *password_manager,
						 SoupMessage          *msg,
						 SoupAuth             *auth,
						 GCancellable         *cancellable);

#endif /* LIBSOUP_I_HAVE_READ_BUG_594377_AND_KNOW_SOUP_PASSWORD_MANAGER_MIGHT_GO_AWAY */

#endif /* SOUP_PASSWORD_MANAGER_H */
