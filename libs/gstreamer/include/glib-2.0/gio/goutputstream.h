/* GIO - GLib Input, Output and Streaming Library
 *
 * Copyright (C) 2006-2007 Red Hat, Inc.
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
 *
 * Author: Alexander Larsson <alexl@redhat.com>
 */

#if !defined (__GIO_GIO_H_INSIDE__) && !defined (GIO_COMPILATION)
#error "Only <gio/gio.h> can be included directly."
#endif

#ifndef __G_OUTPUT_STREAM_H__
#define __G_OUTPUT_STREAM_H__

#include <gio/giotypes.h>

G_BEGIN_DECLS

#define G_TYPE_OUTPUT_STREAM         (g_output_stream_get_type ())
#define G_OUTPUT_STREAM(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), G_TYPE_OUTPUT_STREAM, GOutputStream))
#define G_OUTPUT_STREAM_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), G_TYPE_OUTPUT_STREAM, GOutputStreamClass))
#define G_IS_OUTPUT_STREAM(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), G_TYPE_OUTPUT_STREAM))
#define G_IS_OUTPUT_STREAM_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), G_TYPE_OUTPUT_STREAM))
#define G_OUTPUT_STREAM_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), G_TYPE_OUTPUT_STREAM, GOutputStreamClass))

/**
 * GOutputStream:
 *
 * Base class for writing output.
 *
 * All classes derived from GOutputStream should implement synchronous
 * writing, splicing, flushing and closing streams, but may implement
 * asynchronous versions.
 **/
typedef struct _GOutputStreamClass    GOutputStreamClass;
typedef struct _GOutputStreamPrivate  GOutputStreamPrivate;

struct _GOutputStream
{
  GObject parent_instance;

  /*< private >*/
  GOutputStreamPrivate *priv;
};


struct _GOutputStreamClass
{
  GObjectClass parent_class;

  /* Sync ops: */

  gssize      (* write_fn)      (GOutputStream            *stream,
                                 const void               *buffer,
                                 gsize                     count,
                                 GCancellable             *cancellable,
                                 GError                  **error);
  gssize      (* splice)        (GOutputStream            *stream,
                                 GInputStream             *source,
                                 GOutputStreamSpliceFlags  flags,
                                 GCancellable             *cancellable,
                                 GError                  **error);
  gboolean    (* flush)	        (GOutputStream            *stream,
                                 GCancellable             *cancellable,
                                 GError                  **error);
  gboolean    (* close_fn)      (GOutputStream            *stream,
                                 GCancellable             *cancellable,
                                 GError                  **error);

  /* Async ops: (optional in derived classes) */

  void        (* write_async)   (GOutputStream            *stream,
                                 const void               *buffer,
                                 gsize                     count,
                                 int                       io_priority,
                                 GCancellable             *cancellable,
                                 GAsyncReadyCallback       callback,
                                 gpointer                  user_data);
  gssize      (* write_finish)  (GOutputStream            *stream,
                                 GAsyncResult             *result,
                                 GError                  **error);
  void        (* splice_async)  (GOutputStream            *stream,
                                 GInputStream             *source,
                                 GOutputStreamSpliceFlags  flags,
                                 int                       io_priority,
                                 GCancellable             *cancellable,
                                 GAsyncReadyCallback       callback,
                                 gpointer                  data);
  gssize      (* splice_finish) (GOutputStream            *stream,
                                 GAsyncResult             *result,
                                 GError                  **error);
  void        (* flush_async)   (GOutputStream            *stream,
                                 int                       io_priority,
                                 GCancellable             *cancellable,
                                 GAsyncReadyCallback       callback,
                                 gpointer                  user_data);
  gboolean    (* flush_finish)  (GOutputStream            *stream,
                                 GAsyncResult             *result,
                                 GError                  **error);
  void        (* close_async)   (GOutputStream            *stream,
                                 int                       io_priority,
                                 GCancellable             *cancellable,
                                 GAsyncReadyCallback       callback,
                                 gpointer                  user_data);
  gboolean    (* close_finish)  (GOutputStream            *stream,
                                 GAsyncResult             *result,
                                 GError                  **error);

  /*< private >*/
  /* Padding for future expansion */
  void (*_g_reserved1) (void);
  void (*_g_reserved2) (void);
  void (*_g_reserved3) (void);
  void (*_g_reserved4) (void);
  void (*_g_reserved5) (void);
  void (*_g_reserved6) (void);
  void (*_g_reserved7) (void);
  void (*_g_reserved8) (void);
};

GType    g_output_stream_get_type      (void) G_GNUC_CONST;

gssize   g_output_stream_write         (GOutputStream             *stream,
					const void                *buffer,
					gsize                      count,
					GCancellable              *cancellable,
					GError                   **error);
gboolean g_output_stream_write_all     (GOutputStream             *stream,
					const void                *buffer,
					gsize                      count,
					gsize                     *bytes_written,
					GCancellable              *cancellable,
					GError                   **error);
gssize   g_output_stream_splice        (GOutputStream             *stream,
					GInputStream              *source,
					GOutputStreamSpliceFlags   flags,
					GCancellable              *cancellable,
					GError                   **error);
gboolean g_output_stream_flush         (GOutputStream             *stream,
					GCancellable              *cancellable,
					GError                   **error);
gboolean g_output_stream_close         (GOutputStream             *stream,
					GCancellable              *cancellable,
					GError                   **error);
void     g_output_stream_write_async   (GOutputStream             *stream,
					const void                *buffer,
					gsize                      count,
					int                        io_priority,
					GCancellable              *cancellable,
					GAsyncReadyCallback        callback,
					gpointer                   user_data);
gssize   g_output_stream_write_finish  (GOutputStream             *stream,
					GAsyncResult              *result,
					GError                   **error);
void     g_output_stream_splice_async  (GOutputStream             *stream,
					GInputStream              *source,
					GOutputStreamSpliceFlags   flags,
					int                        io_priority,
					GCancellable              *cancellable,
					GAsyncReadyCallback        callback,
					gpointer                   user_data);
gssize   g_output_stream_splice_finish (GOutputStream             *stream,
					GAsyncResult              *result,
					GError                   **error);
void     g_output_stream_flush_async   (GOutputStream             *stream,
					int                        io_priority,
					GCancellable              *cancellable,
					GAsyncReadyCallback        callback,
					gpointer                   user_data);
gboolean g_output_stream_flush_finish  (GOutputStream             *stream,
					GAsyncResult              *result,
					GError                   **error);
void     g_output_stream_close_async   (GOutputStream             *stream,
					int                        io_priority,
					GCancellable              *cancellable,
					GAsyncReadyCallback        callback,
					gpointer                   user_data);
gboolean g_output_stream_close_finish  (GOutputStream             *stream,
					GAsyncResult              *result,
					GError                   **error);

gboolean g_output_stream_is_closed     (GOutputStream             *stream);
gboolean g_output_stream_is_closing    (GOutputStream             *stream);
gboolean g_output_stream_has_pending   (GOutputStream             *stream);
gboolean g_output_stream_set_pending   (GOutputStream             *stream,
					GError                   **error);
void     g_output_stream_clear_pending (GOutputStream             *stream);


G_END_DECLS

#endif /* __G_OUTPUT_STREAM_H__ */
