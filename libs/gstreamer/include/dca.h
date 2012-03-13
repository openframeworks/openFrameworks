/*
 * dca.h
 * Copyright (C) 2004 Gildas Bazin <gbazin@videolan.org>
 *
 * This file is part of libdca, a free DTS Coherent Acoustics stream decoder.
 * See http://www.videolan.org/developers/libdca.html for updates.
 *
 * libdca is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * libdca is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef LIBDCA_DCA_H
#define LIBDCA_DCA_H

/* x86 accelerations */
#define MM_ACCEL_X86_MMX	0x80000000
#define MM_ACCEL_X86_3DNOW	0x40000000
#define MM_ACCEL_X86_MMXEXT	0x20000000

uint32_t mm_accel (void);

#if defined(LIBDCA_FIXED)
typedef int32_t sample_t;
typedef int32_t level_t;
#elif defined(LIBDCA_DOUBLE)
typedef double sample_t;
typedef double level_t;
#else
typedef float sample_t;
typedef float level_t;
#endif

typedef struct dca_state_s dca_state_t;

#define DCA_MONO 0
#define DCA_CHANNEL 1
#define DCA_STEREO 2
#define DCA_STEREO_SUMDIFF 3
#define DCA_STEREO_TOTAL 4
#define DCA_3F 5
#define DCA_2F1R 6
#define DCA_3F1R 7
#define DCA_2F2R 8
#define DCA_3F2R 9
#define DCA_4F2R 10

#define DCA_DOLBY 101 /* FIXME */

#define DCA_CHANNEL_MAX  DCA_3F2R /* We don't handle anything above that */
#define DCA_CHANNEL_BITS 6
#define DCA_CHANNEL_MASK 0x3F

#define DCA_LFE 0x80
#define DCA_ADJUST_LEVEL 0x100

dca_state_t * dca_init (uint32_t mm_accel);

int dca_syncinfo (dca_state_t *state, uint8_t * buf, int * flags,
                  int * sample_rate, int * bit_rate, int *frame_length);

int dca_frame (dca_state_t * state, uint8_t * buf, int * flags,
               level_t * level, sample_t bias);

void dca_dynrng (dca_state_t * state,
                 level_t (* call) (level_t, void *), void * data);

int dca_blocks_num (dca_state_t * state);
int dca_block (dca_state_t * state);

sample_t * dca_samples (dca_state_t * state);

void dca_free (dca_state_t * state);

#endif /* LIBDCA_DCA_H */
