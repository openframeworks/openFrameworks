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

#ifndef __GST_FFT_F64_H__
#define __GST_FFT_F64_H__

#include <glib.h>
#include <gst/gst.h>

#include "gstfft.h"

typedef struct _GstFFTF64 GstFFTF64;
typedef struct _GstFFTF64Complex GstFFTF64Complex;

/* FIXME 0.11: Move the struct definition to the sources,
 *             there's no reason to have it public.
 */
/**
 * GstFFTF64:
 *
 * Instance structure for #GstFFTF64.
 *
 */
struct _GstFFTF64 {
  /* <private> */
  void * cfg;
  gboolean inverse;
  gint len;
  gpointer _padding[GST_PADDING];
};

/* Copy of kiss_fft_f64_cpx for documentation reasons,
 * do NOT change! */

/**
 * GstFFTF64Complex:
 * @r: Real part
 * @i: Imaginary part
 *
 * Data type for complex numbers composed of
 * 64 bit float.
 *
 */
struct _GstFFTF64Complex
{
  gdouble r;
  gdouble i;
};

/* Functions */

GstFFTF64 * gst_fft_f64_new (gint len, gboolean inverse);
void gst_fft_f64_fft (GstFFTF64 *self, const gdouble *timedata, GstFFTF64Complex *freqdata);
void gst_fft_f64_inverse_fft (GstFFTF64 *self, const GstFFTF64Complex *freqdata, gdouble *timedata);
void gst_fft_f64_free (GstFFTF64 *self);

void gst_fft_f64_window (GstFFTF64 *self, gdouble *timedata, GstFFTWindow window);

#endif /* __GST_FFT_F64_H__ */
