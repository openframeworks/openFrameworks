/* GIO - GLib Input, Output and Streaming Library
 *
 * Copyright (C) 2008 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#if !defined (__GIO_GIO_H_INSIDE__) && !defined (GIO_COMPILATION)
#error "Only <gio/gio.h> can be included directly."
#endif

#ifndef __G_RESOLVER_H__
#define __G_RESOLVER_H__

#include <gio/giotypes.h>

G_BEGIN_DECLS

#define G_TYPE_RESOLVER         (g_resolver_get_type ())
#define G_RESOLVER(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), G_TYPE_RESOLVER, GResolver))
#define G_RESOLVER_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), G_TYPE_RESOLVER, GResolverClass))
#define G_IS_RESOLVER(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), G_TYPE_RESOLVER))
#define G_IS_RESOLVER_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), G_TYPE_RESOLVER))
#define G_RESOLVER_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), G_TYPE_RESOLVER, GResolverClass))

typedef struct _GResolverPrivate GResolverPrivate;
typedef struct _GResolverClass   GResolverClass;

struct _GResolver {
  GObject parent_instance;

  GResolverPrivate *priv;
};

struct _GResolverClass {
  GObjectClass parent_class;

  /* Signals */
  void    ( *reload)                   (GResolver            *resolver);

  /* Virtual methods */
  GList * ( *lookup_by_name)           (GResolver            *resolver,
					const gchar          *hostname,
					GCancellable         *cancellable,
					GError              **error);
  void    ( *lookup_by_name_async)     (GResolver            *resolver,
					const gchar          *hostname,
					GCancellable         *cancellable,
					GAsyncReadyCallback   callback,
					gpointer              user_data);
  GList * ( *lookup_by_name_finish)    (GResolver            *resolver,
					GAsyncResult         *result,
					GError              **error);

  gchar * ( *lookup_by_address)        (GResolver            *resolver,
					GInetAddress         *address,
					GCancellable         *cancellable,
					GError              **error);
  void    ( *lookup_by_address_async)  (GResolver            *resolver,
					GInetAddress         *address,
					GCancellable         *cancellable,
					GAsyncReadyCallback   callback,
					gpointer              user_data);
  gchar * ( *lookup_by_address_finish) (GResolver            *resolver,
					GAsyncResult         *result,
					GError              **error);

  GList * ( *lookup_service)           (GResolver            *resolver,
					const gchar          *rrname,
					GCancellable         *cancellable,
					GError              **error);
  void    ( *lookup_service_async)     (GResolver            *resolver,
					const gchar          *rrname,
					GCancellable         *cancellable,
					GAsyncReadyCallback   callback,
					gpointer              user_data);
  GList * ( *lookup_service_finish)    (GResolver            *resolver,
					GAsyncResult         *result,
					GError              **error);

  /* Padding for future expansion */
  void (*_g_reserved1) (void);
  void (*_g_reserved2) (void);
  void (*_g_reserved3) (void);
  void (*_g_reserved4) (void);
  void (*_g_reserved5) (void);
  void (*_g_reserved6) (void);

};

GType      g_resolver_get_type                  (void) G_GNUC_CONST;
GResolver *g_resolver_get_default               (void);
void       g_resolver_set_default               (GResolver            *resolver);

GList     *g_resolver_lookup_by_name            (GResolver            *resolver,
						 const gchar          *hostname,
						 GCancellable         *cancellable,
						 GError              **error);
void       g_resolver_lookup_by_name_async      (GResolver            *resolver,
						 const gchar          *hostname,
						 GCancellable         *cancellable,
						 GAsyncReadyCallback   callback,
						 gpointer              user_data);
GList     *g_resolver_lookup_by_name_finish     (GResolver            *resolver,
						 GAsyncResult         *result,
						 GError              **error);

void       g_resolver_free_addresses            (GList                *addresses);

gchar     *g_resolver_lookup_by_address         (GResolver            *resolver,
						 GInetAddress         *address,
						 GCancellable         *cancellable,
						 GError              **error);
void       g_resolver_lookup_by_address_async   (GResolver            *resolver,
						 GInetAddress         *address,
						 GCancellable         *cancellable,
						 GAsyncReadyCallback   callback,
						 gpointer              user_data);
gchar     *g_resolver_lookup_by_address_finish  (GResolver            *resolver,
						 GAsyncResult         *result,
						 GError              **error);

GList     *g_resolver_lookup_service            (GResolver            *resolver,
						 const gchar          *service,
						 const gchar          *protocol,
						 const gchar          *domain,
						 GCancellable         *cancellable,
						 GError              **error);
void       g_resolver_lookup_service_async      (GResolver            *resolver,
						 const gchar          *service,
						 const gchar          *protocol,
						 const gchar          *domain,
						 GCancellable         *cancellable,
						 GAsyncReadyCallback   callback,
						 gpointer              user_data);
GList     *g_resolver_lookup_service_finish     (GResolver            *resolver,
						 GAsyncResult         *result,
						 GError              **error);

void       g_resolver_free_targets              (GList                *targets);

/**
 * G_RESOLVER_ERROR:
 *
 * Error domain for #GResolver. Errors in this domain will be from the
 * #GResolverError enumeration. See #GError for more information on
 * error domains.
 */
#define G_RESOLVER_ERROR (g_resolver_error_quark ())
GQuark g_resolver_error_quark (void);

G_END_DECLS

#endif /* __G_RESOLVER_H__ */
