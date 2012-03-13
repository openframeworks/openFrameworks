/* GStreamer
 * Copyright (C) <2003> David A. Schleef <ds@schleef.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GST_VALUE_H__
#define __GST_VALUE_H__

#include <gst/gstconfig.h>
#include <gst/gstcaps.h>
#include <gst/gststructure.h>

G_BEGIN_DECLS

/**
 * GST_MAKE_FOURCC:
 * @a: the first character
 * @b: the second character
 * @c: the third character
 * @d: the fourth character
 *
 * Transform four characters into a #guint32 fourcc value with host
 * endianness.
 * <informalexample>
 * <programlisting>
 * guint32 fourcc = GST_MAKE_FOURCC ('M', 'J', 'P', 'G');
 * </programlisting>
 * </informalexample>
 */
#define GST_MAKE_FOURCC(a,b,c,d)        ((guint32)((a)|(b)<<8|(c)<<16|(d)<<24))

/**
 * GST_STR_FOURCC:
 * @f: a string with at least four characters
 *
 * Transform an input string into a #guint32 fourcc value with host
 * endianness.
 * Caller is responsible for ensuring the input string consists of at least
 * four characters.
 * <informalexample>
 * <programlisting>
 * guint32 fourcc = GST_STR_FOURCC ("MJPG");
 * </programlisting>
 * </informalexample>
 */
#define GST_STR_FOURCC(f)               ((guint32)(((f)[0])|((f)[1]<<8)|((f)[2]<<16)|((f)[3]<<24)))

/**
 * GST_FOURCC_FORMAT:
 *
 * Can be used together with #GST_FOURCC_ARGS to properly output a
 * #guint32 fourcc value in a printf()-style text message.
 * <informalexample>
 * <programlisting>
 * printf ("fourcc: %" GST_FOURCC_FORMAT "\n", GST_FOURCC_ARGS (fcc));
 * </programlisting>
 * </informalexample>
 */
#define GST_FOURCC_FORMAT "c%c%c%c"

/**
 * GST_FOURCC_ARGS:
 * @fourcc: a #guint32 fourcc value to output
 *
 * Can be used together with #GST_FOURCC_FORMAT to properly output a
 * #guint32 fourcc value in a printf()-style text message.
 */
#define GST_FOURCC_ARGS(fourcc) \
        ((gchar) ((fourcc)     &0xff)), \
        ((gchar) (((fourcc)>>8 )&0xff)), \
        ((gchar) (((fourcc)>>16)&0xff)), \
        ((gchar) (((fourcc)>>24)&0xff))

/**
 * GST_VALUE_HOLDS_FOURCC:
 * @x: the #GValue to check
 *
 * Checks if the given #GValue contains a #GST_TYPE_FOURCC value.
 */
#define GST_VALUE_HOLDS_FOURCC(x)       (G_VALUE_HOLDS((x), gst_fourcc_get_type ()))

/**
 * GST_VALUE_HOLDS_INT_RANGE:
 * @x: the #GValue to check
 *
 * Checks if the given #GValue contains a #GST_TYPE_INT_RANGE value.
 */
#define GST_VALUE_HOLDS_INT_RANGE(x)    (G_VALUE_HOLDS((x), gst_int_range_get_type ()))

/**
 * GST_VALUE_HOLDS_INT64_RANGE:
 * @x: the #GValue to check
 *
 * Checks if the given #GValue contains a #GST_TYPE_INT64_RANGE value.
 *
 * Since: 0.10.31
 */
#define GST_VALUE_HOLDS_INT64_RANGE(x)    (G_VALUE_HOLDS((x), gst_int64_range_get_type ()))

/**
 * GST_VALUE_HOLDS_DOUBLE_RANGE:
 * @x: the #GValue to check
 *
 * Checks if the given #GValue contains a #GST_TYPE_DOUBLE_RANGE value.
 */
#define GST_VALUE_HOLDS_DOUBLE_RANGE(x) (G_VALUE_HOLDS((x), gst_double_range_get_type ()))

