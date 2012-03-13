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

#include <glib.h>
#include <math.h>

#ifndef __GST_FFT_H__
#define __GST_FFT_H__

/**
 * GstFFTWindow:
 * @GST_FFT_WINDOW_RECTANGULAR: Rectangular window
 * @GST_FFT_WINDOW_HAMMING: Hamming window
 * @GST_FFT_WINDOW_HANN: Hann (sometimes also called Hanning) window
 * @GST_FFT_WINDOW_BARTLETT: Bartlett window
 * @GST_FFT_WINDOW_BLACKMAN: Blackman window
 *
 * The various window functions available.
 */
typedef enum
{
  GST_FFT_WINDOW_RECTANGULAR,
  GST_FFT_WINDOW_HAMMING,
  GST_FFT_WINDOW_HANN,
  GST_FFT_WINDOW_BARTLETT,
  GST_FFT_WINDOW_BLACKMAN
} GstFFTWindow;

/* Functions */

gint gst_fft_next_fast_length (gint n) G_GNUC_CONST;

#endif /* __GST_FFT_H__ */
