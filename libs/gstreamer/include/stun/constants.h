/*
 * This file is part of the Nice GLib ICE library.
 *
 * (C) 2008 Collabora Ltd.
 *  Contact: Youness Alaoui
 * (C) 2008 Nokia Corporation. All rights reserved.
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

#ifndef _STUN_CONSTANTS_H
#define _STUN_CONSTANTS_H


#define STUN_MESSAGE_TYPE_POS 0
#define STUN_MESSAGE_TYPE_LEN 2
#define STUN_MESSAGE_LENGTH_POS \
  (STUN_MESSAGE_TYPE_POS + STUN_MESSAGE_TYPE_LEN)
#define STUN_MESSAGE_LENGTH_LEN 2
#define STUN_MESSAGE_TRANS_ID_POS \
  (STUN_MESSAGE_LENGTH_POS + STUN_MESSAGE_LENGTH_LEN)
#define STUN_MESSAGE_TRANS_ID_LEN 16
#define STUN_MESSAGE_ATTRIBUTES_POS \
  (STUN_MESSAGE_TRANS_ID_POS + STUN_MESSAGE_TRANS_ID_LEN)

#define STUN_MESSAGE_HEADER_LENGTH STUN_MESSAGE_ATTRIBUTES_POS

#define STUN_ATTRIBUTE_TYPE_POS 0
#define STUN_ATTRIBUTE_TYPE_LEN 2
#define STUN_ATTRIBUTE_LENGTH_POS \
  (STUN_ATTRIBUTE_TYPE_POS + STUN_ATTRIBUTE_TYPE_LEN)
#define STUN_ATTRIBUTE_LENGTH_LEN 2
#define STUN_ATTRIBUTE_VALUE_POS \
  (STUN_ATTRIBUTE_LENGTH_POS + STUN_ATTRIBUTE_LENGTH_LEN)

#define STUN_ATTRIBUTE_HEADER_LENGTH STUN_ATTRIBUTE_VALUE_POS


#define STUN_MAX_MESSAGE_SIZE_IPV4 576
#define STUN_MAX_MESSAGE_SIZE_IPV6 1280
/* #define STUN_MAX_MESSAGE_SIZE STUN_MAX_MESSAGE_SIZE_IPV4 */

#define STUN_ID_LEN 16

#define STUN_AGENT_MAX_SAVED_IDS 200
#define STUN_AGENT_MAX_UNKNOWN_ATTRIBUTES 256

#define STUN_MAGIC_COOKIE 0x2112A442
#define TURN_MAGIC_COOKIE 0x72c64bc6

#ifndef TRUE
#define TRUE (1 == 1)
#endif

#ifndef FALSE
#define FALSE (0 == 1)
#endif

#endif /* _STUN_CONSTANTS_H */
