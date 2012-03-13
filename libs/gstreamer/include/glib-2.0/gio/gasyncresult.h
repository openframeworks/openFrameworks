/* GIO - GLib Input, Output and Streaming Library
 *
 * Copyright (C) 2006-2007 Red Hat, Inc.
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
 *
 * Author: Alexander Larsson <alexl@redhat.com>
 */

#if !defined (__GIO_GIO_H_INSIDE__) && !defined (GIO_COMPILATION)
#error "Only <gio/gio.h> can be included directly."
#endif

#ifndef __G_ASYNC_RESULT_H__
#define __G_ASYNC_RESULT_H__

#include <gio/giotypes.h>

G_BEGIN_DECLS

#define G_TYPE_ASYNC_RESULT            (g_async_result_get_type ())
#define G_ASYNC_RESULT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), G_TYPE_ASYNC_RESULT, GAsyncResult))
#define G_IS_ASYNC_RESULT(obj)	       (G_TYPE_CHECK_INSTANCE_TYPE ((obj), G_TYPE_ASYNC_RESULT))
#define G_ASYNC_RESULT_GET_IFACE(obj)  (G_TYPE_INSTANCE_GET_INTERFACE ((obj), G_TYPE_ASYNC_RESULT, GAsyncResultIface))

/**
 * GAsyncResult:
 *
 * Holds results information for an asynchronous operation,
 * usually passed directly to a asynchronous _finish() operation.
 **/
typedef struct _GAsyncResultIface    GAsyncResultIface;


/**
 * GAsyncResultIface:
 * @g_iface: The parent interface.
 * @get_user_data: Gets the user data passed to the callback.
 * @get_source_object: Gets the source object that issued the asynchronous operation.
 *
 * Interface definition for #GAsyncResult.
 **/
struct _GAsyncResultIface
{
  GTypeInterface g_iface;

  /* Virtual Table */

  gpointer  (* get_user_data)     (GAsyncResult *async_result);
  GObject * (* get_source_object) (GAsyncResult *async_result);
};

GType    g_async_result_get_type          (void) G_GNUC_CONST;

gpointer g_async_result_get_user_data     (GAsyncResult *res);
GObject *g_async_result_get_source_object (GAsyncResult *res);

G_END_DECLS

#endif /* __G_ASYNC_RESULT_H__ */
