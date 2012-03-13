/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wtay@chello.be>
 *                    2002 Thomas Vander Stichele <thomas@apestaart.org>
 *
 * gstutils.h: Header for various utility functions
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


#ifndef __GST_UTILS_H__
#define __GST_UTILS_H__

#include <glib.h>
#include <gst/gstbin.h>
#include <gst/gstparse.h>

G_BEGIN_DECLS

void		gst_util_set_value_from_string	(GValue *value, const gchar *value_str);
void		gst_util_set_object_arg		(GObject *object, const gchar *name, const gchar *value);
void		gst_util_dump_mem		(const guchar *mem, guint size);

guint64         gst_util_gdouble_to_guint64     (gdouble value)  G_GNUC_CONST;
gdouble         gst_util_guint64_to_gdouble     (guint64 value)  G_GNUC_CONST;

/**
 * gst_guint64_to_gdouble:
 * @value: the #guint64 value to convert
 *
 * Convert @value to a gdouble.
 *
 * Returns: @value converted to a #gdouble.
 */

/**
 * gst_gdouble_to_guint64:
 * @value: the #gdouble value to convert
 *
 * Convert @value to a guint64.
 *
 * Returns: @value converted to a #guint64.
 */
#ifdef WIN32
#define         gst_gdouble_to_guint64(value)   gst_util_gdouble_to_guint64(value)
#define         gst_guint64_to_gdouble(value)   gst_util_guint64_to_gdouble(value)
#else
#define         gst_gdouble_to_guint64(value)   ((guint64) (value))
#define         gst_guint64_to_gdouble(value)   ((gdouble) (value))
#endif

guint64		gst_util_uint64_scale		(guint64 val, guint64 num, guint64 denom);
guint64		gst_util_uint64_scale_round	(guint64 val, guint64 num, guint64 denom);
guint64		gst_util_uint64_scale_ceil	(guint64 val, guint64 num, guint64 denom);

guint64         gst_util_uint64_scale_int       (guint64 val, gint num, gint denom);
guint64         gst_util_uint64_scale_int_round (guint64 val, gint num, gint denom);
guint64         gst_util_uint64_scale_int_ceil  (guint64 val, gint num, gint denom);

guint32         gst_util_seqnum_next            (void);
gint32          gst_util_seqnum_compare         (guint32 s1, guint32 s2);

void		gst_print_pad_caps		(GString *buf, gint indent, GstPad *pad);
void		gst_print_element_args		(GString *buf, gint indent, GstElement *element);


GType gst_type_register_static_full (GType parent_type,
			                   const gchar       *type_name,
                               guint              class_size,
                               GBaseInitFunc      base_init,
                               GBaseFinalizeFunc  base_finalize,
                               GClassInitFunc     class_init,
                               GClassFinalizeFunc class_finalize,
                               gconstpointer      class_data,
                               guint              instance_size,
                               guint16            n_preallocs,
                               GInstanceInitFunc  instance_init,
                               const GTypeValueTable *value_table,
                               GTypeFlags	 flags);


/* Macros for defining classes.  Ideas taken from Bonobo, which took theirs
   from Nautilus and GOB. */

/**
 * GST_BOILERPLATE_FULL:
 * @type: the name of the type struct
 * @type_as_function: the prefix for the functions
 * @parent_type: the parent type struct name
 * @parent_type_macro: the parent type macro
 * @additional_initializations: function pointer in the form of
 * void additional_initializations (GType type) that can be used for
 * initializing interfaces and the like
 *
 * Define the boilerplate type stuff to reduce typos and code size.  Defines
 * the get_type method and the parent_class static variable.
 *
 * <informalexample>
 * <programlisting>
 *   GST_BOILERPLATE_FULL (GstFdSink, gst_fdsink, GstElement, GST_TYPE_ELEMENT, _do_init);
 * </programlisting>
 * </informalexample>
 */
#define GST_BOILERPLATE_FULL(type, type_as_function, parent_type, parent_type_macro, additional_initializations)	\
									\
static void type_as_function ## _base_init     (gpointer      g_class);	\
static void type_as_function ## _class_init    (type ## Class *g_class);\
static void type_as_function ## _init	       (type          *object,	\
                                                type ## Class *g_class);\