/**
 * GST_VALUE_HOLDS_FRACTION_RANGE:
 * @x: the #GValue to check
 *
 * Checks if the given #GValue contains a #GST_TYPE_FRACTION_RANGE value.
 */
#define GST_VALUE_HOLDS_FRACTION_RANGE(x)    (G_VALUE_HOLDS((x), gst_fraction_range_get_type ()))

/**
 * GST_VALUE_HOLDS_LIST:
 * @x: the #GValue to check
 *
 * Checks if the given #GValue contains a #GST_TYPE_LIST value.
 */
#define GST_VALUE_HOLDS_LIST(x)		(G_VALUE_HOLDS((x), gst_value_list_get_type ()))

/**
 * GST_VALUE_HOLDS_ARRAY:
 * @x: the #GValue to check
 *
 * Checks if the given #GValue contains a #GST_TYPE_ARRAY value.
 */
#define GST_VALUE_HOLDS_ARRAY(x)	(G_VALUE_HOLDS((x), gst_value_array_get_type ()))

/**
 * GST_VALUE_HOLDS_CAPS:
 * @x: the #GValue to check
 *
 * Checks if the given #GValue contains a #GST_TYPE_CAPS value.
 */
#define GST_VALUE_HOLDS_CAPS(x)		(G_VALUE_HOLDS((x), GST_TYPE_CAPS))

/**
 * GST_VALUE_HOLDS_STRUCTURE:
 * @x: the #GValue to check
 *
 * Checks if the given #GValue contains a #GST_TYPE_STRUCTURE value.
 *
 * Since: 0.10.15
 */
#define GST_VALUE_HOLDS_STRUCTURE(x)		(G_VALUE_HOLDS((x), GST_TYPE_STRUCTURE))

/**
 * GST_VALUE_HOLDS_BUFFER:
 * @x: the #GValue to check
 *
 * Checks if the given #GValue contains a #GST_TYPE_BUFFER value.
 */
#define GST_VALUE_HOLDS_BUFFER(x)       (G_VALUE_HOLDS((x), GST_TYPE_BUFFER))

/**
 * GST_VALUE_HOLDS_FRACTION:
 * @x: the #GValue to check
 *
 * Checks if the given #GValue contains a #GST_TYPE_FRACTION value.
 */
#define GST_VALUE_HOLDS_FRACTION(x)	(G_VALUE_HOLDS((x), gst_fraction_get_type ()))

/**
 * GST_VALUE_HOLDS_DATE:
 * @x: the #GValue to check
 *
 * Checks if the given #GValue contains a #GST_TYPE_DATE value.
 */
#define GST_VALUE_HOLDS_DATE(x)         (G_VALUE_HOLDS((x), gst_date_get_type ()))

/**
 * GST_VALUE_HOLDS_DATE_TIME:
 * @x: the #GValue to check
 *
 * Checks if the given #GValue contains a #GST_TYPE_DATE_TIME value.
 *
 * Since: 0.10.31
 */
#define GST_VALUE_HOLDS_DATE_TIME(x)    (G_VALUE_HOLDS((x), gst_date_time_get_type ()))

/**
 * GST_TYPE_FOURCC:
 *
 * a #GValue type that represents 4 byte identifier (e.g. used for codecs)
 *
 * Returns: the #GType of GstFourcc
 */
#define GST_TYPE_FOURCC                  gst_fourcc_get_type ()

/**
 * GST_TYPE_INT_RANGE:
 *
 * a #GValue type that represents an integer range
 *
 * Returns: the #GType of GstIntRange
 */
#define GST_TYPE_INT_RANGE               gst_int_range_get_type ()

/**
 * GST_TYPE_INT64_RANGE:
 *
 * a #GValue type that represents an #gint64 range
 *
 * Returns: the #GType of GstInt64Range
 *
 * Since: 0.10.31
 */
#define GST_TYPE_INT64_RANGE             gst_int64_range_get_type ()

/**
 * GST_TYPE_DOUBLE_RANGE:
 *
 * a #GValue type that represents a floating point range with double precission
 *
 * Returns: the #GType of GstIntRange
 */
