/*
 * Farsight2 - Farsight Participant
 *
 * Copyright 2007 Collabora Ltd.
 *  @author: Philippe Kalaf <philippe.kalaf@collabora.co.uk>
 * Copyright 2007 Nokia Corp.
 *
 * fs-participant.h - A Farsight Participant gobject (base implementation)
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

#ifndef __FS_PARTICIPANT_H__
#define __FS_PARTICIPANT_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

/* TYPE MACROS */
#define FS_TYPE_PARTICIPANT \
  (fs_participant_get_type ())
#define FS_PARTICIPANT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), FS_TYPE_PARTICIPANT, FsParticipant))
#define FS_PARTICIPANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), FS_TYPE_PARTICIPANT, FsParticipantClass))
#define FS_IS_PARTICIPANT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj), FS_TYPE_PARTICIPANT))
#define FS_IS_PARTICIPANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), FS_TYPE_PARTICIPANT))
#define FS_PARTICIPANT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), FS_TYPE_PARTICIPANT, FsParticipantClass))
#define FS_PARTICIPANT_CAST(obj) ((FsParticipant *) (obj))

typedef struct _FsParticipant FsParticipant;
typedef struct _FsParticipantClass FsParticipantClass;
typedef struct _FsParticipantPrivate FsParticipantPrivate;

/**
 * FsParticipantClass:
 * @parent_class: Our parent
 *
 * The FsParticipant class has no virtual methods to implement,
 * but you may want to override the properties or attach more date to it
 */

struct _FsParticipantClass
{
  GObjectClass parent_class;

  /* virtual functions */

  /*< private >*/
  FsParticipantPrivate *priv;
  gpointer _padding[8];
};

/**
 * FsParticipant:
 *
 * All members are private (access them using the properties)
 */
struct _FsParticipant
{
  GObject parent;

  /*< private >*/

  GMutex *mutex;

  FsParticipantPrivate *priv;

  gpointer _padding[8];
};

/**
 * FS_PARTICIPANT_DATA_LOCK
 * @participant: A #FsParticipant
 *
 * Locks the participant for data set with g_object_set_data() or
 * g_object_set_qdata().
 */

#define FS_PARTICIPANT_DATA_LOCK(participant) \
  g_mutex_lock ((participant)->mutex)

/**
 * FS_PARTICIPANT_DATA_UNLOCK
 * @participant: A #FsParticipant
 *
 * Unlocks the participant for data set with g_object_set_data() or
 * g_object_set_qdata().
 */

#define FS_PARTICIPANT_DATA_UNLOCK(participant) \
  g_mutex_unlock ((participant)->mutex)

GType fs_participant_get_type (void);

G_END_DECLS

#endif /* __FS_PARTICIPANT_H__ */
