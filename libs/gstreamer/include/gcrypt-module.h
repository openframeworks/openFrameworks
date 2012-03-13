/* gcrypt-module.h - GNU Cryptographic Library Interface
   Copyright (C) 2003, 2007 Free Software Foundation, Inc.
  
   This file is part of Libgcrypt.
  
   Libgcrypt is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2.1 of
   the License, or (at your option) any later version.
  
   Libgcrypt is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.
  
   You should have received a copy of the GNU Lesser General Public
   License along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

/*
   This file contains the necessary declarations/definitions for
   working with Libgcrypt modules.  
 */

#ifndef _GCRYPT_MODULE_H
#define _GCRYPT_MODULE_H

#ifdef __cplusplus
extern "C" {
#if 0 /* keep Emacsens's auto-indent happy */
}
#endif
#endif

/* The interfaces using the module system reserve a certain range of
   IDs for application use.  These IDs are not valid within Libgcrypt
   but Libgcrypt makes sure never to allocate such a module ID.  */
#define GCRY_MODULE_ID_USER      1024 
#define GCRY_MODULE_ID_USER_LAST 4095


/* This type represents a `module'.  */
typedef struct gcry_module *gcry_module_t;

/* Check that the library fulfills the version requirement.  */

/* Type for the cipher_setkey function.  */
typedef gcry_err_code_t (*gcry_cipher_setkey_t) (void *c,
						 const unsigned char *key,
						 unsigned keylen);

/* Type for the cipher_encrypt function.  */
typedef void (*gcry_cipher_encrypt_t) (void *c,
				       unsigned char *outbuf,
				       const unsigned char *inbuf);

/* Type for the cipher_decrypt function.  */
typedef void (*gcry_cipher_decrypt_t) (void *c,
				       unsigned char *outbuf,
				       const unsigned char *inbuf);

/* Type for the cipher_stencrypt function.  */
typedef void (*gcry_cipher_stencrypt_t) (void *c,
					 unsigned char *outbuf,
					 const unsigned char *inbuf,
					 unsigned int n);

/* Type for the cipher_stdecrypt function.  */
typedef void (*gcry_cipher_stdecrypt_t) (void *c,
					 unsigned char *outbuf,
					 const unsigned char *inbuf,
					 unsigned int n);

typedef struct gcry_cipher_oid_spec
{
  const char *oid;
  int mode;
} gcry_cipher_oid_spec_t;

/* Module specification structure for ciphers.  */
typedef struct gcry_cipher_spec
{
  const char *name;
  const char **aliases;
  gcry_cipher_oid_spec_t *oids;
  size_t blocksize;
  size_t keylen;
  size_t contextsize;
  gcry_cipher_setkey_t setkey;
  gcry_cipher_encrypt_t encrypt;
  gcry_cipher_decrypt_t decrypt;
  gcry_cipher_stencrypt_t stencrypt;
  gcry_cipher_stdecrypt_t stdecrypt;
} gcry_cipher_spec_t;

/* Register a new cipher module whose specification can be found in
   CIPHER.  On success, a new algorithm ID is stored in ALGORITHM_ID
   and a pointer representing this module is stored in MODULE.  */
gcry_error_t gcry_cipher_register (gcry_cipher_spec_t *cipher,
				   int *algorithm_id,
				   gcry_module_t *module);

/* Unregister the cipher identified by MODULE, which must have been
   registered with gcry_cipher_register.  */
void gcry_cipher_unregister (gcry_module_t module);

/* ********************** */

/* Type for the pk_generate function.  */
typedef gcry_err_code_t (*gcry_pk_generate_t) (int algo,
					       unsigned int nbits,
					       unsigned long use_e,
					       gcry_mpi_t *skey,
					       gcry_mpi_t **retfactors);

/* Type for the pk_check_secret_key function.  */
typedef gcry_err_code_t (*gcry_pk_check_secret_key_t) (int algo,
						       gcry_mpi_t *skey);

