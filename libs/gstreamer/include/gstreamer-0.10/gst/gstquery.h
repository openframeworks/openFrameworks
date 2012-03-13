/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wim.taymans@chello.be>
 *                    2005 Wim Taymans <wim@fluendo.com>
 *
 * gstquery.h: GstQuery API declaration
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


#ifndef __GST_QUERY_H__
#define __GST_QUERY_H__

#include <glib.h>

#include <gst/gstiterator.h>
#include <gst/gstminiobject.h>
#include <gst/gststructure.h>
#include <gst/gstformat.h>

G_BEGIN_DECLS

/**
 * GstQueryType:
 * @GST_QUERY_NONE: invalid query type
 * @GST_QUERY_POSITION: current position in stream
 * @GST_QUERY_DURATION: total duration of the stream
 * @GST_QUERY_LATENCY: latency of stream
 * @GST_QUERY_JITTER: current jitter of stream
 * @GST_QUERY_RATE: current rate of the stream
 * @GST_QUERY_SEEKING: seeking capabilities
 * @GST_QUERY_SEGMENT: segment start/stop positions
 * @GST_QUERY_CONVERT: convert values between formats
 * @GST_QUERY_FORMATS: query supported formats for convert
 * @GST_QUERY_BUFFERING: query available media for efficient seeking. Since
 * 0.10.20.
 * @GST_QUERY_CUSTOM: a custom application or element defined query. Since
 * 0.10.22.
 * @GST_QUERY_URI: query the URI of the source or sink. Since 0.10.22.
 *
 * Standard predefined Query types
 */
/* NOTE: don't forget to update the table in gstquery.c when changing
 * this enum */
typedef enum {
  GST_QUERY_NONE = 0,
  GST_QUERY_POSITION,
  GST_QUERY_DURATION,
  GST_QUERY_LATENCY,
  GST_QUERY_JITTER,     /* not in draft-query, necessary? */
  GST_QUERY_RATE,
  GST_QUERY_SEEKING,
  GST_QUERY_SEGMENT,
  GST_QUERY_CONVERT,
  GST_QUERY_FORMATS,
  GST_QUERY_BUFFERING,
  GST_QUERY_CUSTOM,
  GST_QUERY_URI
} GstQueryType;

/**
 * GstBufferingMode:
 * @GST_BUFFERING_STREAM: a small amount of data is buffered
 * @GST_BUFFERING_DOWNLOAD: the stream is being downloaded
 * @GST_BUFFERING_TIMESHIFT: the stream is being downloaded in a ringbuffer
 * @GST_BUFFERING_LIVE: the stream is a live stream
 *
 * The different types of buffering methods.
 */
typedef enum {
  GST_BUFFERING_STREAM,
  GST_BUFFERING_DOWNLOAD,
  GST_BUFFERING_TIMESHIFT,
  GST_BUFFERING_LIVE
} GstBufferingMode;

typedef struct _GstQueryTypeDefinition GstQueryTypeDefinition;
typedef struct _GstQuery GstQuery;
typedef struct _GstQueryClass GstQueryClass;

/**
 * GstQueryTypeDefinition:
 * @value: the unique id of the Query type
 * @nick: a short nick
 * @description: a longer description of the query type
 * @quark: the quark for the nick
 *
 * A Query Type definition
 */
struct _GstQueryTypeDefinition
{
  GstQueryType   value;
  const gchar   *nick;
  const gchar   *description;
  GQuark         quark;
};

#define GST_TYPE_QUERY                          (gst_query_get_type())
#define GST_IS_QUERY(obj)                      (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_QUERY))
#define GST_IS_QUERY_CLASS(klass)              (G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_QUERY))
#define GST_QUERY_GET_CLASS(obj)               (G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_QUERY, GstQueryClass))
#define GST_QUERY(obj)                         (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_QUERY, GstQuery))
#define GST_QUERY_CAST(obj)                    ((GstQuery*)(obj)) /* only since 0.10.23 */
#define GST_QUERY_CLASS(klass)                 (G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_QUERY, GstQueryClass))


