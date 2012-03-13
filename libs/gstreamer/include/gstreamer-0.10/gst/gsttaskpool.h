/* GStreamer
 * Copyright (C) <2009> Wim Taymans <wim.taymans@gmail.com>
 *
 * gsttaskpool.h: Pool for creating streaming threads
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

#ifndef __GST_TASK_POOL_H__
#define __GST_TASK_POOL_H__

#include <gst/gstobject.h>

G_BEGIN_DECLS

/* --- standard type macros --- */
#define GST_TYPE_TASK_POOL             (gst_task_pool_get_type ())
#define GST_TASK_POOL(pool)            (G_TYPE_CHECK_INSTANCE_CAST ((pool), GST_TYPE_TASK_POOL, GstTaskPool))
#define GST_IS_TASK_POOL(pool)         (G_TYPE_CHECK_INSTANCE_TYPE ((pool), GST_TYPE_TASK_POOL))
#define GST_TASK_POOL_CLASS(pclass)    (G_TYPE_CHECK_CLASS_CAST ((pclass), GST_TYPE_TASK_POOL, GstTaskPoolClass))
#define GST_IS_TASK_POOL_CLASS(pclass) (G_TYPE_CHECK_CLASS_TYPE ((pclass), GST_TYPE_TASK_POOL))
#define GST_TASK_POOL_GET_CLASS(pool)  (G_TYPE_INSTANCE_GET_CLASS ((pool), GST_TYPE_TASK_POOL, GstTaskPoolClass))
#define GST_TASK_POOL_CAST(pool)       ((GstTaskPool*)(pool))

typedef struct _GstTaskPool GstTaskPool;
typedef struct _GstTaskPoolClass GstTaskPoolClass;

/**
 * GstTaskPoolFunction:
 * @data: user data for the task function
 *
 * Task function, see gst_task_pool_push().
 *
 * Since: 0.10.24
 */
typedef void   (*GstTaskPoolFunction)          (void *data);

/**
 * GstTaskPool:
 *
 * The #GstTaskPool object.
 */
struct _GstTaskPool {
  GstObject      object;

  /*< private >*/
  GThreadPool   *pool;

  gpointer _gst_reserved[GST_PADDING];
};

/**
 * GstTaskPoolClass:
 * @parent_class: the parent class structure
 * @prepare: prepare the threadpool
 * @cleanup: make sure all threads are stopped
 * @push: start a new thread
 * @join: join a thread
 *
 * The #GstTaskPoolClass object.
 */
struct _GstTaskPoolClass {
  GstObjectClass parent_class;

  /*< public >*/
  void      (*prepare)  (GstTaskPool *pool, GError **error);
  void      (*cleanup)  (GstTaskPool *pool);

  gpointer  (*push)     (GstTaskPool *pool, GstTaskPoolFunction func,
                         gpointer user_data, GError **error);
  void      (*join)     (GstTaskPool *pool, gpointer id);

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
};

GType           gst_task_pool_get_type    (void);

GstTaskPool *   gst_task_pool_new         (void);
void            gst_task_pool_prepare     (GstTaskPool *pool, GError **error);

gpointer        gst_task_pool_push        (GstTaskPool *pool, GstTaskPoolFunction func, 
                                           gpointer user_data, GError **error);
void            gst_task_pool_join        (GstTaskPool *pool, gpointer id);

void		gst_task_pool_cleanup     (GstTaskPool *pool);

G_END_DECLS

#endif /* __GST_TASK_POOL_H__ */
