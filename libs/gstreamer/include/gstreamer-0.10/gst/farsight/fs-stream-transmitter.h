/*
 * Farsight2 - Farsight Stream Transmitter
 *
 * Copyright 2007 Collabora Ltd.
 *  @author: Olivier Crete <olivier.crete@collabora.co.uk>
 * Copyright 2007 Nokia Corp.
 *
 * fs-stream-transmitter.h - A Farsight Stream Transmitter (base implementation)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */

#ifndef __FS_STREAM_TRANSMITTER_H__
#define __FS_STREAM_TRANSMITTER_H__

#include <glib.h>
#include <glib-object.h>

#include <gst/farsight/fs-candidate.h>

G_BEGIN_DECLS

/* TYPE MACROS */
#define FS_TYPE_STREAM_TRANSMITTER \
  (fs_stream_transmitter_get_type ())
#define FS_STREAM_TRANSMITTER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), FS_TYPE_STREAM_TRANSMITTER, \
                              FsStreamTransmitter))
#define FS_STREAM_TRANSMITTER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), FS_TYPE_STREAM_TRANSMITTER, \
                           FsStreamTransmitterClass))
#define FS_IS_STREAM_TRANSMITTER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj), FS_TYPE_STREAM_TRANSMITTER))
#define FS_IS_STREAM_TRANSMITTER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), FS_TYPE_STREAM_TRANSMITTER))
#define FS_STREAM_TRANSMITTER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), FS_TYPE_STREAM_TRANSMITTER, \
                              FsStreamTransmitterClass))
#define FS_STREAM_TRANSMITTER_CAST(obj) ((FsStreamTransmitter *) (obj))

typedef struct _FsStreamTransmitter FsStreamTransmitter;
typedef struct _FsStreamTransmitterClass FsStreamTransmitterClass;
typedef struct _FsStreamTransmitterPrivate FsStreamTransmitterPrivate;

/**
 * FsStreamTransmitterClass:
 * @parent_class: Our parent
 * @set_remote_candidates: Sets the remote candidates
 * @force_remote_candidates: Forces certain remote candidates
 * @gather_local_candidates: Starts the gathering of local candidates
 * @stop: Stop the stream transmitter synchronously (does any Gst stopping
 * that needs to be done)
 *
 * You must override the add_remote_candidate in a subclass
 */

struct _FsStreamTransmitterClass
{
  GObjectClass parent_class;

  /*virtual functions */
  gboolean (*set_remote_candidates) (FsStreamTransmitter *streamtransmitter,
                                     GList *candidates, GError **error);

  gboolean (*force_remote_candidates) (FsStreamTransmitter *streamtransmitter,
      GList *remote_candidates,
      GError **error);
  gboolean (*gather_local_candidates) (FsStreamTransmitter *streamtransmitter,
                                       GError **error);
  void (*stop) (FsStreamTransmitter *streamtransmitter);

  /*< private >*/
  gpointer _padding[8];
};

/**
 * FsStreamTransmitter:
 *
 * All members are private, access them using methods and properties
 */
struct _FsStreamTransmitter
{
  GObject parent;

  /*< private >*/
  FsStreamTransmitterPrivate *priv;
  gpointer _padding[8];
};

GType fs_stream_transmitter_get_type (void);

gboolean fs_stream_transmitter_set_remote_candidates (
    FsStreamTransmitter *streamtransmitter,
    GList *candidates,
    GError **error);

gboolean fs_stream_transmitter_force_remote_candidates (
    FsStreamTransmitter *streamtransmitter,
    GList *remote_candidates,
    GError **error);

gboolean
fs_stream_transmitter_gather_local_candidates (
    FsStreamTransmitter *streamtransmitter,
    GError **error);

void fs_stream_transmitter_stop (FsStreamTransmitter *streamtransmitter);

void fs_stream_transmitter_emit_error (FsStreamTransmitter *streamtransmitter,
    gint error_no,
    const gchar *error_msg,
    const gchar *debug_msg);

G_END_DECLS

#endif /* __FS_STREAM_TRANSMITTER_H__ */
