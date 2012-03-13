/*
 * Farsight2 - Farsight Base Conference Implementation
 *
 * Copyright 2007 Collabora Ltd.
 *  @author: Philippe Kalaf <philippe.kalaf@collabora.co.uk>
 * Copyright 2007 Nokia Corp.
 *
 * fs-base-conference.h - Base implementation for Farsight Conference Gstreamer
 *                        Elements
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

#ifndef __FS_BASE_CONFERENCE_H__
#define __FS_BASE_CONFERENCE_H__

#include <gst/gst.h>

#include <gst/farsight/fs-conference-iface.h>
#include <gst/farsight/fs-session.h>
#include <gst/farsight/fs-codec.h>

G_BEGIN_DECLS

#define FS_TYPE_BASE_CONFERENCE \
  (fs_base_conference_get_type ())
#define FS_BASE_CONFERENCE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),FS_TYPE_BASE_CONFERENCE,FsBaseConference))
#define FS_BASE_CONFERENCE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),FS_TYPE_BASE_CONFERENCE,FsBaseConferenceClass))
#define FS_BASE_CONFERENCE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS((obj),FS_TYPE_BASE_CONFERENCE,FsBaseConferenceClass))
#define FS_IS_BASE_CONFERENCE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),FS_TYPE_BASE_CONFERENCE))
#define FS_IS_BASE_CONFERENCE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),FS_TYPE_BASE_CONFERENCE))
/* since 0.10.4 */
#define FS_BASE_CONFERENCE_CAST(obj) \
  ((FsBaseConference *)(obj))

typedef struct _FsBaseConference FsBaseConference;
typedef struct _FsBaseConferenceClass FsBaseConferenceClass;
typedef struct _FsBaseConferencePrivate FsBaseConferencePrivate;

/**
 * FsBaseConference
 *
 * The #FsBaseConference structure, all the members are private
 */

struct _FsBaseConference
{
  GstBin parent;

  /*< private >*/

  FsBaseConferencePrivate *priv;

  gpointer _padding[8];
};

/**
 * FsBaseConferenceClass:
 * @parent_class: Our parent
 * @new_session: allocates a new #FsSession for this conference
 * @new_participant: Creates a new #FsParticipant of the type required for
 * this conference
 *
 * The class structure of #FsBaseConference. Derived classes should override
 * all members.
 */

struct _FsBaseConferenceClass
{
  GstBinClass parent_class;

  /*< public >*/
  /* virtual methods */
  FsSession *(*new_session) (FsBaseConference *conference,
                             FsMediaType media_type,
                             GError **error);
  FsParticipant *(*new_participant) (FsBaseConference *conference,
      const gchar *cname,
      GError **error);

  /*< private >*/

  gpointer _padding[8];
};

GType fs_base_conference_get_type (void);

G_END_DECLS

#endif /* __ FS_BASE_CONFERENCE_H__ */
