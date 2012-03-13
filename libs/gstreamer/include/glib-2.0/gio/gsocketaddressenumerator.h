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

#ifndef __G_SOCKET_ADDRESS_ENUMERATOR_H__
#define __G_SOCKET_ADDRESS_ENUMERATOR_H__

#include <gio/giotypes.h>

G_BEGIN_DECLS

#define G_TYPE_SOCKET_ADDRESS_ENUMERATOR         (g_socket_address_enumerator_get_type ())
#define G_SOCKET_ADDRESS_ENUMERATOR(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), G_TYPE_SOCKET_ADDRESS_ENUMERATOR, GSocketAddressEnumerator))
#define G_SOCKET_ADDRESS_ENUMERATOR_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), G_TYPE_SOCKET_ADDRESS_ENUMERATOR, GSocketAddressEnumeratorClass))
#define G_IS_SOCKET_ADDRESS_ENUMERATOR(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), G_TYPE_SOCKET_ADDRESS_ENUMERATOR))
#define G_IS_SOCKET_ADDRESS_ENUMERATOR_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), G_TYPE_SOCKET_ADDRESS_ENUMERATOR))
#define G_SOCKET_ADDRESS_ENUMERATOR_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), G_TYPE_SOCKET_ADDRESS_ENUMERATOR, GSocketAddressEnumeratorClass))

/**
 * GSocketAddressEnumerator:
 *
 * Enumerator type for objects that contain or generate
 * #GSocketAddress<!-- -->es.
 */
typedef struct _GSocketAddressEnumeratorClass GSocketAddressEnumeratorClass;

struct _GSocketAddressEnumerator
{
  GObject parent_instance;

};

struct _GSocketAddressEnumeratorClass
{
  GObjectClass parent_class;

  /* Virtual Table */

  GSocketAddress * (* next)        (GSocketAddressEnumerator  *enumerator,
				    GCancellable              *cancellable,
				    GError                   **error);

  void             (* next_async)  (GSocketAddressEnumerator  *enumerator,
				    GCancellable              *cancellable,
				    GAsyncReadyCallback        callback,
				    gpointer                   user_data);
  GSocketAddress * (* next_finish) (GSocketAddressEnumerator  *enumerator,
				    GAsyncResult              *result,
				    GError                   **error);
};

GType           g_socket_address_enumerator_get_type        (void) G_GNUC_CONST;

GSocketAddress *g_socket_address_enumerator_next        (GSocketAddressEnumerator  *enumerator,
							 GCancellable              *cancellable,
							 GError                   **error);

void            g_socket_address_enumerator_next_async  (GSocketAddressEnumerator  *enumerator,
							 GCancellable              *cancellable,
							 GAsyncReadyCallback        callback,
							 gpointer                   user_data);
GSocketAddress *g_socket_address_enumerator_next_finish (GSocketAddressEnumerator  *enumerator,
							 GAsyncResult              *result,
							 GError                   **error);

G_END_DECLS


#endif /* __G_SOCKET_ADDRESS_ENUMERATOR_H__ */
