/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2009 Red Hat, Inc.
 */

#ifndef SOUP_PROXY_URI_RESOLVER_H
#define SOUP_PROXY_URI_RESOLVER_H 1

#include <libsoup/soup-proxy-resolver.h>

G_BEGIN_DECLS

#define SOUP_TYPE_PROXY_URI_RESOLVER            (soup_proxy_uri_resolver_get_type ())
#define SOUP_PROXY_URI_RESOLVER(object)         (G_TYPE_CHECK_INSTANCE_CAST ((object), SOUP_TYPE_PROXY_URI_RESOLVER, SoupProxyURIResolver))
#define SOUP_PROXY_URI_RESOLVER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SOUP_TYPE_PROXY_URI_RESOLVER, SoupProxyURIResolverInterface))
#define SOUP_IS_PROXY_URI_RESOLVER(object)      (G_TYPE_CHECK_INSTANCE_TYPE ((object), SOUP_TYPE_PROXY_URI_RESOLVER))
#define SOUP_IS_PROXY_URI_RESOLVER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SOUP_TYPE_PROXY_URI_RESOLVER))
#define SOUP_PROXY_URI_RESOLVER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_INTERFACE ((obj), SOUP_TYPE_PROXY_URI_RESOLVER, SoupProxyURIResolverInterface))

typedef struct _SoupProxyURIResolver SoupProxyURIResolver;

typedef void (*SoupProxyURIResolverCallback) (SoupProxyURIResolver *resolver,
					      guint status, SoupURI *proxy_uri,
					      gpointer user_data);

typedef struct {
	GTypeInterface base;

	/* virtual methods */
	void (*get_proxy_uri_async) (SoupProxyURIResolver *proxy_uri_resolver,
				     SoupURI *uri, GMainContext *async_context,
				     GCancellable *cancellable,
				     SoupProxyURIResolverCallback callback,
				     gpointer user_data);
	guint (*get_proxy_uri_sync) (SoupProxyURIResolver *proxy_uri_resolver,
				     SoupURI *uri, GCancellable *cancellable,
				     SoupURI **proxy_uri);

	/* Padding for future expansion */
	void (*_libsoup_reserved1) (void);
	void (*_libsoup_reserved2) (void);
	void (*_libsoup_reserved3) (void);
	void (*_libsoup_reserved4) (void);
} SoupProxyURIResolverInterface;

GType soup_proxy_uri_resolver_get_type (void);

void  soup_proxy_uri_resolver_get_proxy_uri_async (SoupProxyURIResolver  *proxy_uri_resolver,
						   SoupURI               *uri,
						   GMainContext          *async_context,
						   GCancellable          *cancellable,
						   SoupProxyURIResolverCallback callback,
						   gpointer                user_data);
guint soup_proxy_uri_resolver_get_proxy_uri_sync  (SoupProxyURIResolver  *proxy_uri_resolver,
						   SoupURI               *uri,
						   GCancellable          *cancellable,
						   SoupURI              **proxy_uri);

G_END_DECLS

#endif /*SOUP_PROXY_URI_RESOLVER_H*/
