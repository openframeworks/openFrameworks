/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wtay@chello.be>
 *                    2005 Wim Taymans <wim@fluendo.com>
 *
 * gstobject.h: Header for base GstObject
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

#ifndef __GST_OBJECT_H__
#define __GST_OBJECT_H__

#include <gst/gstconfig.h>

#include <glib-object.h>

G_BEGIN_DECLS

#define GST_TYPE_OBJECT			(gst_object_get_type ())
#define GST_IS_OBJECT(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_OBJECT))
#define GST_IS_OBJECT_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_OBJECT))
#define GST_OBJECT_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_OBJECT, GstObjectClass))
#define GST_OBJECT(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_OBJECT, GstObject))
#define GST_OBJECT_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_OBJECT, GstObjectClass))
#define GST_OBJECT_CAST(obj)            ((GstObject*)(obj))
#define GST_OBJECT_CLASS_CAST(klass)    ((GstObjectClass*)(klass))

/* make sure we don't change the object size but still make it compile
 * without libxml */
#if defined(GST_DISABLE_LOADSAVE) || defined(GST_DISABLE_DEPRECATED)
#define GstXmlNodePtr	gpointer
#else
#define GstXmlNodePtr	xmlNodePtr
#endif

/**
 * GstObjectFlags:
 * @GST_OBJECT_DISPOSING: the object is been destroyed, don't use it anymore
 * @GST_OBJECT_FLOATING:  the object has a floating reference count (e.g. its
 *  not assigned to a bin)
 * @GST_OBJECT_FLAG_LAST: subclasses can add additional flags starting from this flag
 *
 * The standard flags that an gstobject may have.
 */
typedef enum
{
  GST_OBJECT_DISPOSING = (1<<0),
  GST_OBJECT_FLOATING = (1<<1),
  /* padding */
  GST_OBJECT_FLAG_LAST = (1<<4)
} GstObjectFlags;

/**
 * GST_OBJECT_REFCOUNT:
 * @obj: a #GstObject
 *
 * Get access to the reference count field of the object.
 */
#define GST_OBJECT_REFCOUNT(obj)                (((GObject*)(obj))->ref_count)
/**
 * GST_OBJECT_REFCOUNT_VALUE:
 * @obj: a #GstObject
 *
 * Get the reference count value of the object.
 */
#define GST_OBJECT_REFCOUNT_VALUE(obj)          g_atomic_int_get ((gint *) &GST_OBJECT_REFCOUNT(obj))

/* we do a GST_OBJECT_CAST to avoid type checking, better call these
 * function with a valid object! */

/**
 * GST_OBJECT_GET_LOCK:
 * @obj: a #GstObject
 *
 * Acquire a reference to the mutex of this object.
 */
#define GST_OBJECT_GET_LOCK(obj)               (GST_OBJECT_CAST(obj)->lock)
/**
 * GST_OBJECT_LOCK:
 * @obj: a #GstObject to lock
 *
 * This macro will obtain a lock on the object, making serialization possible.
 * It blocks until the lock can be obtained.
 */
#define GST_OBJECT_LOCK(obj)                   g_mutex_lock(GST_OBJECT_GET_LOCK(obj))
/**
 * GST_OBJECT_TRYLOCK:
 * @obj: a #GstObject.
 *
 * This macro will try to obtain a lock on the object, but will return with
 * FALSE if it can't get it immediately.
 */
#define GST_OBJECT_TRYLOCK(obj)                g_mutex_trylock(GST_OBJECT_GET_LOCK(obj))
/**
 * GST_OBJECT_UNLOCK:
 * @obj: a #GstObject to unlock.
 *
 * This macro releases a lock on the object.
 */
#define GST_OBJECT_UNLOCK(obj)                 g_mutex_unlock(GST_OBJECT_GET_LOCK(obj))


/**
 * GST_OBJECT_NAME:
 * @obj: a #GstObject
 *
 * Get the name of this object
 */
#define GST_OBJECT_NAME(obj)            (GST_OBJECT_CAST(obj)->name)
/**
 * GST_OBJECT_PARENT:
 * @obj: a #GstObject
 *
 * Get the parent of this object
 */
