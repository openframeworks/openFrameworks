/* gmain.h - the GLib Main loop
 * Copyright (C) 1998-2000 Red Hat, Inc.
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

#if defined(G_DISABLE_SINGLE_INCLUDES) && !defined (__GLIB_H_INSIDE__) && !defined (GLIB_COMPILATION)
#error "Only <glib.h> can be included directly."
#endif

#ifndef __G_MAIN_H__
#define __G_MAIN_H__

#include <glib/gpoll.h>
#include <glib/gslist.h>
#include <glib/gthread.h>

G_BEGIN_DECLS

/**
 * GMainContext:
 *
 * The <structname>GMainContext</structname> struct is an opaque data
 * type representing a set of sources to be handled in a main loop.
 */
typedef struct _GMainContext            GMainContext;

/**
 * GMainLoop:
 *
 * The <structname>GMainLoop</structname> struct is an opaque data type
 * representing the main event loop of a GLib or GTK+ application.
 */
typedef struct _GMainLoop               GMainLoop;

/**
 * GSource:
 *
 * The <structname>GSource</structname> struct is an opaque data type
 * representing an event source.
 */
typedef struct _GSource                 GSource;

/**
 * GSourceCallbackFuncs:
 * @ref: Called when a reference is added to the callback object
 * @unref: Called when a reference to the callback object is dropped
 * @get: Called to extract the callback function and data from the
 *     callback object.

 * The <structname>GSourceCallbackFuncs</structname> struct contains
 * functions for managing callback objects.
 */
typedef struct _GSourceCallbackFuncs    GSourceCallbackFuncs;

/**
 * GSourceFuncs:
 * @prepare: Called before all the file descriptors are polled. If the
 *     source can determine that it is ready here (without waiting for the
 *     results of the poll() call) it should return %TRUE. It can also return
 *     a @timeout_ value which should be the maximum timeout (in milliseconds)
 *     which should be passed to the poll() call. The actual timeout used will
 *     be -1 if all sources returned -1, or it will be the minimum of all the
 *     @timeout_ values returned which were >= 0.
 * @check: Called after all the file descriptors are polled. The source
 *     should return %TRUE if it is ready to be dispatched. Note that some
 *     time may have passed since the previous prepare function was called,
 *     so the source should be checked again here.
 * @dispatch: Called to dispatch the event source, after it has returned
 *     %TRUE in either its @prepare or its @check function. The @dispatch
 *     function is passed in a callback function and data. The callback
 *     function may be %NULL if the source was never connected to a callback
 *     using g_source_set_callback(). The @dispatch function should call the
 *     callback function with @user_data and whatever additional parameters
 *     are needed for this type of event source.
 * @finalize: Called when the source is finalized.
 * @closure_callback:
 * @closure_marshal:
 *
 * The <structname>GSourceFuncs</structname> struct contains a table of
 * functions used to handle event sources in a generic manner.
 *
 * For idle sources, the prepare and check functions always return %TRUE
 * to indicate that the source is always ready to be processed. The prepare
 * function also returns a timeout value of 0 to ensure that the poll() call
 * doesn't block (since that would be time wasted which could have been spent
 * running the idle function).
 *
 * For timeout sources, the prepare and check functions both return %TRUE
 * if the timeout interval has expired. The prepare function also returns
 * a timeout value to ensure that the poll() call doesn't block too long
 * and miss the next timeout.
 *
 * For file descriptor sources, the prepare function typically returns %FALSE,
 * since it must wait until poll() has been called before it knows whether
 * any events need to be processed. It sets the returned timeout to -1 to
 * indicate that it doesn't mind how long the poll() call blocks. In the
 * check function, it tests the results of the poll() call to see if the
 * required condition has been met, and returns %TRUE if so.
 */
typedef struct _GSourceFuncs            GSourceFuncs;

/**
 * GPid:
 *
 * A type which is used to hold a process identification.
 *
 * On UNIX, processes are identified by a process id (an integer),
 * while Windows uses process handles (which are pointers).
 */

typedef gboolean (*GSourceFunc)       (gpointer data);

/**
 * GChildWatchFunc:
 * @pid: the process id of the child process
 * @status: Status information about the child process,
 *     see waitpid(2) for more information about this field
 * @data: user data passed to g_child_watch_add()
 *
 * The type of functions to be called when a child exists.
 */
