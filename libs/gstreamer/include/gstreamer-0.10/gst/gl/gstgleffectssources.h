/* 
 * GStreamer
 * Copyright (C) 2008 Filippo Argiolas <filippo.argiolas@gmail.com>
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

#ifndef __GST_GL_EFFECTS_SOURCES_H__
#define __GST_GL_EFFECTS_SOURCES_H__

extern const gchar *vertex_shader_source;
extern const gchar *identity_fragment_source;
extern const gchar *mirror_fragment_source;
extern const gchar *squeeze_fragment_source;
extern const gchar *stretch_fragment_source;
extern const gchar *tunnel_fragment_source;
extern const gchar *fisheye_fragment_source;
extern const gchar *twirl_fragment_source;
extern const gchar *bulge_fragment_source;
extern const gchar *square_fragment_source;
extern const gchar *luma_threshold_fragment_source;
extern const gchar *sep_sobel_length_fragment_source;
extern const gchar *desaturate_fragment_source;
extern const gchar *sep_sobel_hconv3_fragment_source;
extern const gchar *sep_sobel_vconv3_fragment_source;
extern const gchar *hconv7_fragment_source;
extern const gchar *vconv7_fragment_source;
extern const gchar *sum_fragment_source;
extern const gchar *luma_to_curve_fragment_source;
extern const gchar *rgb_to_curve_fragment_source;
extern const gchar *sin_fragment_source;
extern const gchar *interpolate_fragment_source;
extern const gchar *texture_interp_fragment_source;
extern const gchar *difference_fragment_source;
extern const gchar *multiply_fragment_source;

void fill_gaussian_kernel (float *kernel, int size, float sigma);

#endif /* __GST_GL_EFFECTS_SOURCES_H__ */
