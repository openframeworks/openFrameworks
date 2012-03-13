/* GStreamer byte writer
 *
 * Copyright (C) 2009 Sebastian Dröge <sebastian.droege@collabora.co.uk>.
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

#ifndef __GST_BYTE_WRITER_H__
#define __GST_BYTE_WRITER_H__

#include <gst/gst.h>
#include <gst/base/gstbytereader.h>

#include <string.h>

G_BEGIN_DECLS

#define GST_BYTE_WRITER(writer) ((GstByteWriter *) (writer))

/**
 * GstByteWriter:
 * @parent: #GstByteReader parent
 * @alloc_size: Allocation size of the data
 * @fixed: If %TRUE no reallocations are allowed
 * @owned: If %FALSE no reallocations are allowed and copies of data are returned
 *
 * A byte writer instance.
 */
typedef struct {
  GstByteReader parent;

  guint alloc_size;

  gboolean fixed;
  gboolean owned;
} GstByteWriter;

GstByteWriter * gst_byte_writer_new (void);
GstByteWriter * gst_byte_writer_new_with_size (guint size, gboolean fixed);
GstByteWriter * gst_byte_writer_new_with_data (guint8 *data, guint size, gboolean initialized);
GstByteWriter * gst_byte_writer_new_with_buffer (GstBuffer *buffer, gboolean initialized);

void gst_byte_writer_init (GstByteWriter *writer);
void gst_byte_writer_init_with_size (GstByteWriter *writer, guint size, gboolean fixed);
void gst_byte_writer_init_with_data (GstByteWriter *writer, guint8 *data, guint size, gboolean initialized);
void gst_byte_writer_init_with_buffer (GstByteWriter *writer, GstBuffer *buffer, gboolean initialized);

void gst_byte_writer_free (GstByteWriter *writer);
guint8 * gst_byte_writer_free_and_get_data (GstByteWriter *writer);
GstBuffer *gst_byte_writer_free_and_get_buffer (GstByteWriter *writer);

void gst_byte_writer_reset (GstByteWriter *writer);
guint8 * gst_byte_writer_reset_and_get_data (GstByteWriter *writer);
GstBuffer *gst_byte_writer_reset_and_get_buffer (GstByteWriter *writer);

/**
 * gst_byte_writer_get_pos:
 * @writer: #GstByteWriter instance
 *
 * Returns: The current position of the read/write cursor
 *
 * Since: 0.10.26
 */
/**
 * gst_byte_writer_set_pos:
 * @writer: #GstByteWriter instance
 * @pos: new position
 *
 * Sets the current read/write cursor of @writer. The new position
 * can only be between 0 and the current size.
 *
 * Returns: %TRUE if the new position could be set
 *
 * Since: 0.10.26
 */
/**
 * gst_byte_writer_get_size:
 * @writer: #GstByteWriter instance
 *
 * Returns: The current, initialized size of the data
 *
 * Since: 0.10.26
 */
#ifdef _FOOL_GTK_DOC_
G_INLINE_FUNC guint gst_byte_writer_get_pos (const GstByteWriter *writer);
G_INLINE_FUNC gboolean gst_byte_writer_set_pos (GstByteWriter *writer, guint pos);
G_INLINE_FUNC guint gst_byte_writer_get_size (const GstByteWriter *writer);
#else
static inline guint
gst_byte_writer_get_pos (const GstByteWriter *writer)
{
  return gst_byte_reader_get_pos ((const GstByteReader *) writer);
}

static inline gboolean
gst_byte_writer_set_pos (GstByteWriter *writer, guint pos)
{
  return gst_byte_reader_set_pos (GST_BYTE_READER (writer), pos);
}

static inline guint
gst_byte_writer_get_size (const GstByteWriter *writer)
{
  return gst_byte_reader_get_size ((const GstByteReader *) writer);
}
#endif

guint gst_byte_writer_get_remaining (const GstByteWriter *writer);
gboolean gst_byte_writer_ensure_free_space (GstByteWriter *writer, guint size);