static parent_type ## Class *parent_class = NULL;			\
static void								\
type_as_function ## _class_init_trampoline (gpointer g_class)		\
{									\
  parent_class = (parent_type ## Class *)				\
      g_type_class_peek_parent (g_class);				\
  type_as_function ## _class_init ((type ## Class *)g_class);		\
}									\
									\
GType									\
type_as_function ## _get_type (void)					\
{									\
  /* The typedef for GType may be gulong or gsize, depending on the	\
   * system and whether the compiler is c++ or not. The g_once_init_*	\
   * functions always take a gsize * though ... */			\
  static volatile gsize gonce_data = 0;					\
  if (g_once_init_enter (&gonce_data)) {				\
    GType _type;							\
    _type = gst_type_register_static_full (parent_type_macro,           \
        g_intern_static_string (#type),					\
	sizeof (type ## Class),						\
        type_as_function ## _base_init,					\
        NULL,		  /* base_finalize */				\
        (GClassInitFunc) type_as_function ## _class_init_trampoline,    \
        NULL,		  /* class_finalize */				\
        NULL,               /* class_data */				\
        sizeof (type),							\
        0,                  /* n_preallocs */				\
        (GInstanceInitFunc) type_as_function ## _init,                  \
        NULL,                                                           \
        (GTypeFlags) 0);				                \
    additional_initializations (_type);				        \
    g_once_init_leave (&gonce_data, (gsize) _type);			\
  }									\
  return (GType) gonce_data;						\
}

#define __GST_DO_NOTHING(type)	/* NOP */

/**
 * GST_BOILERPLATE:
 * @type: the name of the type struct
 * @type_as_function: the prefix for the functions
 * @parent_type: the parent type struct name
 * @parent_type_macro: the parent type macro
 *
 * Define the boilerplate type stuff to reduce typos and code size.  Defines
 * the get_type method and the parent_class static variable.
 *
 * <informalexample>
 * <programlisting>
 *   GST_BOILERPLATE (GstFdSink, gst_fdsink, GstElement, GST_TYPE_ELEMENT);
 * </programlisting>
 * </informalexample>
 */
#define GST_BOILERPLATE(type,type_as_function,parent_type,parent_type_macro)	\
  GST_BOILERPLATE_FULL (type, type_as_function, parent_type, parent_type_macro,	\
      __GST_DO_NOTHING)

/* Like GST_BOILERPLATE, but makes the type 1) implement an interface, and 2)
 * implement GstImplementsInterface for that type
 *
 * After this you will need to implement interface_as_function ## _supported
 * and interface_as_function ## _interface_init
 */
/**
 * GST_BOILERPLATE_WITH_INTERFACE:
 * @type: the name of the type struct
 * @type_as_function: the prefix for the functions
 * @parent_type: the parent type struct name
 * @parent_type_as_macro: the parent type macro
 * @interface_type: the name of the interface type struct
 * @interface_type_as_macro: the interface type macro
 * @interface_as_function: the interface function name prefix
 *
 * Like GST_BOILERPLATE, but makes the type 1) implement an interface, and 2)
 * implement GstImplementsInterface for that type.
 *
 * After this you will need to implement interface_as_function ## _supported
 * and interface_as_function ## _interface_init
 */
#define GST_BOILERPLATE_WITH_INTERFACE(type, type_as_function,		\
    parent_type, parent_type_as_macro, interface_type,			\
    interface_type_as_macro, interface_as_function)			\
                                                                        \
static void interface_as_function ## _interface_init (interface_type ## Class *klass);  \
static gboolean interface_as_function ## _supported (type *object, GType iface_type);   \
                                                                        \
static void                                                             \
type_as_function ## _implements_interface_init (GstImplementsInterfaceClass *klass)     \
{                                                                       \
  klass->supported = (gboolean (*)(GstImplementsInterface*, GType))interface_as_function ## _supported;     \
}                                                                       \
                                                                        \
static void                                                             \
type_as_function ## _init_interfaces (GType type)                       \
{                                                                       \
  static const GInterfaceInfo implements_iface_info = {                 \
    (GInterfaceInitFunc) type_as_function ## _implements_interface_init,\
    NULL,                                                               \
    NULL,                                                               \
  };                                                                    \
  static const GInterfaceInfo iface_info = {                            \
    (GInterfaceInitFunc) interface_as_function ## _interface_init,      \
    NULL,                                                               \
    NULL,                                                               \
  };                                                                    \
                                                                        \
  g_type_add_interface_static (type, GST_TYPE_IMPLEMENTS_INTERFACE,     \
      &implements_iface_info);                                          \
  g_type_add_interface_static (type, interface_type_as_macro,		\
      &iface_info);							\
}                                                                       \
                                                                        \
GST_BOILERPLATE_FULL (type, type_as_function, parent_type,              \
    parent_type_as_macro, type_as_function ## _init_interfaces)

/**
 * GST_CALL_PARENT:
 * @parent_class_cast: the name of the class cast macro for the parent type
 * @name: name of the function to call
 * @args: arguments enclosed in '( )'
 *
 * Just call the parent handler.  This assumes that there is a variable
 * named parent_class that points to the (duh!) parent class.  Note that
 * this macro is not to be used with things that return something, use
 * the _WITH_DEFAULT version for that
 */
#define GST_CALL_PARENT(parent_class_cast, name, args)			\
	((parent_class_cast(parent_class)->name != NULL) ?		\
	 parent_class_cast(parent_class)->name args : (void) 0)

/**
 * GST_CALL_PARENT_WITH_DEFAULT:
 * @parent_class_cast: the name of the class cast macro for the parent type
 * @name: name of the function to call
 * @args: arguments enclosed in '( )'
 * @def_return: default result
 *
 * Same as GST_CALL_PARENT(), but in case there is no implementation, it
 * evaluates to @def_return.
 */
#define GST_CALL_PARENT_WITH_DEFAULT(parent_class_cast, name, args, def_return)\
	((parent_class_cast(parent_class)->name != NULL) ?		\
	 parent_class_cast(parent_class)->name args : def_return)

/* Define PUT and GET functions for unaligned memory */
#define _GST_GET(__data, __idx, __size, __shift) \
    (((guint##__size) (((const guint8 *) (__data))[__idx])) << (__shift))

#define _GST_PUT(__data, __idx, __size, __shift, __num) \
    (((guint8 *) (__data))[__idx] = (((guint##__size) (__num)) >> (__shift)) & 0xff)

/**
 * GST_READ_UINT64_BE:
 * @data: memory location
 *
 * Read a 64 bit unsigned integer value in big endian format from the memory buffer.
 */
#define GST_READ_UINT64_BE(data)	(_GST_GET (data, 0, 64, 56) | \
					 _GST_GET (data, 1, 64, 48) | \
					 _GST_GET (data, 2, 64, 40) | \
					 _GST_GET (data, 3, 64, 32) | \
					 _GST_GET (data, 4, 64, 24) | \
					 _GST_GET (data, 5, 64, 16) | \
					 _GST_GET (data, 6, 64,  8) | \
					 _GST_GET (data, 7, 64,  0))

/**
 * GST_READ_UINT64_LE:
 * @data: memory location
 *
 * Read a 64 bit unsigned integer value in little endian format from the memory buffer.
 */
#define GST_READ_UINT64_LE(data)	(_GST_GET (data, 7, 64, 56) | \
					 _GST_GET (data, 6, 64, 48) | \
					 _GST_GET (data, 5, 64, 40) | \
					 _GST_GET (data, 4, 64, 32) | \
					 _GST_GET (data, 3, 64, 24) | \
					 _GST_GET (data, 2, 64, 16) | \
					 _GST_GET (data, 1, 64,  8) | \
					 _GST_GET (data, 0, 64,  0))

/**
 * GST_READ_UINT32_BE:
 * @data: memory location
 *
 * Read a 32 bit unsigned integer value in big endian format from the memory buffer.
 */
#define GST_READ_UINT32_BE(data)	(_GST_GET (data, 0, 32, 24) | \
					 _GST_GET (data, 1, 32, 16) | \
					 _GST_GET (data, 2, 32,  8) | \
					 _GST_GET (data, 3, 32,  0))

/**
 * GST_READ_UINT32_LE:
 * @data: memory location
 *
 * Read a 32 bit unsigned integer value in little endian format from the memory buffer.
 */
#define GST_READ_UINT32_LE(data)	(_GST_GET (data, 3, 32, 24) | \
					 _GST_GET (data, 2, 32, 16) | \
					 _GST_GET (data, 1, 32,  8) | \
					 _GST_GET (data, 0, 32,  0))

/**
 * GST_READ_UINT24_BE:
 * @data: memory location
 *
 * Read a 24 bit unsigned integer value in big endian format from the memory buffer.
 *
 * Since: 0.10.22
 */
#define GST_READ_UINT24_BE(data)	(_GST_GET (data, 0, 32, 16) | \
					 _GST_GET (data, 1, 32,  8) | \
					 _GST_GET (data, 2, 32,  0))

/**
 * GST_READ_UINT24_LE:
 * @data: memory location
 *
 * Read a 24 bit unsigned integer value in little endian format from the memory buffer.
 *
 * Since: 0.10.22
 */
#define GST_READ_UINT24_LE(data)	(_GST_GET (data, 2, 32, 16) | \
					 _GST_GET (data, 1, 32,  8) | \
					 _GST_GET (data, 0, 32,  0))

/**
 * GST_READ_UINT16_BE:
 * @data: memory location
 *
 * Read a 16 bit unsigned integer value in big endian format from the memory buffer.
 */
#define GST_READ_UINT16_BE(data)	(_GST_GET (data, 0, 16,  8) | \
					 _GST_GET (data, 1, 16,  0))

/**
 * GST_READ_UINT16_LE:
 * @data: memory location
 *
 * Read a 16 bit unsigned integer value in little endian format from the memory buffer.
 */
#define GST_READ_UINT16_LE(data)	(_GST_GET (data, 1, 16,  8) | \
					 _GST_GET (data, 0, 16,  0))

/**
 * GST_READ_UINT8:
 * @data: memory location
 *
 * Read an 8 bit unsigned integer value from the memory buffer.
 */
#define GST_READ_UINT8(data)		(_GST_GET (data, 0,  8,  0))

/**
 * GST_WRITE_UINT64_BE:
 * @data: memory location
 * @num: value to store
 *
 * Store a 64 bit unsigned integer value in big endian format into the memory buffer.
 */
#define GST_WRITE_UINT64_BE(data, num)	do { \
					  _GST_PUT (data, 0, 64, 56, num); \
					  _GST_PUT (data, 1, 64, 48, num); \
					  _GST_PUT (data, 2, 64, 40, num); \
					  _GST_PUT (data, 3, 64, 32, num); \
					  _GST_PUT (data, 4, 64, 24, num); \
					  _GST_PUT (data, 5, 64, 16, num); \
					  _GST_PUT (data, 6, 64,  8, num); \
					  _GST_PUT (data, 7, 64,  0, num); \
					} while (0)

/**
 * GST_WRITE_UINT64_LE:
 * @data: memory location
 * @num: value to store
 *
 * Store a 64 bit unsigned integer value in little endian format into the memory buffer.
 */
#define GST_WRITE_UINT64_LE(data, num)	do { \
					  _GST_PUT (data, 0, 64,  0, num); \
					  _GST_PUT (data, 1, 64,  8, num); \
					  _GST_PUT (data, 2, 64, 16, num); \
					  _GST_PUT (data, 3, 64, 24, num); \
					  _GST_PUT (data, 4, 64, 32, num); \
					  _GST_PUT (data, 5, 64, 40, num); \
					  _GST_PUT (data, 6, 64, 48, num); \
					  _GST_PUT (data, 7, 64, 56, num); \
					} while (0)

