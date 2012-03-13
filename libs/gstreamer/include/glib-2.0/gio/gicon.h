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

#ifndef __G_ICON_H__
#define __G_ICON_H__

#include <gio/giotypes.h>

G_BEGIN_DECLS

#define G_TYPE_ICON            (g_icon_get_type ())
#define G_ICON(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), G_TYPE_ICON, GIcon))
#define G_IS_ICON(obj)	       (G_TYPE_CHECK_INSTANCE_TYPE ((obj), G_TYPE_ICON))
#define G_ICON_GET_IFACE(obj)  (G_TYPE_INSTANCE_GET_INTERFACE ((obj), G_TYPE_ICON, GIconIface))

/**
 * GIcon:
 *
 * An abstract type that specifies an icon.
 **/
typedef struct _GIconIface GIconIface;

/**
 * GIconIface:
 * @g_iface: The parent interface.
 * @hash: A hash for a given #GIcon.
 * @equal: Checks if two #GIcon<!-- -->s are equal.
 * @to_tokens: Serializes a #GIcon into tokens. The tokens must not
 * contain any whitespace. Don't implement if the #GIcon can't be
 * serialized (Since 2.20).
 * @from_tokens: Constructs a #GIcon from tokens. Set the #GError if
 * the tokens are malformed. Don't implement if the #GIcon can't be
 * serialized (Since 2.20).
 *
 * GIconIface is used to implement GIcon types for various
 * different systems. See #GThemedIcon and #GLoadableIcon for
 * examples of how to implement this interface.
 */
struct _GIconIface
{
  GTypeInterface g_iface;

  /* Virtual Table */

  guint       (* hash)        (GIcon   *icon);
  gboolean    (* equal)       (GIcon   *icon1,
                               GIcon   *icon2);
  gboolean    (* to_tokens)   (GIcon   *icon,
			       GPtrArray *tokens,
                               gint    *out_version);
  GIcon *     (* from_tokens) (gchar  **tokens,
                               gint     num_tokens,
                               gint     version,
                               GError **error);
};

GType    g_icon_get_type  (void) G_GNUC_CONST;

guint    g_icon_hash            (gconstpointer  icon);
gboolean g_icon_equal           (GIcon         *icon1,
                                 GIcon         *icon2);
gchar   *g_icon_to_string       (GIcon         *icon);
GIcon   *g_icon_new_for_string  (const gchar   *str,
                                 GError       **error);

G_END_DECLS

#endif /* __G_ICON_H__ */