gboolean gst_byte_writer_put_uint8 (GstByteWriter *writer, guint8 val);
gboolean gst_byte_writer_put_int8 (GstByteWriter *writer, gint8 val);
gboolean gst_byte_writer_put_uint16_be (GstByteWriter *writer, guint16 val);
gboolean gst_byte_writer_put_uint16_le (GstByteWriter *writer, guint16 val);
gboolean gst_byte_writer_put_int16_be (GstByteWriter *writer, gint16 val);
gboolean gst_byte_writer_put_int16_le (GstByteWriter *writer, gint16 val);
gboolean gst_byte_writer_put_uint24_be (GstByteWriter *writer, guint32 val);
gboolean gst_byte_writer_put_uint24_le (GstByteWriter *writer, guint32 val);
gboolean gst_byte_writer_put_int24_be (GstByteWriter *writer, gint32 val);
gboolean gst_byte_writer_put_int24_le (GstByteWriter *writer, gint32 val);
gboolean gst_byte_writer_put_uint32_be (GstByteWriter *writer, guint32 val);
gboolean gst_byte_writer_put_uint32_le (GstByteWriter *writer, guint32 val);
gboolean gst_byte_writer_put_int32_be (GstByteWriter *writer, gint32 val);
gboolean gst_byte_writer_put_int32_le (GstByteWriter *writer, gint32 val);
gboolean gst_byte_writer_put_uint64_be (GstByteWriter *writer, guint64 val);
gboolean gst_byte_writer_put_uint64_le (GstByteWriter *writer, guint64 val);
gboolean gst_byte_writer_put_int64_be (GstByteWriter *writer, gint64 val);
gboolean gst_byte_writer_put_int64_le (GstByteWriter *writer, gint64 val);

gboolean gst_byte_writer_put_float32_be (GstByteWriter *writer, gfloat val);
gboolean gst_byte_writer_put_float32_le (GstByteWriter *writer, gfloat val);
gboolean gst_byte_writer_put_float64_be (GstByteWriter *writer, gdouble val);
gboolean gst_byte_writer_put_float64_le (GstByteWriter *writer, gdouble val);

gboolean gst_byte_writer_put_data (GstByteWriter *writer, const guint8 *data, guint size);
gboolean gst_byte_writer_fill (GstByteWriter *writer, guint8 value, guint size);
gboolean gst_byte_writer_put_string_utf8 (GstByteWriter *writer, const gchar *data);
gboolean gst_byte_writer_put_string_utf16 (GstByteWriter *writer, const guint16 *data);
gboolean gst_byte_writer_put_string_utf32 (GstByteWriter *writer, const guint32 *data);

/**
 * gst_byte_writer_put_string:
 * @writer: #GstByteWriter instance
 * @data: Null terminated string
 *
 * Write a NUL-terminated string to @writer (including the terminator). The
 * string is assumed to be in an 8-bit encoding (e.g. ASCII,UTF-8 or
 * ISO-8859-1).
 *
 * Returns: %TRUE if the string could be written
 *
 * Since: 0.10.26
 */
#define gst_byte_writer_put_string(writer, data) \
  gst_byte_writer_put_string_utf8(writer, data)

static inline guint
_gst_byte_writer_next_pow2 (guint n)
{
  guint ret = 16;

  /* We start with 16, smaller allocations make no sense */

  while (ret < n && ret > 0)
    ret <<= 1;

  return ret ? ret : n;
}

static inline gboolean
_gst_byte_writer_ensure_free_space_inline (GstByteWriter * writer, guint size)
{
  guint8 *data;

  if (G_LIKELY (size <= writer->alloc_size - writer->parent.byte))
    return TRUE;
  if (G_UNLIKELY (writer->fixed || !writer->owned))
    return FALSE;
  if (G_UNLIKELY (writer->parent.byte > G_MAXUINT - size))
    return FALSE;

  writer->alloc_size = _gst_byte_writer_next_pow2 (writer->parent.byte + size);
  data = g_try_realloc ((guint8 *) writer->parent.data, writer->alloc_size);
  if (G_UNLIKELY (data == NULL))
    return FALSE;

  writer->parent.data = data;

  return TRUE;
}

#define __GST_BYTE_WRITER_CREATE_WRITE_FUNC(bits,type,name,write_func) \
static inline void \
gst_byte_writer_put_##name##_unchecked (GstByteWriter *writer, type val) \
{ \
  guint8 *write_data; \
  \
  write_data = (guint8 *) writer->parent.data + writer->parent.byte; \
  write_func (write_data, val); \
  writer->parent.byte += bits/8; \
  writer->parent.size = MAX (writer->parent.size, writer->parent.byte); \
} \
\
static inline gboolean \
_gst_byte_writer_put_##name##_inline (GstByteWriter *writer, type val) \
{ \
  g_return_val_if_fail (writer != NULL, FALSE); \
  \
  if (G_UNLIKELY (!_gst_byte_writer_ensure_free_space_inline(writer, bits/8))) \
    return FALSE; \
  \
  gst_byte_writer_put_##name##_unchecked (writer, val); \
  \
  return TRUE; \
}

