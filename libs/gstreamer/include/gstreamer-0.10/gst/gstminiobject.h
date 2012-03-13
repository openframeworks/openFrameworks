/* GStreamer
 * Copyright (C) 2005 David Schleef <ds@schleef.org>
 *
 * gstminiobject.h: Header for GstMiniObject
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


#ifndef __GST_MINI_OBJECT_H__
#define __GST_MINI_OBJECT_H__

#include <gst/gstconfig.h>

#include <glib-object.h>

G_BEGIN_DECLS

#define GST_TYPE_MINI_OBJECT          (gst_mini_object_get_type())
#define GST_IS_MINI_OBJECT(obj)       (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_MINI_OBJECT))
#define GST_IS_MINI_OBJECT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_MINI_OBJECT))
#define GST_MINI_OBJECT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_MINI_OBJECT, GstMiniObjectClass))
#define GST_MINI_OBJECT(obj)          (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_MINI_OBJECT, GstMiniObject))
#define GST_MINI_OBJECT_CLASS(klass)  (G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_MINI_OBJECT, GstMiniObjectClass))
#define GST_MINI_OBJECT_CAST(obj)     ((GstMiniObject*)(obj))
#define GST_MINI_OBJECT_CONST_CAST(obj) ((const GstMiniObject*)(obj))

typedef struct _GstMiniObject GstMiniObject;
typedef struct _GstMiniObjectClass GstMiniObjectClass;

/**
 * GstMiniObjectCopyFunction:
 * @obj: MiniObject to copy
 *
 * Virtual function prototype for methods to create copies of instances.
 *
 * Returns: reference to cloned instance.
 */
typedef GstMiniObject * (*GstMiniObjectCopyFunction) (const GstMiniObject *obj);
/**
 * GstMiniObjectFinalizeFunction:
 * @obj: MiniObject to finalize
 *
 * Virtual function prototype for methods to free ressources used by
 * mini-objects. Subclasses of the mini object are allowed to revive the
 * passed object by doing a gst_mini_object_ref(). If the object is not
 * revived after the finalize function, the memory associated with the
 * object is freed.
 */
typedef void (*GstMiniObjectFinalizeFunction) (GstMiniObject *obj);

/**
 * GST_MINI_OBJECT_FLAGS:
 * @obj: MiniObject to return flags for.
 *
 * This macro returns the entire set of flags for the mini-object.
 */
#define GST_MINI_OBJECT_FLAGS(obj)  (GST_MINI_OBJECT_CAST(obj)->flags)
/**
 * GST_MINI_OBJECT_FLAG_IS_SET:
 * @obj: MiniObject to check for flags.
 * @flag: Flag to check for
 *
 * This macro checks to see if the given flag is set.
 */
#define GST_MINI_OBJECT_FLAG_IS_SET(obj,flag)        !!(GST_MINI_OBJECT_FLAGS (obj) & (flag))
/**
 * GST_MINI_OBJECT_FLAG_SET:
 * @obj: MiniObject to set flag in.
 * @flag: Flag to set, can by any number of bits in guint32.
 *
 * This macro sets the given bits.
 */
#define GST_MINI_OBJECT_FLAG_SET(obj,flag)           (GST_MINI_OBJECT_FLAGS (obj) |= (flag))
/**
 * GST_MINI_OBJECT_FLAG_UNSET:
 * @obj: MiniObject to unset flag in.
 * @flag: Flag to set, must be a single bit in guint32.
 *
 * This macro usets the given bits.
 */
#define GST_MINI_OBJECT_FLAG_UNSET(obj,flag)         (GST_MINI_OBJECT_FLAGS (obj) &= ~(flag))

/**
 * GST_VALUE_HOLDS_MINI_OBJECT:
 * @value: the #GValue to check
 *
 * Checks if the given #GValue contains a #GST_TYPE_MINI_OBJECT value.
 */
#define GST_VALUE_HOLDS_MINI_OBJECT(value)  (G_VALUE_HOLDS(value, GST_TYPE_MINI_OBJECT))

