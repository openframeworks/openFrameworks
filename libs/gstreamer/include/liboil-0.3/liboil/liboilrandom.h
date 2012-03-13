/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2003,2004,2005 David A. Schleef <ds@schleef.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _LIBOIL_RANDOM_H_
#define _LIBOIL_RANDOM_H_

#include <liboil/liboilutils.h>
#include <liboil/liboiltypes.h>
#include <stdlib.h>

OIL_BEGIN_DECLS

#ifdef OIL_ENABLE_UNSTABLE_API

void oil_random_s32(oil_type_s32 *dest, int n);
void oil_random_s64 (oil_type_s64 *dest, int n);
void oil_random_s16 (oil_type_s16 *dest, int n);
void oil_random_s8 (oil_type_s8 *dest, int n);
void oil_random_u32 (oil_type_u32 *dest, int n);
void oil_random_u64 (oil_type_u64 *dest, int n);
void oil_random_u16 (oil_type_u16 *dest, int n);
void oil_random_u8 (oil_type_u8 *dest, int n);
void oil_random_f64 (oil_type_f64 *dest, int n);
void oil_random_f32 (oil_type_f32 *dest, int n);
void oil_random_argb (oil_type_u32 *dest, int n);
void oil_random_alpha (oil_type_u8 *dest, int n);

/**
 * oil_rand_s32:
 *
 * Evaluates to a random integer in the range [-(1<<31), (1<<31)-1].
 */
#define oil_rand_s32() ((rand()&0xffff)<<16 | (rand()&0xffff))
/**
 * oil_rand_s64:
 *
 * Evaluates to a random integer in the range [-(1<<63), (1<<63)-1].
 */
#define oil_rand_s64() ((int64_t)(oil_rand_s32())<<32 | oil_rand_s32())

/**
 * oil_rand_s16:
 *
 * Evaluates to a random integer in the range [-(1<<15), (1<<15)-1].
 */
#define oil_rand_s16() ((int16_t)(rand()&0xffff))

/**
 * oil_rand_s8:
 *
 * Evaluates to a random integer in the range [-(1<<7), (1<<7)-1].
 */
#define oil_rand_s8() ((int8_t)(rand()&0xffff))

/**
 * oil_rand_u32:
 *
 * Evaluates to a random integer in the range [0, (1<<32)-1].
 */
#define oil_rand_u32() ((uint32_t)((rand()&0xffff)<<16 | (rand()&0xffff)))

/**
 * oil_rand_u64:
 *
 * Evaluates to a random integer in the range [0, (1<<64)-1].
 */
#define oil_rand_u64() ((uint64_t)(oil_rand_u32())<<32 | oil_rand_u32())

/**
 * oil_rand_u16:
 *
 * Evaluates to a random integer in the range [0, (1<<16)-1].
 */
#define oil_rand_u16() ((uint16_t)(rand()&0xffff))

/**
 * oil_rand_u8:
 *
 * Evaluates to a random integer in the range [0, (1<<8)-1].
 */
#define oil_rand_u8() ((uint8_t)(rand()&0xffff))


/**
 * oil_rand_f64:
 *
 * Evaluates to a random double-precision floating point number
 * in the range [0, 1.0).
 */
#define oil_rand_f64() (((rand()/(RAND_MAX+1.0))+rand())/(RAND_MAX+1.0))

/**
 * oil_rand_f32:
 *
 * Evaluates to a random single-precision floating point number
 * in the range [0, 1.0).
 */
#define oil_rand_f32() (rand()/(RAND_MAX+1.0))

#endif

OIL_END_DECLS

#endif

