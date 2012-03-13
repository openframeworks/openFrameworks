/* 
   PKCS#11 support for neon
   Copyright (C) 2008, Joe Orton <joe@manyfish.co.uk>

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

#ifndef NE_PKCS11_H
#define NE_PKCS11_H 1

#include "ne_defs.h"
#include "ne_session.h"

NE_BEGIN_DECLS

typedef struct ne_ssl_pkcs11_provider_s ne_ssl_pkcs11_provider;

#define NE_PK11_OK (0)
#define NE_PK11_NOTIMPL (-1)
#define NE_PK11_FAILED (-2)

/* Initialize a PKCS#11 provider of given name.  Returns NE_OK on
 * success, NE_PK11_FAILED if the provider could not be
 * loaded/initialized, and NE_PK11_NOTIMPL if PKCS#11 is not
 * supported.  On success, *provider is set to non-NULL.  */
int ne_ssl_pkcs11_provider_init(ne_ssl_pkcs11_provider **provider,
                                const char *name);

/* Initialize a NSS softoken pseudo-PKCS#11 provider of given name
 * (e.g. "softokn3") to supply a client certificate if requested,
 * using database in given directory name; the other parameters may be
 * NULL.  Returns NE_OK on success, NE_PK11_FAILED if the provider
 * could not be loaded/initialized, and NE_PK11_NOTIMPL if PKCS#11 is
 * not supported.  On success, *provider is set to non-NULL. */
int ne_ssl_pkcs11_nss_provider_init(ne_ssl_pkcs11_provider **provider,
                                    const char *name, const char *directory,
                                    const char *cert_prefix, 
                                    const char *key_prefix,
                                    const char *secmod_db);

/* Destroy a PKCS#11 provider object. */
void ne_ssl_pkcs11_provider_destroy(ne_ssl_pkcs11_provider *provider);


/* Flags passed to PIN entry callback: */
#define NE_SSL_P11PIN_COUNT_LOW (0x01) /* an incorrect PIN has been
                                        * entered. */
#define NE_SSL_P11PIN_FINAL_TRY (0x02) /* token will become locked if
                                        * entered PIN is incorrect */

/* Size of buffer passed to PIN entry callback: */
#define NE_SSL_P11PINLEN (256)

/* Callback for PKCS#11 PIN entry.  The callback provides the PIN code
 * to unlock the token with label 'token_label' in the slot described
 * by 'slot_descr'.
 *
 * The PIN code, as a NUL-terminated ASCII string, should be copied
 * into the 'pin' buffer (of fixed length NE_SSL_P11PINLEN), and
 * return 0 to indicate success. Alternatively, the callback may
 * return -1 to indicate failure and cancel PIN entry (in which case,
 * the contents of the 'pin' parameter are ignored).
 *
 * When a PIN is required, the callback will be invoked repeatedly
 * (and indefinitely) until either the returned PIN code is correct,
 * the callback returns failure, or the token refuses login (e.g. when
 * the token is locked due to too many incorrect PINs!).  For the
 * first such invocation, the 'attempt' counter will have value zero;
 * it will increase by one for each subsequent attempt.
 *
 * The NE_SSL_P11PIN_COUNT_LOW and/or NE_SSL_P11PIN_FINAL_TRY hints
 * may be set in the 'flags' argument, if these hints are made
 * available by the token; not all tokens expose these hints. */
typedef int (*ne_ssl_pkcs11_pin_fn)(void *userdata, int attempt,
                                    const char *slot_descr,
                                    const char *token_label,
                                    unsigned int flags,
                                    char *pin);

/* Set the PIN entry callback for the given provider.  This is
 * necessary for some (but not all) types of token.  For tokens which
 * implement an out-of-band ("protected") authentication path, the PIN
 * entry callback will not be invoked. */
void ne_ssl_pkcs11_provider_pin(ne_ssl_pkcs11_provider *provider,
                                ne_ssl_pkcs11_pin_fn fn,
                                void *userdata);

/* Set up a given PKCS#11 provider to supply an appropriate client
 * certificate if requested by the server.  A provider may be
 * configured for use in multiple sessions. */
void ne_ssl_set_pkcs11_provider(ne_session *sess,
                                ne_ssl_pkcs11_provider *provider);

NE_END_DECLS

#endif /* NE_PKCS11_H */
