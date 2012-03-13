/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2001-2003, Ximian, Inc.
 */

#ifndef SOUP_AUTH_H
#define SOUP_AUTH_H 1

#include <libsoup/soup-types.h>
#include <libsoup/soup-headers.h>

G_BEGIN_DECLS

#define SOUP_TYPE_AUTH            (soup_auth_get_type ())
#define SOUP_AUTH(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SOUP_TYPE_AUTH, SoupAuth))
#define SOUP_AUTH_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SOUP_TYPE_AUTH, SoupAuthClass))
#define SOUP_IS_AUTH(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SOUP_TYPE_AUTH))
#define SOUP_IS_AUTH_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((obj), SOUP_TYPE_AUTH))
#define SOUP_AUTH_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SOUP_TYPE_AUTH, SoupAuthClass))

struct _SoupAuth {
	GObject parent;

	char *realm;
};

typedef struct {
	GObjectClass parent_class;

	const char  *scheme_name;
	guint        strength;

	gboolean     (*update)               (SoupAuth      *auth,
					      SoupMessage   *msg,
					      GHashTable    *auth_params);

	GSList *     (*get_protection_space) (SoupAuth      *auth,
					      SoupURI       *source_uri);

	void         (*authenticate)         (SoupAuth      *auth,
					      const char    *username,
					      const char    *password);
	gboolean     (*is_authenticated)     (SoupAuth      *auth);

	char *       (*get_authorization)    (SoupAuth      *auth,
					      SoupMessage   *msg);
	/* Padding for future expansion */
	void (*_libsoup_reserved1) (void);
	void (*_libsoup_reserved2) (void);
	void (*_libsoup_reserved3) (void);
	void (*_libsoup_reserved4) (void);
} SoupAuthClass;

#define SOUP_AUTH_SCHEME_NAME      "scheme-name"
#define SOUP_AUTH_REALM            "realm"
#define SOUP_AUTH_HOST             "host"
#define SOUP_AUTH_IS_FOR_PROXY     "is-for-proxy"
#define SOUP_AUTH_IS_AUTHENTICATED "is-authenticated"

GType       soup_auth_get_type              (void);

SoupAuth   *soup_auth_new                   (GType          type,
					     SoupMessage   *msg,
					     const char    *auth_header);
gboolean    soup_auth_update                (SoupAuth      *auth,
					     SoupMessage   *msg,
					     const char    *auth_header);

gboolean    soup_auth_is_for_proxy          (SoupAuth      *auth);
const char *soup_auth_get_scheme_name       (SoupAuth      *auth);
const char *soup_auth_get_host              (SoupAuth      *auth);
const char *soup_auth_get_realm             (SoupAuth      *auth);
char       *soup_auth_get_info              (SoupAuth      *auth);

#ifdef LIBSOUP_I_HAVE_READ_BUG_594377_AND_KNOW_SOUP_PASSWORD_MANAGER_MIGHT_GO_AWAY
GSList     *soup_auth_get_saved_users       (SoupAuth      *auth);
const char *soup_auth_get_saved_password    (SoupAuth      *auth,
					     const char    *user);
void        soup_auth_save_password         (SoupAuth      *auth,
					     const char    *username,
					     const char    *password);
#endif

void        soup_auth_authenticate          (SoupAuth      *auth,
					     const char    *username,
					     const char    *password);
gboolean    soup_auth_is_authenticated      (SoupAuth      *auth);

char       *soup_auth_get_authorization     (SoupAuth      *auth, 
					     SoupMessage   *msg);

GSList     *soup_auth_get_protection_space  (SoupAuth      *auth,
					     SoupURI       *source_uri);
void        soup_auth_free_protection_space (SoupAuth      *auth,
					     GSList        *space);

#ifdef LIBSOUP_I_HAVE_READ_BUG_594377_AND_KNOW_SOUP_PASSWORD_MANAGER_MIGHT_GO_AWAY
void        soup_auth_has_saved_password    (SoupAuth      *auth,
					     const char    *username,
					     const char    *password);
#endif

G_END_DECLS

#endif /* SOUP_AUTH_H */
