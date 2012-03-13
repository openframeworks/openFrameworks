
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

#ifndef STUN_CONNCHECK_H
# define STUN_CONNCHECK_H 1

/**
 * SECTION:ice
 * @short_description: STUN ICE Usage
 * @include: stun/usages/ice.h
 * @stability: Stable
 *
 * The STUN ICE usage allows for easily creating and parsing STUN Binding
 * requests and responses used for ICE connectivity checks. The API allows you
 * to create a connectivity check message, parse a response or create a reply
 * to an incoming connectivity check request.
 */

# include "stun/stunagent.h"

# ifdef __cplusplus
extern "C" {
# endif

/**
 * StunUsageIceCompatibility:
 * @STUN_USAGE_ICE_COMPATIBILITY_RFC5245: The ICE compatibility with RFC 5245
 * @STUN_USAGE_ICE_COMPATIBILITY_GOOGLE: The ICE compatibility with Google's
 * implementation of ICE
 * @STUN_USAGE_ICE_COMPATIBILITY_MSN: The ICE compatibility with MSN's
 * implementation of ICE
 * @STUN_USAGE_ICE_COMPATIBILITY_DRAFT19: The ICE compatibility with draft 19
 *
 * This enum defines which compatibility modes this ICE usage can use
 *
 * <warning>@STUN_USAGE_ICE_COMPATIBILITY_DRAFT19 is deprecated and should not
 * be used in newly-written code. It is kept for compatibility reasons and
 * represents the same compatibility as @STUN_USAGE_ICE_COMPATIBILITY_RFC5245
 * </warning>
 */
typedef enum {
  STUN_USAGE_ICE_COMPATIBILITY_RFC5245,
  STUN_USAGE_ICE_COMPATIBILITY_GOOGLE,
  STUN_USAGE_ICE_COMPATIBILITY_MSN,
  STUN_USAGE_ICE_COMPATIBILITY_DRAFT19 = STUN_USAGE_ICE_COMPATIBILITY_RFC5245,
} StunUsageIceCompatibility;


/**
 * StunUsageIceReturn:
 * @STUN_USAGE_ICE_RETURN_SUCCESS: The function succeeded
 * @STUN_USAGE_ICE_RETURN_ERROR: There was an unspecified error
 * @STUN_USAGE_ICE_RETURN_INVALID: The message is invalid for processing
 * @STUN_USAGE_ICE_RETURN_ROLE_CONFLICT: A role conflict was detected
 * @STUN_USAGE_ICE_RETURN_INVALID_REQUEST: The message is an not a request
 * @STUN_USAGE_ICE_RETURN_INVALID_METHOD: The method of the request is invalid
 * @STUN_USAGE_ICE_RETURN_MEMORY_ERROR: The buffer size is too small to hold
 * the STUN reply
 * @STUN_USAGE_ICE_RETURN_INVALID_ADDRESS: The mapped address argument has
 * an invalid address family
 * @STUN_USAGE_ICE_RETURN_NO_MAPPED_ADDRESS: The response is valid but no
 * MAPPED-ADDRESS or XOR-MAPPED-ADDRESS attribute was found
 *
 * Return value of stun_usage_ice_conncheck_process() and
 * stun_usage_ice_conncheck_create_reply() which allows you to see what
 * status the function call returned.
 */
typedef enum {
  STUN_USAGE_ICE_RETURN_SUCCESS,
  STUN_USAGE_ICE_RETURN_ERROR,
  STUN_USAGE_ICE_RETURN_INVALID,
  STUN_USAGE_ICE_RETURN_ROLE_CONFLICT,
  STUN_USAGE_ICE_RETURN_INVALID_REQUEST,
  STUN_USAGE_ICE_RETURN_INVALID_METHOD,
  STUN_USAGE_ICE_RETURN_MEMORY_ERROR,
  STUN_USAGE_ICE_RETURN_INVALID_ADDRESS,
  STUN_USAGE_ICE_RETURN_NO_MAPPED_ADDRESS,
} StunUsageIceReturn;


/**
 * stun_usage_ice_conncheck_create:
 * @agent: The #StunAgent to use to build the request
 * @msg: The #StunMessage to build
 * @buffer: The buffer to use for creating the #StunMessage
 * @buffer_len: The size of the @buffer
 * @username: The username to use in the request
 * @username_len: The length of @username
 * @password: The key to use for building the MESSAGE-INTEGRITY
 * @password_len: The length of @password
 * @cand_use: Set to %TRUE to append the USE-CANDIDATE flag to the request
 * @controlling: Set to %TRUE if you are the controlling agent or set to
 * %FALSE if you are the controlled agent.
 * @priority: The value of the PRIORITY attribute
 * @tie: The value of the tie-breaker to put in the ICE-CONTROLLED or
 * ICE-CONTROLLING attribute
 * @compatibility: The compatibility mode to use for building the conncheck
 * request
 *
 * Builds an ICE connectivity check STUN message.
 * If the compatibility is not #STUN_USAGE_ICE_COMPATIBILITY_RFC5245, the
 * @cand_use, @controlling, @priority and @tie arguments are not used.
 * Returns: The length of the message built.
 */
size_t
stun_usage_ice_conncheck_create (StunAgent *agent, StunMessage *msg,
    uint8_t *buffer, size_t buffer_len,
    const uint8_t *username, const size_t username_len,
    const uint8_t *password, const size_t password_len,
    bool cand_use, bool controlling, uint32_t priority,
    uint64_t tie, StunUsageIceCompatibility compatibility);


/**
 * stun_usage_ice_conncheck_process:
 * @msg: The #StunMessage to process
 * @addr: A pointer to a #sockaddr structure to fill with the mapped address
 * that the STUN connectivity check response contains
 * @addrlen: The length of @addr
 * @compatibility: The compatibility mode to use for processing the conncheck
 * response
 *
 * Process an ICE connectivity check STUN message and retreive the
 * mapped address from the message
 * <para> See also stun_usage_ice_conncheck_priority() and
 * stun_usage_ice_conncheck_use_candidate() </para>
 * Returns: A #StunUsageIceReturn value
 */
StunUsageIceReturn stun_usage_ice_conncheck_process (StunMessage *msg,
    struct sockaddr *addr, socklen_t *addrlen,
    StunUsageIceCompatibility compatibility);

/**
 * stun_usage_ice_conncheck_create_reply:
 * @agent: The #StunAgent to use to build the response
 * @req: The original STUN request to reply to
 * @msg: The #StunMessage to build
 * @buf: The buffer to use for creating the #StunMessage
 * @plen: A pointer containing the size of the @buffer on input.
 * Will contain the length of the message built on output.
 * @src: A pointer to a #sockaddr structure containing the source address from
 * which the request was received. Will be used as the mapped address in the
 * response
 * @srclen: The length of @addr
 * @control: Set to %TRUE if you are the controlling agent or set to
 * %FALSE if you are the controlled agent.
 * @tie: The value of the tie-breaker to put in the ICE-CONTROLLED or
 * ICE-CONTROLLING attribute
 * @compatibility: The compatibility mode to use for building the conncheck
 * response
 *
 * Tries to parse a STUN connectivity check request and builds a
 * response accordingly.
 <note>
   <para>
     In case of error, the @msg is filled with the appropriate error response
     to be sent and the value of @plen is set to the size of that message.
     If @plen has a size of 0, then no error response should be sent.
   </para>
 </note>
 * Returns: A #StunUsageIceReturn value
 */
StunUsageIceReturn
stun_usage_ice_conncheck_create_reply (StunAgent *agent, StunMessage *req,
    StunMessage *msg, uint8_t *buf, size_t *plen,
    const struct sockaddr *src, socklen_t srclen,
    bool *control, uint64_t tie,
    StunUsageIceCompatibility compatibility);

/**
 * stun_usage_ice_conncheck_priority:
 * @msg: The #StunMessage to parse
 *
 * Extracts the priority from a STUN message.
 * Returns: host byte order priority, or 0 if not specified.
 */
uint32_t stun_usage_ice_conncheck_priority (const StunMessage *msg);

/**
 * stun_usage_ice_conncheck_use_candidate:
 * @msg: The #StunMessage to parse
 *
 * Extracts the USE-CANDIDATE attribute flag from a STUN message.
 * Returns: %TRUE if the flag is set, %FALSE if not.
 */
bool stun_usage_ice_conncheck_use_candidate (const StunMessage *msg);

# ifdef __cplusplus
}
# endif

#endif
