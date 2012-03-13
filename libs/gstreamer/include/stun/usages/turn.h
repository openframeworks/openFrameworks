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

#ifndef STUN_TURN_H
# define STUN_TURN_H 1

/**
 * SECTION:turn
 * @short_description: TURN Allocation Usage
 * @include: stun/usages/turn.h
 * @stability: Stable
 *
 * The STUN TURN usage allows for easily creating and parsing STUN Allocate
 * requests and responses used for TURN. The API allows you to create a new
 * allocation or refresh an existing one as well as to parse a response to
 * an allocate or refresh request.
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
 * StunUsageTurnRequestPorts:
 * @STUN_USAGE_TURN_REQUEST_PORT_NORMAL: Request a normal port
 * @STUN_USAGE_TURN_REQUEST_PORT_EVEN: Request an even port
 * @STUN_USAGE_TURN_REQUEST_PORT_EVEN_AND_RESERVE: Request an even port and
 * reserve the next higher port
 *
 * This enum is used to specify which port configuration you want when creating
 * a new Allocation
 */
typedef enum {
  STUN_USAGE_TURN_REQUEST_PORT_NORMAL = 0,
  STUN_USAGE_TURN_REQUEST_PORT_EVEN = 1,
  STUN_USAGE_TURN_REQUEST_PORT_EVEN_AND_RESERVE = 2
} StunUsageTurnRequestPorts;

/**
 * StunUsageTurnCompatibility:
 * @STUN_USAGE_TURN_COMPATIBILITY_DRAFT9: Use the specification compatible with
 * TURN Draft 09
 * @STUN_USAGE_TURN_COMPATIBILITY_GOOGLE: Use the specification compatible with
 * Google Talk's relay server
 * @STUN_USAGE_TURN_COMPATIBILITY_MSN: Use the specification compatible with
 * MSN TURN servers
 *
 * Specifies which TURN specification compatibility to use
 */
typedef enum {
  STUN_USAGE_TURN_COMPATIBILITY_DRAFT9,
  STUN_USAGE_TURN_COMPATIBILITY_GOOGLE,
  STUN_USAGE_TURN_COMPATIBILITY_MSN,
} StunUsageTurnCompatibility;

/**
 * StunUsageTurnReturn:
 * @STUN_USAGE_TURN_RETURN_RELAY_SUCCESS: The response was successful and a relay
 * address is provided
 * @STUN_USAGE_TURN_RETURN_MAPPED_SUCCESS: The response was successful and a
 * relay address as well as a mapped address are provided
 * @STUN_USAGE_TURN_RETURN_ERROR: The response resulted in an error
 * @STUN_USAGE_TURN_RETURN_INVALID: The response is not a valid response
 * @STUN_USAGE_TURN_RETURN_ALTERNATE_SERVER: The server requests the message
 * to be sent to an alternate server
 *
 * Return value of stun_usage_turn_process() and
 * stun_usage_turn_refresh_process() which allows you to see what status the
 * function call returned.
 */
typedef enum {
  STUN_USAGE_TURN_RETURN_RELAY_SUCCESS,
  STUN_USAGE_TURN_RETURN_MAPPED_SUCCESS,
  STUN_USAGE_TURN_RETURN_ERROR,
  STUN_USAGE_TURN_RETURN_INVALID,
  STUN_USAGE_TURN_RETURN_ALTERNATE_SERVER,
} StunUsageTurnReturn;


/**
 * stun_usage_turn_create:
 * @agent: The #StunAgent to use to build the request
 * @msg: The #StunMessage to build
 * @buffer: The buffer to use for creating the #StunMessage
 * @buffer_len: The size of the @buffer
 * @previous_response: If this is the first request you are sending, set this
 * argument to NULL, if it's a subsequent request you are building, then set this
 * argument to the response you have received. This argument is used for building
 * long term credentials (using the REALM and NONCE attributes) as well as for
 * getting the RESERVATION-TOKEN attribute when you previously requested an
 * allocation which reserved two ports
 * @request_ports: Specify how you want to request the allocated port(s).
 * This is only used if the compatibility is set to
 * #STUN_USAGE_TURN_COMPATIBILITY_DRAFT9
 * <para>See #StunUsageTurnRequestPorts </para>
 * @bandwidth: The bandwidth to request from the server for the allocation. If
 * this value is negative, then no BANDWIDTH attribute is added to the request.
 * This is only used if the compatibility is set to
 * #STUN_USAGE_TURN_COMPATIBILITY_DRAFT9
 * @lifetime: The lifetime of the allocation to request from the server. If
 * this value is negative, then no LIFETIME attribute is added to the request.
 * This is only used if the compatibility is set to
 * #STUN_USAGE_TURN_COMPATIBILITY_DRAFT9
 * @username: The username to use in the request
 * @username_len: The length of @username
 * @password: The key to use for building the MESSAGE-INTEGRITY
 * @password_len: The length of @password
 * @compatibility: The compatibility mode to use for building the Allocation
 * request
 *
 * Create a new TURN Allocation request
 * Returns: The length of the message to send
 */
