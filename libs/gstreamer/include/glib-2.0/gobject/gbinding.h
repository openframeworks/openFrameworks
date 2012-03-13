/* gbinding.h: Binding for object properties
 *
 * Copyright (C) 2010  Intel Corp.
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
 * Author: Emmanuele Bassi <ebassi@linux.intel.com>
 */

#if !defined (__GLIB_GOBJECT_H_INSIDE__) && !defined (GOBJECT_COMPILATION)
#error "Only <glib-object.h> can be included directly."
#endif

#ifndef __G_BINDING_H__
#define __G_BINDING_H__

#include <glib.h>
#include <gobject/gobject.h>

G_BEGIN_DECLS

#define G_TYPE_BINDING_FLAGS    (g_binding_flags_get_type ())

#define G_TYPE_BINDING          (g_binding_get_type ())
#define G_BINDING(obj)          (G_TYPE_CHECK_INSTANCE_CAST ((obj), G_TYPE_BINDING, GBinding))
#define G_IS_BINDING(obj)       (G_TYPE_CHECK_INSTANCE_TYPE ((obj), G_TYPE_BINDING))

/**
 * GBinding:
 *
 * <structname>GBinding</structname> is an opaque structure whose members
 * cannot be accessed directly.
 *
 * Since: 2.26
 */
typedef struct _GBinding        GBinding;

/**
 * GBindingTransformFunc:
 * @binding: a #GBinding
 * @source_value: the value of the source property
 * @target_value: the value of the target property
 * @user_data: data passed to the transform function
 *
 * A function to be called to transform the source property of @source
 * from @source_value into the target property of @target
 * using @target_value.
 *
 * Return value: %TRUE if the transformation was successful, and %FALSE
 *   otherwise
 *
 * Since: 2.26
 */
typedef gboolean (* GBindingTransformFunc) (GBinding     *binding,
                                            const GValue *source_value,
                                            GValue       *target_value,
                                            gpointer      user_data);

/**
 * GBindingFlags:
 * @G_BINDING_DEFAULT: The default binding; if the source property
 *   changes, the target property is updated with its value.
 * @G_BINDING_BIDIRECTIONAL: Bidirectional binding; if either the
 *   property of the source or the property of the target changes,
 *   the other is updated.
 * @G_BINDING_SYNC_CREATE: Synchronize the values of the source and
 *   target properties when creating the binding; the direction of
 *   the synchronization is always from the source to the target.
 * @G_BINDING_INVERT_BOOLEAN: If the two properties being bound are
 *   booleans, setting one to %TRUE will result in the other being
 *   set to %FALSE and vice versa. This flag will only work for
 *   boolean properties, and cannot be used when passing custom
 *   transformation functions to g_object_bind_property_full().
 *
 * Flags to be passed to g_object_bind_property() or
 * g_object_bind_property_full().
 *
 * This enumeration can be extended at later date.
 *
 * Since: 2.26
 */
typedef enum { /*< prefix=G_BINDING >*/
  G_BINDING_DEFAULT        = 0,

  G_BINDING_BIDIRECTIONAL  = 1 << 0,
  G_BINDING_SYNC_CREATE    = 1 << 1,
  G_BINDING_INVERT_BOOLEAN = 1 << 2
} GBindingFlags;

GType                 g_binding_flags_get_type      (void) G_GNUC_CONST;
GType                 g_binding_get_type            (void) G_GNUC_CONST;

GBindingFlags         g_binding_get_flags           (GBinding *binding);
GObject *             g_binding_get_source          (GBinding *binding);
GObject *             g_binding_get_target          (GBinding *binding);
G_CONST_RETURN gchar *g_binding_get_source_property (GBinding *binding);
G_CONST_RETURN gchar *g_binding_get_target_property (GBinding *binding);

GBinding *g_object_bind_property               (gpointer               source,
                                                const gchar           *source_property,
                                                gpointer               target,
                                                const gchar           *target_property,
                                                GBindingFlags          flags);
GBinding *g_object_bind_property_full          (gpointer               source,
                                                const gchar           *source_property,
                                                gpointer               target,
                                                const gchar           *target_property,
                                                GBindingFlags          flags,
                                                GBindingTransformFunc  transform_to,
                                                GBindingTransformFunc  transform_from,
                                                gpointer               user_data,
                                                GDestroyNotify         notify);
GBinding *g_object_bind_property_with_closures (gpointer               source,
                                                const gchar           *source_property,
                                                gpointer               target,
                                                const gchar           *target_property,
                                                GBindingFlags          flags,
                                                GClosure              *transform_to,
                                                GClosure              *transform_from);

G_END_DECLS

#endif /* __G_BINDING_H__ */
