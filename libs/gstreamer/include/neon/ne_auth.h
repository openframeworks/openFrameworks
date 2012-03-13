/* 
   HTTP authentication routines
   Copyright (C) 1999-2009, Joe Orton <joe@manyfish.co.uk>

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

#ifndef NE_AUTH_H
#define NE_AUTH_H

#include "ne_session.h" /* for ne_session */

NE_BEGIN_DECLS

/* Size of username/password buffers passed to ne_auth_creds
 * callback. */
#define NE_ABUFSIZ (256)

/* The callback used to request the username and password in the given
 * realm. The username and password must be copied into the buffers
 * which are both of size NE_ABUFSIZ.  The 'attempt' parameter is zero
 * on the first call to the callback, and increases by one each time
 * an attempt to authenticate fails.
 *
 * The callback must return zero to indicate that authentication
 * should be attempted with the username/password, or non-zero to
 * cancel the request. (if non-zero, username and password are
 * ignored.)
 *
 * IMPORTANT NOTE: The callback will be invoked repeatedly until
 * either it returns non-zero, or authentication is successful.
 *
 * Hint: if you just wish to attempt authentication just once (even if
 * the user gets the username/password wrong), have the callback
 * function use 'attempt' value as the function return value. */
typedef int (*ne_auth_creds)(void *userdata, const char *realm, int attempt,
			     char *username, char *password);

/* Set callbacks to provide credentials for server and proxy
 * authentication, using the default set of authentication protocols.
 * userdata is passed as the first argument to the callback. */
void ne_set_server_auth(ne_session *sess, ne_auth_creds creds, void *userdata);
void ne_set_proxy_auth(ne_session *sess, ne_auth_creds creds, void *userdata);

/* As an alternative to using ne_set_server_auth and
 * ne_set_proxy_auth, the following interfaces may be used; these
 * allow control over which authentication protocol is used. */

/* NE_AUTH_BASIC: Basic authentication transmits the username and
 * password unprotected over the channel; this allows a passive attack
 * to steal the credentials if using an unsecured channel
 * (i.e. non-SSL). */
#define NE_AUTH_BASIC (0x0001)

/* NE_AUTH_DIGEST: Digest authentication uses a hash of the username,
 * password, and certain aspects of the request, so prevents passive
 * attackers from obtaining the credentials; active attackers can
 * still modify most of the request/response if using an unsecured
 * channel. */ 
#define NE_AUTH_DIGEST (0x0002)

/* NE_AUTH_NEGOTIATE: Negotiate uses GSSAPI/SSPI, or NTLM, to
 * authenticate the user; an active attacker can modify any of the
 * request/response at will, so this must not be used over an
 * unsecured channel.  NE_AUTH_NEGOTIATE is currently equivalent to
 * use of (NE_AUTH_GSSAPI | NE_AUTH_NTLM). */
#define NE_AUTH_NEGOTIATE (0x0004)

/* NE_AUTH_GSSAPI: Use GSSAPI or SSPI to authenticate the user; an
 * active attacker can modify any of the request/response at will, so
 * this must not be used over an unsecured channel. */
#define NE_AUTH_GSSAPI (0x0008)

/* NE_AUTH_NTLM: Use NTLM to authenticate the user; an active attacker
 * can modify any of the request/response at will, so this must not be
 * used over an unsecured channel. */
#define NE_AUTH_NTLM (0x0010)

/* The default set of supported protocols, as deemed appropriate for
 * the given session scheme. */
#define NE_AUTH_DEFAULT (0x1000)

/* All protocols supported by the library. */
#define NE_AUTH_ALL (0x2000)

/* Add a callback to provide credentials for server and proxy
 * authentication using a particular auth protocol or set of
 * protocols.  The protocol is supplied as a bitmask of NE_AUTH_*
 * values.  For NE_AUTH_NEGOTIATE, the creds and userdata arguments
 * are ignored and may be NULL.
 *
 * These functions may be called multiple times per session to
 * register callbacks for different protocols.  If the server presents
 * more than one protocol in an auth challenge, the following
 * algorithm will be used to determine which callback is used:
 *
 * - iterate over the registered callbacks in the order registered
 * - for each each callback, iterate over the known set of protocols
 *   in order of algorithm strength (strongest first).
 * - if the protocol mask for that callback matches the protocol,
 *   attempt authentication using this protocol.
 *
 * Therefore, if multiple calls to ne_add_server_auth or
 * ne_add_proxy_auth are used for a given session, the caller must
 * ensure that the order in which those calls are made reflects the
 * precedence of protocols to be used. */
void ne_add_server_auth(ne_session *sess, unsigned protocol, 
                        ne_auth_creds creds, void *userdata);
void ne_add_proxy_auth(ne_session *sess, unsigned protocol, 
                       ne_auth_creds creds, void *userdata);

/* Clear any cached authentication credentials for the given
 * session. */
void ne_forget_auth(ne_session *sess);

NE_END_DECLS

#endif /* NE_AUTH_H */
