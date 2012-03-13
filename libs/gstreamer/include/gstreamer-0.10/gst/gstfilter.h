/* GStreamer
 * Copyright (C) <1999> Erik Walthinsen <omega@cse.ogi.edu>
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

#ifndef __GST_FILTER_H__
#define __GST_FILTER_H__

#include <glib.h>

G_BEGIN_DECLS

/**
 * GstFilterFunc:
 * @obj: the object
 * @user_data: filter data
 *
 * Function prototype for a filter callback taht can be use in gst_filter_run().
 * The function should apply its filtering to @obj. Additional data passed to
 * gst_filter_run() are in @data.
 *
 * Returns: %TRUE for success.
 */
typedef gboolean (*GstFilterFunc)	(gpointer obj, gpointer user_data);

GList*	gst_filter_run 	(const GList *list, GstFilterFunc func, gboolean first, gpointer user_data);

G_END_DECLS

#endif /* __GST_FILTER_H_ */
