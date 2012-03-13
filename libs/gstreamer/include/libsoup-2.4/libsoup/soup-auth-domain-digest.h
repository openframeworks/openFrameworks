/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2007 Novell, Inc.
 */

#ifndef SOUP_AUTH_DOMAIN_DIGEST_H
#define SOUP_AUTH_DOMAIN_DIGEST_H 1

#include <libsoup/soup-auth-domain.h>

G_BEGIN_DECLS

#define SOUP_TYPE_AUTH_DOMAIN_DIGEST            (soup_auth_domain_digest_get_type ())
#define SOUP_AUTH_DOMAIN_DIGEST(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SOUP_TYPE_AUTH_DOMAIN_DIGEST, SoupAuthDomainDigest))
#define SOUP_AUTH_DOMAIN_DIGEST_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SOUP_TYPE_AUTH_DOMAIN_DIGEST, SoupAuthDomainDigestClass))
#define SOUP_IS_AUTH_DOMAIN_DIGEST(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SOUP_TYPE_AUTH_DOMAIN_DIGEST))
#define SOUP_IS_AUTH_DOMAIN_DIGEST_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((obj), SOUP_TYPE_AUTH_DOMAIN_DIGEST))
#define SOUP_AUTH_DOMAIN_DIGEST_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SOUP_TYPE_AUTH_DOMAIN_DIGEST, SoupAuthDomainDigestClass))

typedef struct {
	SoupAuthDomain parent;

} SoupAuthDomainDigest;

typedef struct {
	SoupAuthDomainClass parent_class;

	/* Padding for future expansion */
	void (*_libsoup_reserved1) (void);
	void (*_libsoup_reserved2) (void);
	void (*_libsoup_reserved3) (void);
	void (*_libsoup_reserved4) (void);
} SoupAuthDomainDigestClass;

#define SOUP_AUTH_DOMAIN_DIGEST_AUTH_CALLBACK "auth-callback"
#define SOUP_AUTH_DOMAIN_DIGEST_AUTH_DATA     "auth-data"

GType soup_auth_domain_digest_get_type (void);

SoupAuthDomain *soup_auth_domain_digest_new (const char *optname1,
					    ...) G_GNUC_NULL_TERMINATED;

typedef	char * (*SoupAuthDomainDigestAuthCallback) (SoupAuthDomain *domain,
						    SoupMessage    *msg,
						    const char     *username,
						    gpointer        user_data);

void    soup_auth_domain_digest_set_auth_callback  (SoupAuthDomain *domain,
						    SoupAuthDomainDigestAuthCallback callback,
						    gpointer        user_data,
						    GDestroyNotify  dnotify);

char   *soup_auth_domain_digest_encode_password    (const char     *username,
						    const char     *realm,
						    const char     *password);

G_END_DECLS

#endif /* SOUP_AUTH_DOMAIN_DIGEST_H */
