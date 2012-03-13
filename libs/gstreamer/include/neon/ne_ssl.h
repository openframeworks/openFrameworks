/* 
   SSL/TLS abstraction layer for neon
   Copyright (C) 2003-2006, 2009, Joe Orton <joe@manyfish.co.uk>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
   MA 02111-1307, USA

*/

/* ne_ssl.h defines an interface for loading and accessing the
 * properties of SSL certificates. */

#ifndef NE_SSL_H
#define NE_SSL_H 1

#include <sys/types.h>

#include "ne_defs.h"

NE_BEGIN_DECLS

/* A "distinguished name"; a unique name for some entity. */
typedef struct ne_ssl_dname_s ne_ssl_dname;

/* Returns a single-line string representation of a distinguished
 * name, intended to be human-readable (e.g. "Acme Ltd., Norfolk,
 * GB").  Return value is a UTF-8-encoded malloc-allocated string and
 * must be free'd by the caller. */
char *ne_ssl_readable_dname(const ne_ssl_dname *dn);

/* Returns zero if 'dn1' and 'dn2' refer to same name, or non-zero if
 * they are different. */
int ne_ssl_dname_cmp(const ne_ssl_dname *dn1, const ne_ssl_dname *dn2);

/* An SSL certificate. */
typedef struct ne_ssl_certificate_s ne_ssl_certificate;

/* Read a certificate from a file in PEM format; returns NULL if the
 * certificate could not be parsed. */
ne_ssl_certificate *ne_ssl_cert_read(const char *filename);

/* Write a certificate to a file in PEM format; returns non-zero if
 * the certificate could not be written. */
int ne_ssl_cert_write(const ne_ssl_certificate *cert, const char *filename);

/* Export a certificate to a base64-encoded, NUL-terminated string.
 * The returned string is malloc-allocated and must be free()d by the
 * caller. */
char *ne_ssl_cert_export(const ne_ssl_certificate *cert);

/* Import a certificate from a base64-encoded string as returned by
 * ne_ssl_cert_export(). Returns a certificate object or NULL if
 * 'data' was not valid. */
ne_ssl_certificate *ne_ssl_cert_import(const char *data);

/* Returns the identity of the certificate, or NULL if none is given.
 * For a server certificate this will be the hostname of the server to
 * which the cert was issued.  A NUL-terminated UTF-8-encoded string
 * is returned, which is valid for the lifetime of the certificate
 * object. */
const char *ne_ssl_cert_identity(const ne_ssl_certificate *cert);

/* Return the certificate of the entity which signed certificate
 * 'cert'.  Returns NULL if 'cert' is self-signed or the issuer
 * certificate is not available; if non-NULL, the pointer is valid for
 * the lifetime of the certificate object. */
const ne_ssl_certificate *ne_ssl_cert_signedby(const ne_ssl_certificate *cert);

/* Returns the distinguished name of the certificate issuer. */
const ne_ssl_dname *ne_ssl_cert_issuer(const ne_ssl_certificate *cert);

/* Returns the distinguished name of the certificate subject. */
const ne_ssl_dname *ne_ssl_cert_subject(const ne_ssl_certificate *cert);

#define NE_SSL_DIGESTLEN (60)

/* Calculate the certificate digest ("fingerprint") and format it as a
 * NUL-terminated hex string in 'digest', of the form "aa:bb:...:ff".
 * Returns zero on success or non-zero if there was an internal error
 * whilst calculating the digest.  'digest' must be at least 
 * NE_SSL_DIGESTLEN bytes in length. */
int ne_ssl_cert_digest(const ne_ssl_certificate *cert, char *digest);

/* Copy the validity times for the certificate 'cert' into 'from' and
 * 'until' (either may be NULL).  If the time cannot be represented by
 * a time_t value, then (time_t)-1 will be written. */
void ne_ssl_cert_validity_time(const ne_ssl_certificate *cert,
                               time_t *from, time_t *until);

