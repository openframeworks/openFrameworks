/* GStreamer Tuner
 * Copyright (C) 2003 Ronald Bultje <rbultje@ronald.bitfreak.net>
 *
 * tunerchannel.h: tuner channel object design
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

#ifndef __GST_TUNER_CHANNEL_H__
#define __GST_TUNER_CHANNEL_H__

#include <gst/gst.h>

G_BEGIN_DECLS

#define GST_TYPE_TUNER_CHANNEL \
  (gst_tuner_channel_get_type ())
#define GST_TUNER_CHANNEL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_TUNER_CHANNEL, \
                               GstTunerChannel))
#define GST_TUNER_CHANNEL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_TUNER_CHANNEL, \
                            GstTunerChannelClass))
#define GST_IS_TUNER_CHANNEL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_TUNER_CHANNEL))
#define GST_IS_TUNER_CHANNEL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_TUNER_CHANNEL))

typedef struct _GstTunerChannel GstTunerChannel;
typedef struct _GstTunerChannelClass GstTunerChannelClass;

/**
 * GstTunerChannelFlags:
 * @GST_TUNER_CHANNEL_INPUT: The channel is for input
 * @GST_TUNER_CHANNEL_OUTPUT: The channel is for output
 * @GST_TUNER_CHANNEL_FREQUENCY: The channel has a frequency setting
 *                               and signal strength.
 * @GST_TUNER_CHANNEL_AUDIO: The channel carries audio.
 * 
 * An enumeration for flags indicating the available capabilities
 * of a #GstTunerChannel.
 */
typedef enum {
  GST_TUNER_CHANNEL_INPUT     = (1<<0),
  GST_TUNER_CHANNEL_OUTPUT    = (1<<1),
  GST_TUNER_CHANNEL_FREQUENCY = (1<<2),
  GST_TUNER_CHANNEL_AUDIO     = (1<<3)
} GstTunerChannelFlags;

/**
 * GST_TUNER_CHANNEL_HAS_FLAG:
 * @channel: A #GstTunerChannel
 * @flag: The flag to check for
 * 
 * Macro to check if the given flag is set on a channel
 */
#define GST_TUNER_CHANNEL_HAS_FLAG(channel, flag) \
  ((channel)->flags & flag)

/**
 * GstTunerChannel:
 * @label: A string containing a descriptive name for this channel
 * @flags: A set of #GstTunerChannelFlags for this channel
 * @freq_multiplicator: The step size (in Hz) for the frequency setting.
 * @min_frequency: Minimum valid frequency setting (in Hz).
 * @max_frequency: Maximum valid frequency setting (in Hz).
 * @min_signal: Minimum reported signal strength value.
 * @max_signal: Maximum reported signal strength value.
 */
struct _GstTunerChannel {
  GObject              parent;

  /*< public >*/
  gchar               *label;
  GstTunerChannelFlags flags;
  gfloat               freq_multiplicator;
  gulong               min_frequency;
  gulong               max_frequency;
  gint                 min_signal;
  gint                 max_signal;
};

struct _GstTunerChannelClass {
  GObjectClass parent;

  /*< private >*/
  /* signals */
  void (*frequency_changed) (GstTunerChannel *channel,
                             gulong           frequency);
  void (*signal_changed)    (GstTunerChannel *channel,
                             gint             signal);

  gpointer _gst_reserved[GST_PADDING];
};

GType           gst_tuner_channel_get_type      (void);

G_END_DECLS

#endif /* __GST_TUNER_CHANNEL_H__ */