typedef void     (*GChildWatchFunc)   (GPid     pid,
                                       gint     status,
                                       gpointer data);
struct _GSource
{
  /*< private >*/
  gpointer callback_data;
  GSourceCallbackFuncs *callback_funcs;

  GSourceFuncs *source_funcs;
  guint ref_count;

  GMainContext *context;

  gint priority;
  guint flags;
  guint source_id;

  GSList *poll_fds;
  
  GSource *prev;
  GSource *next;

  char    *name;
  gpointer reserved2;
};

struct _GSourceCallbackFuncs
{
  void (*ref)   (gpointer     cb_data);
  void (*unref) (gpointer     cb_data);
  void (*get)   (gpointer     cb_data,
                 GSource     *source, 
                 GSourceFunc *func,
                 gpointer    *data);
};

typedef void (*GSourceDummyMarshal) (void);

struct _GSourceFuncs
{
  gboolean (*prepare)  (GSource    *source,
                        gint       *timeout_);
  gboolean (*check)    (GSource    *source);
  gboolean (*dispatch) (GSource    *source,
                        GSourceFunc callback,
                        gpointer    user_data);
  void     (*finalize) (GSource    *source); /* Can be NULL */

  /* For use by g_source_set_closure */
  GSourceFunc     closure_callback;        
  GSourceDummyMarshal closure_marshal; /* Really is of type GClosureMarshal */
};

/* Standard priorities */

/**
 * G_PRIORITY_HIGH:
 *
 * Use this for high priority event sources.
 *
 * It is not used within GLib or GTK+.
 */
#define G_PRIORITY_HIGH            -100

/**
 * G_PRIORITY_DEFAULT:
 *
 * Use this for default priority event sources.
 *
 * In GLib this priority is used when adding timeout functions
 * with g_timeout_add(). In GDK this priority is used for events
 * from the X server.
 */
#define G_PRIORITY_DEFAULT          0

/**
 * G_PRIORITY_HIGH_IDLE:
 *
 * Use this for high priority idle functions.
 *
 * GTK+ uses #G_PRIORITY_HIGH_IDLE + 10 for resizing operations,
 * and #G_PRIORITY_HIGH_IDLE + 20 for redrawing operations. (This is
 * done to ensure that any pending resizes are processed before any
 * pending redraws, so that widgets are not redrawn twice unnecessarily.)
 */
#define G_PRIORITY_HIGH_IDLE        100

/**
 * G_PRIORITY_DEFAULT_IDLE:
 *
 * Use this for default priority idle functions.
 *
 * In GLib this priority is used when adding idle functions with
 * g_idle_add().
 */
#define G_PRIORITY_DEFAULT_IDLE     200

/**
 * G_PRIORITY_LOW:
 *
 * Use this for very low priority background tasks.
 *
 * It is not used within GLib or GTK+.
 */
#define G_PRIORITY_LOW              300

/* GMainContext: */

GMainContext *g_main_context_new       (void);
GMainContext *g_main_context_ref       (GMainContext *context);
void          g_main_context_unref     (GMainContext *context);
GMainContext *g_main_context_default   (void);

gboolean      g_main_context_iteration (GMainContext *context,
                                        gboolean      may_block);
gboolean      g_main_context_pending   (GMainContext *context);

/* For implementation of legacy interfaces
 */
GSource      *g_main_context_find_source_by_id              (GMainContext *context,
                                                             guint         source_id);
GSource      *g_main_context_find_source_by_user_data       (GMainContext *context,
                                                             gpointer      user_data);
GSource      *g_main_context_find_source_by_funcs_user_data (GMainContext *context,
                                                             GSourceFuncs *funcs,
                                                             gpointer      user_data);

/* Low level functions for implementing custom main loops.
 */
void     g_main_context_wakeup  (GMainContext *context);
gboolean g_main_context_acquire (GMainContext *context);
void     g_main_context_release (GMainContext *context);
gboolean g_main_context_is_owner (GMainContext *context);
gboolean g_main_context_wait    (GMainContext *context,
                                 GCond        *cond,
                                 GMutex       *mutex);

gboolean g_main_context_prepare  (GMainContext *context,
                                  gint         *priority);
gint     g_main_context_query    (GMainContext *context,
                                  gint          max_priority,
                                  gint         *timeout_,
                                  GPollFD      *fds,
                                  gint          n_fds);
