/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2008 Red Hat, Inc.
 */

#ifndef SOUP_PROXY_RESOLVER_H
#define SOUP_PROXY_RESOLVER_H 1

#include <libsoup/soup-types.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#ifndef LIBSOUP_DISABLE_DEPRECATED

#define SOUP_TYPE_PROXY_RESOLVER            (soup_proxy_resolver_get_type ())
#define SOUP_PROXY_RESOLVER(object)         (G_TYPE_CHECK_INSTANCE_CAST ((object), SOUP_TYPE_PROXY_RESOLVER, SoupProxyResolver))
#define SOUP_PROXY_RESOLVER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SOUP_TYPE_PROXY_RESOLVER, SoupProxyResolverInterface))
#define SOUP_IS_PROXY_RESOLVER(object)      (G_TYPE_CHECK_INSTANCE_TYPE ((object), SOUP_TYPE_PROXY_RESOLVER))
#define SOUP_IS_PROXY_RESOLVER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SOUP_TYPE_PROXY_RESOLVER))
#define SOUP_PROXY_RESOLVER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_INTERFACE ((obj), SOUP_TYPE_PROXY_RESOLVER, SoupProxyResolverInterface))

typedef struct _SoupProxyResolver SoupProxyResolver;

typedef void (*SoupProxyResolverCallback) (SoupProxyResolver *, SoupMessage *,
					   guint, SoupAddress *, gpointer);

typedef struct {
	GTypeInterface base;

	/* virtual methods */
	void (*get_proxy_async) (SoupProxyResolver *proxy_resolver,
				 SoupMessage *msg, GMainContext *async_context,
				 GCancellable *cancellable,
				 SoupProxyResolverCallback callaback,
				 gpointer user_data);
	guint (*get_proxy_sync) (SoupProxyResolver *proxy_resolver,
				 SoupMessage *msg, GCancellable *cancellable,
				 SoupAddress **addr);

} SoupProxyResolverInterface;

GType soup_proxy_resolver_get_type (void);

void  soup_proxy_resolver_get_proxy_async (SoupProxyResolver  *proxy_resolver,
					   SoupMessage        *msg,
					   GMainContext       *async_context,
					   GCancellable       *cancellable,
					   SoupProxyResolverCallback callback,
					   gpointer            user_data);
guint soup_proxy_resolver_get_proxy_sync  (SoupProxyResolver  *proxy_resolver,
					   SoupMessage        *msg,
					   GCancellable       *cancellable,
					   SoupAddress       **addr);

#endif

G_END_DECLS

#endif /*SOUP_PROXY_RESOLVER_H*/
