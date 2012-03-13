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

#ifndef __G_VFS_H__
#define __G_VFS_H__

#include <gio/giotypes.h>

G_BEGIN_DECLS

#define G_TYPE_VFS         (g_vfs_get_type ())
#define G_VFS(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), G_TYPE_VFS, GVfs))
#define G_VFS_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), G_TYPE_VFS, GVfsClass))
#define G_VFS_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), G_TYPE_VFS, GVfsClass))
#define G_IS_VFS(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), G_TYPE_VFS))
#define G_IS_VFS_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), G_TYPE_VFS))

/**
 * G_VFS_EXTENSION_POINT_NAME:
 *
 * Extension point for #GVfs functionality.
 * See <link linkend="extending-gio">Extending GIO</link>.
 */
#define G_VFS_EXTENSION_POINT_NAME "gio-vfs"

/**
 * GVfs:
 *
 * Virtual File System object.
 **/
typedef struct _GVfsClass    GVfsClass;

struct _GVfs
{
  GObject parent_instance;
};

struct _GVfsClass
{
  GObjectClass parent_class;

  /* Virtual Table */

  gboolean              (* is_active)                 (GVfs       *vfs);
  GFile               * (* get_file_for_path)         (GVfs       *vfs,
                                                       const char *path);
  GFile               * (* get_file_for_uri)          (GVfs       *vfs,
                                                       const char *uri);
  const gchar * const * (* get_supported_uri_schemes) (GVfs       *vfs);
  GFile               * (* parse_name)                (GVfs       *vfs,
                                                       const char *parse_name);

  /*< private >*/
  void                  (* local_file_add_info)       (GVfs       *vfs,
						       const char *filename,
						       guint64     device,
						       GFileAttributeMatcher *attribute_matcher,
						       GFileInfo  *info,
						       GCancellable *cancellable,
						       gpointer   *extra_data,
						       GDestroyNotify *free_extra_data);
  void                  (* add_writable_namespaces)   (GVfs       *vfs,
						       GFileAttributeInfoList *list);
  gboolean              (* local_file_set_attributes) (GVfs       *vfs,
						       const char *filename,
						       GFileInfo  *info,
                                                       GFileQueryInfoFlags flags,
                                                       GCancellable *cancellable,
						       GError    **error);
  void                  (* local_file_removed)        (GVfs       *vfs,
						       const char *filename);
  void                  (* local_file_moved)          (GVfs       *vfs,
						       const char *source,
						       const char *dest);
  /* Padding for future expansion */
  void (*_g_reserved1) (void);
  void (*_g_reserved2) (void);
  void (*_g_reserved3) (void);
  void (*_g_reserved4) (void);
  void (*_g_reserved5) (void);
  void (*_g_reserved6) (void);
  void (*_g_reserved7) (void);
};

GType                 g_vfs_get_type                  (void) G_GNUC_CONST;

gboolean              g_vfs_is_active                 (GVfs       *vfs);
GFile *               g_vfs_get_file_for_path         (GVfs       *vfs,
                                                       const char *path);
GFile *               g_vfs_get_file_for_uri          (GVfs       *vfs,
                                                       const char *uri);
const gchar* const * g_vfs_get_supported_uri_schemes  (GVfs       *vfs);

GFile *               g_vfs_parse_name                (GVfs       *vfs,
                                                       const char *parse_name);

GVfs *                g_vfs_get_default               (void);
GVfs *                g_vfs_get_local                 (void);

G_END_DECLS

#endif /* __G_VFS_H__ */