gint     g_main_context_check    (GMainContext *context,
                                  gint          max_priority,
                                  GPollFD      *fds,
                                  gint          n_fds);
void     g_main_context_dispatch (GMainContext *context);

void     g_main_context_set_poll_func (GMainContext *context,
                                       GPollFunc     func);
GPollFunc g_main_context_get_poll_func (GMainContext *context);

/* Low level functions for use by source implementations
 */
void     g_main_context_add_poll    (GMainContext *context,
                                     GPollFD      *fd,
                                     gint          priority);
void     g_main_context_remove_poll (GMainContext *context,
                                     GPollFD      *fd);

gint     g_main_depth               (void);
GSource *g_main_current_source      (void);

/* GMainContexts for other threads
 */
void          g_main_context_push_thread_default (GMainContext *context);
void          g_main_context_pop_thread_default  (GMainContext *context);
GMainContext *g_main_context_get_thread_default  (void);

/* GMainLoop: */

GMainLoop *g_main_loop_new        (GMainContext *context,
                                   gboolean      is_running);
void       g_main_loop_run        (GMainLoop    *loop);
void       g_main_loop_quit       (GMainLoop    *loop);
GMainLoop *g_main_loop_ref        (GMainLoop    *loop);
void       g_main_loop_unref      (GMainLoop    *loop);
gboolean   g_main_loop_is_running (GMainLoop    *loop);
GMainContext *g_main_loop_get_context (GMainLoop    *loop);

/* GSource: */

GSource *g_source_new             (GSourceFuncs   *source_funcs,
                                   guint           struct_size);
GSource *g_source_ref             (GSource        *source);
void     g_source_unref           (GSource        *source);

guint    g_source_attach          (GSource        *source,
                                   GMainContext   *context);
void     g_source_destroy         (GSource        *source);

void     g_source_set_priority    (GSource        *source,
                                   gint            priority);
gint     g_source_get_priority    (GSource        *source);
void     g_source_set_can_recurse (GSource        *source,
                                   gboolean        can_recurse);
gboolean g_source_get_can_recurse (GSource        *source);
guint    g_source_get_id          (GSource        *source);

GMainContext *g_source_get_context (GSource       *source);

void     g_source_set_callback    (GSource        *source,
                                   GSourceFunc     func,
                                   gpointer        data,
                                   GDestroyNotify  notify);

void     g_source_set_funcs       (GSource        *source,
                                   GSourceFuncs   *funcs);
gboolean g_source_is_destroyed    (GSource        *source);

void                 g_source_set_name       (GSource        *source,
                                              const char     *name);
G_CONST_RETURN char* g_source_get_name       (GSource        *source);
void                 g_source_set_name_by_id (guint           tag,
                                              const char     *name);


/* Used to implement g_source_connect_closure and internally*/
void g_source_set_callback_indirect (GSource              *source,
                                     gpointer              callback_data,
                                     GSourceCallbackFuncs *callback_funcs);

void     g_source_add_poll         (GSource        *source,
                                    GPollFD        *fd);
void     g_source_remove_poll      (GSource        *source,
                                    GPollFD        *fd);

void     g_source_get_current_time (GSource        *source,
                                    GTimeVal       *timeval);

 /* void g_source_connect_closure (GSource        *source,
                                  GClosure       *closure);
 */

/* Specific source types
 */
GSource *g_idle_source_new        (void);
GSource *g_child_watch_source_new (GPid pid);
GSource *g_timeout_source_new     (guint interval);
GSource *g_timeout_source_new_seconds (guint interval);

/* Miscellaneous functions
 */
void g_get_current_time                 (GTimeVal       *result);

/* ============== Compat main loop stuff ================== */

#ifndef G_DISABLE_DEPRECATED

/**
 * g_main_new:
 * @is_running: set to %TRUE to indicate that the loop is running. This
 *     is not very important since calling g_main_run() will set this
 *     to %TRUE anyway.
 *
 * Creates a new #GMainLoop for th default main context.
 *
 * Returns: a new #GMainLoop
 *
 * Deprecated: 2.2: Use g_main_loop_new() instead
 */
#define         g_main_new(is_running)  g_main_loop_new (NULL, is_running)

