/* GStreamer
 * Copyright (C) 2009 Axis Communications <dev-gstreamer at axis dot com>
 * @author Jonas Holmberg <jonas dot holmberg at axis dot com>
 *
 * gstbufferlist.h: Header for GstBufferList object
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

#ifndef __GST_BUFFER_LIST_H__
#define __GST_BUFFER_LIST_H__

#include <gst/gstbuffer.h>

G_BEGIN_DECLS

#define GST_TYPE_BUFFER_LIST (gst_buffer_list_get_type ())
#define GST_IS_BUFFER_LIST(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_BUFFER_LIST))
#define GST_IS_BUFFER_LIST_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_BUFFER_LIST))
#define GST_BUFFER_LIST_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_BUFFER_LIST, GstBufferListClass))
#define GST_BUFFER_LIST(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_BUFFER_LIST, GstBufferList))
#define GST_BUFFER_LIST_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_BUFFER_LIST, GstBufferListClass))
#define GST_BUFFER_LIST_CAST(obj) ((GstBufferList *)obj)

typedef struct _GstBufferList GstBufferList;
typedef struct _GstBufferListClass GstBufferListClass;
typedef struct _GstBufferListIterator GstBufferListIterator;

/**
 * GstBufferListDoFunction:
 * @buffer: the #GstBuffer
 * @user_data: user data
 *
 * A function for accessing the last buffer returned by
 * gst_buffer_list_iterator_next(). The function can leave @buffer in the list,
 * replace @buffer in the list or remove @buffer from the list, depending on
 * the return value. If the function returns NULL, @buffer will be removed from
 * the list, otherwise @buffer will be replaced with the returned buffer.
 *
 * The last buffer returned by gst_buffer_list_iterator_next() will be replaced
 * with the buffer returned from the function. The function takes ownership of
 * @buffer and if a different value than @buffer is returned, @buffer must be
 * unreffed. If NULL is returned, the buffer will be removed from the list. The
 * list must be writable.
 *
 * Returns: the buffer to replace @buffer in the list, or NULL to remove @buffer
 * from the list
 *
 * Since: 0.10.24
 */
typedef GstBuffer* (*GstBufferListDoFunction) (GstBuffer * buffer, gpointer user_data);

/**
 * GstBufferListItem:
 * @GST_BUFFER_LIST_CONTINUE:   Retrieve next buffer
 * @GST_BUFFER_LIST_SKIP_GROUP: Skip to next group
 * @GST_BUFFER_LIST_END:        End iteration
 *
 * The result of the #GstBufferListFunc.
 *
 * Since: 0.10.24
 */
typedef enum {
  GST_BUFFER_LIST_CONTINUE,
  GST_BUFFER_LIST_SKIP_GROUP,
  GST_BUFFER_LIST_END
} GstBufferListItem;

/**
 * GstBufferListFunc:
 * @buffer: pointer the buffer
 * @group: the group index of @buffer
 * @idx: the index in @group of @buffer
 * @user_data: user data passed to gst_buffer_list_foreach()
 *
 * A function that will be called from gst_buffer_list_foreach(). The @buffer
 * field will point to a the reference of the buffer at @idx in @group.
 *
 * When this function returns #GST_BUFFER_LIST_CONTINUE, the next buffer will be
 * returned. When #GST_BUFFER_LIST_SKIP_GROUP is returned, all remaining buffers
 * in the current group will be skipped and the first buffer of the next group
 * is returned (if any). When GST_BUFFER_LIST_END is returned,
 * gst_buffer_list_foreach() will return.
 *
 * When @buffer is set to NULL, the item will be removed from the bufferlist.
 * When @buffer has been made writable, the new buffer reference can be assigned
 * to @buffer. This function is responsible for unreffing the old buffer when
 * removing or modifying.
 *
 * Returns: a #GstBufferListItem
 *
 * Since: 0.10.24
 */
typedef GstBufferListItem (*GstBufferListFunc)   (GstBuffer **buffer, guint group, guint idx,
                                                  gpointer user_data);


GType gst_buffer_list_get_type (void);

/* allocation */
GstBufferList *gst_buffer_list_new (void);

/* refcounting */
/**
 * gst_buffer_list_ref:
 * @list: a #GstBufferList
 *
 * Increases the refcount of the given buffer list by one.
 *
 * Note that the refcount affects the writeability of @list and its data, see
 * gst_buffer_list_make_writable(). It is important to note that keeping
 * additional references to GstBufferList instances can potentially increase
 * the number of memcpy operations in a pipeline.
 *
 * Returns: @list
 *
 * Since: 0.10.24
 */
