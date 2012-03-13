/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2007 Novell, Inc.
 */

#ifndef SOUP_AUTH_DOMAIN_H
#define SOUP_AUTH_DOMAIN_H 1

#include <libsoup/soup-types.h>

G_BEGIN_DECLS

#define SOUP_TYPE_AUTH_DOMAIN            (soup_auth_domain_get_type ())
#define SOUP_AUTH_DOMAIN(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SOUP_TYPE_AUTH_DOMAIN, SoupAuthDomain))
#define SOUP_AUTH_DOMAIN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SOUP_TYPE_AUTH_DOMAIN, SoupAuthDomainClass))
#define SOUP_IS_AUTH_DOMAIN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SOUP_TYPE_AUTH_DOMAIN))
#define SOUP_IS_AUTH_DOMAIN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((obj), SOUP_TYPE_AUTH_DOMAIN))
#define SOUP_AUTH_DOMAIN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SOUP_TYPE_AUTH_DOMAIN, SoupAuthDomainClass))

struct _SoupAuthDomain {
	GObject parent;

};

typedef struct {
	GObjectClass parent_class;

	char *   (*accepts)        (SoupAuthDomain *domain,
				    SoupMessage    *msg,
				    const char     *header);
	char *   (*challenge)      (SoupAuthDomain *domain,
				    SoupMessage    *msg);
	gboolean (*check_password) (SoupAuthDomain *domain,
				    SoupMessage    *msg,
				    const char     *username,
				    const char     *password);

	/* Padding for future expansion */
	void (*_libsoup_reserved2) (void);
	void (*_libsoup_reserved3) (void);
	void (*_libsoup_reserved4) (void);
} SoupAuthDomainClass;

#define SOUP_AUTH_DOMAIN_REALM       "realm"
#define SOUP_AUTH_DOMAIN_PROXY       "proxy"
#define SOUP_AUTH_DOMAIN_ADD_PATH    "add-path"
#define SOUP_AUTH_DOMAIN_REMOVE_PATH "remove-path"
#define SOUP_AUTH_DOMAIN_FILTER      "filter"
#define SOUP_AUTH_DOMAIN_FILTER_DATA "filter-data"
#define SOUP_AUTH_DOMAIN_GENERIC_AUTH_CALLBACK "generic-auth-callback"
#define SOUP_AUTH_DOMAIN_GENERIC_AUTH_DATA     "generic-auth-data"

typedef gboolean (*SoupAuthDomainFilter) (SoupAuthDomain *domain,
					  SoupMessage    *msg,
					  gpointer        user_data);

typedef gboolean (*SoupAuthDomainGenericAuthCallback) (SoupAuthDomain *domain,
						       SoupMessage    *msg,
						       const char     *username,
						       gpointer        user_data);

GType       soup_auth_domain_get_type    (void);

void        soup_auth_domain_add_path    (SoupAuthDomain       *domain,
					  const char           *path);
void        soup_auth_domain_remove_path (SoupAuthDomain       *domain,
					  const char           *path);

void        soup_auth_domain_set_filter  (SoupAuthDomain       *domain,
					  SoupAuthDomainFilter  filter,
					  gpointer              filter_data,
					  GDestroyNotify        dnotify);

const char *soup_auth_domain_get_realm   (SoupAuthDomain       *domain);

void        soup_auth_domain_set_generic_auth_callback (SoupAuthDomain *domain,
							SoupAuthDomainGenericAuthCallback auth_callback,
							gpointer        auth_data,
							GDestroyNotify  dnotify);
gboolean    soup_auth_domain_check_password (SoupAuthDomain    *domain,
					     SoupMessage       *msg,
					     const char        *username,
					     const char        *password);

gboolean    soup_auth_domain_covers      (SoupAuthDomain       *domain,
					  SoupMessage          *msg);
char       *soup_auth_domain_accepts     (SoupAuthDomain       *domain,
					  SoupMessage          *msg);
void        soup_auth_domain_challenge   (SoupAuthDomain       *domain,
					  SoupMessage          *msg);

/* protected */
gboolean    soup_auth_domain_try_generic_auth_callback (SoupAuthDomain *domain,
							SoupMessage    *msg,
							const char     *username);

G_END_DECLS

#endif /* SOUP_AUTH_DOMAIN_H */
