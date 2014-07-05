/*
 * This file is part of the OpenKinect Project. http://www.openkinect.org
 *
 * Copyright (c) 2011 individual OpenKinect contributors. See the CONTRIB file
 * for details.
 *
 * This code is licensed to you under the terms of the Apache License, version
 * 2.0, or, at your option, the terms of the GNU General Public License,
 * version 2.0. See the APACHE20 and GPL2 files for the text of the licenses,
 * or the following URLs:
 * http://www.apache.org/licenses/LICENSE-2.0
 * http://www.gnu.org/licenses/gpl-2.0.txt
 *
 * If you redistribute this file in source form, modified or unmodified, you
 * may:
 *   1) Leave this header intact and distribute it under the same terms,
 *      accompanying it with the APACHE20 and GPL20 files, or
 *   2) Delete the Apache 2.0 clause and accompany it with the GPL2 file, or
 *   3) Delete the GPL v2 clause and accompany it with the APACHE20 file
 * In all cases you must keep the copyright notice intact and include a copy
 * of the CONTRIB file.
 *
 * Binary distributions must follow the binary distribution requirements of
 * either License.
 */
#pragma once

#include "libfreenect.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Internal Kinect registration parameters.
/// Structure matches that of the line protocol
/// of the Kinect.
typedef struct {
	int32_t dx_center; // not used by mapping algorithm

	int32_t ax;
	int32_t bx;
	int32_t cx;
	int32_t dx;

	int32_t dx_start;

	int32_t ay;
	int32_t by;
	int32_t cy;
	int32_t dy;

	int32_t dy_start;

	int32_t dx_beta_start;
	int32_t dy_beta_start;

	int32_t rollout_blank; // not used by mapping algorithm
	int32_t rollout_size;  // not used by mapping algorithm

	int32_t dx_beta_inc;
	int32_t dy_beta_inc;

	int32_t dxdx_start;
	int32_t dxdy_start;
	int32_t dydx_start;
	int32_t dydy_start;

	int32_t dxdxdx_start;
	int32_t dydxdx_start;
	int32_t dxdxdy_start;
	int32_t dydxdy_start;

	int32_t back_comp1; // not used by mapping algorithm

	int32_t dydydx_start;

	int32_t back_comp2; // not used by mapping algorithm

	int32_t dydydy_start;
} freenect_reg_info;

/// registration padding info (?)
typedef struct {
	uint16_t start_lines;
	uint16_t end_lines;
	uint16_t cropping_lines;
} freenect_reg_pad_info;

/// internal Kinect zero plane data
typedef struct {
	float dcmos_emitter_dist;    // Distance between IR camera and IR emitter, in cm.
	float dcmos_rcmos_dist;      // Distance between IR camera and RGB camera, in cm.
	float reference_distance;    // The focal length of the IR camera, in mm.
	float reference_pixel_size;  // The size of a single pixel on the zero plane, in mm.
} freenect_zero_plane_info;

/// all data needed for depth->RGB mapping
typedef struct {
	freenect_reg_info        reg_info;
	freenect_reg_pad_info    reg_pad_info;
	freenect_zero_plane_info zero_plane_info;

	double const_shift;

	uint16_t* raw_to_mm_shift;
	int32_t* depth_to_rgb_shift;
	int32_t (*registration_table)[2];  // A table of 640*480 pairs of x,y values.
	                                   // Index first by pixel, then x:0 and y:1.
} freenect_registration;


// These allow clients to export registration parameters; proper docs will
// come later
FREENECTAPI freenect_registration freenect_copy_registration(freenect_device* dev);
FREENECTAPI int freenect_destroy_registration(freenect_registration* reg);

// convenience function to convert a single x-y coordinate pair from camera
// to world coordinates
FREENECTAPI void freenect_camera_to_world(freenect_device* dev,
	int cx, int cy, int wz, double* wx, double* wy);

#ifdef __cplusplus
}
#endif
