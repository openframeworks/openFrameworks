/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wim.taymans@chello.be>
 *
 * gstindex.h: Header for GstIndex, base class to handle efficient
 *             storage or caching of seeking information.
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

#ifndef __GST_INDEX_H__
#define __GST_INDEX_H__

#include <gst/gstobject.h>
#include <gst/gstformat.h>
#include <gst/gstpluginfeature.h>

G_BEGIN_DECLS

#define GST_TYPE_INDEX                  (gst_index_get_type ())
#define GST_INDEX(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_INDEX, GstIndex))
#define GST_IS_INDEX(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_INDEX))
#define GST_INDEX_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_INDEX, GstIndexClass))
#define GST_IS_INDEX_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_INDEX))
#define GST_INDEX_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_INDEX, GstIndexClass))

#define GST_TYPE_INDEX_ENTRY            (gst_index_entry_get_type())

typedef struct _GstIndexEntry GstIndexEntry;
typedef struct _GstIndexGroup GstIndexGroup;
typedef struct _GstIndex GstIndex;
typedef struct _GstIndexClass GstIndexClass;

/**
 * GstIndexCertainty:
 * @GST_INDEX_UNKNOWN: accuracy is not known
 * @GST_INDEX_CERTAIN: accuracy is perfect
 * @GST_INDEX_FUZZY: accuracy is fuzzy
 *
 * The certainty of a group in the index.
 */
typedef enum {
  GST_INDEX_UNKNOWN,
  GST_INDEX_CERTAIN,
  GST_INDEX_FUZZY
} GstIndexCertainty;

/**
 * GstIndexEntryType:
 * @GST_INDEX_ENTRY_ID: This entry is an id that maps an index id to its owner object
 * @GST_INDEX_ENTRY_ASSOCIATION: This entry is an association between formats
 * @GST_INDEX_ENTRY_OBJECT: An object
 * @GST_INDEX_ENTRY_FORMAT: A format definition
 *
 * The different types of entries in the index.
 */
typedef enum {
  GST_INDEX_ENTRY_ID,
  GST_INDEX_ENTRY_ASSOCIATION,
  GST_INDEX_ENTRY_OBJECT,
  GST_INDEX_ENTRY_FORMAT
} GstIndexEntryType;

/**
 * GstIndexLookupMethod:
 * @GST_INDEX_LOOKUP_EXACT: There has to be an exact indexentry with the given format/value
 * @GST_INDEX_LOOKUP_BEFORE: The exact entry or the one before it
 * @GST_INDEX_LOOKUP_AFTER: The exact entry or the one after it
 *
 * Specify the method to find an index entry in the index.
 */
typedef enum {
  GST_INDEX_LOOKUP_EXACT,
  GST_INDEX_LOOKUP_BEFORE,
  GST_INDEX_LOOKUP_AFTER
} GstIndexLookupMethod;

/**
 * GST_INDEX_NASSOCS:
 * @entry: The entry to query
 *
 * Get the number of associations in the entry.
 */
#define GST_INDEX_NASSOCS(entry)                ((entry)->data.assoc.nassocs)

/**
 * GST_INDEX_ASSOC_FLAGS:
 * @entry: The entry to query
 *
 *  Get the flags for this entry.
 */
#define GST_INDEX_ASSOC_FLAGS(entry)            ((entry)->data.assoc.flags)

/**
 * GST_INDEX_ASSOC_FORMAT:
 * @entry: The entry to query
 * @i: The format index
 *
 * Get the i-th format of the entry.
 */
#define GST_INDEX_ASSOC_FORMAT(entry,i)         ((entry)->data.assoc.assocs[(i)].format)

/**
 * GST_INDEX_ASSOC_VALUE:
 * @entry: The entry to query
 * @i: The value index
 *
 * Get the i-th value of the entry.
 */
#define GST_INDEX_ASSOC_VALUE(entry,i)          ((entry)->data.assoc.assocs[(i)].value)

typedef struct _GstIndexAssociation GstIndexAssociation;

/**
 * GstIndexAssociation:
 * @format: the format of the association
 * @value: the value of the association
 *
 * An association in an entry.
 */
struct _GstIndexAssociation {
  GstFormat     format;
  gint64        value;
};

/**
 * GstAssocFlags:
 * @GST_ASSOCIATION_FLAG_NONE: no extra flags
 * @GST_ASSOCIATION_FLAG_KEY_UNIT: the entry marks a key unit, a key unit is one
 *  that marks a place where one can randomly seek to.
 * @GST_ASSOCIATION_FLAG_DELTA_UNIT: the entry marks a delta unit, a delta unit
 *  is one that marks a place where one can relatively seek to.
 * @GST_ASSOCIATION_FLAG_LAST: extra user defined flags should start here.
 *
 * Flags for an association entry.
 */
