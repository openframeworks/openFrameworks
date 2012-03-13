/* GObject - GLib Type, Object, Parameter and Signal Library
 * Copyright (C) 1998-1999, 2000-2001 Tim Janik and Red Hat, Inc.
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
 * gvaluecollector.h: GValue varargs stubs
 */
/**
 * SECTION:value_collection
 * @Short_description: Converting varargs to generic values
 * @See_also:#GValueTable
 * @Title: Varargs Value Collection
 * 
 * The macros in this section provide the varargs parsing support needed
 * in variadic GObject functions such as g_object_new() or g_object_set().
 * They currently support the collection of integral types, floating point 
 * types and pointers.
 */
#ifndef __G_VALUE_COLLECTOR_H__
#define __G_VALUE_COLLECTOR_H__

#include <glib-object.h>

G_BEGIN_DECLS

/* we may want to add aggregate types here some day, if requested
 * by users. the basic C types are covered already, everything
 * smaller than an int is promoted to an integer and floats are
 * always promoted to doubles for varargs call constructions.
 */
enum	/*< skip >*/
{
  G_VALUE_COLLECT_INT		= 'i',
  G_VALUE_COLLECT_LONG		= 'l',
  G_VALUE_COLLECT_INT64         = 'q',
  G_VALUE_COLLECT_DOUBLE	= 'd',
  G_VALUE_COLLECT_POINTER	= 'p'
};


/* vararg union holding actual values collected
 */
/**
 * GTypeCValue:
 * @v_int: the field for holding integer values
 * @v_long: the field for holding long integer values
 * @v_int64: the field for holding 64 bit integer values
 * @v_double: the field for holding floating point values
 * @v_pointer: the field for holding pointers
 * 
 * A union holding one collected value.
 */
union _GTypeCValue
{
  gint     v_int;
  glong    v_long;
  gint64   v_int64;
  gdouble  v_double;
  gpointer v_pointer;
};

/**
 * G_VALUE_COLLECT_INIT:
 * @value: a #GValue return location. @value must contain only 0 bytes.
 * @_value_type: the #GType to use for @value.
 * @var_args: the va_list variable; it may be evaluated multiple times
 * @flags: flags which are passed on to the collect_value() function of
 *  the #GTypeValueTable of @value.
 * @__error: a #gchar** variable that will be modified to hold a g_new()
 *  allocated error messages if something fails
 * 
 * Collects a variable argument value from a va_list. We have to
 * implement the varargs collection as a macro, because on some systems
 * va_list variables cannot be passed by reference.
 *
 * Since: 2.24
 */
#define G_VALUE_COLLECT_INIT(value, _value_type, var_args, flags, __error)		\
G_STMT_START {										\
  GValue *_val = (value);								\
  guint _flags = (flags);								\
  GTypeValueTable *_vtab = g_type_value_table_peek (_value_type);			\
  gchar *_collect_format = _vtab->collect_format;					\
  GTypeCValue _cvalues[G_VALUE_COLLECT_FORMAT_MAX_LENGTH] = { { 0, }, };		\
  guint _n_values = 0;									\
                                                                                        \
  _val->g_type = _value_type;		/* value_meminit() from gvalue.c */		\
  while (*_collect_format)								\
    {											\
      GTypeCValue *_cvalue = _cvalues + _n_values++;					\
                                                                                        \
      switch (*_collect_format++)							\
	{										\
	case G_VALUE_COLLECT_INT:							\
	  _cvalue->v_int = va_arg ((var_args), gint);					\
	  break;									\
	case G_VALUE_COLLECT_LONG:							\
	  _cvalue->v_long = va_arg ((var_args), glong);					\
	  break;									\
	case G_VALUE_COLLECT_INT64:							\
	  _cvalue->v_int64 = va_arg ((var_args), gint64);				\
	  break;									\
	case G_VALUE_COLLECT_DOUBLE:							\
	  _cvalue->v_double = va_arg ((var_args), gdouble);				\
	  break;									\
	case G_VALUE_COLLECT_POINTER:							\
	  _cvalue->v_pointer = va_arg ((var_args), gpointer);				\
	  break;									\
	default:									\
	  g_assert_not_reached ();							\
	}										\
    }											\
  *(__error) = _vtab->collect_value (_val,						\
				       _n_values,					\
				       _cvalues,					\
				       _flags);						\
} G_STMT_END