#define GST_TYPE_DOUBLE_RANGE            gst_double_range_get_type ()

/**
 * GST_TYPE_FRACTION_RANGE:
 *
 * a #GValue type that represents a GstFraction range
 *
 * Returns: the #GType of GstFractionRange
 */
#define GST_TYPE_FRACTION_RANGE            gst_fraction_range_get_type ()

/**
 * GST_TYPE_LIST:
 *
 * a #GValue type that represents an unordered list of #GValue values. This
 * is used for example to express a list of possible values for a field in
 * a caps structure, like a list of possible sample rates, of which only one
 * will be chosen in the end. This means that all values in the list are
 * meaningful on their own.
 *
 * Returns: the #GType of GstValueList (which is not explicitly typed)
 */
#define GST_TYPE_LIST                    gst_value_list_get_type ()

/**
 * GST_TYPE_ARRAY:
 *
 * a #GValue type that represents an ordered list of #GValue values. This is
 * used to express a set of values that is meaningful only in their specific
 * combination and order of values. Each value on its own is not particularly
 * meaningful, only the ordered array in its entirety is meaningful. This is
 * used for example to express channel layouts for multichannel audio where
 * each channel needs to be mapped to a position in the room.
 *
 * Returns: the #GType of GstArrayList (which is not explicitly typed)
 */
#define GST_TYPE_ARRAY			 gst_value_array_get_type ()

/**
 * GST_TYPE_FRACTION:
 *
 * a #GValue type that represents a fraction of an integer numerator over
 * an integer denominator
 *
 * Returns: the #GType of GstFraction (which is not explicitly typed)
 */

#define GST_TYPE_FRACTION                gst_fraction_get_type ()

/**
 * GST_TYPE_DATE:
 *
 * a boxed #GValue type for #GDate that represents a date.
 *
 * Returns: the #GType of GstDate
 */

#define GST_TYPE_DATE                    gst_date_get_type ()

/**
 * GST_TYPE_DATE_TIME:
 *
 * a boxed #GValue type for #GstDateTime that represents a date and time.
 *
 * Returns: the #GType of GstDateTime
 * Since: 0.10.31
 */

#define GST_TYPE_DATE_TIME               gst_date_time_get_type ()

/**
 * GST_VALUE_LESS_THAN:
 *
 * Indicates that the first value provided to a comparison function
 * (gst_value_compare()) is lesser than the second one.
 */
#define GST_VALUE_LESS_THAN              (-1)

/**
 * GST_VALUE_EQUAL:
 *
 * Indicates that the first value provided to a comparison function
 * (gst_value_compare()) is equal to the second one.
 */
#define GST_VALUE_EQUAL                   0

/**
 * GST_VALUE_GREATER_THAN:
 *
 * Indicates that the first value provided to a comparison function
 * (gst_value_compare()) is greater than the second one.
 */
#define GST_VALUE_GREATER_THAN            1

/**
 * GST_VALUE_UNORDERED:
 *
 * Indicates that the comparison function (gst_value_compare()) can not
 * determine a order for the two provided values.
 */
#define GST_VALUE_UNORDERED               2

/**
 * GstValueCompareFunc:
 * @value1: first value for comparison
 * @value2: second value for comparison
 *
 * Used together with gst_value_compare() to compare #GValue items.
 *
 * Returns: one of GST_VALUE_LESS_THAN, GST_VALUE_EQUAL, GST_VALUE_GREATER_THAN
 * or GST_VALUE_UNORDERED
 */
typedef gint     (* GstValueCompareFunc)     (const GValue *value1,
					      const GValue *value2);

/**
 * GstValueSerializeFunc:
 * @value1: a #GValue
 *
 * Used by gst_value_serialize() to obtain a non-binary form of the #GValue.
 *
 * Returns: the string representation of the value
 */
typedef gchar *  (* GstValueSerializeFunc)   (const GValue *value1);

