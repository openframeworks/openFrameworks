/* GStreamer
 * Copyright (C) 2004 Wim Taymans <wim@fluendo.com>
 *
 * gstiterator.h: Header for GstIterator
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

#ifndef __GST_ITERATOR_H__
#define __GST_ITERATOR_H__

#include <glib-object.h> /* for GValue in the fold */
#include <gst/gstconfig.h>

G_BEGIN_DECLS

/**
 * GstIteratorResult:
 * @GST_ITERATOR_DONE:   No more items in the iterator
 * @GST_ITERATOR_OK:     An item was retrieved
 * @GST_ITERATOR_RESYNC: Datastructure changed while iterating
 * @GST_ITERATOR_ERROR:  An error happened
 *
 * The result of gst_iterator_next().
 */
typedef enum {
  GST_ITERATOR_DONE	= 0,
  GST_ITERATOR_OK	= 1,
  GST_ITERATOR_RESYNC	= 2,
  GST_ITERATOR_ERROR	= 3
} GstIteratorResult;

typedef struct _GstIterator GstIterator;

/**
 * GstIteratorItem:
 * @GST_ITERATOR_ITEM_SKIP:  Skip this item
 * @GST_ITERATOR_ITEM_PASS:  Return item
 * @GST_ITERATOR_ITEM_END:   Stop after this item.
 *
 * The result of a #GstIteratorItemFunction.
 */
typedef enum {
  GST_ITERATOR_ITEM_SKIP	= 0,
  GST_ITERATOR_ITEM_PASS	= 1,
  GST_ITERATOR_ITEM_END		= 2
} GstIteratorItem;

/**
 * GstIteratorDisposeFunction:
 * @owner: the owner of the iterator
 *
 * The function that will be called when a #GList iterator is freed. The
 * owner of the #GList iterator can then clean up its resources.
 */
typedef void		  (*GstIteratorDisposeFunction)	(gpointer owner);

/**
 * GstIteratorNextFunction:
 * @it: the iterator
 * @result: a pointer to hold the next item
 *
 * The function that will be called when the next element of the iterator
 * should be retrieved. 
 *
 * Implementors of a #GstIterator should implement this
 * function and pass it to the constructor of the custom iterator.
 * The function will be called with the iterator lock held.
 *
 * Returns: the result of the operation.
 */
typedef GstIteratorResult (*GstIteratorNextFunction)	(GstIterator *it, gpointer *result);
/**
 * GstIteratorItemFunction:
 * @it: the iterator
 * @item: the item being retrieved.
 *
 * The function that will be called after the next item of the iterator
 * has been retrieved. This function will typically increase the refcount
 * of the item or make a copy.
 *
 * Implementors of a #GstIterator should implement this
 * function and pass it to the constructor of the custom iterator.
 * The function will be called with the iterator lock held.
 *
 * Returns: the result of the operation.
 */
typedef GstIteratorItem	  (*GstIteratorItemFunction)	(GstIterator *it, gpointer item);
/**
 * GstIteratorResyncFunction:
 * @it: the iterator
 *
 * This function will be called whenever a concurrent update happened
 * to the iterated datastructure. The implementor of the iterator should
 * restart the iterator from the beginning and clean up any state it might
 * have.
 *
 * Implementors of a #GstIterator should implement this
 * function and pass it to the constructor of the custom iterator.
 * The function will be called with the iterator lock held.
 */
typedef void		  (*GstIteratorResyncFunction)	(GstIterator *it);
/**
 * GstIteratorFreeFunction:
 * @it: the iterator
 *
 * This function will be called when the iterator is freed.
 *
 * Implementors of a #GstIterator should implement this
 * function and pass it to the constructor of the custom iterator.
 * The function will be called with the iterator lock held.
 */
typedef void		  (*GstIteratorFreeFunction)	(GstIterator *it);

/**
 * GstIteratorFoldFunction:
 * @item: the item to fold
 * @ret: a #GValue collecting the result
 * @user_data: data passed to gst_iterator_fold()
 *
 * A function to be passed to gst_iterator_fold().
 *
 * Returns: TRUE if the fold should continue, FALSE if it should stop.
 */
typedef gboolean	  (*GstIteratorFoldFunction)    (gpointer item, GValue *ret, gpointer user_data);

/**
 * GstCopyFunction:
 * @object: The object to copy
 *
 * A function to create a copy of some object or
 * increase its reference count.
 *
 * Returns: a copy of the object or the same object with increased reference count
 *
 * Since: 0.10.25
 */
