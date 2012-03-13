/* pkcs1.h
 *
 * PKCS1 embedding.
 */

/* nettle, low-level cryptographics library
 *
 * Copyright (C) 2003 Niels Möller
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

#ifndef NETTLE_PKCS1_H_INCLUDED
#define NETTLE_PKCS1_H_INCLUDED

#include <gmp.h>
#include "nettle-types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Name mangling */
#define pkcs1_signature_prefix nettle_pkcs1_signature_prefix
#define pkcs1_rsa_md5_encode nettle_pkcs1_rsa_md5_encode
#define pkcs1_rsa_md5_encode_digest nettle_pkcs1_rsa_md5_encode_digest
#define pkcs1_rsa_sha1_encode nettle_pkcs1_rsa_sha1_encode
#define pkcs1_rsa_sha1_encode_digest nettle_pkcs1_rsa_sha1_encode_digest
#define pkcs1_rsa_sha256_encode nettle_pkcs1_rsa_sha256_encode
#define pkcs1_rsa_sha256_encode_digest nettle_pkcs1_rsa_sha256_encode_digest
#define pkcs1_rsa_sha512_encode nettle_pkcs1_rsa_sha512_encode
#define pkcs1_rsa_sha512_encode_digest nettle_pkcs1_rsa_sha512_encode_digest

struct md5_ctx;
struct sha1_ctx;
struct sha256_ctx;
struct sha512_ctx;

int
pkcs1_signature_prefix(unsigned size,
		       uint8_t *buffer,
		       unsigned id_size,
		       const uint8_t *id,
		       unsigned digest_size);

int
pkcs1_rsa_md5_encode(mpz_t m, unsigned length, struct md5_ctx *hash);

int
pkcs1_rsa_md5_encode_digest(mpz_t m, unsigned length, const uint8_t *digest);

int
pkcs1_rsa_sha1_encode(mpz_t m, unsigned length, struct sha1_ctx *hash);

int
pkcs1_rsa_sha1_encode_digest(mpz_t m, unsigned length, const uint8_t *digest);

int
pkcs1_rsa_sha256_encode(mpz_t m, unsigned length, struct sha256_ctx *hash);

int
pkcs1_rsa_sha256_encode_digest(mpz_t m, unsigned length, const uint8_t *digest);

int
pkcs1_rsa_sha512_encode(mpz_t m, unsigned length, struct sha512_ctx *hash);

int
pkcs1_rsa_sha512_encode_digest(mpz_t m, unsigned length, const uint8_t *digest);

#ifdef __cplusplus
}
#endif

#endif /* NETTLE_PKCS1_H_INCLUDED */
