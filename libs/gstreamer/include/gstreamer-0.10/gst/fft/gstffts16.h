/* GStreamer
 * Copyright (C) <2007> Sebastian Dröge <slomo@circular-chaos.org>
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

#ifndef __GST_FFT_S16_H__
#define __GST_FFT_S16_H__

#include <glib.h>
#include <gst/gst.h>

#include "gstfft.h"

typedef struct _GstFFTS16 GstFFTS16;
typedef struct _GstFFTS16Complex GstFFTS16Complex;

/* FIXME 0.11: Move the struct definition to the sources,
 *             there's no reason to have it public.
 */
/**
 * GstFFTS16:
 *
 * Instance structure for #GstFFTS16.
 *
 */
struct _GstFFTS16 {
  /* <private> */
  void *cfg;
  gboolean inverse;
  gint len;
  gpointer _padding[GST_PADDING];
};

/* Copy of kiss_fft_s16_cpx for documentation reasons,
 * do NOT change! */

/**
 * GstFFTS16Complex:
 * @r: Real part
 * @i: Imaginary part
 *
 * Data type for complex numbers composed of
 * signed 16 bit integers.
 *
 */
struct _GstFFTS16Complex
{
  gint16 r;
  gint16 i;
};

/* Functions */

GstFFTS16 * gst_fft_s16_new (gint len, gboolean inverse);
void gst_fft_s16_fft (GstFFTS16 *self, const gint16 *timedata, GstFFTS16Complex *freqdata);
void gst_fft_s16_inverse_fft (GstFFTS16 *self, const GstFFTS16Complex *freqdata, gint16 *timedata);
void gst_fft_s16_free (GstFFTS16 *self);

void gst_fft_s16_window (GstFFTS16 *self, gint16 *timedata, GstFFTWindow window);
#endif /* __GST_FFT_S16_H__ */
