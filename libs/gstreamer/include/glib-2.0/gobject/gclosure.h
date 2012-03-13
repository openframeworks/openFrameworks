/* GObject - GLib Type, Object, Parameter and Signal Library
 * Copyright (C) 2000-2001 Red Hat, Inc.
 * Copyright (C) 2005 Imendio AB
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 */
#if !defined (__GLIB_GOBJECT_H_INSIDE__) && !defined (GOBJECT_COMPILATION)
#error "Only <glib-object.h> can be included directly."
#endif

#ifndef __G_CLOSURE_H__
#define __G_CLOSURE_H__

#include        <gobject/gtype.h>

G_BEGIN_DECLS

/* --- defines --- */
/**
 * G_CLOSURE_NEEDS_MARSHAL:
 * @closure: a #GClosure
 * 
 * Check if the closure still needs a marshaller. See g_closure_set_marshal().
 *
 * Returns: %TRUE if a #GClosureMarshal marshaller has not yet been set on 
 * @closure.
 */
#define	G_CLOSURE_NEEDS_MARSHAL(closure) (((GClosure*) (closure))->marshal == NULL)
/**
 * G_CLOSURE_N_NOTIFIERS:
 * @cl: a #GClosure
 * 
 * Get the total number of notifiers connected with the closure @cl. 
 * The count includes the meta marshaller, the finalize and invalidate notifiers 
 * and the marshal guards. Note that each guard counts as two notifiers. 
 * See g_closure_set_meta_marshal(), g_closure_add_finalize_notifier(),
 * g_closure_add_invalidate_notifier() and g_closure_add_marshal_guards().
 *
 * Returns: number of notifiers
 */
#define	G_CLOSURE_N_NOTIFIERS(cl)	 ((cl)->meta_marshal + ((cl)->n_guards << 1L) + \
                                          (cl)->n_fnotifiers + (cl)->n_inotifiers)
/**
 * G_CCLOSURE_SWAP_DATA:
 * @cclosure: a #GCClosure
 * 
 * Checks whether the user data of the #GCClosure should be passed as the
 * first parameter to the callback. See g_cclosure_new_swap().
 *
 * Returns: %TRUE if data has to be swapped.
 */
#define	G_CCLOSURE_SWAP_DATA(cclosure)	 (((GClosure*) (cclosure))->derivative_flag)
/**
 * G_CALLBACK:
 * @f: a function pointer.
 * 
 * Cast a function pointer to a #GCallback.
 */
#define	G_CALLBACK(f)			 ((GCallback) (f))


/* -- typedefs --- */
typedef struct _GClosure		 GClosure;
typedef struct _GClosureNotifyData	 GClosureNotifyData;

/**
 * GCallback:
 * 
 * The type used for callback functions in structure definitions and function 
 * signatures. This doesn't mean that all callback functions must take no 
 * parameters and return void. The required signature of a callback function 
 * is determined by the context in which is used (e.g. the signal to which it 
 * is connected). Use G_CALLBACK() to cast the callback function to a #GCallback. 
 */
typedef void  (*GCallback)              (void);
/**
 * GClosureNotify:
 * @data: data specified when registering the notification callback
 * @closure: the #GClosure on which the notification is emitted
 * 
 * The type used for the various notification callbacks which can be registered
 * on closures.
 */
typedef void  (*GClosureNotify)		(gpointer	 data,
					 GClosure	*closure);
/**
 * GClosureMarshal:
 * @closure: the #GClosure to which the marshaller belongs
 * @return_value: a #GValue to store the return value. May be %NULL if the
 *  callback of @closure doesn't return a value.
 * @n_param_values: the length of the @param_values array
 * @param_values: an array of #GValue<!-- -->s holding the arguments on
 *  which to invoke the callback of @closure
 * @invocation_hint: the invocation hint given as the last argument
 *  to g_closure_invoke()
 * @marshal_data: additional data specified when registering the marshaller,
 *  see g_closure_set_marshal() and g_closure_set_meta_marshal()
 * 
 * The type used for marshaller functions.
 */
typedef void  (*GClosureMarshal)	(GClosure	*closure,
					 GValue         *return_value,
					 guint           n_param_values,
					 const GValue   *param_values,
					 gpointer        invocation_hint,
					 gpointer	 marshal_data);
/**
 * GCClosure:
 * @closure: the #GClosure
 * @callback: the callback function
 * 
 * A #GCClosure is a specialization of #GClosure for C function callbacks.
 */
typedef struct _GCClosure		 GCClosure;


