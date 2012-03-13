/*
 * Farsight2 - Farsight Stream
 *
 * Copyright 2007 Collabora Ltd.
 *  @author: Philippe Kalaf <philippe.kalaf@collabora.co.uk>
 * Copyright 2007 Nokia Corp.
 *
 * fs-stream.h - A Farsight Stream (base implementation)
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

#ifndef __FS_STREAM_H__
#define __FS_STREAM_H__

#include <glib.h>
#include <glib-object.h>

#include <gst/farsight/fs-candidate.h>
#include <gst/farsight/fs-codec.h>

G_BEGIN_DECLS

/**
 * FsStreamDirection:
 * @FS_DIRECTION_NONE: No direction specified
 * @FS_DIRECTION_SEND: Send only
 * @FS_DIRECTION_RECV: Receive only
 * @FS_DIRECTION_BOTH: Send and receive
 *
 * An enum for specifying the direction of a stream
 *
 */
typedef enum
{
  FS_DIRECTION_NONE = 0,
  FS_DIRECTION_SEND = 1<<0,
  FS_DIRECTION_RECV = 1<<1,
  FS_DIRECTION_BOTH = FS_DIRECTION_SEND | FS_DIRECTION_RECV
} FsStreamDirection;

/**
 * FsStreamState:
 * @FS_STREAM_STATE_FAILED: connectivity checks have been completed,
 *                          but connectivity was not established
 * @FS_STREAM_STATE_DISCONNECTED: no activity scheduled
 * @FS_STREAM_STATE_GATHERING: gathering local candidates
 * @FS_STREAM_STATE_CONNECTING: establishing connectivity
 * @FS_STREAM_STATE_CONNECTED: at least one working candidate pair
 * @FS_STREAM_STATE_READY: ICE concluded, candidate pair selection is now final
 *
 * These are the possible states of a stream, a simple multicast stream
 * could only be in "disconnected" or "ready" state.
 * An stream using an ICE transmitter would use all of these.
 */

typedef enum
{
  FS_STREAM_STATE_FAILED,
  FS_STREAM_STATE_DISCONNECTED,
  FS_STREAM_STATE_GATHERING,
  FS_STREAM_STATE_CONNECTING,
  FS_STREAM_STATE_CONNECTED,
  FS_STREAM_STATE_READY
} FsStreamState;

/* TYPE MACROS */
#define FS_TYPE_STREAM \
  (fs_stream_get_type ())
#define FS_STREAM(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), FS_TYPE_STREAM, FsStream))
#define FS_STREAM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), FS_TYPE_STREAM, FsStreamClass))
#define FS_IS_STREAM(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj), FS_TYPE_STREAM))
#define FS_IS_STREAM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), FS_TYPE_STREAM))
#define FS_STREAM_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), FS_TYPE_STREAM, FsStreamClass))
#define FS_STREAM_CAST(obj) ((FsStream *) (obj))

typedef struct _FsStream FsStream;
typedef struct _FsStreamClass FsStreamClass;
typedef struct _FsStreamPrivate FsStreamPrivate;


/**
 * FsStreamClass:
 * @parent_class: Our parent
 * @set_remote_candidates: Set sthe remote candidates
 * @force_remote_candidates: Forces certain remote candidates
 * @set_remote_codecs: Sets the list of remote codecs
 * @add_id: Add a known id to be associated with this stream
 *
 * You must override add_remote_candidate in a subclass.
 * If you have to negotiate codecs, then you must override set_remote_codecs too
 */

struct _FsStreamClass
{
  GObjectClass parent_class;

  /*virtual functions */
  gboolean (*set_remote_candidates) (FsStream *stream,
                                     GList *candidates,
                                     GError **error);

  gboolean (*force_remote_candidates) (FsStream *stream,
      GList *remote_candidates,
      GError **error);

  gboolean (*set_remote_codecs) (FsStream *stream,
                                 GList *remote_codecs, GError **error);

  void (*add_id) (FsStream *stream,
                  guint id);

  /*< private >*/
  gpointer _padding[7];
};

/**
 * FsStream:
 *
 * All members are private, access them using methods and properties
 */
struct _FsStream
{
  GObject parent;

  /*< private >*/

  FsStreamPrivate *priv;

  gpointer _padding[8];
};

GType fs_stream_get_type (void);

gboolean fs_stream_set_remote_candidates (FsStream *stream,
                                          GList *candidates,
                                          GError **error);

gboolean fs_stream_force_remote_candidates (FsStream *stream,
    GList *remote_candidates,
    GError **error);

gboolean fs_stream_set_remote_codecs (FsStream *stream,
                                      GList *remote_codecs, GError **error);

void fs_stream_add_id (FsStream *stream, guint id);

void fs_stream_emit_error (FsStream *stream,
    gint error_no,
    const gchar *error_msg,
    const gchar *debug_msg);

void fs_stream_emit_src_pad_added (FsStream *stream,
    GstPad *pad,
    FsCodec *codec);

GstIterator *fs_stream_get_src_pads_iterator (FsStream *stream);


G_END_DECLS

#endif /* __FS_STREAM_H__ */