#ifdef _FOOL_GTK_DOC_
G_INLINE_FUNC GstBufferList * gst_buffer_list_ref (GstBufferList * list);
#endif

static inline GstBufferList *
gst_buffer_list_ref (GstBufferList * list)
{
  return GST_BUFFER_LIST_CAST (gst_mini_object_ref (GST_MINI_OBJECT_CAST (
      list)));
}

/**
 * gst_buffer_list_unref:
 * @list: a #GstBufferList
 *
 * Decreases the refcount of the buffer list. If the refcount reaches 0, the
 * buffer list will be freed.
 *
 * Since: 0.10.24
 */
#ifdef _FOOL_GTK_DOC_
G_INLINE_FUNC void gst_buffer_list_unref (GstBufferList * list);
#endif

static inline void
gst_buffer_list_unref (GstBufferList * list)
{
  gst_mini_object_unref (GST_MINI_OBJECT_CAST (list));
}

/* copy */
/**
 * gst_buffer_list_copy:
 * @list: a #GstBufferList
 *
 * Create a shallow copy of the given buffer list. This will make a newly
 * allocated copy of the source list with copies of buffer pointers. The
 * refcount of buffers pointed to will be increased by one.
 *
 * Returns: a new copy of @list.
 *
 * Since: 0.10.24
 */
#ifdef _FOOL_GTK_DOC_
G_INLINE_FUNC GstBufferList * gst_buffer_list_copy (const GstBufferList * list);
#endif

static inline GstBufferList *
gst_buffer_list_copy (const GstBufferList * list)
{
  return GST_BUFFER_LIST_CAST (gst_mini_object_copy (GST_MINI_OBJECT_CONST_CAST (list)));
}

/**
 * gst_buffer_list_is_writable:
 * @list: a #GstBufferList
 *
 * Tests if you can safely add buffers and groups into a buffer list.
 *
 * Since: 0.10.24
 */
#define gst_buffer_list_is_writable(list) gst_mini_object_is_writable (GST_MINI_OBJECT_CAST (list))

/**
 * gst_buffer_list_make_writable:
 * @list: a #GstBufferList
 *
 * Makes a writable buffer list from the given buffer list. If the source buffer
 * list is already writable, this will simply return the same buffer list. A
 * copy will otherwise be made using gst_buffer_list_copy().
 *
 * Since: 0.10.24
 */
#define gst_buffer_list_make_writable(list) GST_BUFFER_LIST_CAST (gst_mini_object_make_writable (GST_MINI_OBJECT_CAST (list)))

guint                    gst_buffer_list_n_groups              (GstBufferList *list);

void                     gst_buffer_list_foreach               (GstBufferList *list,
                                                                GstBufferListFunc func,
								gpointer user_data);
GstBuffer *              gst_buffer_list_get                   (GstBufferList *list, guint group, guint idx);

/* iterator */
GstBufferListIterator *  gst_buffer_list_iterate               (GstBufferList *list);
void                     gst_buffer_list_iterator_free         (GstBufferListIterator *it);

guint                    gst_buffer_list_iterator_n_buffers    (const GstBufferListIterator *it);
GstBuffer *              gst_buffer_list_iterator_next         (GstBufferListIterator *it);
gboolean                 gst_buffer_list_iterator_next_group   (GstBufferListIterator *it);

void                     gst_buffer_list_iterator_add          (GstBufferListIterator *it, GstBuffer *buffer);
void                     gst_buffer_list_iterator_add_list     (GstBufferListIterator *it, GList *list);
void                     gst_buffer_list_iterator_add_group    (GstBufferListIterator *it);
void                     gst_buffer_list_iterator_remove       (GstBufferListIterator *it);
GstBuffer *              gst_buffer_list_iterator_steal        (GstBufferListIterator *it);
void                     gst_buffer_list_iterator_take         (GstBufferListIterator *it, GstBuffer *buffer);

GstBuffer *              gst_buffer_list_iterator_do           (GstBufferListIterator *it, GstBufferListDoFunction do_func,
                                                                gpointer user_data);

/* conversion */
GstBuffer *              gst_buffer_list_iterator_merge_group  (const GstBufferListIterator *it);

G_END_DECLS

#endif /* __GST_BUFFER_LIST_H__ */
