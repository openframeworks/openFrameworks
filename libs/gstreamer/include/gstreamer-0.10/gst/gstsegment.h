/* GStreamer
 * Copyright (C) 2005 Wim Taymans <wim@fluendo.com>
 *
 * gstsegment.h: Header for GstSegment subsystem
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


#ifndef __GST_SEGMENT_H__
#define __GST_SEGMENT_H__

#include <gst/gstevent.h>
#include <gst/gstformat.h>

G_BEGIN_DECLS

#define GST_TYPE_SEGMENT             (gst_segment_get_type())

typedef struct _GstSegment GstSegment;

/**
 * GstSegment:
 * @rate: the rate of the segment
 * @abs_rate: absolute value of @rate
 * @format: the format of the segment values
 * @flags: flags for this segment
 * @start: the start of the segment
 * @stop: the stop of the segment
 * @time: the stream time of the segment
 * @accum: accumulated segment
 * @last_stop: last known stop time
 * @duration: total duration of segment
 * @applied_rate: the already applied rate to the segment
 *
 * A helper structure that holds the configured region of
 * interest in a media file.
 */
struct _GstSegment {
  /*< public >*/
  gdouble        rate;
  gdouble        abs_rate;
  GstFormat      format;
  GstSeekFlags   flags;
  gint64         start;
  gint64         stop;
  gint64         time;
  gint64         accum;

  gint64         last_stop;
  gint64         duration;

  /* API added 0.10.6 */
  gdouble        applied_rate;

  /*< private >*/
  /*gpointer _gst_reserved[GST_PADDING-2];*/
  guint8 _gst_reserved[(sizeof (gpointer) * GST_PADDING) - sizeof (gdouble)];
};

GType        gst_segment_get_type            (void);

GstSegment * gst_segment_new                 (void);
GstSegment * gst_segment_copy                (GstSegment *segment);
void         gst_segment_free                (GstSegment *segment);

void         gst_segment_init                (GstSegment *segment, GstFormat format);

void         gst_segment_set_duration        (GstSegment *segment, GstFormat format, gint64 duration);
void         gst_segment_set_last_stop       (GstSegment *segment, GstFormat format, gint64 position);

void         gst_segment_set_seek            (GstSegment *segment, gdouble rate,
                                              GstFormat format, GstSeekFlags flags,
                                              GstSeekType start_type, gint64 start,
                                              GstSeekType stop_type, gint64 stop,
                                              gboolean *update);

void         gst_segment_set_newsegment      (GstSegment *segment, gboolean update, gdouble rate,
                                              GstFormat format, gint64 start, gint64 stop, gint64 time);
void         gst_segment_set_newsegment_full (GstSegment *segment, gboolean update, gdouble rate,
                                              gdouble applied_rate, GstFormat format, gint64 start,
                                              gint64 stop, gint64 time);

gint64       gst_segment_to_stream_time      (GstSegment *segment, GstFormat format, gint64 position);
gint64       gst_segment_to_running_time     (GstSegment *segment, GstFormat format, gint64 position);
gint64       gst_segment_to_position         (GstSegment *segment, GstFormat format, gint64 running_time);

gboolean     gst_segment_clip                (GstSegment *segment, GstFormat format, gint64 start,
                                              gint64 stop, gint64 *clip_start, gint64 *clip_stop);

gboolean     gst_segment_set_running_time    (GstSegment *segment, GstFormat format, gint64 running_time);


G_END_DECLS

#endif /* __GST_SEGMENT_H__ */
