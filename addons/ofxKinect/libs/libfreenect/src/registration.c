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

#include "libfreenect.h"
#include "freenect_internal.h"
#include "registration.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


#define REG_X_VAL_SCALE 256 // "fixed-point" precision for double -> int32_t conversion

#define S2D_PIXEL_CONST 10
#define S2D_CONST_OFFSET 0.375

#define DEPTH_SENSOR_X_RES 1280
#define DEPTH_MIRROR_X 0

#define DEPTH_MAX_METRIC_VALUE FREENECT_DEPTH_MM_MAX_VALUE
#define DEPTH_NO_MM_VALUE      FREENECT_DEPTH_MM_NO_VALUE
#define DEPTH_MAX_RAW_VALUE    FREENECT_DEPTH_RAW_MAX_VALUE
#define DEPTH_NO_RAW_VALUE     FREENECT_DEPTH_RAW_NO_VALUE

#define DEPTH_X_OFFSET 1
#define DEPTH_Y_OFFSET 1
#define DEPTH_X_RES 640
#define DEPTH_Y_RES 480

// try to fill single empty pixels AKA "salt-and-pepper noise"
// disabled by default, noise removal better handled in later stages
// #define DENSE_REGISTRATION


/// fill the table of horizontal shift values for metric depth -> RGB conversion
static void freenect_init_depth_to_rgb(int32_t* depth_to_rgb, freenect_zero_plane_info* zpi)
{
	uint32_t i,x_scale = DEPTH_SENSOR_X_RES / DEPTH_X_RES;

	double pixel_size = 1.0 / (zpi->reference_pixel_size * x_scale * S2D_PIXEL_CONST);
	double pixels_between_rgb_and_ir_cmos = zpi->dcmos_rcmos_dist * pixel_size * S2D_PIXEL_CONST;
	double reference_distance_in_pixels = zpi->reference_distance * pixel_size * S2D_PIXEL_CONST;

	memset(depth_to_rgb, DEPTH_NO_MM_VALUE, DEPTH_MAX_METRIC_VALUE * sizeof(int32_t));

	for (i = 0; i < DEPTH_MAX_METRIC_VALUE; i++) {
		double current_depth_in_pixels = i * pixel_size;
		depth_to_rgb[i] = (( pixels_between_rgb_and_ir_cmos * (current_depth_in_pixels - reference_distance_in_pixels) / current_depth_in_pixels) + S2D_CONST_OFFSET) * REG_X_VAL_SCALE;
	}
}

// unrolled inner loop of the 11-bit unpacker
static inline void unpack_8_pixels(uint8_t *raw, uint16_t *frame)
{
	uint16_t baseMask = 0x7FF;

	uint8_t r0  = *(raw+0);
	uint8_t r1  = *(raw+1);
	uint8_t r2  = *(raw+2);
	uint8_t r3  = *(raw+3);
	uint8_t r4  = *(raw+4);
	uint8_t r5  = *(raw+5);
	uint8_t r6  = *(raw+6);
	uint8_t r7  = *(raw+7);
	uint8_t r8  = *(raw+8);
	uint8_t r9  = *(raw+9);
	uint8_t r10 = *(raw+10);

	frame[0] =  (r0<<3)  | (r1>>5);
	frame[1] = ((r1<<6)  | (r2>>2) )           & baseMask;
	frame[2] = ((r2<<9)  | (r3<<1) | (r4>>7) ) & baseMask;
	frame[3] = ((r4<<4)  | (r5>>4) )           & baseMask;
	frame[4] = ((r5<<7)  | (r6>>1) )           & baseMask;
	frame[5] = ((r6<<10) | (r7<<2) | (r8>>6) ) & baseMask;
	frame[6] = ((r8<<5)  | (r9>>3) )           & baseMask;
	frame[7] = ((r9<<8)  | (r10)   )           & baseMask;
}