#define GST_OBJECT_PARENT(obj)          (GST_OBJECT_CAST(obj)->parent)


/**
 * GST_OBJECT_FLAGS:
 * @obj: a #GstObject
 *
 * This macro returns the entire set of flags for the object.
 */
#define GST_OBJECT_FLAGS(obj)                  (GST_OBJECT_CAST (obj)->flags)
/**
 * GST_OBJECT_FLAG_IS_SET:
 * @obj: a #GstObject
 * @flag: Flag to check for
 *
 * This macro checks to see if the given flag is set.
 */
#define GST_OBJECT_FLAG_IS_SET(obj,flag)       ((GST_OBJECT_FLAGS (obj) & (flag)) == (flag))
/**
 * GST_OBJECT_FLAG_SET:
 * @obj: a #GstObject
 * @flag: Flag to set
 *
 * This macro sets the given bits.
 */
#define GST_OBJECT_FLAG_SET(obj,flag)          (GST_OBJECT_FLAGS (obj) |= (flag))
/**
 * GST_OBJECT_FLAG_UNSET:
 * @obj: a #GstObject
 * @flag: Flag to set
 *
 * This macro usets the given bits.
 */
#define GST_OBJECT_FLAG_UNSET(obj,flag)        (GST_OBJECT_FLAGS (obj) &= ~(flag))


/**
 * GST_OBJECT_IS_DISPOSING:
 * @obj: a #GstObject
 *
 * Check if the given object is beeing destroyed.
 */
#define GST_OBJECT_IS_DISPOSING(obj)    (GST_OBJECT_FLAG_IS_SET (obj, GST_OBJECT_DISPOSING))
/**
 * GST_OBJECT_IS_FLOATING:
 * @obj: a #GstObject
 *
 * Check if the given object is floating (has no owner).
 */
#define GST_OBJECT_IS_FLOATING(obj)     (GST_OBJECT_FLAG_IS_SET (obj, GST_OBJECT_FLOATING))

typedef struct _GstObject GstObject;
typedef struct _GstObjectClass GstObjectClass;

/**
 * GstObject:
 * @refcount: unused
 * @lock: object LOCK
 * @name: The name of the object
 * @name_prefix: unused
 * @parent: this object's parent, weak ref
 * @flags: use GST_OBJECT_IS_XXX macros to access the flags
 *
 * GStreamer base object class.
 */
struct _GstObject {
  GObject 	 object;

  /*< public >*/
  gint           refcount;    /* unused (FIXME 0.11: remove) */

  /*< public >*/ /* with LOCK */
  GMutex        *lock;        /* object LOCK */
  gchar         *name;        /* object name */
  gchar         *name_prefix; /* (un)used for debugging (FIXME 0.11: remove) */
  GstObject     *parent;      /* this object's parent, weak ref */
  guint32        flags;

  /*< private >*/
  gpointer _gst_reserved;
};

/**
 * GST_CLASS_GET_LOCK:
 * @obj: a #GstObjectClass
 *
 * This macro will return the class lock used to protect deep_notify signal
 * emission on thread-unsafe glib versions (glib < 2.8).
 */
#define GST_CLASS_GET_LOCK(obj)         (GST_OBJECT_CLASS_CAST(obj)->lock)
/**
 * GST_CLASS_LOCK:
 * @obj: a #GstObjectClass
 *
 * Lock the class.
 */
#define GST_CLASS_LOCK(obj)             (g_static_rec_mutex_lock(GST_CLASS_GET_LOCK(obj)))
/**
 * GST_CLASS_TRYLOCK:
 * @obj: a #GstObjectClass
 *
 * Try to lock the class, returns TRUE if class could be locked.
 */
#define GST_CLASS_TRYLOCK(obj)          (g_static_rec_mutex_trylock(GST_CLASS_GET_LOCK(obj)))
/**
 * GST_CLASS_UNLOCK:
 * @obj: a #GstObjectClass
 *
 * Unlock the class.
 */
#define GST_CLASS_UNLOCK(obj)           (g_static_rec_mutex_unlock(GST_CLASS_GET_LOCK(obj)))

