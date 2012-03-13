/* GStreamer
 * Copyright (C) 1999 Erik Walthinsen <omega@cse.ogi.edu>
 * Copyright (C) 2005 Tim-Philipp Müller <tim centricular net>
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

#ifndef __GST_CDDA_BASE_SRC_H__
#define __GST_CDDA_BASE_SRC_H__

#include <gst/gst.h>
#include <gst/base/gstpushsrc.h>

/* must include this for backwards-compatibility so the
 * GST_TAG_CDDA_* defines are included. Remove in 0.11 */
#include <gst/tag/tag.h>

G_BEGIN_DECLS

#define GST_TYPE_CDDA_BASE_SRC            (gst_cdda_base_src_get_type())
#define GST_CDDA_BASE_SRC(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_CDDA_BASE_SRC, GstCddaBaseSrc))
#define GST_CDDA_BASE_SRC_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), GST_TYPE_CDDA_BASE_SRC, GstCddaBaseSrcClass))
#define GST_IS_CDDA_BASE_SRC(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_CDDA_BASE_SRC))
#define GST_IS_CDDA_BASE_SRC_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), GST_TYPE_CDDA_BASE_SRC))
#define GST_CDDA_BASE_SRC_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_CDDA_BASE_SRC, GstCddaBaseSrcClass))

typedef struct _GstCddaBaseSrc GstCddaBaseSrc;
typedef struct _GstCddaBaseSrcClass GstCddaBaseSrcClass;
typedef struct _GstCddaBaseSrcTrack GstCddaBaseSrcTrack;

/**
 * GstCddaBaseSrcMode:
 * @GST_CDDA_BASE_SRC_MODE_NORMAL     : each single track is a stream
 * @GST_CDDA_BASE_SRC_MODE_CONTINUOUS : the entire disc is a single stream
 *
 * Mode in which the CD audio source operates. Influences timestamping,
 * EOS handling and seeking.
 */
typedef enum {
  GST_CDDA_BASE_SRC_MODE_NORMAL,          /* stream = one track  */
  GST_CDDA_BASE_SRC_MODE_CONTINUOUS       /* stream = whole disc */
} GstCddaBaseSrcMode;

#define GST_TYPE_CDDA_BASE_SRC_MODE          (gst_cdda_base_src_mode_get_type ())

/**
 * GstCddaBaseSrcTrack:
 * @is_audio: Whether this is an audio track
 * @num: Track number in TOC (usually starts from 1, but not always)
 * @start: The first sector of this track (LBA)
 * @end: The last sector of this track (LBA)
 * @tags: Track-specific tags (e.g. from cd-text information), or NULL
 *
 * CD track abstraction to communicate TOC entries to the base class.
 */
struct _GstCddaBaseSrcTrack {
  gboolean     is_audio;      /* TRUE if this is an audio track             */
  guint        num;           /* real track number (usually starts from 1)  */
  guint        start;         /* first sector of track (LBA, not LSN!)      */
  guint        end;           /* last sector of track  (LBA, not LSN!)      */
  GstTagList  *tags;          /* NULL or tags for track (e.g. from cd-text) */

  /*< private >*/
  guint        _gst_reserved1[GST_PADDING/2];
  gpointer     _gst_reserved2[GST_PADDING/2];
};

struct _GstCddaBaseSrc {
  GstPushSrc            pushsrc;

  /*< protected >*/ /* for use by sub-classes only */
  GstTagList           *tags;            /* tags that apply to all tracks   */

  /*< private >*/
  GstCddaBaseSrcMode    mode;

  gchar                *device;

  guint                 num_tracks;
  guint                 num_all_tracks;
  GstCddaBaseSrcTrack  *tracks;

  gint                  cur_track;       /* current track (starting from 0) */
  gint                  prev_track;      /* current track last time         */
  gint                  cur_sector;      /* current sector                  */
  gint                  seek_sector;     /* -1 or sector to seek to         */

  gint                  uri_track;
  gchar                *uri;

  guint32               discid;          /* cddb disc id (for unit test)    */
  gchar                 mb_discid[32];   /* musicbrainz discid              */

  GstIndex             *index;
  gint                  index_id;

  gint                  toc_offset;
  gboolean              toc_bias;

  /*< private >*/
  guint                 _gst_reserved1[GST_PADDING/2];
  gpointer              _gst_reserved2[GST_PADDING/2];
};

struct _GstCddaBaseSrcClass {
  GstPushSrcClass pushsrc_class;

  /* open/close the CD device */
  gboolean    (*open)               (GstCddaBaseSrc *src, const gchar *device);
  void        (*close)              (GstCddaBaseSrc *src);

  /* read one sector (LBA) */
  GstBuffer * (*read_sector)        (GstCddaBaseSrc *src, gint sector);

  /* return default device or NULL (optional) */
  gchar *     (*get_default_device) (GstCddaBaseSrc *src);

  /* return NULL-terminated string array of CD devices, or NULL (optional) */
  gchar **    (*probe_devices)      (GstCddaBaseSrc *src);

  /*< private >*/
  gpointer       _gst_reserved[GST_PADDING];
};

GType    gst_cdda_base_src_get_type (void);
GType    gst_cdda_base_src_mode_get_type (void);

gboolean gst_cdda_base_src_add_track (GstCddaBaseSrc      * src,
                                      GstCddaBaseSrcTrack * track);

#if 0
/*
 * GST_TAG_CDDA_TRACK_TAGS:
 *
 * Tag details for all available tracks
 * FiXME: find out which type we want for this!
 */
#define GST_TAG_CDDA_TRACK_TAGS               "track-tags"
#endif

G_END_DECLS

#endif /* __GST_CDDA_BASE_SRC_H__ */