/**
 * G_VALUE_COLLECT:
 * @value: a #GValue return location. @value is supposed to be initialized
 *  according to the value type to be collected
 * @var_args: the va_list variable; it may be evaluated multiple times
 * @flags: flags which are passed on to the collect_value() function of
 *  the #GTypeValueTable of @value.
 * @__error: a #gchar** variable that will be modified to hold a g_new()
 *  allocated error messages if something fails
 *
 * Collects a variable argument value from a va_list. We have to
 * implement the varargs collection as a macro, because on some systems
 * va_list variables cannot be passed by reference.
 *
 * Note: If you are creating the @value argument just before calling this macro,
 * you should use the #G_VALUE_COLLECT_INIT variant and pass the unitialized
 * #GValue. That variant is faster than #G_VALUE_COLLECT.
 */
#define G_VALUE_COLLECT(value, var_args, flags, __error) G_STMT_START {			\
  GValue *_value = (value);								\
  GType _value_type = G_VALUE_TYPE (_value);						\
  GTypeValueTable *_vtable = g_type_value_table_peek (_value_type);			\
											\
  if (_vtable->value_free)								\
    _vtable->value_free (_value);							\
  memset (_value->data, 0, sizeof (_value->data));					\
											\
  G_VALUE_COLLECT_INIT(value, _value_type, var_args, flags, __error);			\
} G_STMT_END

/**
 * G_VALUE_LCOPY:
 * @value: a #GValue return location. @value is supposed to be initialized 
 *  according to the value type to be collected
 * @var_args: the va_list variable; it may be evaluated multiple times
 * @flags: flags which are passed on to the lcopy_value() function of
 *  the #GTypeValueTable of @value.
 * @__error: a #gchar** variable that will be modified to hold a g_new()
 *  allocated error messages if something fails
 * 
 * Collects a value's variable argument locations from a va_list. Usage is
 * analogous to G_VALUE_COLLECT().
 */
#define G_VALUE_LCOPY(value, var_args, flags, __error)					\
G_STMT_START {										\
  const GValue *_value = (value);							\
  guint _flags = (flags);								\
  GType _value_type = G_VALUE_TYPE (_value);						\
  GTypeValueTable *_vtable = g_type_value_table_peek (_value_type);			\
  gchar *_lcopy_format = _vtable->lcopy_format;						\
  GTypeCValue _cvalues[G_VALUE_COLLECT_FORMAT_MAX_LENGTH] = { { 0, }, };		\
  guint _n_values = 0;									\
                                                                                        \
  while (*_lcopy_format)								\
    {											\
      GTypeCValue *_cvalue = _cvalues + _n_values++;					\
                                                                                        \
      switch (*_lcopy_format++)								\
	{										\
	case G_VALUE_COLLECT_INT:							\
	  _cvalue->v_int = va_arg ((var_args), gint);					\
	  break;									\
	case G_VALUE_COLLECT_LONG:							\
	  _cvalue->v_long = va_arg ((var_args), glong);					\
	  break;									\
	case G_VALUE_COLLECT_INT64:							\
	  _cvalue->v_int64 = va_arg ((var_args), gint64);				\
	  break;									\
	case G_VALUE_COLLECT_DOUBLE:							\
	  _cvalue->v_double = va_arg ((var_args), gdouble);				\
	  break;									\
	case G_VALUE_COLLECT_POINTER:							\
	  _cvalue->v_pointer = va_arg ((var_args), gpointer);				\
	  break;									\
	default:									\
	  g_assert_not_reached ();							\
	}										\
    }											\
  *(__error) = _vtable->lcopy_value (_value,						\
				     _n_values,						\
				     _cvalues,						\
				     _flags);						\
} G_STMT_END


/**
 * G_VALUE_COLLECT_FORMAT_MAX_LENGTH:
 * 
 * The maximal number of #GTypeCValue<!-- -->s which can be collected for a 
 * single #GValue.
 */
#define	G_VALUE_COLLECT_FORMAT_MAX_LENGTH	(8)

G_END_DECLS

#endif /* __G_VALUE_COLLECTOR_H__ */
