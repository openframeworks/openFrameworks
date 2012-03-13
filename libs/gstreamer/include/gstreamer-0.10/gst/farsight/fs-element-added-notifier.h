/*
 * Farsight2 - Recursive element addition notifier
 *
 * Copyright 2007-2008 Collabora Ltd.
 *  @author: Olivier Crete <olivier.crete@collabora.co.uk>
 * Copyright 2007-2008 Nokia Corp.
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

#ifndef __FS_ELEMENT_ADDED_NOTIFIER_H__
#define __FS_ELEMENT_ADDED_NOTIFIER_H__

#include <gst/gst.h>

G_BEGIN_DECLS


/* TYPE MACROS */
#define FS_TYPE_ELEMENT_ADDED_NOTIFIER \
  (fs_element_added_notifier_get_type ())
#define FS_ELEMENT_ADDED_NOTIFIER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), FS_TYPE_ELEMENT_ADDED_NOTIFIER,    \
      FsElementAddedNotifier))
#define FS_ELEMENT_ADDED_NOTIFIER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), FS_TYPE_ELEMENT_ADDED_NOTIFIER,     \
      FsElementAddedNotifierClass))
#define FS_IS_ELEMENT_ADDED_NOTIFIER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj), FS_TYPE_ELEMENT_ADDED_NOTIFIER))
#define FS_IS_ELEMENT_ADDED_NOTIFIER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), FS_TYPE_ELEMENT_ADDED_NOTIFIER))
#define FS_ELEMENT_ADDED_NOTIFIER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), FS_TYPE_ELEMENT_ADDED_NOTIFIER,    \
      FsElementAddedNotifierClass))


typedef struct _FsElementAddedNotifier FsElementAddedNotifier;
typedef struct _FsElementAddedNotifierClass FsElementAddedNotifierClass;
typedef struct _FsElementAddedNotifierPrivate FsElementAddedNotifierPrivate;

/**
 * FsElementAddedNotifier:
 * @parent: the #GObject parent
 *
 * All members are private
 */

struct _FsElementAddedNotifier
{
  GObject parent;

  /*< private >*/

  FsElementAddedNotifierPrivate *priv;
};

/**
 * FsElementAddedNotifierClass:
 * @parent_class: the #GObjectClass parent
 *
 * All members are private
 */
struct _FsElementAddedNotifierClass
{
  GObjectClass parent_class;
};


GType fs_element_added_notifier_get_type (void);

FsElementAddedNotifier *fs_element_added_notifier_new (void);

void fs_element_added_notifier_add (FsElementAddedNotifier *notifier,
    GstBin *bin);

gboolean fs_element_added_notifier_remove (FsElementAddedNotifier *notifier,
    GstBin *bin);

void fs_element_added_notifier_set_properties_from_keyfile (
    FsElementAddedNotifier *notifier,
    GKeyFile *keyfile);

gboolean fs_element_added_notifier_set_properties_from_file (
    FsElementAddedNotifier *notifier,
    const gchar *filename,
    GError **error);

G_END_DECLS

#endif /* __FS_ELEMENT_ADDED_NOTIFIER_H__ */
