/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wtay@chello.be>
 *
 * gsttrace.h: Header for tracing functions (deprecated)
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


#ifndef __GST_TRACE_H__
#define __GST_TRACE_H__

#include <glib.h>

G_BEGIN_DECLS

/**
 * GstAllocTraceFlags:
 * @GST_ALLOC_TRACE_LIVE: Trace number of non-freed memory
 * @GST_ALLOC_TRACE_MEM_LIVE: trace pointers of unfreed memory
 *
 * Flags indicating which tracing feature to enable.
 */
typedef enum {
  GST_ALLOC_TRACE_LIVE		= (1 << 0),
  GST_ALLOC_TRACE_MEM_LIVE	= (1 << 1)
} GstAllocTraceFlags;

typedef struct _GstAllocTrace 	GstAllocTrace;

/**
 * GstAllocTrace:
 * @name: The name of the tracing object
 * @flags: Flags for this object
 * @live: counter for live memory
 * @mem_live: list with pointers to unfreed memory
 *
 * The main tracing object
 */
struct _GstAllocTrace {
  gchar		*name;
  gint		 flags;

  gint		 live;
  GSList	*mem_live;
};

#ifndef GST_DISABLE_TRACE

typedef struct _GstTrace 	GstTrace;
typedef struct _GstTraceEntry 	GstTraceEntry;

/**
 * GstTrace:
 *
 * Opaque #GstTrace structure.
 */
struct _GstTrace {
  /*< private >*/
  /* where this trace is going */
  gchar *filename;
  int fd;

  /* current buffer, size, head offset */
  GstTraceEntry *buf;
  gint bufsize;
  gint bufoffset;
};

struct _GstTraceEntry {
  gint64 timestamp;
  guint32 sequence;
  guint32 data;
  gchar message[112];
};

GstTrace*	gst_trace_new			(const gchar *filename, gint size);

void 		gst_trace_destroy		(GstTrace *trace);
void 		gst_trace_flush			(GstTrace *trace);
void 		gst_trace_text_flush		(GstTrace *trace);
/**
 * gst_trace_get_size:
 * @trace: a #GstTrace
 *
 * Retrieve the buffer size of @trace.
 */
#define 	gst_trace_get_size(trace) 	((trace)->bufsize)
/**
 * gst_trace_get_offset:
 * @trace: a #GstTrace
 *
 * Retrieve the current buffer offset of @trace.
 */
#define 	gst_trace_get_offset(trace) 	((trace)->bufoffset)
/**
 * gst_trace_get_remaining:
 * @trace: a #GstTrace
 *
 * Retrieve the remaining size in the @trace buffer.
 */
#define 	gst_trace_get_remaining(trace) 	((trace)->bufsize - (trace)->bufoffset)
void 		gst_trace_set_default		(GstTrace *trace);

void 		_gst_trace_add_entry		(GstTrace *trace, guint32 seq,
						 guint32 data, gchar *msg);

void 		gst_trace_read_tsc		(gint64 *dst);


extern GStaticMutex     _gst_trace_mutex;

gboolean		gst_alloc_trace_available	(void);
G_CONST_RETURN GList*	gst_alloc_trace_list		(void);
GstAllocTrace*		_gst_alloc_trace_register	(const gchar *name);

int			gst_alloc_trace_live_all	(void);
void			gst_alloc_trace_print_all	(void);
void			gst_alloc_trace_print_live	(void);
void			gst_alloc_trace_set_flags_all	(GstAllocTraceFlags flags);

GstAllocTrace*		gst_alloc_trace_get		(const gchar *name);
void			gst_alloc_trace_print		(const GstAllocTrace *trace);
void			gst_alloc_trace_set_flags	(GstAllocTrace *trace, GstAllocTraceFlags flags);


#ifndef GST_DISABLE_ALLOC_TRACE
/**
 * gst_alloc_trace_register:
 * @name: The name of the tracer object
 *
 * Register a new alloc tracer with the given name
 */