#define NE_SSL_VDATELEN (30)
/* Copy the validity times into buffers 'from' and 'until' as
 * NUL-terminated human-readable strings, using RFC 1123-style date
 * formatting (and not localized, so always using English month/week
 * names).  The buffers must be at least NE_SSL_VDATELEN bytes in
 * length, and either may be NULL. */
void ne_ssl_cert_validity(const ne_ssl_certificate *cert,
                          char *from, char *until);

/* Returns zero if 'c1' and 'c2' refer to the same certificate, or
 * non-zero otherwise. */
int ne_ssl_cert_cmp(const ne_ssl_certificate *c1,
                    const ne_ssl_certificate *c2);

/* Deallocate memory associated with certificate. */
void ne_ssl_cert_free(ne_ssl_certificate *cert);

/* A client certificate (and private key).  A client certificate
 * object has state; the object is either in the "encrypted" or
 * "decrypted" state.  */
typedef struct ne_ssl_client_cert_s ne_ssl_client_cert;

/* Read a client certificate and private key from a PKCS12 file;
 * returns NULL if the file could not be parsed, or otherwise
 * returning a client certificate object.  The returned object may be
 * in either the encrypted or decrypted state. */
ne_ssl_client_cert *ne_ssl_clicert_read(const char *filename);

/* Returns non-zero if client cert is in the encrypted state. */
int ne_ssl_clicert_encrypted(const ne_ssl_client_cert *ccert);

/* Returns the "friendly name" given for the client cert, or NULL if
 * none given.  Returns a NUL-terminated, UTF-8-encoded string.  This
 * function may be used on a ccert object in either encrypted or
 * decrypted state. */
const char *ne_ssl_clicert_name(const ne_ssl_client_cert *ccert);

/* Decrypt the encrypted client cert using the given password.
 * Returns non-zero on failure, in which case, the ccert object
 * remains in the encrypted state and the function may be called again
 * with a different password.  This function has undefined behaviour
 * for a ccert object which is in the decrypted state. */
int ne_ssl_clicert_decrypt(ne_ssl_client_cert *ccert, const char *password);

/* Return the actual certificate part of the client certificate (never
 * returns NULL).  This function has undefined behaviour for a ccert
 * object which is in the encrypted state. */
const ne_ssl_certificate *ne_ssl_clicert_owner(const ne_ssl_client_cert *ccert);

/* Destroy a client certificate object.  This function may be used on
 * a ccert object in either the encrypted or decrypted state. */
void ne_ssl_clicert_free(ne_ssl_client_cert *ccert);


/* SSL context object.  The interfaces to manipulate an SSL context
 * are only needed when interfacing directly with ne_socket.h. */
typedef struct ne_ssl_context_s ne_ssl_context;

/* Context creation modes: */
#define NE_SSL_CTX_CLIENT (0) /* client context */
#define NE_SSL_CTX_SERVER (1) /* default server context */
#define NE_SSL_CTX_SERVERv2 (2) /* SSLv2-specific server context */

/* Create an SSL context. */
ne_ssl_context *ne_ssl_context_create(int mode);

/* Client mode: trust the given certificate 'cert' in context 'ctx'. */
void ne_ssl_context_trustcert(ne_ssl_context *ctx, const ne_ssl_certificate *cert);

/* Server mode: use given cert and key (filenames to PEM certificates). */
int ne_ssl_context_keypair(ne_ssl_context *ctx,
                           const char *cert, const char *key);

/* Server mode: set client cert verification options: required is non-zero if 
 * a client cert is required, if ca_names is non-NULL it is a filename containing
 * a set of PEM certs from which CA names are sent in the ccert request. */
int ne_ssl_context_set_verify(ne_ssl_context *ctx, int required,
                              const char *ca_names, const char *verify_cas);

#define NE_SSL_CTX_SSLv2 (0)
/* Set a flag for the SSL context. */
void ne_ssl_context_set_flag(ne_ssl_context *ctx, int flag, int value);

/* Destroy an SSL context. */
void ne_ssl_context_destroy(ne_ssl_context *ctx);

NE_END_DECLS

#endif
