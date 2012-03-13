/* GObject - GLib Type, Object, Parameter and Signal Library
 * Copyright (C) 2001 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * gvaluearray.h: GLib array type holding GValues
 */
#if !defined (__GLIB_GOBJECT_H_INSIDE__) && !defined (GOBJECT_COMPILATION)
#error "Only <glib-object.h> can be included directly."
#endif

#ifndef __G_VALUE_ARRAY_H__
#define __G_VALUE_ARRAY_H__

#include	<gobject/gvalue.h>


G_BEGIN_DECLS


/* --- typedefs & structs --- */
typedef struct _GValueArray GValueArray;
/**
 * GValueArray:
 * @n_values: number of values contained in the array
 * @values: array of values
 * 
 * A #GValueArray contains an array of #GValue elements.
 */
struct _GValueArray
{
  guint   n_values;
  GValue *values;

  /*< private >*/
  guint   n_prealloced;
};


/* --- prototypes --- */
GValue*		g_value_array_get_nth	     (GValueArray	*value_array,
					      guint		 index_);
GValueArray*	g_value_array_new	     (guint		 n_prealloced);
void		g_value_array_free	     (GValueArray	*value_array);
GValueArray*	g_value_array_copy	     (const GValueArray *value_array);
GValueArray*	g_value_array_prepend	     (GValueArray	*value_array,
					      const GValue	*value);
GValueArray*	g_value_array_append	     (GValueArray	*value_array,
					      const GValue	*value);
GValueArray*	g_value_array_insert	     (GValueArray	*value_array,
					      guint		 index_,
					      const GValue	*value);
GValueArray*	g_value_array_remove	     (GValueArray	*value_array,
					      guint		 index_);
GValueArray*	g_value_array_sort	     (GValueArray	*value_array,
					      GCompareFunc	 compare_func);
GValueArray*	g_value_array_sort_with_data (GValueArray	*value_array,
					      GCompareDataFunc	 compare_func,
					      gpointer		 user_data);


G_END_DECLS

#endif /* __G_VALUE_ARRAY_H__ */