/**
 * GST_WRITE_UINT32_BE:
 * @data: memory location
 * @num: value to store
 *
 * Store a 32 bit unsigned integer value in big endian format into the memory buffer.
 */
#define GST_WRITE_UINT32_BE(data, num)	do { \
					  _GST_PUT (data, 0, 32, 24, num); \
					  _GST_PUT (data, 1, 32, 16, num); \
					  _GST_PUT (data, 2, 32,  8, num); \
					  _GST_PUT (data, 3, 32,  0, num); \
					} while (0)

/**
 * GST_WRITE_UINT32_LE:
 * @data: memory location
 * @num: value to store
 *
 * Store a 32 bit unsigned integer value in little endian format into the memory buffer.
 */
#define GST_WRITE_UINT32_LE(data, num)	do { \
					  _GST_PUT (data, 0, 32,  0, num); \
					  _GST_PUT (data, 1, 32,  8, num); \
					  _GST_PUT (data, 2, 32, 16, num); \
					  _GST_PUT (data, 3, 32, 24, num); \
					} while (0)

/**
 * GST_WRITE_UINT24_BE:
 * @data: memory location
 * @num: value to store
 *
 * Store a 24 bit unsigned integer value in big endian format into the memory buffer.
 *
 * Since: 0.10.22
 */