// apply registration data to a single packed frame
FN_INTERNAL int freenect_apply_registration(freenect_device* dev, uint8_t* input_packed, uint16_t* output_mm)
{
	freenect_registration* reg = &(dev->registration);
	// set output buffer to zero using pointer-sized memory access (~ 30-40% faster than memset)
	size_t i, *wipe = (size_t*)output_mm;
	for (i = 0; i < DEPTH_X_RES * DEPTH_Y_RES * sizeof(uint16_t) / sizeof(size_t); i++) wipe[i] = DEPTH_NO_MM_VALUE;

	uint16_t unpack[8];

	uint32_t target_offset = DEPTH_Y_RES * reg->reg_pad_info.start_lines;
	uint32_t x,y,source_index = 8;

	for (y = 0; y < DEPTH_Y_RES; y++) {
		for (x = 0; x < DEPTH_X_RES; x++) {

			// get 8 pixels from the packed frame
			if (source_index == 8) {
				unpack_8_pixels( input_packed, unpack );
				source_index = 0;
				input_packed += 11;
			}

			// get the value at the current depth pixel, convert to millimeters
			uint16_t metric_depth = reg->raw_to_mm_shift[ unpack[source_index++] ];

			// so long as the current pixel has a depth value
			if (metric_depth == DEPTH_NO_MM_VALUE) continue;
			if (metric_depth >= DEPTH_MAX_METRIC_VALUE) continue;

			// calculate the new x and y location for that pixel
			// using registration_table for the basic rectification
			// and depth_to_rgb_shift for determining the x shift
			uint32_t reg_index = DEPTH_MIRROR_X ? ((y + 1) * DEPTH_X_RES - x - 1) : (y * DEPTH_X_RES + x);
			uint32_t nx = (reg->registration_table[reg_index][0] + reg->depth_to_rgb_shift[metric_depth]) / REG_X_VAL_SCALE;
			uint32_t ny =  reg->registration_table[reg_index][1];

			// ignore anything outside the image bounds
			if (nx >= DEPTH_X_RES) continue;

			// convert nx, ny to an index in the depth image array
			uint32_t target_index = (DEPTH_MIRROR_X ? ((ny + 1) * DEPTH_X_RES - nx - 1) : (ny * DEPTH_X_RES + nx)) - target_offset;

			// get the current value at the new location
			uint16_t current_depth = output_mm[target_index];

			// make sure the new location is empty, or the new value is closer
			if ((current_depth == DEPTH_NO_MM_VALUE) || (current_depth > metric_depth)) {
				output_mm[target_index] = metric_depth; // always save depth at current location

				#ifdef DENSE_REGISTRATION
					// if we're not on the first row, or the first column
					if ((nx > 0) && (ny > 0)) {
						output_mm[target_index - DEPTH_X_RES    ] = metric_depth; // save depth at (x,y-1)
						output_mm[target_index - DEPTH_X_RES - 1] = metric_depth; // save depth at (x-1,y-1)
						output_mm[target_index               - 1] = metric_depth; // save depth at (x-1,y)
					} else if (ny > 0) {
						output_mm[target_index - DEPTH_X_RES] = metric_depth; // save depth at (x,y-1)
					} else if (nx > 0) {
						output_mm[target_index - 1] = metric_depth; // save depth at (x-1,y)
					}
				#endif
			}
		}
	}
	return 0;
}

// Same as freenect_apply_registration, but don't bother aligning to the RGB image
FN_INTERNAL int freenect_apply_depth_to_mm(freenect_device* dev, uint8_t* input_packed, uint16_t* output_mm)
{
	freenect_registration* reg = &(dev->registration);
	uint16_t unpack[8];
	uint32_t x,y,source_index = 8;
	for (y = 0; y < DEPTH_Y_RES; y++) {
		for (x = 0; x < DEPTH_X_RES; x++) {
			// get 8 pixels from the packed frame
			if (source_index == 8) {
				unpack_8_pixels( input_packed, unpack );
				source_index = 0;
				input_packed += 11;
			}
			// get the value at the current depth pixel, convert to millimeters
			uint16_t metric_depth = reg->raw_to_mm_shift[ unpack[source_index++] ];
			output_mm[y * DEPTH_X_RES + x] = metric_depth < DEPTH_MAX_METRIC_VALUE ? metric_depth : DEPTH_MAX_METRIC_VALUE;
		}
	}
	return 0;
}

