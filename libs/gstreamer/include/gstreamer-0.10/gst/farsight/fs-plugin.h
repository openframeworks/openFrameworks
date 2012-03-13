/*
 * fs-plugin.h - Header for farsight plugin infrastructure
 *
 * Farsight Voice+Video library
 * Copyright (c) 2005 INdT.
 *   @author: Andre Moreira Magalhaes <andre.magalhaes@indt.org.br>
 * Copyright (c) 2005-2007 Collabora Ltd.
 * Copyright (c) 2005-2007 Nokia Corp.
 *   @author: Rob Taylor <rob.taylor@collabora.co.uk>
 *   @author: Olivier Crete <olivier.crete@collabora.co.uk>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */

#ifndef __FS_PLUGIN_H__
#define __FS_PLUGIN_H__

#include <glib.h>
#include <glib-object.h>
#include <gmodule.h>
#include <gst/gst.h>

#include <stdarg.h>

G_BEGIN_DECLS


/* TYPE MACROS */
#define FS_TYPE_PLUGIN \
  (fs_plugin_get_type ())
#define FS_PLUGIN(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), FS_TYPE_PLUGIN, FsPlugin))
#define FS_PLUGIN_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), FS_TYPE_PLUGIN, FsPluginClass))
#define FS_IS_PLUGIN(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj), FS_TYPE_PLUGIN))
#define FS_IS_PLUGIN_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), FS_TYPE_PLUGIN))
#define FS_PLUGIN_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), FS_TYPE_PLUGIN, FsPluginClass))

/**
 * FsPlugin:
 * @parent: the parent object
 *
 * This structure represents a plugin, it is opaque.
 */

typedef struct _FsPlugin FsPlugin;
typedef struct _FsPluginClass FsPluginClass;
typedef struct _FsPluginPrivate FsPluginPrivate;

struct _FsPlugin
{
  GTypeModule parent;

  /*< private >*/

  GType  type;

  gchar *name;                  /* name of the plugin */

  /*< private >*/

  FsPluginPrivate *priv;

  gpointer unused[4];
};

struct _FsPluginClass
{
  GTypeModuleClass parent_class;

  /*< private >*/

  gpointer unused[4];
};

GType fs_plugin_get_type (void);


GObject *fs_plugin_create_valist (const gchar *name,
                                  const gchar *type_suffix,
                                  GError **error,
                                  const gchar *first_property_name,
                                  va_list var_args);

GObject *fs_plugin_create (const gchar *name,
                           const gchar *type_suffix,
                           GError **error,
                           const gchar *first_property_name, ...);

gchar **fs_plugin_list_available (const gchar *type_suffix);

/**
 * FS_INIT_PLUGIN:
 * @type_register_func: A function that register a #GType and returns it
 *
 * This macro is used to declare Farsight plugins and must be used once
 * in any farsight plugin.
 */

#define FS_INIT_PLUGIN(type_register_func)            \
    G_MODULE_EXPORT void fs_init_plugin (FsPlugin *plugin) {    \
      plugin->type = (type_register_func (plugin));             \
    }


G_END_DECLS
#endif

