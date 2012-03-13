/* GStreamer Tuner
 * Copyright (C) 2003 Ronald Bultje <rbultje@ronald.bitfreak.net>
 *
 * tuner.h: tuner interface design
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

#ifndef __GST_TUNER_H__
#define __GST_TUNER_H__

#include <gst/gst.h>
#include <gst/interfaces/tunernorm.h>
#include <gst/interfaces/tunerchannel.h>
#include <gst/interfaces/interfaces-enumtypes.h>

G_BEGIN_DECLS

#define GST_TYPE_TUNER \
  (gst_tuner_get_type ())
#define GST_TUNER(obj) \
  (GST_IMPLEMENTS_INTERFACE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_TUNER, GstTuner))
#define GST_TUNER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_TUNER, GstTunerClass))
#define GST_IS_TUNER(obj) \
  (GST_IMPLEMENTS_INTERFACE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_TUNER))
#define GST_IS_TUNER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_TUNER))
#define GST_TUNER_GET_CLASS(inst) \
  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), GST_TYPE_TUNER, GstTunerClass))

typedef struct _GstTuner GstTuner;
typedef struct _GstTunerClass GstTunerClass;

struct _GstTunerClass {
  GTypeInterface klass;

  /* virtual functions */
  const GList * (* list_channels)   (GstTuner        *tuner);
  void          (* set_channel)     (GstTuner        *tuner,
                                     GstTunerChannel *channel);
  GstTunerChannel *
                (* get_channel)     (GstTuner        *tuner);

  const GList * (* list_norms)      (GstTuner        *tuner);
  void          (* set_norm)        (GstTuner        *tuner,
                                     GstTunerNorm    *norm);
  GstTunerNorm *(* get_norm)        (GstTuner        *tuner);

  void          (* set_frequency)   (GstTuner        *tuner,
                                     GstTunerChannel *channel,
                                     gulong           frequency);
  gulong        (* get_frequency)   (GstTuner        *tuner,
                                     GstTunerChannel *channel);
  gint          (* signal_strength) (GstTuner        *tuner,
                                     GstTunerChannel *channel);

  /* signals */
  void (*channel_changed)   (GstTuner        *tuner,
                             GstTunerChannel *channel);
  void (*norm_changed)      (GstTuner        *tuner,
                             GstTunerNorm    *norm);
  void (*frequency_changed) (GstTuner        *tuner,
                             GstTunerChannel *channel,
                             gulong           frequency);
  void (*signal_changed)    (GstTuner        *tuner,
                             GstTunerChannel *channel,
                             gint             signal);

  gpointer _gst_reserved[GST_PADDING];
};

GType           gst_tuner_get_type              (void);

/* virtual class function wrappers */
const GList *   gst_tuner_list_channels         (GstTuner        *tuner);
void            gst_tuner_set_channel           (GstTuner        *tuner,
                                                 GstTunerChannel *channel);
GstTunerChannel *
                gst_tuner_get_channel           (GstTuner        *tuner);

const GList *   gst_tuner_list_norms            (GstTuner        *tuner);
void            gst_tuner_set_norm              (GstTuner        *tuner,
                                                 GstTunerNorm    *norm);
GstTunerNorm *  gst_tuner_get_norm              (GstTuner        *tuner);

void            gst_tuner_set_frequency         (GstTuner        *tuner,
                                                 GstTunerChannel *channel,
                                                 gulong           frequency);
gulong          gst_tuner_get_frequency         (GstTuner        *tuner,
                                                 GstTunerChannel *channel);
gint            gst_tuner_signal_strength       (GstTuner        *tuner,
                                                 GstTunerChannel *channel);

/* helper functions */
GstTunerNorm *  gst_tuner_find_norm_by_name     (GstTuner        *tuner,
                                                 gchar           *norm);
GstTunerChannel *gst_tuner_find_channel_by_name (GstTuner        *tuner,
                                                 gchar           *channel);

/* trigger signals */
void            gst_tuner_channel_changed       (GstTuner        *tuner,
                                                 GstTunerChannel *channel);
void            gst_tuner_norm_changed          (GstTuner        *tuner,
                                                 GstTunerNorm    *norm);
void            gst_tuner_frequency_changed     (GstTuner        *tuner,
                                                 GstTunerChannel *channel,
                                                 gulong           frequency);
void            gst_tuner_signal_changed        (GstTuner        *tuner,
                                                 GstTunerChannel *channel,
                                                 gint             signal);

G_END_DECLS

#endif /* __GST_TUNER_H__ */