typedef enum {
  GST_ASSOCIATION_FLAG_NONE       = 0,
  GST_ASSOCIATION_FLAG_KEY_UNIT   = (1 << 0),
  GST_ASSOCIATION_FLAG_DELTA_UNIT = (1 << 1),

  /* new flags should start here */
  GST_ASSOCIATION_FLAG_LAST     = (1 << 8)
} GstAssocFlags;

/**
 * GST_INDEX_FORMAT_FORMAT:
 * @entry: The entry to query
 *
 * Get the format of the format entry
 */
#define GST_INDEX_FORMAT_FORMAT(entry)          ((entry)->data.format.format)

/**
 * GST_INDEX_FORMAT_KEY:
 * @entry: The entry to query
 *
 * Get the key of the format entry
 */
#define GST_INDEX_FORMAT_KEY(entry)             ((entry)->data.format.key)

/**
 * GST_INDEX_ID_INVALID:
 *
 * Constant for an invalid index id
 */
#define GST_INDEX_ID_INVALID                    (-1)

/**
 * GST_INDEX_ID_DESCRIPTION:
 * @entry: The entry to query
 *
 * Get the description of the id entry
 */
#define GST_INDEX_ID_DESCRIPTION(entry)         ((entry)->data.id.description)

/**
 * GstIndexEntry:
 *
 * The basic element of an index.
 */
struct _GstIndexEntry {
  /*< private >*/
  GstIndexEntryType      type;
  gint                   id;

  union {
    struct {
      gchar             *description;
    } id;
    struct {
      gint               nassocs;
      GstIndexAssociation
                        *assocs;
      GstAssocFlags      flags;
    } assoc;
    struct {
      gchar             *key;
      GType              type;
      gpointer           object;
    } object;
    struct {
      GstFormat          format;
      const gchar       *key;
    } format;
  } data;
};

/**
 * GstIndexGroup:
 *
 * A group of related entries in an index.
 */

struct _GstIndexGroup {
  /*< private >*/
  /* unique ID of group in index */
  gint groupnum;

  /* list of entries */
  GList *entries;

  /* the certainty level of the group */
  GstIndexCertainty certainty;

  /* peer group that contains more certain entries */
  gint peergroup;
};

/**
 * GstIndexFilter:
 * @index: The index being queried
 * @entry: The entry to be added.
 * @user_data: User data passed to the function.
 *
 * Function to filter out entries in the index.
 *
 * Returns: This function should return %TRUE if the entry is to be added
 * to the index, %FALSE otherwise.
 *
 */
typedef gboolean        (*GstIndexFilter)               (GstIndex *index,
                                                         GstIndexEntry *entry,
                                                         gpointer user_data);
/**
 * GstIndexResolverMethod:
 * @GST_INDEX_RESOLVER_CUSTOM: Use a custom resolver
 * @GST_INDEX_RESOLVER_GTYPE: Resolve based on the GType of the object
 * @GST_INDEX_RESOLVER_PATH: Resolve on the path in graph
 *
 * The method used to resolve index writers
 */
typedef enum {
  GST_INDEX_RESOLVER_CUSTOM,
  GST_INDEX_RESOLVER_GTYPE,
  GST_INDEX_RESOLVER_PATH
} GstIndexResolverMethod;

/**
 * GstIndexResolver:
 * @index: the index being queried.
 * @writer: The object that wants to write
 * @writer_string: A description of the writer.
 * @user_data: user_data as registered
 *
 * Function to resolve ids to writer descriptions.
 *
 * Returns: %TRUE if an id could be assigned to the writer.
 */
typedef gboolean        (*GstIndexResolver)             (GstIndex *index,
                                                         GstObject *writer,
                                                         gchar **writer_string,
                                                         gpointer user_data);

/**
 * GstIndexFlags:
 * @GST_INDEX_WRITABLE: The index is writable
 * @GST_INDEX_READABLE: The index is readable
 * @GST_INDEX_FLAG_LAST: First flag that can be used by subclasses
 *
 * Flags for this index
 */
typedef enum {
  GST_INDEX_WRITABLE    = (GST_OBJECT_FLAG_LAST << 0),
  GST_INDEX_READABLE    = (GST_OBJECT_FLAG_LAST << 1),

  GST_INDEX_FLAG_LAST   = (GST_OBJECT_FLAG_LAST << 8)
} GstIndexFlags;

/**
 * GST_INDEX_IS_READABLE:
 * @obj: The index to check
 *
 * Check if the index can be read from
 */