#define GST_WRITE_UINT24_BE(data, num)	do { \
					  _GST_PUT (data, 0, 32,  16, num); \
					  _GST_PUT (data, 1, 32,  8, num); \
					  _GST_PUT (data, 2, 32,  0, num); \
					} while (0)

/**
 * GST_WRITE_UINT24_LE:
 * @data: memory location
 * @num: value to store
 *
 * Store a 24 bit unsigned integer value in little endian format into the memory buffer.
 *
 * Since: 0.10.22
 */
#define GST_WRITE_UINT24_LE(data, num)	do { \
					  _GST_PUT (data, 0, 32,  0, num); \
					  _GST_PUT (data, 1, 32,  8, num); \
					  _GST_PUT (data, 2, 32,  16, num); \
					} while (0)

/**
 * GST_WRITE_UINT16_BE:
 * @data: memory location
 * @num: value to store
 *
 * Store a 16 bit unsigned integer value in big endian format into the memory buffer.
 */
#define GST_WRITE_UINT16_BE(data, num)	do { \
					  _GST_PUT (data, 0, 16,  8, num); \
					  _GST_PUT (data, 1, 16,  0, num); \
					} while (0)

/**
 * GST_WRITE_UINT16_LE:
 * @data: memory location
 * @num: value to store
 *
 * Store a 16 bit unsigned integer value in little endian format into the memory buffer.
 */
#define GST_WRITE_UINT16_LE(data, num)	do { \
					  _GST_PUT (data, 0, 16,  0, num); \
					  _GST_PUT (data, 1, 16,  8, num); \
					} while (0)

/**
 * GST_WRITE_UINT8:
 * @data: memory location
 * @num: value to store
 *
 * Store an 8 bit unsigned integer value into the memory buffer.
 */
#define GST_WRITE_UINT8(data, num)	do { \
					  _GST_PUT (data, 0,  8,  0, num); \
					} while (0)

/* Float endianess conversion macros */

/* FIXME: Remove this once we depend on a GLib version with this */
#ifndef GFLOAT_FROM_LE
/**
 * GFLOAT_SWAP_LE_BE:
 * @in: input value
 *
 * Swap byte order of a 32-bit floating point value (float).
 *
 * Returns: @in byte-swapped.
 *
 * Since: 0.10.22
 *
 */
#ifdef _FOOL_GTK_DOC_
G_INLINE_FUNC gfloat GFLOAT_SWAP_LE_BE (gfloat in);
#endif

inline static gfloat
GFLOAT_SWAP_LE_BE(gfloat in)
{
  union
  {
    guint32 i;
    gfloat f;
  } u;

  u.f = in;
  u.i = GUINT32_SWAP_LE_BE (u.i);
  return u.f;
}

/**
 * GDOUBLE_SWAP_LE_BE:
 * @in: input value
 *
 * Swap byte order of a 64-bit floating point value (double).
 *
 * Returns: @in byte-swapped.
 *
 * Since: 0.10.22
 *
 */
#ifdef _FOOL_GTK_DOC_
G_INLINE_FUNC gdouble GDOUBLE_SWAP_LE_BE (gdouble in);
#endif

