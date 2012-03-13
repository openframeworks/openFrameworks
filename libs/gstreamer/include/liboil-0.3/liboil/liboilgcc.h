/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2003,2004 David A. Schleef <ds@schleef.org>
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

#ifndef _LIBOIL_GCC_H_
#define _LIBOIL_GCC_H_

#ifndef __GNUC__
#error liboilgcc.h should only be included if the compiler is GCC
#endif

#include <liboil/liboilutils.h>

OIL_BEGIN_DECLS

#ifdef OIL_ENABLE_UNSTABLE_API

typedef int8_t vec_s8 __attribute__ ((mode(V8QI)));
typedef uint8_t vec_u8 __attribute__ ((mode(V8QI)));
typedef int16_t vec_s16 __attribute__ ((mode(V4HI)));
typedef uint16_t vec_u16 __attribute__ ((mode(V4HI)));
typedef int32_t vec_s32 __attribute__ ((mode(V2SI)));
typedef uint32_t vec_u32 __attribute__ ((mode(V2SI)));

#endif

OIL_END_DECLS

#endif