/**
 * GstObjectClass:
 * @parent_class: parent
 * @path_string_separator: separator used by gst_object_get_path_string()
 * @signal_object: is used to signal to the whole class
 * @lock: class lock to be used with GST_CLASS_GET_LOCK(), GST_CLASS_LOCK(), GST_CLASS_UNLOCK() and others.
 * @parent_set: default signal handler
 * @parent_unset: default signal handler
 * @object_saved: default signal handler
 * @deep_notify: default signal handler
 * @save_thyself: xml serialisation
 * @restore_thyself: xml de-serialisation
 *
 * GStreamer base object class.
 */
struct _GstObjectClass {
  GObjectClass	parent_class;

  const gchar	*path_string_separator;
  GObject	*signal_object;

  /* FIXME-0.11: remove this, plus the above GST_CLASS_*_LOCK macros */
  GStaticRecMutex *lock;

  /* signals */
  /* FIXME-0.11: remove, and pass NULL in g_signal_new(), we never used them */
  void          (*parent_set)       (GstObject * object, GstObject * parent);
  void          (*parent_unset)     (GstObject * object, GstObject * parent);
  /* FIXME 0.11: Remove this, it's deprecated */
  void          (*object_saved)     (GstObject * object, GstXmlNodePtr parent);
  void          (*deep_notify)      (GstObject * object, GstObject * orig, GParamSpec * pspec);

  /*< public >*/
  /* virtual methods for subclasses */
  /* FIXME 0.11: Remove this, it's deprecated */
  GstXmlNodePtr (*save_thyself)     (GstObject * object, GstXmlNodePtr parent);
  void          (*restore_thyself)  (GstObject * object, GstXmlNodePtr self);

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
};

/* normal GObject stuff */
GType		gst_object_get_type		(void);

/* name routines */
gboolean	gst_object_set_name		(GstObject *object, const gchar *name);
gchar*		gst_object_get_name		(GstObject *object);

#ifndef GST_DISABLE_DEPRECATED
void		gst_object_set_name_prefix	(GstObject *object, const gchar *name_prefix);
gchar*		gst_object_get_name_prefix	(GstObject *object);
#endif

/* parentage routines */
gboolean	gst_object_set_parent		(GstObject *object, GstObject *parent);
GstObject*	gst_object_get_parent		(GstObject *object);
void		gst_object_unparent		(GstObject *object);
gboolean	gst_object_has_ancestor		(GstObject *object, GstObject *ancestor);

void            gst_object_default_deep_notify 	(GObject *object, GstObject *orig,
                                                 GParamSpec *pspec, gchar **excluded_props);

/* refcounting + life cycle */
gpointer	gst_object_ref			(gpointer object);
void		gst_object_unref		(gpointer object);
void 		gst_object_ref_sink		(gpointer object);
void 		gst_object_sink			(gpointer object);

/* replace object pointer */
void 		gst_object_replace		(GstObject **oldobj, GstObject *newobj);

/* printing out the 'path' of the object */
gchar *		gst_object_get_path_string	(GstObject *object);

/* misc utils */
gboolean	gst_object_check_uniqueness	(GList *list, const gchar *name);

/* load/save */
#ifndef GST_DISABLE_DEPRECATED
#ifndef GST_DISABLE_LOADSAVE
GstXmlNodePtr   gst_object_save_thyself    (GstObject *object, GstXmlNodePtr parent);
void            gst_object_restore_thyself (GstObject *object, GstXmlNodePtr self);
#else
#if defined __GNUC__ && __GNUC__ >= 3
#pragma GCC poison gst_object_save_thyself
#pragma GCC poison gst_object_restore_thyself
#endif
#endif
#endif

/* class signal stuff */
guint		gst_class_signal_connect	(GstObjectClass	*klass,
						 const gchar	*name,
						 gpointer	 func,
						 gpointer	 func_data);

#ifndef GST_DISABLE_DEPRECATED
#ifndef GST_DISABLE_LOADSAVE
void        gst_class_signal_emit_by_name   (GstObject     * object,
                                             const gchar   * name,
                                             GstXmlNodePtr   self);
#else
#if defined __GNUC__ && __GNUC__ >= 3
#pragma GCC poison gst_class_signal_emit_by_name
#endif
#endif
#endif

G_END_DECLS

#endif /* __GST_OBJECT_H__ */