/**
 * GstMiniObjectFlags:
 * @GST_MINI_OBJECT_FLAG_READONLY: is the miniobject readonly or writable
 * @GST_MINI_OBJECT_FLAG_LAST: first flag that can be used by subclasses.
 *
 * Flags for the padtemplate
 */

typedef enum
{
  GST_MINI_OBJECT_FLAG_READONLY = (1<<0),
  /* padding */
  GST_MINI_OBJECT_FLAG_LAST = (1<<4)
} GstMiniObjectFlags;

/**
 * GST_MINI_OBJECT_REFCOUNT:
 * @obj: a #GstMiniObject
 *
 * Get access to the reference count field of the mini-object.
 */
#define GST_MINI_OBJECT_REFCOUNT(obj)           ((GST_MINI_OBJECT_CAST(obj))->refcount)
/**
 * GST_MINI_OBJECT_REFCOUNT_VALUE:
 * @obj: a #GstMiniObject
 *
 * Get the reference count value of the mini-object.
 */
#define GST_MINI_OBJECT_REFCOUNT_VALUE(obj)     (g_atomic_int_get (&(GST_MINI_OBJECT_CAST(obj))->refcount))

/**
 * GstMiniObject:
 * @instance: type instance
 * @refcount: atomic refcount
 * @flags: extra flags.
 * 
 * Base class for refcounted lightweight objects.
 * Ref Func: gst_mini_object_ref
 * Unref Func: gst_mini_object_unref
 * Set Value Func: gst_value_set_mini_object
 * Get Value Func: gst_value_get_mini_object
 */
struct _GstMiniObject {
  GTypeInstance instance;
  /*< public >*/ /* with COW */
  gint refcount;
  guint flags;

  /*< private >*/
  gpointer _gst_reserved;
};

struct _GstMiniObjectClass {
  GTypeClass type_class;

  GstMiniObjectCopyFunction copy;
  GstMiniObjectFinalizeFunction finalize;

  /*< private >*/
  gpointer _gst_reserved;
};

GType 		gst_mini_object_get_type 	(void);

GstMiniObject* 	gst_mini_object_new 		(GType type);
GstMiniObject* 	gst_mini_object_copy 		(const GstMiniObject *mini_object);
gboolean 	gst_mini_object_is_writable 	(const GstMiniObject *mini_object);
GstMiniObject*  gst_mini_object_make_writable 	(GstMiniObject *mini_object);

/* refcounting */
GstMiniObject* 	gst_mini_object_ref 		(GstMiniObject *mini_object);
void 		gst_mini_object_unref 		(GstMiniObject *mini_object);
void 		gst_mini_object_replace 	(GstMiniObject **olddata, GstMiniObject *newdata);

/* GParamSpec */

#define	GST_TYPE_PARAM_MINI_OBJECT	        (gst_param_spec_mini_object_get_type())
#define GST_IS_PARAM_SPEC_MINI_OBJECT(pspec)    (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), \
                                                 GST_TYPE_PARAM_MINI_OBJECT))
#define GST_PARAM_SPEC_MINI_OBJECT(pspec)       (G_TYPE_CHECK_INSTANCE_CAST ((pspec), \
                                                 GST_TYPE_PARAM_MINI_OBJECT, \
						 GstParamSpecMiniObject))

typedef struct _GstParamSpecMiniObject GstParamSpecMiniObject;

/**
 * GstParamSpecMiniObject:
 * @parent_instance: private %GParamSpec portion
 * 
 * A %GParamSpec derived structure that contains the meta data
 * for %GstMiniObject properties.
 */
struct _GstParamSpecMiniObject
{
  GParamSpec parent_instance;
};


GType gst_param_spec_mini_object_get_type (void);

GParamSpec* 	gst_param_spec_mini_object 	(const char *name, const char *nick,
    						 const char *blurb, GType object_type, 
						 GParamFlags flags);

/* GValue stuff */

void 		gst_value_set_mini_object 	(GValue *value, GstMiniObject *mini_object);
void 		gst_value_take_mini_object 	(GValue *value, GstMiniObject *mini_object);
GstMiniObject* 	gst_value_get_mini_object 	(const GValue *value);
GstMiniObject*  gst_value_dup_mini_object       (const GValue *value);


G_END_DECLS

#endif