// create temporary x/y shift tables
static void freenect_create_dxdy_tables(double* reg_x_table, double* reg_y_table, int32_t resolution_x, int32_t resolution_y, freenect_reg_info* regdata )
{

	int64_t AX6 = regdata->ax;
	int64_t BX6 = regdata->bx;
	int64_t CX2 = regdata->cx;
	int64_t DX2 = regdata->dx;

	int64_t AY6 = regdata->ay;
	int64_t BY6 = regdata->by;
	int64_t CY2 = regdata->cy;
	int64_t DY2 = regdata->dy;

	// don't merge the shift operations - necessary for proper 32-bit clamping of extracted values
	int64_t dX0 = (regdata->dx_start << 13) >> 4;
	int64_t dY0 = (regdata->dy_start << 13) >> 4;

	int64_t dXdX0 = (regdata->dxdx_start << 11) >> 3;
	int64_t dXdY0 = (regdata->dxdy_start << 11) >> 3;
	int64_t dYdX0 = (regdata->dydx_start << 11) >> 3;
	int64_t dYdY0 = (regdata->dydy_start << 11) >> 3;

	int64_t dXdXdX0 = (regdata->dxdxdx_start << 5) << 3;
	int64_t dYdXdX0 = (regdata->dydxdx_start << 5) << 3;
	int64_t dYdXdY0 = (regdata->dydxdy_start << 5) << 3;
	int64_t dXdXdY0 = (regdata->dxdxdy_start << 5) << 3;
	int64_t dYdYdX0 = (regdata->dydydx_start << 5) << 3;
	int64_t dYdYdY0 = (regdata->dydydy_start << 5) << 3;

	int32_t row,col,tOffs = 0;

	for (row = 0 ; row < resolution_y ; row++) {

		dXdXdX0 += CX2;

		dXdX0   += dYdXdX0 >> 8;
		dYdXdX0 += DX2;

		dX0     += dYdX0 >> 6;
		dYdX0   += dYdYdX0 >> 8;
		dYdYdX0 += BX6;

		dXdXdY0 += CY2;

		dXdY0   += dYdXdY0 >> 8;
		dYdXdY0 += DY2;

		dY0     += dYdY0 >> 6;
		dYdY0   += dYdYdY0 >> 8;
		dYdYdY0 += BY6;

		int64_t coldXdXdY0 = dXdXdY0, coldXdY0 = dXdY0, coldY0 = dY0;

		int64_t coldXdXdX0 = dXdXdX0, coldXdX0 = dXdX0, coldX0 = dX0;

		for (col = 0 ; col < resolution_x ; col++, tOffs++) {
			reg_x_table[tOffs] = coldX0 * (1.0/(1<<17));
			reg_y_table[tOffs] = coldY0 * (1.0/(1<<17));

			coldX0     += coldXdX0 >> 6;
			coldXdX0   += coldXdXdX0 >> 8;
			coldXdXdX0 += AX6;

			coldY0     += coldXdY0 >> 6;
			coldXdY0   += coldXdXdY0 >> 8;
			coldXdXdY0 += AY6;
		}
	}
}

static void freenect_init_registration_table(int32_t (*registration_table)[2], freenect_reg_info* reg_info) {

	double* regtable_dx = (double*)malloc(DEPTH_X_RES*DEPTH_Y_RES*sizeof(double));
	double* regtable_dy = (double*)malloc(DEPTH_X_RES*DEPTH_Y_RES*sizeof(double));
	memset(regtable_dx, 0, DEPTH_X_RES*DEPTH_Y_RES * sizeof(double));
	memset(regtable_dy, 0, DEPTH_X_RES*DEPTH_Y_RES * sizeof(double));
	int32_t x,y,index = 0;

	// create temporary dx/dy tables
	freenect_create_dxdy_tables( regtable_dx, regtable_dy, DEPTH_X_RES, DEPTH_Y_RES, reg_info );

	for (y = 0; y < DEPTH_Y_RES; y++) {
		for (x = 0; x < DEPTH_X_RES; x++, index++) {
			double new_x = x + regtable_dx[index] + DEPTH_X_OFFSET;
			double new_y = y + regtable_dy[index] + DEPTH_Y_OFFSET;

			if ((new_x < 0) || (new_y < 0) || (new_x >= DEPTH_X_RES) || (new_y >= DEPTH_Y_RES))
				new_x = 2 * DEPTH_X_RES; // intentionally set value outside image bounds

			registration_table[index][0] = new_x * REG_X_VAL_SCALE;
			registration_table[index][1] = new_y;
		}
	}
	free(regtable_dx);
	free(regtable_dy);
}

// These are just constants.
static double parameter_coefficient = 4;
static double shift_scale = 10;
static double pixel_size_factor = 1;