/**
 * GST_QUERY_TYPE:
 * @query: the query to query
 *
 * Get the #GstQueryType of the query.
 */
#define GST_QUERY_TYPE(query)  (((GstQuery*)(query))->type)

/**
 * GST_QUERY_TYPE_NAME:
 * @query: the query to query
 *
 * Get a constant string representation of the #GstQueryType of the query.
 *
 * Since: 0.10.4
 */
#define GST_QUERY_TYPE_NAME(query) (gst_query_type_get_name(GST_QUERY_TYPE(query)))


/**
 * GstQuery:
 * @mini_object: The parent #GstMiniObject type
 * @type: the #GstQueryType
 * @structure: the #GstStructure containing the query details.
 *
 * The #GstQuery structure.
 */
struct _GstQuery
{
  GstMiniObject mini_object;

  /*< public > *//* with COW */
  GstQueryType type;

  GstStructure *structure;

  /*< private >*/
  gpointer _gst_reserved;
};

struct _GstQueryClass {
  GstMiniObjectClass mini_object_class;

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
};

const gchar*    gst_query_type_get_name        (GstQueryType query);
GQuark          gst_query_type_to_quark        (GstQueryType query);

GType           gst_query_get_type             (void);

/* register a new query */
GstQueryType    gst_query_type_register        (const gchar *nick,
                                                const gchar *description);
GstQueryType    gst_query_type_get_by_nick     (const gchar *nick);

/* check if a query is in an array of querys */
gboolean        gst_query_types_contains       (const GstQueryType *types,
                                                GstQueryType type);

/* query for query details */

G_CONST_RETURN GstQueryTypeDefinition*
                gst_query_type_get_details         (GstQueryType type);
GstIterator*    gst_query_type_iterate_definitions (void);

/* refcounting */
/**
 * gst_query_ref:
 * @q: a #GstQuery to increase the refcount of.
 *
 * Increases the refcount of the given query by one.
 *
 * Returns: @q
 */
#ifdef _FOOL_GTK_DOC_
G_INLINE_FUNC GstQuery * gst_query_ref (GstQuery * q);
#endif

static inline GstQuery *
gst_query_ref (GstQuery * q)
{
  return GST_QUERY_CAST (gst_mini_object_ref (GST_MINI_OBJECT_CAST (q)));
}

/**
 * gst_query_unref:
 * @q: a #GstQuery to decrease the refcount of.
 *
 * Decreases the refcount of the query. If the refcount reaches 0, the query
 * will be freed.
 */
#ifdef _FOOL_GTK_DOC_
G_INLINE_FUNC void gst_query_unref (GstQuery * q);
#endif

static inline void
gst_query_unref (GstQuery * q)
{
  gst_mini_object_unref (GST_MINI_OBJECT_CAST (q));
}

/* copy query */
/**
 * gst_query_copy:
 * @q: a #GstQuery to copy.
 *
 * Copies the given query using the copy function of the parent #GstStructure.
 *
 * Returns: a new copy of @q.
 */
#ifdef _FOOL_GTK_DOC_
G_INLINE_FUNC GstQuery * gst_query_copy (const GstQuery * q);
#endif

static inline GstQuery *
gst_query_copy (const GstQuery * q)
{
  return GST_QUERY_CAST (gst_mini_object_copy (GST_MINI_OBJECT_CONST_CAST (q)));
}

/**
 * gst_query_make_writable:
 * @q: a #GstQuery to make writable
 *
 * Makes a writable query from the given query.
 */
#define         gst_query_make_writable(q)      GST_QUERY_CAST (gst_mini_object_make_writable (GST_MINI_OBJECT_CAST (q)))

/* position query */
GstQuery*       gst_query_new_position          (GstFormat format);
void            gst_query_set_position          (GstQuery *query, GstFormat format, gint64 cur);
void            gst_query_parse_position        (GstQuery *query, GstFormat *format, gint64 *cur);

/* duration query */
GstQuery*       gst_query_new_duration          (GstFormat format);
void            gst_query_set_duration          (GstQuery *query, GstFormat format, gint64 duration);
void            gst_query_parse_duration        (GstQuery *query, GstFormat *format, gint64 *duration);