size_t stun_usage_turn_create (StunAgent *agent, StunMessage *msg,
    uint8_t *buffer, size_t buffer_len,
    StunMessage *previous_response,
    StunUsageTurnRequestPorts request_ports,
    int32_t bandwidth, int32_t lifetime,
    uint8_t *username, size_t username_len,
    uint8_t *password, size_t password_len,
    StunUsageTurnCompatibility compatibility);

/**
 * stun_usage_turn_create_refresh:
 * @agent: The #StunAgent to use to build the request
 * @msg: The #StunMessage to build
 * @buffer: The buffer to use for creating the #StunMessage
 * @buffer_len: The size of the @buffer
 * @previous_response: If this is the first request you are sending, set this
 * argument to NULL, if it's a subsequent request you are building, then set this
 * argument to the response you have received. This argument is used for building
 * long term credentials (using the REALM and NONCE attributes)
 * @lifetime: The lifetime of the allocation to request from the server. If
 * this value is negative, then no LIFETIME attribute is added to the request.
 * This is only used if the compatibility is set to
 * #STUN_USAGE_TURN_COMPATIBILITY_DRAFT9
 * @username: The username to use in the request
 * @username_len: The length of @username
 * @password: The key to use for building the MESSAGE-INTEGRITY
 * @password_len: The length of @password
 * @compatibility: The compatibility mode to use for building the Allocation
 * request
 *
 * Create a new TURN Refresh request
 * Returns: The length of the message to send
 */
size_t stun_usage_turn_create_refresh (StunAgent *agent, StunMessage *msg,
    uint8_t *buffer, size_t buffer_len,
    StunMessage *previous_response, int32_t lifetime,
    uint8_t *username, size_t username_len,
    uint8_t *password, size_t password_len,
    StunUsageTurnCompatibility compatibility);

/**
 * stun_usage_turn_process:
 * @msg: The message containing the response
 * @relay_addr: A pointer to a #sockaddr structure to fill with the relay address
 * that the TURN server allocated for us
 * @relay_addrlen: The length of @relay_addr
 * @addr: A pointer to a #sockaddr structure to fill with the mapped address
 * that the STUN response contains.
 * This argument will only be filled if the return value
 * of the function is #STUN_USAGE_TURN_RETURN_MAPPED_SUCCESS
 * @addrlen: The length of @addr
 * @alternate_server: A pointer to a #sockaddr structure to fill with the
 * address of an alternate server to which we should send our new STUN
 * Allocate request, in case the currently used TURN server is requesting the use
 * of an alternate server. This argument will only be filled if the return value
 * of the function is #STUN_USAGE_TURN_RETURN_ALTERNATE_SERVER
 * @alternate_server_len: The length of @alternate_server
 * @bandwidth: A pointer to fill with the bandwidth the TURN server allocated us
 * @lifetime: A pointer to fill with the lifetime of the allocation
 * @compatibility: The compatibility mode to use for processing the Allocation
 * response
 *
 * Process a TURN Allocate response and extract the necessary information from
 * the message
 * Returns: A #StunUsageTurnReturn value
 */
StunUsageTurnReturn stun_usage_turn_process (StunMessage *msg,
    struct sockaddr *relay_addr, socklen_t *relay_addrlen,
    struct sockaddr *addr, socklen_t *addrlen,
    struct sockaddr *alternate_server, socklen_t *alternate_server_len,
    uint32_t *bandwidth, uint32_t *lifetime,
    StunUsageTurnCompatibility compatibility);

/**
 * stun_usage_turn_refresh_process:
 * @msg: The message containing the response
 * @lifetime: A pointer to fill with the lifetime of the allocation
 * @compatibility: The compatibility mode to use for processing the Refresh
 * response
 *
 * Process a TURN Refresh response and extract the necessary information from
 * the message
 * Returns: A #StunUsageTurnReturn value. A #STUN_USAGE_TURN_RETURN_RELAY_SUCCESS
 * means the Refresh was successful, but no relay address is given (kept the same
 * as for the original allocation)
 */
StunUsageTurnReturn stun_usage_turn_refresh_process (StunMessage *msg,
    uint32_t *lifetime, StunUsageTurnCompatibility compatibility);


# ifdef __cplusplus
}
# endif

#endif