__GST_BYTE_WRITER_CREATE_WRITE_FUNC (8, guint8, uint8, GST_WRITE_UINT8)
__GST_BYTE_WRITER_CREATE_WRITE_FUNC (8, gint8, int8, GST_WRITE_UINT8)
__GST_BYTE_WRITER_CREATE_WRITE_FUNC (16, guint16, uint16_le, GST_WRITE_UINT16_LE)
__GST_BYTE_WRITER_CREATE_WRITE_FUNC (16, guint16, uint16_be, GST_WRITE_UINT16_BE)
__GST_BYTE_WRITER_CREATE_WRITE_FUNC (16, gint16, int16_le, GST_WRITE_UINT16_LE)
__GST_BYTE_WRITER_CREATE_WRITE_FUNC (16, gint16, int16_be, GST_WRITE_UINT16_BE)
__GST_BYTE_WRITER_CREATE_WRITE_FUNC (24, guint32, uint24_le, GST_WRITE_UINT24_LE)
__GST_BYTE_WRITER_CREATE_WRITE_FUNC (24, guint32, uint24_be, GST_WRITE_UINT24_BE)
__GST_BYTE_WRITER_CREATE_WRITE_FUNC (24, gint32, int24_le, GST_WRITE_UINT24_LE)
__GST_BYTE_WRITER_CREATE_WRITE_FUNC (24, gint32, int24_be, GST_WRITE_UINT24_BE)
__GST_BYTE_WRITER_CREATE_WRITE_FUNC (32, guint32, uint32_le, GST_WRITE_UINT32_LE)
__GST_BYTE_WRITER_CREATE_WRITE_FUNC (32, guint32, uint32_be, GST_WRITE_UINT32_BE)
__GST_BYTE_WRITER_CREATE_WRITE_FUNC (32, gint32, int32_le, GST_WRITE_UINT32_LE)
__GST_BYTE_WRITER_CREATE_WRITE_FUNC (32, gint32, int32_be, GST_WRITE_UINT32_BE)
__GST_BYTE_WRITER_CREATE_WRITE_FUNC (64, guint64, uint64_le, GST_WRITE_UINT64_LE)
__GST_BYTE_WRITER_CREATE_WRITE_FUNC (64, guint64, uint64_be, GST_WRITE_UINT64_BE)
__GST_BYTE_WRITER_CREATE_WRITE_FUNC (64, gint64, int64_le, GST_WRITE_UINT64_LE)
__GST_BYTE_WRITER_CREATE_WRITE_FUNC (64, gint64, int64_be, GST_WRITE_UINT64_BE)

__GST_BYTE_WRITER_CREATE_WRITE_FUNC (32, gfloat, float32_be, GST_WRITE_FLOAT_BE)
__GST_BYTE_WRITER_CREATE_WRITE_FUNC (32, gfloat, float32_le, GST_WRITE_FLOAT_LE)
__GST_BYTE_WRITER_CREATE_WRITE_FUNC (64, gdouble, float64_be, GST_WRITE_DOUBLE_BE)
__GST_BYTE_WRITER_CREATE_WRITE_FUNC (64, gdouble, float64_le, GST_WRITE_DOUBLE_LE)

#undef __GST_BYTE_WRITER_CREATE_WRITE_FUNC

static inline void
gst_byte_writer_put_data_unchecked (GstByteWriter * writer, const guint8 * data,
    guint size)
{
  memcpy ((guint8 *) & writer->parent.data[writer->parent.byte], data, size);
  writer->parent.byte += size;
  writer->parent.size = MAX (writer->parent.size, writer->parent.byte);
}

static inline gboolean
_gst_byte_writer_put_data_inline (GstByteWriter * writer, const guint8 * data,
    guint size)
{
  g_return_val_if_fail (writer != NULL, FALSE);

  if (G_UNLIKELY (!_gst_byte_writer_ensure_free_space_inline (writer, size)))
    return FALSE;

  gst_byte_writer_put_data_unchecked (writer, data, size);

  return TRUE;
}

static inline void
gst_byte_writer_fill_unchecked (GstByteWriter * writer, guint8 value, guint size)
{
  memset ((guint8 *) & writer->parent.data[writer->parent.byte], value, size);
  writer->parent.byte += size;
  writer->parent.size = MAX (writer->parent.size, writer->parent.byte);
}