/**
 * g_main_run:
 * @loop: a #GMainLoop
 *
 * Runs a main loop until it stops running.
 *
 * Deprecated: 2.2: Use g_main_loop_run() instead
 */
#define         g_main_run(loop)        g_main_loop_run(loop)

/**
 * g_main_quit:
 * @loop: a #GMainLoop
 *
 * Stops the #GMainLoop.
 * If g_main_run() was called to run the #GMainLoop, it will now return.
 *
 * Deprecated: 2.2: Use g_main_loop_quit() instead
 */
#define g_main_quit(loop)       g_main_loop_quit(loop)

/**
 * g_main_destroy:
 * @loop: a #GMainLoop
 *
 * Frees the memory allocated for the #GMainLoop.
 *
 * Deprecated: 2.2: Use g_main_loop_unref() instead
 */
#define g_main_destroy(loop)    g_main_loop_unref(loop)

/**
 * g_main_is_running:
 * @loop: a #GMainLoop
 *
 * Checks if the main loop is running.
 *
 * Returns: %TRUE if the main loop is running
 *
 * Deprecated: 2.2: Use g_main_loop_is_running() instead
 */
#define g_main_is_running(loop) g_main_loop_is_running(loop)

/**
 * g_main_iteration:
 * @may_block: set to %TRUE if it should block (i.e. wait) until an event
 *     source becomes ready. It will return after an event source has been
 *     processed. If set to %FALSE it will return immediately if no event
 *     source is ready to be processed.
 *
 * Runs a single iteration for the default #GMainContext.
 *
 * Returns: %TRUE if more events are pending.
 *
 * Deprecated: 2.2: Use g_main_context_iteration() instead.
 */
#define g_main_iteration(may_block) g_main_context_iteration (NULL, may_block)

/**
 * g_main_pending:
 *
 * Checks if any events are pending for the default #GMainContext
 * (i.e. ready to be processed).
 *
 * Returns: %TRUE if any events are pending.
 *
 * Deprected: 2.2: Use g_main_context_pending() instead.
 */
#define g_main_pending()            g_main_context_pending (NULL)

/**
 * g_main_set_poll_func:
 * @func: the function to call to poll all file descriptors
 *
 * Sets the function to use for the handle polling of file descriptors
 * for the default main context.
 *
 * Deprecated: 2.2: Use g_main_context_set_poll_func() again
 */
#define g_main_set_poll_func(func)  g_main_context_set_poll_func (NULL, func)

#endif /* G_DISABLE_DEPRECATED */

/* Source manipulation by ID */
gboolean g_source_remove                     (guint          tag);
gboolean g_source_remove_by_user_data        (gpointer       user_data);
gboolean g_source_remove_by_funcs_user_data  (GSourceFuncs  *funcs,
                                              gpointer       user_data);

/* Idles, child watchers and timeouts */
guint    g_timeout_add_full         (gint            priority,
                                     guint           interval,
                                     GSourceFunc     function,
                                     gpointer        data,
                                     GDestroyNotify  notify);
guint    g_timeout_add              (guint           interval,
                                     GSourceFunc     function,
                                     gpointer        data);
guint    g_timeout_add_seconds_full (gint            priority,
                                     guint           interval,
                                     GSourceFunc     function,
                                     gpointer        data,
                                     GDestroyNotify  notify);
guint    g_timeout_add_seconds      (guint           interval,
                                     GSourceFunc     function,
                                     gpointer        data);
guint    g_child_watch_add_full     (gint            priority,
                                     GPid            pid,
                                     GChildWatchFunc function,
                                     gpointer        data,
                                     GDestroyNotify  notify);
guint    g_child_watch_add          (GPid            pid,
                                     GChildWatchFunc function,
                                     gpointer        data);
guint    g_idle_add                 (GSourceFunc     function,
                                     gpointer        data);
guint    g_idle_add_full            (gint            priority,
                                     GSourceFunc     function,
                                     gpointer        data,
                                     GDestroyNotify  notify);
gboolean g_idle_remove_by_data      (gpointer        data);

/* Hook for GClosure / GSource integration. Don't touch */
GLIB_VAR GSourceFuncs g_timeout_funcs;
GLIB_VAR GSourceFuncs g_child_watch_funcs;
GLIB_VAR GSourceFuncs g_idle_funcs;

G_END_DECLS

#endif /* __G_MAIN_H__ */