/* Type for the pk_encrypt function.  */
typedef gcry_err_code_t (*gcry_pk_encrypt_t) (int algo,
					      gcry_mpi_t *resarr,
					      gcry_mpi_t data,
					      gcry_mpi_t *pkey,
					      int flags);

/* Type for the pk_decrypt function.  */
typedef gcry_err_code_t (*gcry_pk_decrypt_t) (int algo,
					      gcry_mpi_t *result,
					      gcry_mpi_t *data,
					      gcry_mpi_t *skey,
					      int flags);

/* Type for the pk_sign function.  */
typedef gcry_err_code_t (*gcry_pk_sign_t) (int algo,
					   gcry_mpi_t *resarr,
					   gcry_mpi_t data,
					   gcry_mpi_t *skey);

/* Type for the pk_verify function.  */
typedef gcry_err_code_t (*gcry_pk_verify_t) (int algo,
					     gcry_mpi_t hash,
					     gcry_mpi_t *data,
					     gcry_mpi_t *pkey,
					     int (*cmp) (void *, gcry_mpi_t),
					     void *opaquev);

/* Type for the pk_get_nbits function.  */
typedef unsigned (*gcry_pk_get_nbits_t) (int algo, gcry_mpi_t *pkey);

/* Module specification structure for message digests.  */
typedef struct gcry_pk_spec
{
  const char *name;
  const char **aliases;
  const char *elements_pkey;
  const char *elements_skey;
  const char *elements_enc;
  const char *elements_sig;
  const char *elements_grip;
  int use;
  gcry_pk_generate_t generate;
  gcry_pk_check_secret_key_t check_secret_key;
  gcry_pk_encrypt_t encrypt;
  gcry_pk_decrypt_t decrypt;
  gcry_pk_sign_t sign;
  gcry_pk_verify_t verify;
  gcry_pk_get_nbits_t get_nbits;
} gcry_pk_spec_t;

/* Register a new pubkey module whose specification can be found in
   PUBKEY.  On success, a new algorithm ID is stored in ALGORITHM_ID
   and a pointer representhing this module is stored in MODULE.  */
gcry_error_t gcry_pk_register (gcry_pk_spec_t *pubkey,
			       unsigned int *algorithm_id,
			       gcry_module_t *module);

/* Unregister the pubkey identified by ID, which must have been
   registered with gcry_pk_register.  */
void gcry_pk_unregister (gcry_module_t module);

/* ********************** */

/* Type for the md_init function.  */
typedef void (*gcry_md_init_t) (void *c);

/* Type for the md_write function.  */
typedef void (*gcry_md_write_t) (void *c, const void *buf, size_t nbytes);

/* Type for the md_final function.  */
typedef void (*gcry_md_final_t) (void *c);

/* Type for the md_read function.  */
typedef unsigned char *(*gcry_md_read_t) (void *c);

typedef struct gcry_md_oid_spec
{
  const char *oidstring;
} gcry_md_oid_spec_t;

/* Module specification structure for message digests.  */
typedef struct gcry_md_spec
{
  const char *name;
  unsigned char *asnoid;
  int asnlen;
  gcry_md_oid_spec_t *oids;
  int mdlen;
  gcry_md_init_t init;
  gcry_md_write_t write;
  gcry_md_final_t final;
  gcry_md_read_t read;
  size_t contextsize; /* allocate this amount of context */
} gcry_md_spec_t;

/* Register a new digest module whose specification can be found in
   DIGEST.  On success, a new algorithm ID is stored in ALGORITHM_ID
   and a pointer representhing this module is stored in MODULE.  */
gcry_error_t gcry_md_register (gcry_md_spec_t *digest,
			       unsigned int *algorithm_id,
			       gcry_module_t *module);

/* Unregister the digest identified by ID, which must have been
   registered with gcry_digest_register.  */
void gcry_md_unregister (gcry_module_t module);

#if 0 /* keep Emacsens's auto-indent happy */
{
#endif
#ifdef __cplusplus
}
#endif
#endif