/**
 * GstValueDeserializeFunc:
 * @dest: a #GValue
 * @s: a string
 *
 * Used by gst_value_deserialize() to parse a non-binary form into the #GValue.
 *
 * Returns: %TRUE for success
 */
typedef gboolean (* GstValueDeserializeFunc) (GValue       *dest,
					      const gchar  *s);

/**
 * GstValueUnionFunc:
 * @dest: a #GValue for the result
 * @value1: a #GValue operand
 * @value2: a #GValue operand
 *
 * Used by gst_value_union() to perform unification for a specific #GValue
 * type. Register a new implementation with gst_value_register_union_func().
 *
 * Returns: %TRUE if a union was successful
 */
typedef gboolean (* GstValueUnionFunc)       (GValue       *dest,
					      const GValue *value1,
					      const GValue *value2);

/**
 * GstValueIntersectFunc:
 * @dest: a #GValue for the result
 * @value1: a #GValue operand
 * @value2: a #GValue operand
 *
 * Used by gst_value_intersect() to perform intersection for a specific #GValue
 * type. If the intersection is non-empty, the result is
 * placed in @dest and TRUE is returned.  If the intersection is
 * empty, @dest is unmodified and FALSE is returned.
 * Register a new implementation with gst_value_register_intersect_func().
 *
 * Returns: %TRUE if the values can intersect
 */
typedef gboolean (* GstValueIntersectFunc)   (GValue       *dest,
					      const GValue *value1,
					      const GValue *value2);

/**
 * GstValueSubtractFunc:
 * @dest: a #GValue for the result
 * @minuend: a #GValue operand
 * @subtrahend: a #GValue operand
 *
 * Used by gst_value_subtract() to perform subtraction for a specific #GValue
 * type. Register a new implementation with gst_value_register_subtract_func().
 *
 * Returns: %TRUE if the subtraction is not empty
 */
typedef gboolean (* GstValueSubtractFunc)    (GValue       *dest,
					      const GValue *minuend,
					      const GValue *subtrahend);

typedef struct _GstValueTable GstValueTable;
/**
 * GstValueTable:
 * @type: a #GType
 * @compare: a #GstValueCompareFunc
 * @serialize: a #GstValueSerializeFunc
 * @deserialize: a #GstValueDeserializeFunc
 *
 * VTable for the #GValue @type.
 */
struct _GstValueTable {
  GType type;
  GstValueCompareFunc compare;
  GstValueSerializeFunc serialize;
  GstValueDeserializeFunc deserialize;

  /*< private >*/
  void *_gst_reserved [GST_PADDING];
};

GType gst_int_range_get_type (void);
GType gst_int64_range_get_type (void);
GType gst_double_range_get_type (void);
GType gst_fraction_range_get_type (void);
GType gst_fourcc_get_type (void);
GType gst_fraction_get_type (void);
GType gst_value_list_get_type (void);
GType gst_value_array_get_type (void);

GType gst_date_get_type (void);
GType gst_date_time_get_type (void);

void		gst_value_register		(const GstValueTable   *table);
void		gst_value_init_and_copy		(GValue                *dest,
						 const GValue          *src);

gchar *		gst_value_serialize		(const GValue          *value);
gboolean	gst_value_deserialize		(GValue                *dest,
						 const gchar           *src);

/* list */
void		gst_value_list_append_value	(GValue		*value,
						 const GValue	*append_value);
void		gst_value_list_prepend_value	(GValue		*value,
						 const GValue	*prepend_value);
void		gst_value_list_concat		(GValue		*dest,
						 const GValue   *value1,
						 const GValue	*value2);
guint		gst_value_list_get_size		(const GValue	*value);
G_CONST_RETURN GValue *
		gst_value_list_get_value	(const GValue	*value,
						 guint		index);

/* array */
void		gst_value_array_append_value	(GValue		*value,
						 const GValue	*append_value);
void		gst_value_array_prepend_value	(GValue		*value,
						 const GValue	*prepend_value);