inline static gdouble
GDOUBLE_SWAP_LE_BE(gdouble in)
{
  union
  {
    guint64 i;
    gdouble d;
  } u;

  u.d = in;
  u.i = GUINT64_SWAP_LE_BE (u.i);
  return u.d;
}

/**
 * GDOUBLE_TO_LE:
 * @val: value
 *
 * Convert 64-bit floating point value (double) from native byte order into
 * little endian byte order.
 *
 * Since: 0.10.22
 *
 */
/**
 * GDOUBLE_TO_BE:
 * @val: value
 *
 * Convert 64-bit floating point value (double) from native byte order into
 * big endian byte order.
 *
 * Since: 0.10.22
 *
 */
/**
 * GDOUBLE_FROM_LE:
 * @val: value
 *
 * Convert 64-bit floating point value (double) from little endian byte order
 * into native byte order.
 *
 * Since: 0.10.22
 *
 */
/**
 * GDOUBLE_FROM_BE:
 * @val: value
 *
 * Convert 64-bit floating point value (double) from big endian byte order
 * into native byte order.
 *
 * Since: 0.10.22
 *
 */

/**
 * GFLOAT_TO_LE:
 * @val: value
 *
 * Convert 32-bit floating point value (float) from native byte order into
 * little endian byte order.
 *
 * Since: 0.10.22
 *
 */
/**
 * GFLOAT_TO_BE:
 * @val: value
 *
 * Convert 32-bit floating point value (float) from native byte order into
 * big endian byte order.
 *
 * Since: 0.10.22
 *
 */
/**
 * GFLOAT_FROM_LE:
 * @val: value
 *
 * Convert 32-bit floating point value (float) from little endian byte order
 * into native byte order.
 *
 * Since: 0.10.22
 *
 */
/**
 * GFLOAT_FROM_BE:
 * @val: value
 *
 * Convert 32-bit floating point value (float) from big endian byte order
 * into native byte order.
 *
 * Since: 0.10.22
 *
 */

#if G_BYTE_ORDER == G_LITTLE_ENDIAN
#define GFLOAT_TO_LE(val)    ((gfloat) (val))
#define GFLOAT_TO_BE(val)    (GFLOAT_SWAP_LE_BE (val))
#define GDOUBLE_TO_LE(val)   ((gdouble) (val))
#define GDOUBLE_TO_BE(val)   (GDOUBLE_SWAP_LE_BE (val))

#elif G_BYTE_ORDER == G_BIG_ENDIAN
#define GFLOAT_TO_LE(val)    (GFLOAT_SWAP_LE_BE (val))
#define GFLOAT_TO_BE(val)    ((gfloat) (val))
#define GDOUBLE_TO_LE(val)   (GDOUBLE_SWAP_LE_BE (val))
#define GDOUBLE_TO_BE(val)   ((gdouble) (val))

#else /* !G_LITTLE_ENDIAN && !G_BIG_ENDIAN */
#error unknown ENDIAN type
#endif /* !G_LITTLE_ENDIAN && !G_BIG_ENDIAN */

#define GFLOAT_FROM_LE(val)  (GFLOAT_TO_LE (val))
#define GFLOAT_FROM_BE(val)  (GFLOAT_TO_BE (val))
#define GDOUBLE_FROM_LE(val) (GDOUBLE_TO_LE (val))
#define GDOUBLE_FROM_BE(val) (GDOUBLE_TO_BE (val))

#endif /* !defined(GFLOAT_FROM_LE) */

/**
 * GST_READ_FLOAT_LE:
 * @data: memory location
 *
 * Read a 32 bit float value in little endian format from the memory buffer.
 *
 * Returns: The floating point value read from @data
 *
 * Since: 0.10.22
 *
 */
#ifdef _FOOL_GTK_DOC_
G_INLINE_FUNC gfloat GST_READ_FLOAT_LE (const guint8 *data);
#endif

inline static gfloat
GST_READ_FLOAT_LE(const guint8 *data)
{
  union
  {
    guint32 i;
    gfloat f;
  } u;

  u.i = GST_READ_UINT32_LE (data);
  return u.f;
}

/**
 * GST_READ_FLOAT_BE:
 * @data: memory location
 *
 * Read a 32 bit float value in big endian format from the memory buffer.
 *
 * Returns: The floating point value read from @data
 *
 * Since: 0.10.22
 *
 */
#ifdef _FOOL_GTK_DOC_
G_INLINE_FUNC gfloat GST_READ_FLOAT_BE (const guint8 *data);
#endif

inline static gfloat
GST_READ_FLOAT_BE(const guint8 *data)
{
  union
  {
    guint32 i;
    gfloat f;
  } u;

  u.i = GST_READ_UINT32_BE (data);
  return u.f;
}

/**
 * GST_READ_DOUBLE_LE:
 * @data: memory location
 *
 * Read a 64 bit double value in little endian format from the memory buffer.
 *
 * Returns: The double-precision floating point value read from @data
 *
 * Since: 0.10.22
 *
 */
#ifdef _FOOL_GTK_DOC_
G_INLINE_FUNC gdouble GST_READ_DOUBLE_LE (const guint8 *data);
#endif

