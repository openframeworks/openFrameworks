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

#ifndef __G_FILE_ENUMERATOR_H__
#define __G_FILE_ENUMERATOR_H__

#include <gio/giotypes.h>

G_BEGIN_DECLS

#define G_TYPE_FILE_ENUMERATOR         (g_file_enumerator_get_type ())
#define G_FILE_ENUMERATOR(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), G_TYPE_FILE_ENUMERATOR, GFileEnumerator))
#define G_FILE_ENUMERATOR_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), G_TYPE_FILE_ENUMERATOR, GFileEnumeratorClass))
#define G_IS_FILE_ENUMERATOR(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), G_TYPE_FILE_ENUMERATOR))
#define G_IS_FILE_ENUMERATOR_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), G_TYPE_FILE_ENUMERATOR))
#define G_FILE_ENUMERATOR_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), G_TYPE_FILE_ENUMERATOR, GFileEnumeratorClass))

/**
 * GFileEnumerator:
 *
 * A per matched file iterator.
 **/
typedef struct _GFileEnumeratorClass    GFileEnumeratorClass;
typedef struct _GFileEnumeratorPrivate  GFileEnumeratorPrivate;

struct _GFileEnumerator
{
  GObject parent_instance;

  /*< private >*/
  GFileEnumeratorPrivate *priv;
};

struct _GFileEnumeratorClass
{
  GObjectClass parent_class;

  /* Virtual Table */

  GFileInfo * (* next_file)         (GFileEnumerator      *enumerator,
                                     GCancellable         *cancellable,
                                     GError              **error);
  gboolean    (* close_fn)          (GFileEnumerator      *enumerator,
                                     GCancellable         *cancellable,
                                     GError              **error);

  void        (* next_files_async)  (GFileEnumerator      *enumerator,
                                     int                   num_files,
                                     int                   io_priority,
                                     GCancellable         *cancellable,
                                     GAsyncReadyCallback   callback,
                                     gpointer              user_data);
  GList *     (* next_files_finish) (GFileEnumerator      *enumerator,
                                     GAsyncResult         *res,
                                     GError              **error);
  void        (* close_async)       (GFileEnumerator      *enumerator,
                                     int                   io_priority,
                                     GCancellable         *cancellable,
                                     GAsyncReadyCallback   callback,
                                     gpointer              user_data);
  gboolean    (* close_finish)      (GFileEnumerator      *enumerator,
                                     GAsyncResult         *res,
                                     GError              **error);

  /*< private >*/
  /* Padding for future expansion */
  void (*_g_reserved1) (void);
  void (*_g_reserved2) (void);
  void (*_g_reserved3) (void);
  void (*_g_reserved4) (void);
  void (*_g_reserved5) (void);
  void (*_g_reserved6) (void);
  void (*_g_reserved7) (void);
};

GType      g_file_enumerator_get_type          (void) G_GNUC_CONST;

GFileInfo *g_file_enumerator_next_file         (GFileEnumerator      *enumerator,
						GCancellable         *cancellable,
						GError              **error);
gboolean   g_file_enumerator_close             (GFileEnumerator      *enumerator,
						GCancellable         *cancellable,
						GError              **error);
void       g_file_enumerator_next_files_async  (GFileEnumerator      *enumerator,
						int                   num_files,
						int                   io_priority,
						GCancellable         *cancellable,
						GAsyncReadyCallback   callback,
						gpointer              user_data);
GList *    g_file_enumerator_next_files_finish (GFileEnumerator      *enumerator,
						GAsyncResult         *result,
						GError              **error);
void       g_file_enumerator_close_async       (GFileEnumerator      *enumerator,
						int                   io_priority,
						GCancellable         *cancellable,
						GAsyncReadyCallback   callback,
						gpointer              user_data);
gboolean   g_file_enumerator_close_finish      (GFileEnumerator      *enumerator,
						GAsyncResult         *result,
						GError              **error);
gboolean   g_file_enumerator_is_closed         (GFileEnumerator      *enumerator);
gboolean   g_file_enumerator_has_pending       (GFileEnumerator      *enumerator);
void       g_file_enumerator_set_pending       (GFileEnumerator      *enumerator,
						gboolean              pending);
GFile *    g_file_enumerator_get_container     (GFileEnumerator *enumerator);

G_END_DECLS

#endif /* __G_FILE_ENUMERATOR_H__ */
