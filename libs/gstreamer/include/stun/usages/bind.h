/*
 * This file is part of the Nice GLib ICE library.
 *
 * (C) 2008-2009 Collabora Ltd.
 *  Contact: Youness Alaoui
 * (C) 2007-2009 Nokia Corporation. All rights reserved.
 *  Contact: Rémi Denis-Courmont
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Nice GLib ICE library.
 *
 * The Initial Developers of the Original Code are Collabora Ltd and Nokia
 * Corporation. All Rights Reserved.
 *
 * Contributors:
 *   Youness Alaoui, Collabora Ltd.
 *   Rémi Denis-Courmont, Nokia
 *
 * Alternatively, the contents of this file may be used under the terms of the
 * the GNU Lesser General Public License Version 2.1 (the "LGPL"), in which
 * case the provisions of LGPL are applicable instead of those above. If you
 * wish to allow use of your version of this file only under the terms of the
 * LGPL and not to allow others to use your version of this file under the
 * MPL, indicate your decision by deleting the provisions above and replace
 * them with the notice and other provisions required by the LGPL. If you do
 * not delete the provisions above, a recipient may use your version of this
 * file under either the MPL or the LGPL.
 */

#ifndef STUN_BIND_H
# define STUN_BIND_H 1

/**
 * SECTION:bind
 * @short_description: STUN Binding Usage
 * @include: stun/usages/bind.h
 * @stability: Stable
 *
 * The STUN Binding usage allows for easily creating and parsing STUN Binding
 * requests and responses. It offers both an asynchronous and a synchronous API
 * that uses the STUN timer usage.
 */


#ifdef _WIN32
#include "win32_common.h"
#else
# include <stdbool.h>
# include <stdint.h>
#endif

# include "stun/stunagent.h"

# ifdef __cplusplus
extern "C" {
# endif

/**
 * StunUsageBindReturn:
 * @STUN_USAGE_BIND_RETURN_SUCCESS: The binding usage succeeded
 * @STUN_USAGE_BIND_RETURN_ERROR: There was an unknown error in the bind usage
 * @STUN_USAGE_BIND_RETURN_INVALID: The message is invalid and should be ignored
 * @STUN_USAGE_BIND_RETURN_ALTERNATE_SERVER: The binding request has an
 * ALTERNATE-SERVER attribute
 * @STUN_USAGE_BIND_RETURN_TIMEOUT: The binding was unsuccessful because it has
 * timed out.
 *
 * Return value of stun_usage_bind_process() and stun_usage_bind_run() which
 * allows you to see what status the function call returned.
 */
typedef enum {
  STUN_USAGE_BIND_RETURN_SUCCESS,
  STUN_USAGE_BIND_RETURN_ERROR,
  STUN_USAGE_BIND_RETURN_INVALID,
  STUN_USAGE_BIND_RETURN_ALTERNATE_SERVER,
  STUN_USAGE_BIND_RETURN_TIMEOUT,
} StunUsageBindReturn;


/**
 * stun_usage_bind_create:
 * @agent: The #StunAgent to use to create the binding request
 * @msg: The #StunMessage to build
 * @buffer: The buffer to use for creating the #StunMessage
 * @buffer_len: The size of the @buffer
 *
 * Create a new STUN binding request to use with a STUN server.
 * Returns: The length of the built message.
 */
size_t stun_usage_bind_create (StunAgent *agent, StunMessage *msg,
    uint8_t *buffer, size_t buffer_len);

/**
 * stun_usage_bind_process:
 * @msg: The #StunMessage to process
 * @addr: A pointer to a #sockaddr structure to fill with the mapped address
 * that the STUN server gives us
 * @addrlen: The length of @add. rMust be set to the size of the @addr socket
 * address and will be set to the actual length of the socket address.
 * @alternate_server: A pointer to a #sockaddr structure to fill with the
 * address of an alternate server to which we should send our new STUN
 * binding request, in case the currently used STUN server is requesting the use
 * of an alternate server. This argument will only be filled if the return value
 * of the function is #STUN_USAGE_BIND_RETURN_ALTERNATE_SERVER
 * @alternate_server_len: The length of @alternate_server. Must be set to
 * the size of the @alternate_server socket address and will be set to the
 * actual length of the socket address.
 *
 * Process a STUN binding response and extracts the mapped address from the STUN
 * message. Also checks for the ALTERNATE-SERVER attribute.
 * Returns: A #StunUsageBindReturn value.
 * Note that #STUN_USAGE_BIND_RETURN_TIMEOUT cannot be returned by this function
 */
StunUsageBindReturn stun_usage_bind_process (StunMessage *msg,
    struct sockaddr *addr, socklen_t *addrlen,
    struct sockaddr *alternate_server, socklen_t *alternate_server_len);

/**
 * stun_usage_bind_keepalive:
 * @agent: The #StunAgent to use to build the message
 * @msg: The #StunMessage to build
 * @buf: The buffer to use for creating the #StunMessage
 * @len: The size of the @buf
 *
 * Creates a STUN binding indication that can be used for a keepalive.
 * Since this is an indication message, no STUN response will be generated
 * and it can only be used as a keepalive message.
 * Returns: The length of the message to send
 */
size_t stun_usage_bind_keepalive (StunAgent *agent, StunMessage *msg,
    uint8_t *buf, size_t len);

/**
 * stun_usage_bind_run:
 * @srv: A pointer to the #sockaddr structure representing the STUN server's
 * address
 * @srvlen: The length of @srv
 * @addr: A pointer to a #sockaddr structure to fill with the mapped address
 * that the STUN server gives us
 * @addrlen: The length of @addr
 *
 * This is a convenience function that will do a synchronous Binding request to
 * a server and wait for its answer. It will create the socket transports and
 * use the #StunTimer usage to send the request and handle the response.
 * Returns: A #StunUsageBindReturn.
 * Possible return values are #STUN_USAGE_BIND_RETURN_SUCCESS,
 * #STUN_USAGE_BIND_RETURN_ERROR and #STUN_USAGE_BIND_RETURN_TIMEOUT
 */
StunUsageBindReturn stun_usage_bind_run (const struct sockaddr *srv,
    socklen_t srvlen, struct sockaddr *addr, socklen_t *addrlen);

# ifdef __cplusplus
}
# endif

#endif