/* latency query */
GstQuery*       gst_query_new_latency           (void);
void            gst_query_set_latency           (GstQuery *query, gboolean live, GstClockTime min_latency,
                                                 GstClockTime max_latency);
void            gst_query_parse_latency         (GstQuery *query, gboolean *live, GstClockTime *min_latency,
                                                 GstClockTime *max_latency);

/* convert query */
GstQuery*       gst_query_new_convert           (GstFormat src_format, gint64 value, GstFormat dest_format);
void            gst_query_set_convert           (GstQuery *query, GstFormat src_format, gint64 src_value,
                                                 GstFormat dest_format, gint64 dest_value);
void            gst_query_parse_convert         (GstQuery *query, GstFormat *src_format, gint64 *src_value,
                                                 GstFormat *dest_format, gint64 *dest_value);
/* segment query */
GstQuery*       gst_query_new_segment           (GstFormat format);
void            gst_query_set_segment           (GstQuery *query, gdouble rate, GstFormat format,
                                                 gint64 start_value, gint64 stop_value);
void            gst_query_parse_segment         (GstQuery *query, gdouble *rate, GstFormat *format,
                                                 gint64 *start_value, gint64 *stop_value);

/* application specific query */
GstQuery *      gst_query_new_application       (GstQueryType type,
                                                 GstStructure *structure);
GstStructure *  gst_query_get_structure         (GstQuery *query);

/* seeking query */
GstQuery*       gst_query_new_seeking           (GstFormat format);
void            gst_query_set_seeking           (GstQuery *query, GstFormat format,
                                                 gboolean seekable,
                                                 gint64 segment_start,
                                                 gint64 segment_end);
void            gst_query_parse_seeking         (GstQuery *query, GstFormat *format,
                                                 gboolean *seekable,
                                                 gint64 *segment_start,
                                                 gint64 *segment_end);
/* formats query */
GstQuery*       gst_query_new_formats           (void);
void            gst_query_set_formats           (GstQuery *query, gint n_formats, ...);
void            gst_query_set_formatsv          (GstQuery *query, gint n_formats, GstFormat *formats);
void            gst_query_parse_formats_length  (GstQuery *query, guint *n_formats);
void            gst_query_parse_formats_nth     (GstQuery *query, guint nth, GstFormat *format);

/* buffering query */
GstQuery*       gst_query_new_buffering           (GstFormat format);
void            gst_query_set_buffering_percent   (GstQuery *query, gboolean busy, gint percent);
void            gst_query_parse_buffering_percent (GstQuery *query, gboolean *busy, gint *percent);

void            gst_query_set_buffering_stats     (GstQuery *query, GstBufferingMode mode,
                                                   gint avg_in, gint avg_out,
                                                   gint64 buffering_left);
void            gst_query_parse_buffering_stats    (GstQuery *query, GstBufferingMode *mode,
                                                   gint *avg_in, gint *avg_out,
                                                   gint64 *buffering_left);

void            gst_query_set_buffering_range     (GstQuery *query, GstFormat format,
                                                   gint64 start, gint64 stop,
                                                   gint64 estimated_total);
void            gst_query_parse_buffering_range   (GstQuery *query, GstFormat *format,
                                                   gint64 *start, gint64 *stop,
                                                   gint64 *estimated_total);
gboolean        gst_query_add_buffering_range     (GstQuery *query,
                                                   gint64 start, gint64 stop);

guint           gst_query_get_n_buffering_ranges  (GstQuery *query);

gboolean        gst_query_parse_nth_buffering_range (GstQuery *query,
                                                     guint index, gint64 *start,
                                                     gint64 *stop);

/* URI query */
GstQuery *      gst_query_new_uri                 (void);
void            gst_query_parse_uri               (GstQuery *query, gchar **uri);
void            gst_query_set_uri                 (GstQuery *query, const gchar *uri);

G_END_DECLS

#endif /* __GST_QUERY_H__ */

