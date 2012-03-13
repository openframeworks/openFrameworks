/*
 * Copyright © 2009, 2010 Codethink Limited
 * Copyright © 2010 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the licence, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Authors: Ryan Lortie <desrt@desrt.ca>
 *          Matthias Clasen <mclasen@redhat.com>
 */

#ifndef __G_SETTINGS_BACKEND_H__
#define __G_SETTINGS_BACKEND_H__

#if !defined (G_SETTINGS_ENABLE_BACKEND) && !defined (GIO_COMPILATION)
#error "You must define G_SETTINGS_ENABLE_BACKEND before including <gio/gsettingsbackend.h>."
#endif

#define __GIO_GIO_H_INSIDE__
#include <gio/giotypes.h>
#undef __GIO_GIO_H_INSIDE__

G_BEGIN_DECLS

#define G_TYPE_SETTINGS_BACKEND                             (g_settings_backend_get_type ())
#define G_SETTINGS_BACKEND(inst)                            (G_TYPE_CHECK_INSTANCE_CAST ((inst),                     \
                                                             G_TYPE_SETTINGS_BACKEND, GSettingsBackend))
#define G_SETTINGS_BACKEND_CLASS(class)                     (G_TYPE_CHECK_CLASS_CAST ((class),                       \
                                                             G_TYPE_SETTINGS_BACKEND, GSettingsBackendClass))
#define G_IS_SETTINGS_BACKEND(inst)                         (G_TYPE_CHECK_INSTANCE_TYPE ((inst),                     \
                                                             G_TYPE_SETTINGS_BACKEND))
#define G_IS_SETTINGS_BACKEND_CLASS(class)                  (G_TYPE_CHECK_CLASS_TYPE ((class),                       \
                                                             G_TYPE_SETTINGS_BACKEND))
#define G_SETTINGS_BACKEND_GET_CLASS(inst)                  (G_TYPE_INSTANCE_GET_CLASS ((inst),                      \
                                                             G_TYPE_SETTINGS_BACKEND, GSettingsBackendClass))

/**
 * G_SETTINGS_BACKEND_EXTENSION_POINT_NAME:
 *
 * Extension point for #GSettingsBackend functionality.
 **/
#define G_SETTINGS_BACKEND_EXTENSION_POINT_NAME "gsettings-backend"

/**
 * GSettingsBackend:
 *
 * An implementation of a settings storage repository.
 **/
typedef struct _GSettingsBackendPrivate                     GSettingsBackendPrivate;
typedef struct _GSettingsBackendClass                       GSettingsBackendClass;

struct _GSettingsBackendClass
{
  GObjectClass parent_class;

  GVariant *    (*read)             (GSettingsBackend    *backend,
                                     const gchar         *key,
                                     const GVariantType  *expected_type,
                                     gboolean             default_value);

  gboolean      (*get_writable)     (GSettingsBackend    *backend,
                                     const gchar         *key);

  gboolean      (*write)            (GSettingsBackend    *backend,
                                     const gchar         *key,
                                     GVariant            *value,
                                     gpointer             origin_tag);
  gboolean      (*write_tree)       (GSettingsBackend    *backend,
                                     GTree               *tree,
                                     gpointer             origin_tag);
  void          (*reset)            (GSettingsBackend    *backend,
                                     const gchar         *key,
                                     gpointer             origin_tag);

  void          (*subscribe)        (GSettingsBackend    *backend,
                                     const gchar         *name);
  void          (*unsubscribe)      (GSettingsBackend    *backend,
                                     const gchar         *name);
  void          (*sync)             (GSettingsBackend    *backend);

  GPermission * (*get_permission)   (GSettingsBackend    *backend,
                                     const gchar         *path);

  gpointer padding[24];
};

struct _GSettingsBackend
{
  GObject parent_instance;

  /*< private >*/
  GSettingsBackendPrivate *priv;
};

GType                   g_settings_backend_get_type                     (void);

void                    g_settings_backend_changed                      (GSettingsBackend    *backend,
                                                                         const gchar         *key,
                                                                         gpointer             origin_tag);
void                    g_settings_backend_path_changed                 (GSettingsBackend    *backend,
                                                                         const gchar         *path,
                                                                         gpointer             origin_tag);
void                    g_settings_backend_flatten_tree                 (GTree               *tree,
                                                                         gchar              **path,
                                                                         const gchar       ***keys,
                                                                         GVariant          ***values);
void                    g_settings_backend_keys_changed                 (GSettingsBackend    *backend,
                                                                         const gchar         *path,
                                                                         gchar const * const *items,
                                                                         gpointer             origin_tag);

void                    g_settings_backend_path_writable_changed        (GSettingsBackend    *backend,
                                                                         const gchar         *path);
void                    g_settings_backend_writable_changed             (GSettingsBackend    *backend,
                                                                         const gchar         *key);
void                    g_settings_backend_changed_tree                 (GSettingsBackend    *backend,
                                                                         GTree               *tree,
                                                                         gpointer             origin_tag);

GSettingsBackend *      g_keyfile_settings_backend_new                  (const gchar         *filename,
                                                                         const gchar         *root_path,
                                                                         const gchar         *root_group);

G_END_DECLS

#endif /* __G_SETTINGS_BACKEND_H__ */