#define GST_INDEX_IS_READABLE(obj)    (GST_OBJECT_FLAG_IS_SET (obj, GST_INDEX_READABLE))

/**
 * GST_INDEX_IS_WRITABLE:
 * @obj: The index to check
 *
 * Check if the index can be written to
 */
#define GST_INDEX_IS_WRITABLE(obj)    (GST_OBJECT_FLAG_IS_SET (obj, GST_INDEX_WRITABLE))

/**
 * GstIndex:
 * 
 * Opaque #GstIndex structure.
 */
struct _GstIndex {
  GstObject              object;

  /*< private >*/
  GList                 *groups;
  GstIndexGroup         *curgroup;
  gint                   maxgroup;

  GstIndexResolverMethod method;
  GstIndexResolver       resolver;
  gpointer               resolver_user_data;

  GstIndexFilter         filter;
  gpointer               filter_user_data;
  GDestroyNotify         filter_user_data_destroy;

  GHashTable            *writers;
  gint                   last_id;

  /* ABI added since 0.10.18 */
  GDestroyNotify         resolver_user_data_destroy;

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING - 1];
};

struct _GstIndexClass {
  GstObjectClass parent_class;

  /*< protected >*/
  gboolean      (*get_writer_id)        (GstIndex *index, gint *writer_id, gchar *writer_string);

  void          (*commit)               (GstIndex *index, gint id);

  /* abstract methods */
  void          (*add_entry)            (GstIndex *index, GstIndexEntry *entry);

  GstIndexEntry* (*get_assoc_entry)     (GstIndex *index, gint id,
                                         GstIndexLookupMethod method, GstAssocFlags flags,
                                         GstFormat format, gint64 value,
                                         GCompareDataFunc func,
                                         gpointer user_data);
  /* signals */
  void          (*entry_added)          (GstIndex *index, GstIndexEntry *entry);

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
};

GType                   gst_index_get_type              (void);
GstIndex*               gst_index_new                   (void);
void                    gst_index_commit                (GstIndex *index, gint id);

gint                    gst_index_get_group             (GstIndex *index);
gint                    gst_index_new_group             (GstIndex *index);
gboolean                gst_index_set_group             (GstIndex *index, gint groupnum);

void                    gst_index_set_certainty         (GstIndex *index,
                                                         GstIndexCertainty certainty);
GstIndexCertainty       gst_index_get_certainty         (GstIndex *index);

void                    gst_index_set_filter            (GstIndex *index,
                                                         GstIndexFilter filter, gpointer user_data);
void                    gst_index_set_filter_full       (GstIndex *index,
                                                         GstIndexFilter filter, gpointer user_data,
                                                         GDestroyNotify user_data_destroy);
void                    gst_index_set_resolver          (GstIndex *index,
                                                         GstIndexResolver resolver, gpointer user_data);
void                    gst_index_set_resolver_full     (GstIndex *index, GstIndexResolver resolver,
                                                         gpointer user_data,
                                                         GDestroyNotify user_data_destroy);

gboolean                gst_index_get_writer_id         (GstIndex *index, GstObject *writer, gint *id);

GstIndexEntry*          gst_index_add_format            (GstIndex *index, gint id, GstFormat format);
GstIndexEntry*          gst_index_add_associationv      (GstIndex * index, gint id, GstAssocFlags flags,
                                                         gint n, const GstIndexAssociation * list);
GstIndexEntry*          gst_index_add_association       (GstIndex *index, gint id, GstAssocFlags flags,
                                                         GstFormat format, gint64 value, ...);
GstIndexEntry*          gst_index_add_object            (GstIndex *index, gint id, gchar *key,
                                                         GType type, gpointer object);
GstIndexEntry*          gst_index_add_id                (GstIndex *index, gint id,
                                                         gchar *description);

GstIndexEntry*          gst_index_get_assoc_entry       (GstIndex *index, gint id,
                                                         GstIndexLookupMethod method, GstAssocFlags flags,
                                                         GstFormat format, gint64 value);
GstIndexEntry*          gst_index_get_assoc_entry_full  (GstIndex *index, gint id,
                                                         GstIndexLookupMethod method, GstAssocFlags flags,
                                                         GstFormat format, gint64 value,
                                                         GCompareDataFunc func,
                                                         gpointer user_data);

/* working with index entries */
GType gst_index_entry_get_type (void);
GstIndexEntry *         gst_index_entry_copy            (GstIndexEntry *entry);
void                    gst_index_entry_free            (GstIndexEntry *entry);
gboolean                gst_index_entry_assoc_map       (GstIndexEntry *entry,
                                                         GstFormat format, gint64 *value);

G_END_DECLS

#endif /* __GST_INDEX_H__ */