inline static gdouble
GST_READ_DOUBLE_LE(const guint8 *data)
{
  union
  {
    guint64 i;
    gdouble d;
  } u;

  u.i = GST_READ_UINT64_LE (data);
  return u.d;
}

/**
 * GST_READ_DOUBLE_BE:
 * @data: memory location
 *
 * Read a 64 bit double value in big endian format from the memory buffer.
 *
 * Returns: The double-precision floating point value read from @data
 *
 * Since: 0.10.22
 *
 */
#ifdef _FOOL_GTK_DOC_
G_INLINE_FUNC gdouble GST_READ_DOUBLE_BE (const guint8 *data);
#endif

inline static gdouble
GST_READ_DOUBLE_BE(const guint8 *data)
{
  union
  {
    guint64 i;
    gdouble d;
  } u;

  u.i = GST_READ_UINT64_BE (data);
  return u.d;
}

/**
 * GST_WRITE_FLOAT_LE:
 * @data: memory location
 * @num: value to store
 *
 * Store a 32 bit float value in little endian format into the memory buffer.
 *
 * Since: 0.10.22
 *
 */
#ifdef _FOOL_GTK_DOC_
G_INLINE_FUNC void GST_WRITE_FLOAT_LE (guint8 *data, gfloat num);
#endif

inline static void
GST_WRITE_FLOAT_LE(guint8 *data, gfloat num)
{
  union
  {
    guint32 i;
    gfloat f;
  } u;

  u.f = num;
  GST_WRITE_UINT32_LE (data, u.i);
}

/**
 * GST_WRITE_FLOAT_BE:
 * @data: memory location
 * @num: value to store
 *
 * Store a 32 bit float value in big endian format into the memory buffer.
 *
 * Since: 0.10.22
 *
 */
#ifdef _FOOL_GTK_DOC_
G_INLINE_FUNC void GST_WRITE_FLOAT_BE (guint8 *data, gfloat num);
#endif

inline static void
GST_WRITE_FLOAT_BE(guint8 *data, gfloat num)
{
  union
  {
    guint32 i;
    gfloat f;
  } u;

  u.f = num;
  GST_WRITE_UINT32_BE (data, u.i);
}

/**
 * GST_WRITE_DOUBLE_LE:
 * @data: memory location
 * @num: value to store
 *
 * Store a 64 bit double value in little endian format into the memory buffer.
 *
 * Since: 0.10.22
 *
 */
#ifdef _FOOL_GTK_DOC_
G_INLINE_FUNC void GST_WRITE_DOUBLE_LE (guint8 *data, gdouble num);
#endif

inline static void
GST_WRITE_DOUBLE_LE(guint8 *data, gdouble num)
{
  union
  {
    guint64 i;
    gdouble d;
  } u;

  u.d = num;
  GST_WRITE_UINT64_LE (data, u.i);
}

/**
 * GST_WRITE_DOUBLE_BE:
 * @data: memory location
 * @num: value to store
 *
 * Store a 64 bit double value in big endian format into the memory buffer.
 *
 * Since: 0.10.22
 *
 */
#ifdef _FOOL_GTK_DOC_
G_INLINE_FUNC void GST_WRITE_DOUBLE_BE (guint8 *data, gdouble num);
#endif

inline static void
GST_WRITE_DOUBLE_BE(guint8 *data, gdouble num)
{
  union
  {
    guint64 i;
    gdouble d;
  } u;

  u.d = num;
  GST_WRITE_UINT64_BE (data, u.i);
}

/* Miscellaneous utility macros */

/**
 * GST_ROUND_UP_2:
 * @num: integer value to round up
 *
 * Rounds an integer value up to the next multiple of 2.
 */
#define GST_ROUND_UP_2(num)  (((num)+1)&~1)
/**
 * GST_ROUND_UP_4:
 * @num: integer value to round up
 *
 * Rounds an integer value up to the next multiple of 4.
 */
#define GST_ROUND_UP_4(num)  (((num)+3)&~3)
/**
 * GST_ROUND_UP_8:
 * @num: integer value to round up
 *
 * Rounds an integer value up to the next multiple of 8.
 */
#define GST_ROUND_UP_8(num)  (((num)+7)&~7)
/**
 * GST_ROUND_UP_16:
 * @num: integer value to round up
 *
 * Rounds an integer value up to the next multiple of 16.
 */
#define GST_ROUND_UP_16(num) (((num)+15)&~15)
/**
 * GST_ROUND_UP_32:
 * @num: integer value to round up
 *
 * Rounds an integer value up to the next multiple of 32.
 */
#define GST_ROUND_UP_32(num) (((num)+31)&~31)
/**
 * GST_ROUND_UP_64:
 * @num: integer value to round up
 *
 * Rounds an integer value up to the next multiple of 64.
 */
#define GST_ROUND_UP_64(num) (((num)+63)&~63)

/**
 * GST_ROUND_DOWN_2:
 * @num: integer value to round down
 *
 * Rounds an integer value down to the next multiple of 2.
 *
 * Since: 0.10.12
 */
#define GST_ROUND_DOWN_2(num)  ((num)&(~1))
/**
 * GST_ROUND_DOWN_4:
 * @num: integer value to round down
 *
 * Rounds an integer value down to the next multiple of 4.
 *
 * Since: 0.10.12
 */