#define	gst_alloc_trace_register(name) _gst_alloc_trace_register (name);

/**
 * gst_alloc_trace_new:
 * @trace: The tracer to use
 * @mem: The memory allocated
 *
 * Use the tracer to trace a new memory allocation
 */
#define	gst_alloc_trace_new(trace, mem) 		\
G_STMT_START {						\
  if (G_UNLIKELY ((trace)->flags)) {                    \
    g_static_mutex_lock (&_gst_trace_mutex);            \
    if ((trace)->flags & GST_ALLOC_TRACE_LIVE) 		\
      (trace)->live++;					\
    if ((trace)->flags & GST_ALLOC_TRACE_MEM_LIVE) 	\
      (trace)->mem_live = 				\
        g_slist_prepend ((trace)->mem_live, mem);	\
    g_static_mutex_unlock (&_gst_trace_mutex);          \
  }                                                     \
} G_STMT_END

/**
 * gst_alloc_trace_free:
 * @trace: The tracer to use
 * @mem: The memory that is freed
 *
 * Trace a memory free operation
 */
#define	gst_alloc_trace_free(trace, mem) 		\
G_STMT_START {						\
  if (G_UNLIKELY ((trace)->flags)) {                    \
    g_static_mutex_lock (&_gst_trace_mutex);            \
    if ((trace)->flags & GST_ALLOC_TRACE_LIVE) 		\
      (trace)->live--;					\
    if ((trace)->flags & GST_ALLOC_TRACE_MEM_LIVE) 	\
      (trace)->mem_live = 				\
        g_slist_remove ((trace)->mem_live, mem); 	\
    g_static_mutex_unlock (&_gst_trace_mutex);          \
  }                                                     \
} G_STMT_END

#else
#define	gst_alloc_trace_register(name) (NULL)
#define	gst_alloc_trace_new(trace, mem)
#define	gst_alloc_trace_free(trace, mem)
#endif


extern gint _gst_trace_on;
/**
 * gst_trace_add_entry:
 * @trace: a #GstTrace
 * @seq: a sequence number
 * @data: the data to trace
 * @msg: the trace message
 *
 * Add an entry to @trace with sequence number @seq, @data and @msg.
 * If @trace is NULL, the entry will be added to the default #GstTrace.
 */
#define gst_trace_add_entry(trace,seq,data,msg) \
  if (_gst_trace_on) { \
    _gst_trace_add_entry(trace,(guint32)seq,(guint32)data,msg); \
  }

#else /* GST_DISABLE_TRACE */

#if defined __GNUC__ && __GNUC__ >= 3
#pragma GCC poison 	gst_trace_new
#pragma GCC poison	gst_trace_destroy
#pragma GCC poison 	gst_trace_flush
#pragma GCC poison	gst_trace_text_flush
#pragma GCC poison 	gst_trace_get_size
#pragma GCC poison 	gst_trace_get_offset
#pragma GCC poison 	gst_trace_get_remaining
#pragma GCC poison 	gst_trace_set_default
#pragma GCC poison 	_gst_trace_add_entry
#pragma GCC poison 	gst_trace_read_tsc
#endif

#define		gst_alloc_trace_register(name)	(NULL)
#define		gst_alloc_trace_new(trace, mem)
#define		gst_alloc_trace_free(trace, mem)

#define		gst_alloc_trace_available()	(FALSE)
#define		gst_alloc_trace_list()		(NULL)
#define		_gst_alloc_trace_register(name)	(NULL)

#define		gst_alloc_trace_live_all()	(0)
#define		gst_alloc_trace_print_all()
#define		gst_alloc_trace_print_live()
#define		gst_alloc_trace_set_flags_all(flags)

#define		gst_alloc_trace_get(name)	(NULL)
#define		gst_alloc_trace_print(trace)
#define		gst_alloc_trace_set_flags(trace,flags)

#define         gst_trace_add_entry(trace,seq,data,msg)

#endif /* GST_DISABLE_TRACE */

G_END_DECLS

#endif /* __GST_TRACE_H__ */