/* --- structures --- */
struct _GClosureNotifyData
{
  gpointer       data;
  GClosureNotify notify;
};
/**
 * GClosure:
 * @in_marshal: Indicates whether the closure is currently being invoked with 
 *  g_closure_invoke()
 * @is_invalid: Indicates whether the closure has been invalidated by 
 *  g_closure_invalidate()
 * 
 * A #GClosure represents a callback supplied by the programmer.
 */
struct _GClosure
{
  /*< private >*/
  volatile      	guint	 ref_count : 15;
  volatile       	guint	 meta_marshal : 1;
  volatile       	guint	 n_guards : 1;
  volatile       	guint	 n_fnotifiers : 2;	/* finalization notifiers */
  volatile       	guint	 n_inotifiers : 8;	/* invalidation notifiers */
  volatile       	guint	 in_inotify : 1;
  volatile       	guint	 floating : 1;
  /*< protected >*/
  volatile         	guint	 derivative_flag : 1;
  /*< public >*/
  volatile       	guint	 in_marshal : 1;
  volatile       	guint	 is_invalid : 1;

  /*< private >*/	void   (*marshal)  (GClosure       *closure,
					    GValue /*out*/ *return_value,
					    guint           n_param_values,
					    const GValue   *param_values,
					    gpointer        invocation_hint,
					    gpointer	    marshal_data);
  /*< protected >*/	gpointer data;

  /*< private >*/	GClosureNotifyData *notifiers;

  /* invariants/constrains:
   * - ->marshal and ->data are _invalid_ as soon as ->is_invalid==TRUE
   * - invocation of all inotifiers occours prior to fnotifiers
   * - order of inotifiers is random
   *   inotifiers may _not_ free/invalidate parameter values (e.g. ->data)
   * - order of fnotifiers is random
   * - each notifier may only be removed before or during its invocation
   * - reference counting may only happen prior to fnotify invocation
   *   (in that sense, fnotifiers are really finalization handlers)
   */
};
/* closure for C function calls, callback() is the user function
 */
struct _GCClosure
{
  GClosure	closure;
  gpointer	callback;
};


/* --- prototypes --- */
GClosure* g_cclosure_new			(GCallback	callback_func,
						 gpointer	user_data,
						 GClosureNotify destroy_data);
GClosure* g_cclosure_new_swap			(GCallback	callback_func,
						 gpointer	user_data,
						 GClosureNotify destroy_data);
GClosure* g_signal_type_cclosure_new		(GType          itype,
						 guint          struct_offset);


/* --- prototypes --- */
GClosure* g_closure_ref				(GClosure	*closure);
void	  g_closure_sink			(GClosure	*closure);
void	  g_closure_unref			(GClosure	*closure);
/* intimidating */
GClosure* g_closure_new_simple			(guint		 sizeof_closure,
						 gpointer	 data);
void	  g_closure_add_finalize_notifier	(GClosure       *closure,
						 gpointer	 notify_data,
						 GClosureNotify	 notify_func);
void	  g_closure_remove_finalize_notifier	(GClosure       *closure,
						 gpointer	 notify_data,
						 GClosureNotify	 notify_func);
void	  g_closure_add_invalidate_notifier	(GClosure       *closure,
						 gpointer	 notify_data,
						 GClosureNotify	 notify_func);
void	  g_closure_remove_invalidate_notifier	(GClosure       *closure,
						 gpointer	 notify_data,
						 GClosureNotify	 notify_func);
void	  g_closure_add_marshal_guards		(GClosure	*closure,
						 gpointer        pre_marshal_data,
						 GClosureNotify	 pre_marshal_notify,
						 gpointer        post_marshal_data,
						 GClosureNotify	 post_marshal_notify);
void	  g_closure_set_marshal			(GClosure	*closure,
						 GClosureMarshal marshal);
void	  g_closure_set_meta_marshal		(GClosure       *closure,
						 gpointer	 marshal_data,
						 GClosureMarshal meta_marshal);
void	  g_closure_invalidate			(GClosure	*closure);
void	  g_closure_invoke			(GClosure 	*closure,
						 GValue	/*out*/	*return_value,
						 guint		 n_param_values,
						 const GValue	*param_values,
						 gpointer	 invocation_hint);

/* FIXME:
   OK:  data_object::destroy		-> closure_invalidate();
   MIS:	closure_invalidate()		-> disconnect(closure);
   MIS:	disconnect(closure)		-> (unlink) closure_unref();
   OK:	closure_finalize()		-> g_free (data_string);

   random remarks:
   - need marshaller repo with decent aliasing to base types
   - provide marshaller collection, virtually covering anything out there
*/

G_END_DECLS

#endif /* __G_CLOSURE_H__ */