#define GST_ROUND_DOWN_4(num)  ((num)&(~3))
/**
 * GST_ROUND_DOWN_8:
 * @num: integer value to round down
 *
 * Rounds an integer value down to the next multiple of 8.
 *
 * Since: 0.10.12
 */
#define GST_ROUND_DOWN_8(num)  ((num)&(~7))
/**
 * GST_ROUND_DOWN_16:
 * @num: integer value to round down
 *
 * Rounds an integer value down to the next multiple of 16.
 *
 * Since: 0.10.12
 */
#define GST_ROUND_DOWN_16(num) ((num)&(~15))
/**
 * GST_ROUND_DOWN_32:
 * @num: integer value to round down
 *
 * Rounds an integer value down to the next multiple of 32.
 *
 * Since: 0.10.12
 */
#define GST_ROUND_DOWN_32(num) ((num)&(~31))
/**
 * GST_ROUND_DOWN_64:
 * @num: integer value to round down
 *
 * Rounds an integer value down to the next multiple of 64.
 *
 * Since: 0.10.12
 */
#define GST_ROUND_DOWN_64(num) ((num)&(~63))

void			gst_object_default_error	(GstObject * source,
							 GError * error, gchar * debug);

/* element functions */
void                    gst_element_create_all_pads     (GstElement *element);
GstPad*                 gst_element_get_compatible_pad  (GstElement *element, GstPad *pad,
		                                         const GstCaps *caps);

GstPadTemplate*         gst_element_get_compatible_pad_template (GstElement *element, GstPadTemplate *compattempl);

G_CONST_RETURN gchar*   gst_element_state_get_name      (GstState state);
G_CONST_RETURN gchar *  gst_element_state_change_return_get_name (GstStateChangeReturn state_ret);

gboolean		gst_element_link                (GstElement *src, GstElement *dest);
gboolean		gst_element_link_many           (GstElement *element_1,
		                                         GstElement *element_2, ...) G_GNUC_NULL_TERMINATED;
gboolean		gst_element_link_filtered	(GstElement * src,
                                                         GstElement * dest,
                                                         GstCaps *filter);
void                    gst_element_unlink              (GstElement *src, GstElement *dest);
void                    gst_element_unlink_many         (GstElement *element_1,
		                                         GstElement *element_2, ...) G_GNUC_NULL_TERMINATED;

gboolean		gst_element_link_pads           (GstElement *src, const gchar *srcpadname,
		                                         GstElement *dest, const gchar *destpadname);
gboolean		gst_element_link_pads_full      (GstElement *src, const gchar *srcpadname,
		                                         GstElement *dest, const gchar *destpadname,
							 GstPadLinkCheck flags);
void                    gst_element_unlink_pads         (GstElement *src, const gchar *srcpadname,
		                                         GstElement *dest, const gchar *destpadname);

gboolean		gst_element_link_pads_filtered	(GstElement * src, const gchar * srcpadname,
                                                         GstElement * dest, const gchar * destpadname,
                                                         GstCaps *filter);

gboolean                gst_element_seek_simple         (GstElement   *element,
                                                         GstFormat     format,
                                                         GstSeekFlags  seek_flags,
                                                         gint64        seek_pos);

/* util elementfactory functions */
gboolean		gst_element_factory_can_src_caps(GstElementFactory *factory, const GstCaps *caps);
gboolean		gst_element_factory_can_sink_caps(GstElementFactory *factory, const GstCaps *caps);

/* util query functions */
gboolean                gst_element_query_position      (GstElement *element, GstFormat *format,
		                                         gint64 *cur);
gboolean                gst_element_query_duration      (GstElement *element, GstFormat *format,
		                                         gint64 *duration);
gboolean                gst_element_query_convert       (GstElement *element, GstFormat src_format, gint64 src_val,
		                                         GstFormat *dest_format, gint64 *dest_val);

/* element class functions */
void			gst_element_class_install_std_props (GstElementClass * klass,
							 const gchar * first_name, ...) G_GNUC_NULL_TERMINATED;

/* pad functions */
void			gst_pad_use_fixed_caps		(GstPad *pad);
GstCaps*		gst_pad_get_fixed_caps_func	(GstPad *pad);
GstCaps*		gst_pad_proxy_getcaps		(GstPad * pad);
gboolean		gst_pad_proxy_setcaps		(GstPad * pad, GstCaps * caps);

GstElement*		gst_pad_get_parent_element	(GstPad *pad);

/* util query functions */
gboolean                gst_pad_query_position          (GstPad *pad, GstFormat *format,
		                                         gint64 *cur);
gboolean                gst_pad_query_duration          (GstPad *pad, GstFormat *format,
		                                         gint64 *duration);
gboolean                gst_pad_query_convert           (GstPad *pad, GstFormat src_format, gint64 src_val,
		                                         GstFormat *dest_format, gint64 *dest_val);

gboolean                gst_pad_query_peer_position     (GstPad *pad, GstFormat *format,
		                                         gint64 *cur);
gboolean                gst_pad_query_peer_duration     (GstPad *pad, GstFormat *format,
		                                         gint64 *duration);
