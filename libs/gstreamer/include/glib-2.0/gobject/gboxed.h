/* GObject - GLib Type, Object, Parameter and Signal Library
 * Copyright (C) 2000-2001 Red Hat, Inc.
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
#if !defined (__GLIB_GOBJECT_H_INSIDE__) && !defined (GOBJECT_COMPILATION)
#error "Only <glib-object.h> can be included directly."
#endif

#ifndef __G_BOXED_H__
#define __G_BOXED_H__

#include        <gobject/gtype.h>

G_BEGIN_DECLS

/* --- type macros --- */
#define G_TYPE_IS_BOXED(type)	   (G_TYPE_FUNDAMENTAL (type) == G_TYPE_BOXED)
/**
 * G_VALUE_HOLDS_BOXED:
 * @value: a valid #GValue structure
 * 
 * Checks whether the given #GValue can hold values derived from type %G_TYPE_BOXED.
 * 
 * Returns: %TRUE on success.
 */
#define G_VALUE_HOLDS_BOXED(value) (G_TYPE_CHECK_VALUE_TYPE ((value), G_TYPE_BOXED))


/* --- typedefs --- */
/**
 * GBoxedCopyFunc:
 * @boxed: The boxed structure to be copied.
 * 
 * This function is provided by the user and should produce a copy of the passed
 * in boxed structure.
 * 
 * Returns: The newly created copy of the boxed structure.
 */
typedef gpointer (*GBoxedCopyFunc) (gpointer boxed);

/**
 * GBoxedFreeFunc:
 * @boxed: The boxed structure to be freed.
 * 
 * This function is provided by the user and should free the boxed
 * structure passed.
 */
typedef void (*GBoxedFreeFunc) (gpointer boxed);


/* --- prototypes --- */
gpointer	g_boxed_copy			(GType		 boxed_type,
						 gconstpointer	 src_boxed);
void		g_boxed_free			(GType		 boxed_type,
						 gpointer	 boxed);
void		g_value_set_boxed		(GValue		*value,
						 gconstpointer	 v_boxed);
void		g_value_set_static_boxed	(GValue		*value,
						 gconstpointer	 v_boxed);
gpointer	g_value_get_boxed		(const GValue	*value);
gpointer	g_value_dup_boxed		(const GValue	*value);


/* --- convenience --- */
GType	g_boxed_type_register_static		(const gchar	*name,
						 GBoxedCopyFunc	 boxed_copy,
						 GBoxedFreeFunc	 boxed_free);


/* --- GLib boxed types --- */
/**
 * G_TYPE_CLOSURE:
 * 
 * The #GType for #GClosure.
 */
#define	G_TYPE_CLOSURE		(g_closure_get_type ())
/**
 * G_TYPE_VALUE:
 * 
 * The type ID of the "GValue" type which is a boxed type,
 * used to pass around pointers to GValues.
 */
#define	G_TYPE_VALUE		(g_value_get_type ())
/**
 * G_TYPE_VALUE_ARRAY:
 * 
 * The type ID of the "GValueArray" type which is a boxed type,
 * used to pass around pointers to GValueArrays.
 */
#define	G_TYPE_VALUE_ARRAY	(g_value_array_get_type ())
/**
 * G_TYPE_DATE:
 * 
 * The #GType for #GDate.
 */
#define	G_TYPE_DATE	        (g_date_get_type ())
/**
 * G_TYPE_STRV:
 * 
 * The #GType for a boxed type holding a %NULL-terminated array of strings.
 * 
 * The code fragments in the following example show the use of a property of
 * type #G_TYPE_STRV with g_object_class_install_property(), g_object_set()
 * and g_object_get().
 * 
 * |[
 * g_object_class_install_property (object_class,
 *                                  PROP_AUTHORS,
 *                                  g_param_spec_boxed ("authors",
 *                                                      _("Authors"),
 *                                                      _("List of authors"),
 *                                                      G_TYPE_STRV,
 *                                                      G_PARAM_READWRITE));
 * 
 * 
 * gchar *authors[] = { "Owen", "Tim", NULL };
 * g_object_set (obj, "authors", authors, NULL);
 * 
 * 
 * gchar *writers[];
 * g_object_get (obj, "authors", &writers, NULL);
 * // do something with writers
 * g_strfreev (writers);
 * ]|
 * 
 * Since: 2.4
 */
