/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wtay@chello.be>
 *
 * gstbin.h: Header for GstBin container object
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


#ifndef __GST_BIN_H__
#define __GST_BIN_H__

#include <gst/gstelement.h>
#include <gst/gstiterator.h>
#include <gst/gstbus.h>

G_BEGIN_DECLS

#define GST_TYPE_BIN             (gst_bin_get_type ())
#define GST_IS_BIN(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_BIN))
#define GST_IS_BIN_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_BIN))
#define GST_BIN_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_BIN, GstBinClass))
#define GST_BIN(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_BIN, GstBin))
#define GST_BIN_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_BIN, GstBinClass))
#define GST_BIN_CAST(obj)        ((GstBin*)(obj))

/**
 * GstBinFlags:
 * @GST_BIN_FLAG_LAST: the last enum in the series of flags for bins.
 * Derived classes can use this as first value in a list of flags.
 *
 * GstBinFlags are a set of flags specific to bins. Most are set/used
 * internally. They can be checked using the GST_OBJECT_FLAG_IS_SET () macro,
 * and (un)set using GST_OBJECT_FLAG_SET () and GST_OBJECT_FLAG_UNSET ().
 */
typedef enum {
  /* padding */
  GST_BIN_FLAG_LAST		= (GST_ELEMENT_FLAG_LAST << 5)
} GstBinFlags;

typedef struct _GstBin GstBin;
typedef struct _GstBinClass GstBinClass;
typedef struct _GstBinPrivate GstBinPrivate;

/**
 * GST_BIN_NUMCHILDREN:
 * @bin: a #GstBin
 *
 * Gets the number of children in a bin.
 */
#define GST_BIN_NUMCHILDREN(bin)	(GST_BIN_CAST(bin)->numchildren)
/**
 * GST_BIN_CHILDREN:
 * @bin: a #GstBin
 *
 * Gets the list with children in a bin.
 */
#define GST_BIN_CHILDREN(bin)		(GST_BIN_CAST(bin)->children)
/**
 * GST_BIN_CHILDREN_COOKIE:
 * @bin: a #GstBin
 *
 * Gets the children cookie that watches the children list.
 */
#define GST_BIN_CHILDREN_COOKIE(bin)	(GST_BIN_CAST(bin)->children_cookie)

/**
 * GstBin:
 * @numchildren: the number of children in this bin
 * @children: the list of children in this bin
 * @children_cookie: updated whenever @children changes
 * @child_bus: internal bus for handling child messages
 * @messages: queued and cached messages
 * @polling: the bin is currently calculating its state
 * @state_dirty: the bin needs to recalculate its state (deprecated)
 * @clock_dirty: the bin needs to select a new clock
 * @provided_clock: the last clock selected
 * @clock_provider: the element that provided @provided_clock
 *
 * The GstBin base class. Subclasses can access these fields provided
 * the LOCK is taken.
 */
struct _GstBin {
  GstElement	 element;

  /*< public >*/ /* with LOCK */
  /* our children, subclass are supposed to update these
   * fields to reflect their state with _iterate_*() */
  gint		 numchildren;
  GList		*children;
  guint32	 children_cookie;

  GstBus        *child_bus;
  GList         *messages;

  gboolean	 polling;
  gboolean       state_dirty;

  gboolean       clock_dirty;
  GstClock	*provided_clock;
  GstElement    *clock_provider;

  /*< private >*/
  GstBinPrivate *priv;

  gpointer _gst_reserved[GST_PADDING - 1];
};

/**
 * GstBinClass:
 * @parent_class: bin parent class
 * @add_element: method to add an element to a bin
 * @remove_element: method to remove an element from a bin
 * @handle_message: method to handle a message from the children
 *
 * Subclasses can override the @add_element and @remove_element to
 * update the list of children in the bin.
 *
 * The @handle_message method can be overridden to implement custom
 * message handling.  @handle_message takes ownership of the message, just like
 * #gst_element_post_message.
 */
struct _GstBinClass {
  GstElementClass parent_class;

  /*< private >*/
  GThreadPool  *pool;

  /* signals */
  void		(*element_added)	(GstBin *bin, GstElement *child);
  void		(*element_removed)	(GstBin *bin, GstElement *child);

  /*< public >*/
  /* virtual methods for subclasses */
  gboolean	(*add_element)		(GstBin *bin, GstElement *element);
  gboolean	(*remove_element)	(GstBin *bin, GstElement *element);

  void		(*handle_message)	(GstBin *bin, GstMessage *message);

  /*< private >*/
  /* signal added 0.10.22 */
  gboolean	(*do_latency)           (GstBin *bin);

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING-1];
};

GType		gst_bin_get_type		(void);
GstElement*	gst_bin_new			(const gchar *name);

/* add and remove elements from the bin */
gboolean	gst_bin_add			(GstBin *bin, GstElement *element);
gboolean	gst_bin_remove			(GstBin *bin, GstElement *element);

/* retrieve a single child */
GstElement*	gst_bin_get_by_name		 (GstBin *bin, const gchar *name);
GstElement*	gst_bin_get_by_name_recurse_up	 (GstBin *bin, const gchar *name);
GstElement*	gst_bin_get_by_interface	 (GstBin *bin, GType iface);

/* retrieve multiple children */
GstIterator*    gst_bin_iterate_elements	 (GstBin *bin);
GstIterator*    gst_bin_iterate_sorted		 (GstBin *bin);
GstIterator*    gst_bin_iterate_recurse		 (GstBin *bin);

GstIterator*	gst_bin_iterate_sinks		 (GstBin *bin);
GstIterator*	gst_bin_iterate_sources		 (GstBin *bin);
GstIterator*	gst_bin_iterate_all_by_interface (GstBin *bin, GType iface);

/* latency */
gboolean        gst_bin_recalculate_latency      (GstBin * bin);


G_END_DECLS


#endif /* __GST_BIN_H__ */