gboolean                gst_pad_query_peer_convert      (GstPad *pad, GstFormat src_format, gint64 src_val,
		                                         GstFormat *dest_format, gint64 *dest_val);

/* bin functions */
void                    gst_bin_add_many                (GstBin *bin, GstElement *element_1, ...) G_GNUC_NULL_TERMINATED;
void                    gst_bin_remove_many             (GstBin *bin, GstElement *element_1, ...) G_GNUC_NULL_TERMINATED;
GstPad *                gst_bin_find_unlinked_pad       (GstBin *bin, GstPadDirection direction);
#ifndef GST_DISABLE_DEPRECATED
GstPad *                gst_bin_find_unconnected_pad    (GstBin *bin, GstPadDirection direction);
#endif

/* buffer functions */
GstBuffer *		gst_buffer_merge		(GstBuffer * buf1, GstBuffer * buf2);
GstBuffer *		gst_buffer_join			(GstBuffer * buf1, GstBuffer * buf2);
#ifndef GST_DISABLE_DEPRECATED
void			gst_buffer_stamp		(GstBuffer * dest, const GstBuffer * src);
#endif /* GST_DISABLE_DEPRECATED */

/* atomic functions */
#ifndef GST_DISABLE_DEPRECATED
void                    gst_atomic_int_set              (gint * atomic_int, gint value);
#endif

/* probes */
gulong			gst_pad_add_data_probe		(GstPad   * pad,
							 GCallback  handler,
							 gpointer   data);

gulong			gst_pad_add_data_probe_full	(GstPad       * pad,
							 GCallback      handler,
							 gpointer       data,
							 GDestroyNotify notify);

void			gst_pad_remove_data_probe	(GstPad * pad, guint handler_id);

gulong			gst_pad_add_event_probe		(GstPad   * pad,
							 GCallback  handler,
							 gpointer   data);

gulong			gst_pad_add_event_probe_full	(GstPad       * pad,
							 GCallback      handler,
							 gpointer       data,
							 GDestroyNotify notify);

void			gst_pad_remove_event_probe	(GstPad * pad, guint handler_id);

gulong			gst_pad_add_buffer_probe	(GstPad   * pad,
							 GCallback  handler,
							 gpointer   data);

gulong			gst_pad_add_buffer_probe_full	(GstPad       * pad,
							 GCallback      handler,
							 gpointer       data,
							 GDestroyNotify notify);

void			gst_pad_remove_buffer_probe	(GstPad * pad, guint handler_id);

/* tag emission utility functions */
void			gst_element_found_tags_for_pad	(GstElement * element,
							 GstPad * pad,
							 GstTagList * list);
void			gst_element_found_tags		(GstElement * element,
							 GstTagList * list);

/* parse utility functions */
GstElement *            gst_parse_bin_from_description      (const gchar     * bin_description,
                                                             gboolean          ghost_unlinked_pads,
                                                             GError         ** err);

GstElement *            gst_parse_bin_from_description_full (const gchar     * bin_description,
                                                             gboolean          ghost_unlinked_pads,
                                                             GstParseContext * context,
                                                             GstParseFlags     flags,
                                                             GError         ** err);

GstClockTime            gst_util_get_timestamp          (void);

/**
 * GstSearchMode:
 * @GST_SEARCH_MODE_EXACT : Only search for exact matches.
 * @GST_SEARCH_MODE_BEFORE: Search for an exact match or the element just before.
 * @GST_SEARCH_MODE_AFTER : Search for an exact match or the element just after.
 *
 * The different search modes.
 *
 * Since: 0.10.23
 */
typedef enum {
  GST_SEARCH_MODE_EXACT = 0,
  GST_SEARCH_MODE_BEFORE,
  GST_SEARCH_MODE_AFTER
} GstSearchMode;

gpointer                gst_util_array_binary_search      (gpointer array, guint num_elements,
                                                           gsize element_size, GCompareDataFunc search_func,
							   GstSearchMode mode, gconstpointer search_data,
							   gpointer user_data);

gint gst_util_greatest_common_divisor (gint a, gint b);
void gst_util_fraction_to_double (gint src_n, gint src_d, gdouble *dest);
void gst_util_double_to_fraction (gdouble src, gint *dest_n, gint *dest_d);
gboolean gst_util_fraction_multiply (gint a_n, gint a_d, gint b_n, gint b_d, gint *res_n, gint *res_d);
gboolean gst_util_fraction_add (gint a_n, gint a_d, gint b_n, gint b_d, gint *res_n, gint *res_d);
gint gst_util_fraction_compare (gint a_n, gint a_d, gint b_n, gint b_d);


/* sink message event
 *
 * FIXME: This should be in gstevent.h but can't because
 * it needs GstMessage and this would introduce circular
 * header includes. And forward declarations of typedefs
 * are unfortunately not possible. The implementation of
 * these functions is in gstevent.c.
 */
GstEvent*       gst_event_new_sink_message      (GstMessage *msg);
void            gst_event_parse_sink_message    (GstEvent *event, GstMessage **msg);


G_END_DECLS

#endif /* __GST_UTILS_H__ */