#define	G_TYPE_STRV	        (g_strv_get_type ())
/**
 * G_TYPE_GSTRING:
 * 
 * The #GType for #GString.
 */
#define	G_TYPE_GSTRING		(g_gstring_get_type ())
/**
 * G_TYPE_HASH_TABLE:
 * 
 * The #GType for a boxed type holding a #GHashTable reference.
 * 
 * Since: 2.10
 */
#define	G_TYPE_HASH_TABLE (g_hash_table_get_type ())
/**
 * G_TYPE_REGEX:
 * 
 * The #GType for a boxed type holding a #GRegex reference.
 * 
 * Since: 2.14
 */
#define	G_TYPE_REGEX (g_regex_get_type ())
/**
 * G_TYPE_ARRAY:
 *
 * The #GType for a boxed type holding a #GArray reference.
 *
 * Since: 2.22
 */
#define	G_TYPE_ARRAY (g_array_get_type ())
/**
 * G_TYPE_BYTE_ARRAY:
 *
 * The #GType for a boxed type holding a #GByteArray reference.
 *
 * Since: 2.22
 */
#define	G_TYPE_BYTE_ARRAY (g_byte_array_get_type ())
/**
 * G_TYPE_PTR_ARRAY:
 *
 * The #GType for a boxed type holding a #GPtrArray reference.
 *
 * Since: 2.22
 */
#define	G_TYPE_PTR_ARRAY (g_ptr_array_get_type ())
/**
 * G_TYPE_VARIANT_TYPE:
 * 
 * The #GType for a boxed type holding a #GVariantType.
 * 
 * Since: 2.24
 */
#define	G_TYPE_VARIANT_TYPE (g_variant_type_get_gtype ())
/**
 * G_TYPE_ERROR:
 * 
 * The #GType for a boxed type holding a #GError.
 * 
 * Since: 2.26
 */
#define	G_TYPE_ERROR (g_error_get_type ())
/**
 * G_TYPE_DATE_TIME:
 *
 * The #GType for a boxed type holding a #GDateTime.
 *
 * Since: 2.26
 */
#define G_TYPE_DATE_TIME (g_date_time_get_type ())

void    g_value_take_boxed      (GValue		*value,
				 gconstpointer	 v_boxed);
#ifndef G_DISABLE_DEPRECATED
void	g_value_set_boxed_take_ownership	(GValue		*value,
						 gconstpointer	 v_boxed);
#endif
GType	g_closure_get_type	(void)	G_GNUC_CONST;
GType	g_value_get_type	(void)	G_GNUC_CONST;
GType	g_value_array_get_type	(void)	G_GNUC_CONST;
GType	g_date_get_type	        (void)	G_GNUC_CONST;
GType	g_strv_get_type	        (void)	G_GNUC_CONST;
GType	g_gstring_get_type      (void)	G_GNUC_CONST;
GType   g_hash_table_get_type   (void)  G_GNUC_CONST;
GType   g_array_get_type        (void)  G_GNUC_CONST;
GType   g_byte_array_get_type   (void)  G_GNUC_CONST;
GType   g_ptr_array_get_type    (void)  G_GNUC_CONST;
GType   g_variant_type_get_gtype(void)  G_GNUC_CONST;
GType   g_regex_get_type        (void)  G_GNUC_CONST;
GType   g_error_get_type        (void)  G_GNUC_CONST;
GType   g_date_time_get_type    (void)  G_GNUC_CONST;

#ifndef G_DISABLE_DEPRECATED
GType   g_variant_get_gtype     (void)  G_GNUC_CONST;
#endif

/**
 * GStrv:
 * 
 * A C representable type name for #G_TYPE_STRV.
 */
typedef gchar** GStrv;
     
G_END_DECLS

#endif	/* __G_BOXED_H__ */
