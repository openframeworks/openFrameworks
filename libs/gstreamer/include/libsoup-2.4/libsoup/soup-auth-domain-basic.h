/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2007 Novell, Inc.
 */

#ifndef SOUP_AUTH_DOMAIN_BASIC_H
#define SOUP_AUTH_DOMAIN_BASIC_H 1

#include <libsoup/soup-auth-domain.h>

G_BEGIN_DECLS

#define SOUP_TYPE_AUTH_DOMAIN_BASIC            (soup_auth_domain_basic_get_type ())
#define SOUP_AUTH_DOMAIN_BASIC(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SOUP_TYPE_AUTH_DOMAIN_BASIC, SoupAuthDomainBasic))
#define SOUP_AUTH_DOMAIN_BASIC_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SOUP_TYPE_AUTH_DOMAIN_BASIC, SoupAuthDomainBasicClass))
#define SOUP_IS_AUTH_DOMAIN_BASIC(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SOUP_TYPE_AUTH_DOMAIN_BASIC))
#define SOUP_IS_AUTH_DOMAIN_BASIC_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((obj), SOUP_TYPE_AUTH_DOMAIN_BASIC))
#define SOUP_AUTH_DOMAIN_BASIC_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SOUP_TYPE_AUTH_DOMAIN_BASIC, SoupAuthDomainBasicClass))

typedef struct {
	SoupAuthDomain parent;

} SoupAuthDomainBasic;

typedef struct {
	SoupAuthDomainClass parent_class;

	/* Padding for future expansion */
	void (*_libsoup_reserved1) (void);
	void (*_libsoup_reserved2) (void);
	void (*_libsoup_reserved3) (void);
	void (*_libsoup_reserved4) (void);
} SoupAuthDomainBasicClass;

#define SOUP_AUTH_DOMAIN_BASIC_AUTH_CALLBACK "auth-callback"
#define SOUP_AUTH_DOMAIN_BASIC_AUTH_DATA     "auth-data"

GType soup_auth_domain_basic_get_type (void);

SoupAuthDomain *soup_auth_domain_basic_new (const char *optname1,
					    ...) G_GNUC_NULL_TERMINATED;

typedef	gboolean (*SoupAuthDomainBasicAuthCallback) (SoupAuthDomain *domain,
						     SoupMessage    *msg,
						     const char     *username,
						     const char     *password,
						     gpointer        user_data);

void      soup_auth_domain_basic_set_auth_callback  (SoupAuthDomain *domain,
						     SoupAuthDomainBasicAuthCallback callback,
						     gpointer        user_data,
						     GDestroyNotify  dnotify);

G_END_DECLS

#endif /* SOUP_AUTH_DOMAIN_BASIC_H */