guint		gst_value_array_get_size	(const GValue	*value);
G_CONST_RETURN GValue *
		gst_value_array_get_value	(const GValue	*value,
						 guint		index);

/* fourcc */
void		gst_value_set_fourcc		(GValue		*value,
						 guint32	fourcc);
guint32		gst_value_get_fourcc		(const GValue   *value);

/* int range */
void		gst_value_set_int_range		(GValue		*value,
						 gint		start,
						 gint		end);
gint		gst_value_get_int_range_min	(const GValue	*value);
gint		gst_value_get_int_range_max	(const GValue	*value);

/* int64 range */
void		gst_value_set_int64_range      	(GValue		*value,
						 gint64		start,
						 gint64		end);
gint64		gst_value_get_int64_range_min	(const GValue	*value);
gint64		gst_value_get_int64_range_max	(const GValue	*value);

/* double range */
void		gst_value_set_double_range	(GValue		*value,
						 gdouble	start,
						 gdouble	end);
gdouble		gst_value_get_double_range_min	(const GValue	*value);
gdouble		gst_value_get_double_range_max	(const GValue	*value);

/* caps */
G_CONST_RETURN GstCaps *
		gst_value_get_caps		(const GValue	*value);
void		gst_value_set_caps		(GValue		*value,
						 const GstCaps  *caps);

/* structure */
G_CONST_RETURN GstStructure *
		gst_value_get_structure		(const GValue	*value);
void		gst_value_set_structure		(GValue		*value,
						 const GstStructure  *structure);

/* fraction */
void		gst_value_set_fraction		(GValue		*value,
						 gint		numerator,
						 gint		denominator);
gint		gst_value_get_fraction_numerator (const GValue	*value);
gint		gst_value_get_fraction_denominator(const GValue	*value);
gboolean	gst_value_fraction_multiply	(GValue		*product,
						 const GValue	*factor1,
						 const GValue	*factor2);
gboolean 	gst_value_fraction_subtract (GValue * dest,
					     const GValue * minuend, 
					     const GValue * subtrahend);

/* fraction range */
void		gst_value_set_fraction_range	(GValue		*value,
						 const GValue 	*start,
						 const GValue	*end);
void		gst_value_set_fraction_range_full (GValue	*value,
						 gint numerator_start, 
						 gint denominator_start,
						 gint numerator_end, 
						 gint denominator_end);
const GValue 	*gst_value_get_fraction_range_min (const GValue	*value);
const GValue 	*gst_value_get_fraction_range_max (const GValue	*value);

/* date */
G_CONST_RETURN GDate *
		gst_value_get_date		(const GValue	*value);
void		gst_value_set_date		(GValue		*value,
						 const GDate    *date);

/* compare */
gint		gst_value_compare		(const GValue	*value1,
						 const GValue	*value2);
gboolean	gst_value_can_compare		(const GValue	*value1,
						 const GValue	*value2);
/* union */
gboolean	gst_value_union			(GValue		*dest,
						 const GValue	*value1,
						 const GValue	*value2);
gboolean	gst_value_can_union		(const GValue	*value1,
						 const GValue   *value2);
void		gst_value_register_union_func	(GType		type1,
						 GType		type2,
						 GstValueUnionFunc func);

/* intersection */
gboolean	gst_value_intersect		(GValue		*dest,
						 const GValue	*value1,
						 const GValue	*value2);
gboolean	gst_value_can_intersect		(const GValue	*value1,
						 const GValue	*value2);
void		gst_value_register_intersect_func (GType	type1,
						GType		type2,
						GstValueIntersectFunc func);

/* subtraction */
gboolean	gst_value_subtract		(GValue		*dest,
						 const GValue	*minuend,
						 const GValue	*subtrahend);
gboolean	gst_value_can_subtract		(const GValue	*minuend,
						 const GValue	*subtrahend);
void		gst_value_register_subtract_func (GType		minuend_type,
						GType		subtrahend_type,
						GstValueSubtractFunc func);

/* fixation */
gboolean	gst_value_is_fixed		(const GValue   *value);

G_END_DECLS

#endif


