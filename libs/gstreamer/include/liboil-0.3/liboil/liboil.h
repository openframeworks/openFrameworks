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

#ifndef _LIBOIL_H_
#define _LIBOIL_H_

#include <liboil/liboilutils.h>

#include <liboil/liboilfuncs.h>

OIL_BEGIN_DECLS

void oil_init (void);

/**
 * oil_memcpy:
 * @dest:
 * @src:
 * @n_bytes:
 *
 * Macro that uses oil_copy_u8() to provide an implementation of
 * memcpy().  Note that oil_copy_u8() is optimized for short copies,
 * and may be very slow for large copies compared to alternatives,
 * including the system memcpy().
 */
#define oil_memcpy(dest,src,n_bytes) \
  oil_copy_u8((void *)(dest),(const void *)(src),(n_bytes))

/**
 * oil_trans8x8_s16:
 * @dest:
 * @dstr:
 * @src:
 * @sstr:
 *
 * Macro wrapping trans8x8_u16().
 */
#define oil_trans8x8_s16(dest, dstr, src, sstr) \
  oil_trans8x8_u16((uint16_t *)dest, dstr, (const uint16_t *)src, sstr)

OIL_END_DECLS

#endif