static inline gboolean
_gst_byte_writer_fill_inline (GstByteWriter * writer, guint8 value, guint size)
{
  g_return_val_if_fail (writer != NULL, FALSE);

  if (G_UNLIKELY (!_gst_byte_writer_ensure_free_space_inline (writer, size)))
    return FALSE;

  gst_byte_writer_fill_unchecked (writer, value, size);

  return TRUE;
}

#ifndef GST_BYTE_WRITER_DISABLE_INLINES

/* we use defines here so we can add the G_LIKELY() */

#define gst_byte_writer_ensure_free_space(writer, size) \
    G_LIKELY (_gst_byte_writer_ensure_free_space_inline (writer, size))
#define gst_byte_writer_put_uint8(writer, val) \
    G_LIKELY (_gst_byte_writer_put_uint8_inline (writer, val))
#define gst_byte_writer_put_int8(writer, val) \
    G_LIKELY (_gst_byte_writer_put_int8_inline (writer, val))
#define gst_byte_writer_put_uint16_be(writer, val) \
    G_LIKELY (_gst_byte_writer_put_uint16_be_inline (writer, val))
#define gst_byte_writer_put_uint16_le(writer, val) \
    G_LIKELY (_gst_byte_writer_put_uint16_le_inline (writer, val))
#define gst_byte_writer_put_int16_be(writer, val) \
    G_LIKELY (_gst_byte_writer_put_int16_be_inline (writer, val))
#define gst_byte_writer_put_int16_le(writer, val) \
    G_LIKELY (_gst_byte_writer_put_int16_le_inline (writer, val))
#define gst_byte_writer_put_uint24_be(writer, val) \
    G_LIKELY (_gst_byte_writer_put_uint24_be_inline (writer, val))
#define gst_byte_writer_put_uint24_le(writer, val) \
    G_LIKELY (_gst_byte_writer_put_uint24_le_inline (writer, val))
#define gst_byte_writer_put_int24_be(writer, val) \
    G_LIKELY (_gst_byte_writer_put_int24_be_inline (writer, val))
#define gst_byte_writer_put_int24_le(writer, val) \
    G_LIKELY (_gst_byte_writer_put_int24_le_inline (writer, val))
#define gst_byte_writer_put_uint32_be(writer, val) \
    G_LIKELY (_gst_byte_writer_put_uint32_be_inline (writer, val))
#define gst_byte_writer_put_uint32_le(writer, val) \
    G_LIKELY (_gst_byte_writer_put_uint32_le_inline (writer, val))
#define gst_byte_writer_put_int32_be(writer, val) \
    G_LIKELY (_gst_byte_writer_put_int32_be_inline (writer, val))
#define gst_byte_writer_put_int32_le(writer, val) \
    G_LIKELY (_gst_byte_writer_put_int32_le_inline (writer, val))
#define gst_byte_writer_put_uint64_be(writer, val) \
    G_LIKELY (_gst_byte_writer_put_uint64_be_inline (writer, val))
#define gst_byte_writer_put_uint64_le(writer, val) \
    G_LIKELY (_gst_byte_writer_put_uint64_le_inline (writer, val))
#define gst_byte_writer_put_int64_be(writer, val) \
    G_LIKELY (_gst_byte_writer_put_int64_be_inline (writer, val))
#define gst_byte_writer_put_int64_le(writer, val) \
    G_LIKELY (_gst_byte_writer_put_int64_le_inline (writer, val))

#define gst_byte_writer_put_float32_be(writer, val) \
    G_LIKELY (_gst_byte_writer_put_float32_be_inline (writer, val))
#define gst_byte_writer_put_float32_le(writer, val) \
    G_LIKELY (_gst_byte_writer_put_float32_le_inline (writer, val))
#define gst_byte_writer_put_float64_be(writer, val) \
    G_LIKELY (_gst_byte_writer_put_float64_be_inline (writer, val))
#define gst_byte_writer_put_float64_le(writer, val) \
    G_LIKELY (_gst_byte_writer_put_float64_le_inline (writer, val))

#define gst_byte_writer_put_data(writer, data, size) \
    G_LIKELY (_gst_byte_writer_put_data_inline (writer, data, size))
#define gst_byte_writer_fill(writer, val, size) \
    G_LIKELY (_gst_byte_writer_fill_inline (writer, val, size))

#endif

G_END_DECLS

#endif /* __GST_BYTE_WRITER_H__ */
