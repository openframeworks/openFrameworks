/*
 * Farsight2 - Farsight Transmitter
 *
 * Copyright 2007 Collabora Ltd.
 *  @author: Olivier Crete <olivier.crete@collabora.co.uk>
 * Copyright 2007 Nokia Corp.
 *
 * fs-transmitter.h - A Farsight Transmitter (base implementation)
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

#ifndef __FS_TRANSMITTER_H__
#define __FS_TRANSMITTER_H__

#include <gst/gst.h>

#include <gst/farsight/fs-participant.h>
#include <gst/farsight/fs-session.h>
#include <gst/farsight/fs-stream-transmitter.h>

G_BEGIN_DECLS

/* TYPE MACROS */
#define FS_TYPE_TRANSMITTER \
  (fs_transmitter_get_type ())
#define FS_TRANSMITTER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), FS_TYPE_TRANSMITTER, FsTransmitter))
#define FS_TRANSMITTER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), FS_TYPE_TRANSMITTER, FsTransmitterClass))
#define FS_IS_TRANSMITTER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj), FS_TYPE_TRANSMITTER))
#define FS_IS_TRANSMITTER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), FS_TYPE_TRANSMITTER))
#define FS_TRANSMITTER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), FS_TYPE_TRANSMITTER, FsTransmitterClass))
#define FS_TRANSMITTER_CAST(obj) ((FsTransmitter *) (obj))

typedef struct _FsTransmitter FsTransmitter;
typedef struct _FsTransmitterClass FsTransmitterClass;
typedef struct _FsTransmitterPrivate FsTransmitterPrivate;

/**
 * FsTransmitterClass:
 * @parent_class: Our parent
 * @new_stream_transmitter: Creates a new #FsStreamTransmitter
 * @get_stream_transmitter_type: Returns the #GType of the stream transmitter
 *  created by this class (useful for bindings)
 *
 * You must override both methods in a subclass.
 */

struct _FsTransmitterClass
{
  GObjectClass parent_class;

  /*virtual functions */
  FsStreamTransmitter *(*new_stream_transmitter) (FsTransmitter *transmitter,
                                                  FsParticipant *participant,
                                                  guint n_parameters,
                                                  GParameter *parameters,
                                                  GError **error);
  GType (*get_stream_transmitter_type) (FsTransmitter *transmitter);

  /*< private >*/
  gpointer _padding[8];
};

/**
 * FsTransmitter:
 *
 * All members are private, access them using methods and properties
 */
struct _FsTransmitter
{
  GObject parent;

  /*< private >*/
  FsTransmitterPrivate *priv;

  /* This parameter should only be set by the construction methods
   * of the subclasses
   */
  GError *construction_error;

  gpointer _padding[8];
};

GType fs_transmitter_get_type (void);

FsStreamTransmitter *fs_transmitter_new_stream_transmitter (
    FsTransmitter *transmitter, FsParticipant *participant,
    guint n_parameters, GParameter *parameters, GError **error);

FsTransmitter *fs_transmitter_new (const gchar *type,
    guint components,
    guint tos,
    GError **error);

GType fs_transmitter_get_stream_transmitter_type (FsTransmitter *transmitter);

void fs_transmitter_emit_error (FsTransmitter *transmitter,
    gint error_no,
    const gchar *error_msg,
    const gchar *debug_msg);

char **fs_transmitter_list_available (void);

GstElement *
fs_transmitter_get_recvonly_filter (FsTransmitter *transmitter,
    guint component);

G_END_DECLS

#endif /* __FS_TRANSMITTER_H__ */