typedef gpointer          (*GstCopyFunction)             (gpointer object);

/**
 * GST_ITERATOR:
 * @it: the #GstIterator value
 *
 * Macro to cast to a #GstIterator
 */
#define GST_ITERATOR(it)		((GstIterator*)(it))
/**
 * GST_ITERATOR_LOCK:
 * @it: the #GstIterator to get the lock of
 *
 * Macro to get the lock protecting the datastructure being iterated.
 */
#define GST_ITERATOR_LOCK(it)		(GST_ITERATOR(it)->lock)
/**
 * GST_ITERATOR_COOKIE:
 * @it: the #GstIterator to get the cookie of
 *
 * Macro to get the cookie of a #GstIterator. The cookie of the
 * iterator is the value of the master cookie when the iterator
 * was created.
 * Whenever the iterator is iterated, the value is compared to the
 * value of the master cookie. If they are different, a concurrent
 * modification happened to the iterator and a resync is needed.
 */
#define GST_ITERATOR_COOKIE(it)		(GST_ITERATOR(it)->cookie)
/**
 * GST_ITERATOR_ORIG_COOKIE:
 * @it: the #GstIterator to get the master cookie of
 *
 * Macro to get a pointer to where the master cookie is stored. The
 * master cookie protects the structure being iterated and gets updated
 * whenever the datastructure changes.
 */
#define GST_ITERATOR_ORIG_COOKIE(it)	(GST_ITERATOR(it)->master_cookie)

/**
 * GstIterator:
 * @next: The function to get the next item in the iterator
 * @item: The function to be called for each item retrieved
 * @resync: The function to call when a resync is needed.
 * @free: The function to call when the iterator is freed
 * @pushed: The iterator that is currently pushed with gst_iterator_push()
 * @type: The type of the object that this iterator will return
 * @lock: The lock protecting the data structure and the cookie.
 * @cookie: The cookie; the value of the master_cookie when this iterator was
 *          created.
 * @master_cookie: A pointer to the master cookie.
 *
 * #GstIterator base structure. The values of this structure are 
 * protected for subclasses, use the methods to use the #GstIterator.
 */
struct _GstIterator {
  /*< protected >*/
  GstIteratorNextFunction next;
  GstIteratorItemFunction item;
  GstIteratorResyncFunction resync;
  GstIteratorFreeFunction free;

  GstIterator *pushed;		/* pushed iterator */

  GType     type;
  GMutex   *lock;
  guint32   cookie;		/* cookie of the iterator */
  guint32  *master_cookie;	/* pointer to guint32 holding the cookie when this
				   iterator was created */

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
};

/* creating iterators */
GstIterator*		gst_iterator_new		(guint size,
							 GType type,
							 GMutex *lock,
							 guint32 *master_cookie,
							 GstIteratorNextFunction next,
							 GstIteratorItemFunction item,
							 GstIteratorResyncFunction resync,
							 GstIteratorFreeFunction free);

GstIterator*		gst_iterator_new_list		(GType type,
							 GMutex *lock,
							 guint32 *master_cookie,
							 GList **list,
							 gpointer owner,
							 GstIteratorItemFunction item,
							 GstIteratorDisposeFunction free);

GstIterator*            gst_iterator_new_single         (GType type,
                                                         gpointer object,
                                                         GstCopyFunction copy,
                                                         GFreeFunc free);

/* using iterators */
GstIteratorResult	gst_iterator_next		(GstIterator *it, gpointer *elem);
void			gst_iterator_resync		(GstIterator *it);
void			gst_iterator_free		(GstIterator *it);

void			gst_iterator_push		(GstIterator *it, GstIterator *other);

/* higher-order functions that operate on iterators */
GstIterator*		gst_iterator_filter		(GstIterator *it, GCompareFunc func,
                                                         gpointer user_data);
GstIteratorResult	gst_iterator_fold		(GstIterator *it,
                                                         GstIteratorFoldFunction func,
                                                         GValue *ret, gpointer user_data);
GstIteratorResult	gst_iterator_foreach		(GstIterator *it,
                                                         GFunc func, gpointer user_data);
gpointer		gst_iterator_find_custom	(GstIterator *it, GCompareFunc func,
                                                         gpointer user_data);

G_END_DECLS

#endif /* __GST_ITERATOR_H__ */
