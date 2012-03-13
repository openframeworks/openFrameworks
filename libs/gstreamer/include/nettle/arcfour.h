/* arcfour.h
 *
 * The arcfour/rc4 stream cipher.
 */

/* nettle, low-level cryptographics library
 *
 * Copyright (C) 2001 Niels Möller
 *  
 * The nettle library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 * 
 * The nettle library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with the nettle library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */
 
#ifndef NETTLE_ARCFOUR_H_INCLUDED
#define NETTLE_ARCFOUR_H_INCLUDED

#include "nettle-types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Name mangling */
#define arcfour_set_key nettle_arcfour_set_key
#define arcfour_crypt nettle_arcfour_crypt
#define arcfour_stream nettle_arcfour_stream

/* Minimum and maximum keysizes, and a reasonable default. In
 * octets.*/
#define ARCFOUR_MIN_KEY_SIZE 1
#define ARCFOUR_MAX_KEY_SIZE 256
#define ARCFOUR_KEY_SIZE 16

struct arcfour_ctx
{
  uint8_t S[256];
  uint8_t i;
  uint8_t j;
};

void
arcfour_set_key(struct arcfour_ctx *ctx,
		unsigned length, const uint8_t *key);

void
arcfour_crypt(struct arcfour_ctx *ctx,
	      unsigned length, uint8_t *dst,
	      const uint8_t *src);

void
arcfour_stream(struct arcfour_ctx *ctx,
	       unsigned length, uint8_t *dst);

#ifdef __cplusplus
}
#endif

#endif /* NETTLE_ARCFOUR_H_INCLUDED */

