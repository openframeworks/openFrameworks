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

#ifndef __GST_FFT_S32_H__
#define __GST_FFT_S32_H__

#include <glib.h>
#include <gst/gst.h>

#include "gstfft.h"

typedef struct _GstFFTS32 GstFFTS32;
typedef struct _GstFFTS32Complex GstFFTS32Complex;

/* FIXME 0.11: Move the struct definition to the sources,
 *             there's no reason to have it public.
 */
/**
 * GstFFTS32:
 *
 * Instance structure for #GstFFTS32.
 *
 */
struct _GstFFTS32 {
  /* <private> */
  void * cfg;
  gboolean inverse;
  gint len;
  gpointer _padding[GST_PADDING];
};

/* Copy of kiss_fft_s32_cpx for documentation reasons,
 * do NOT change! */

/**
 * GstFFTS32Complex:
 * @r: Real part
 * @i: Imaginary part
 *
 * Data type for complex numbers composed of
 * signed 32 bit integers.
 *
 */
struct _GstFFTS32Complex
{
  gint32 r;
  gint32 i;
};

/* Functions */

GstFFTS32 * gst_fft_s32_new (gint len, gboolean inverse);
void gst_fft_s32_fft (GstFFTS32 *self, const gint32 *timedata, GstFFTS32Complex *freqdata);
void gst_fft_s32_inverse_fft (GstFFTS32 *self, const GstFFTS32Complex *freqdata, gint32 *timedata);
void gst_fft_s32_free (GstFFTS32 *self);

void gst_fft_s32_window (GstFFTS32 *self, gint32 *timedata, GstFFTWindow window);

#endif /* __GST_FFT_S32_H__ */