/// convert raw shift value to metric depth (in mm)
static uint16_t freenect_raw_to_mm(uint16_t raw, freenect_registration* reg)
{
	freenect_zero_plane_info* zpi = &(reg->zero_plane_info);
	double fixed_ref_x = ((raw - (parameter_coefficient * reg->const_shift / pixel_size_factor)) / parameter_coefficient) - S2D_CONST_OFFSET;
	double metric = fixed_ref_x * zpi->reference_pixel_size * pixel_size_factor;
	return shift_scale * ((metric * zpi->reference_distance / (zpi->dcmos_emitter_dist - metric)) + zpi->reference_distance);
}

/// Compute registration tables.
static void complete_tables(freenect_registration* reg) {
	uint16_t i;
	for (i = 0; i < DEPTH_MAX_RAW_VALUE; i++)
		reg->raw_to_mm_shift[i] = freenect_raw_to_mm( i, reg);
	reg->raw_to_mm_shift[DEPTH_NO_RAW_VALUE] = DEPTH_NO_MM_VALUE;

	freenect_init_depth_to_rgb( reg->depth_to_rgb_shift, &(reg->zero_plane_info) );

	freenect_init_registration_table( reg->registration_table, &(reg->reg_info) );
}

/// camera -> world coordinate helper function
void freenect_camera_to_world(freenect_device* dev, int cx, int cy, int wz, double* wx, double* wy)
{
	double ref_pix_size = dev->registration.zero_plane_info.reference_pixel_size;
	double ref_distance = dev->registration.zero_plane_info.reference_distance;
	// We multiply cx and cy by these factors because they come from a 640x480 image,
	// but the zero plane pixel size is for a 1280x1024 image.
	// However, the 640x480 image is produced by cropping the 1280x1024 image
	// to 1280x960 and then scaling by .5, so aspect ratio is maintained, and
	// we should simply multiply by two in each dimension.
	double factor = 2 * ref_pix_size * wz / ref_distance;
	*wx = (double)(cx - DEPTH_X_RES/2) * factor;
	*wy = (double)(cy - DEPTH_Y_RES/2) * factor;
}

/// Allocate and fill registration tables
/// This function should be called every time a new video (not depth!) mode is
/// activated.
FN_INTERNAL int freenect_init_registration(freenect_device* dev)
{
	freenect_registration* reg = &(dev->registration);

	// Ensure that we free the previous tables before dropping the pointers, if there were any.
	freenect_destroy_registration(&(dev->registration));

	// Allocate tables.
	reg->raw_to_mm_shift    = (uint16_t*)malloc( sizeof(uint16_t) * DEPTH_MAX_RAW_VALUE );
	reg->depth_to_rgb_shift = (int32_t*)malloc( sizeof( int32_t) * DEPTH_MAX_METRIC_VALUE );
	reg->registration_table = (int32_t (*)[2])malloc( sizeof( int32_t) * DEPTH_X_RES * DEPTH_Y_RES * 2 );

	// Fill tables.
	complete_tables(reg);

	return 0;
}

freenect_registration freenect_copy_registration(freenect_device* dev)
{
	freenect_registration retval;
	retval.reg_info = dev->registration.reg_info;
	retval.reg_pad_info = dev->registration.reg_pad_info;
	retval.zero_plane_info = dev->registration.zero_plane_info;
	retval.const_shift = dev->registration.const_shift;
	retval.raw_to_mm_shift    = (uint16_t*)malloc( sizeof(uint16_t) * DEPTH_MAX_RAW_VALUE );
	retval.depth_to_rgb_shift = (int32_t*)malloc( sizeof( int32_t) * DEPTH_MAX_METRIC_VALUE );
	retval.registration_table = (int32_t (*)[2])malloc( sizeof( int32_t) * DEPTH_X_RES * DEPTH_Y_RES * 2 );
	complete_tables(&retval);
	return retval;
}

int freenect_destroy_registration(freenect_registration* reg)
{
	if (reg->raw_to_mm_shift) {
		free(reg->raw_to_mm_shift);
		reg->raw_to_mm_shift = NULL;
	}
	if (reg->depth_to_rgb_shift) {
		free(reg->depth_to_rgb_shift);
		reg->depth_to_rgb_shift = NULL;
	}
	if (reg->registration_table) {
		free(reg->registration_table);
		reg->registration_table = NULL;
	}
	return 0;
}
