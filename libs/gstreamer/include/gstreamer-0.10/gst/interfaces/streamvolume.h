/* GStreamer StreamVolume
 * Copyright (C) 2009 Sebastian Dröge <sebastian.droege@collabora.co.uk>
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

#ifndef __GST_STREAM_VOLUME_H__
#define __GST_STREAM_VOLUME_H__

#include <gst/gst.h>

G_BEGIN_DECLS

#define GST_TYPE_STREAM_VOLUME \
  (gst_stream_volume_get_type ())
#define GST_STREAM_VOLUME(obj) \
  (GST_IMPLEMENTS_INTERFACE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_STREAM_VOLUME, GstStreamVolume))
#define GST_STREAM_VOLUME_INTERFACE(iface) \
  (G_TYPE_CHECK_INTERFACE_CAST ((iface), GST_TYPE_STREAM_VOLUME, GstStreamVolumeInterface))
#define GST_IS_STREAM_VOLUME(obj) \
  (GST_IMPLEMENTS_INTERFACE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_STREAM_VOLUME))
#define GST_IS_STREAM_VOLUME_INTERFACE(iface) \
  (G_TYPE_CHECK_INTERFACE_TYPE ((iface), GST_TYPE_STREAM_VOLUME))
#define GST_STREAM_VOLUME_GET_INTERFACE(inst) \
  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), GST_TYPE_STREAM_VOLUME, GstStreamVolumeInterface))

typedef struct _GstStreamVolume GstStreamVolume;
typedef struct _GstStreamVolumeInterface GstStreamVolumeInterface;

struct _GstStreamVolumeInterface {
  GTypeInterface parent;

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
};

/**
 * GstStreamVolumeFormat:
 * @GST_STREAM_VOLUME_FORMAT_LINEAR: Linear scale factor, 1.0 = 100%
 * @GST_STREAM_VOLUME_FORMAT_CUBIC: Cubic volume scale
 * @GST_STREAM_VOLUME_FORMAT_DB: Logarithmic volume scale (dB, amplitude not power)
 *
 * Different representations of a stream volume. gst_stream_volume_convert()
 * allows to convert between the different representations.
 *
 * Formulas to convert from a linear to a cubic or dB volume are
 * cbrt(val) and 20 * log10 (val).
 * 
 * Since: 0.10.25
 */
typedef enum {
  GST_STREAM_VOLUME_FORMAT_LINEAR = 0,
  GST_STREAM_VOLUME_FORMAT_CUBIC,
  GST_STREAM_VOLUME_FORMAT_DB
} GstStreamVolumeFormat;

GType           gst_stream_volume_get_type        (void);
void            gst_stream_volume_set_volume      (GstStreamVolume *volume,
                                                   GstStreamVolumeFormat format,
                                                   gdouble val);
gdouble         gst_stream_volume_get_volume      (GstStreamVolume *volume,
                                                   GstStreamVolumeFormat format);

void            gst_stream_volume_set_mute        (GstStreamVolume *volume,
                                                   gboolean mute);
gboolean        gst_stream_volume_get_mute        (GstStreamVolume *volume);

gdouble         gst_stream_volume_convert_volume  (GstStreamVolumeFormat from,
                                                   GstStreamVolumeFormat to,
                                                   gdouble val) G_GNUC_CONST;

G_END_DECLS

#endif /* __GST_STREAM_VOLUME_H__ */
