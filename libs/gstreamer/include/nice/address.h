/*
 * This file is part of the Nice GLib ICE library.
 *
 * (C) 2006-2009 Collabora Ltd.
 *  Contact: Youness Alaoui
 * (C) 2006-2009 Nokia Corporation. All rights reserved.
 *  Contact: Kai Vehmanen
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
 *   Dafydd Harries, Collabora Ltd.
 *   Kai Vehmanen
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

#ifndef _ADDRESS_H
#define _ADDRESS_H

/**
 * SECTION:address
 * @short_description: IP address convenience library
 * @stability: Stable
 *
 * The #NiceAddress structure will allow you to easily set/get and modify an IPv4
 * or IPv6 address in order to communicate with the #NiceAgent.
 */


#include <glib.h>

#ifdef G_OS_WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#if !(defined(__CYGWIN__) || defined(__MINGW32__) || defined(_UWIN))
#include <wspiapi.h>
#endif
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

G_BEGIN_DECLS


/**
 * NiceAddress:
 * @addr: Generic sockaddr address
 * @ip4: IPv4 sockaddr address
 * @ip6: IPv6 sockaddr address
 *
 * The #NiceAddress structure that represents an IPv4 or IPv6 address.
 */
struct _NiceAddress
{
  union
  {
    struct sockaddr     addr;
    struct sockaddr_in  ip4;
    struct sockaddr_in6 ip6;
  } s;
};


/**
 * NICE_ADDRESS_STRING_LEN:
 *
 * The maximum string length representation of an address.
 * When using nice_address_to_string() make sure the string has a size of
 * at least %NICE_ADDRESS_STRING_LEN
 */
#define NICE_ADDRESS_STRING_LEN INET6_ADDRSTRLEN

typedef struct _NiceAddress NiceAddress;


/**
 * nice_address_init:
 * @addr: The #NiceAddress to init
 *
 * Initialize a #NiceAddress into an undefined address
 */
void
nice_address_init (NiceAddress *addr);

/**
 * nice_address_new:
 *
 * Create a new #NiceAddress with undefined address
 * You must free it with nice_address_free()
 *
 * Returns: The new #NiceAddress
 */
NiceAddress *
nice_address_new (void);

/**
 * nice_address_free:
 * @addr: The #NiceAddress to free
 *
 * Frees a #NiceAddress created with nice_address_new() or nice_address_dup()
 */
void
nice_address_free (NiceAddress *addr);

/**
 * nice_address_dup:
 * @addr: The #NiceAddress to dup
 *
 * Creates a new #NiceAddress with the same address as @addr
 *
 * Returns: The new #NiceAddress
 */
NiceAddress *
nice_address_dup (const NiceAddress *addr);


/**
 * nice_address_set_ipv4:
 * @addr: The #NiceAddress to modify
 * @addr_ipv4: The IPv4 address
 *
 * Set @addr to an IPv4 address using the data from @addr_ipv4
 *
 <note>
  <para>
   This function will reset the port to 0, so make sure you call it before
   nice_address_set_port()
  </para>
 </note>
 */
void
nice_address_set_ipv4 (NiceAddress *addr, guint32 addr_ipv4);


/**
 * nice_address_set_ipv6:
 * @addr: The #NiceAddress to modify
 * @addr_ipv6: The IPv6 address
 *
 * Set @addr to an IPv6 address using the data from @addr_ipv6
 *
 <note>
  <para>
   This function will reset the port to 0, so make sure you call it before
   nice_address_set_port()
  </para>
 </note>
 */
void
nice_address_set_ipv6 (NiceAddress *addr, const guchar *addr_ipv6);


/**
 * nice_address_set_port:
 * @addr: The #NiceAddress to modify
 * @port: The port to set
 *
 * Set the port of @addr to @port
 */
void
nice_address_set_port (NiceAddress *addr, guint port);

/**
 * nice_address_get_port:
 * @addr: The #NiceAddress to query
 *
 * Retreive the port of @addr
 *
 * Returns: The port of @addr
 */
guint
nice_address_get_port (const NiceAddress *addr);

/**
 * nice_address_set_from_string:
 * @addr: The #NiceAddress to modify
 * @str: The string to set
 *
 * Sets an IPv4 or IPv6 address from the string @str
 *
 * Returns: %TRUE if success, %FALSE on error
 */
gboolean
nice_address_set_from_string (NiceAddress *addr, const gchar *str);

/**
 * nice_address_set_from_sockaddr:
 * @addr: The #NiceAddress to modify
 * @sin: The sockaddr to set
 *
 * Sets an IPv4 or IPv6 address from the sockaddr structure @sin
 *
 */
void
nice_address_set_from_sockaddr (NiceAddress *addr, const struct sockaddr *sin);


/**
 * nice_address_copy_to_sockaddr:
 * @addr: The #NiceAddress to query
 * @sin: The sockaddr to fill
 *
 * Fills the sockaddr structure @sin with the address contained in @addr
 *
 */
void
nice_address_copy_to_sockaddr (const NiceAddress *addr, struct sockaddr *sin);

/**
 * nice_address_equal:
 * @a: First #NiceAddress to compare
 * @b: Second #NiceAddress to compare
 *
 * Compares two #NiceAddress structures to see if they contain the same address
 *
 * Returns: %TRUE if @a and @b are the same address, %FALSE if they are different
 */
gboolean
nice_address_equal (const NiceAddress *a, const NiceAddress *b);

/**
 * nice_address_to_string:
 * @addr: The #NiceAddress to query
 * @dst: The string to fill
 *
 * Transforms the address @addr into a human readable string
 *
 */
void
nice_address_to_string (const NiceAddress *addr, gchar *dst);

/**
 * nice_address_is_private:
 * @addr: The #NiceAddress to query
 *
 * Verifies if the address in @addr is a private address or not
 *
 * Returns: %TRUE if @addr is a private address, %FALSE otherwise
 */
gboolean
nice_address_is_private (const NiceAddress *addr);

/**
 * nice_address_is_valid:
 * @addr: The #NiceAddress to query
 *
 * Validate whether the #NiceAddress @addr is a valid IPv4 or IPv6 address
 *
 * Returns: %TRUE if @addr is valid, %FALSE otherwise
 */
G_GNUC_WARN_UNUSED_RESULT
gboolean
nice_address_is_valid (const NiceAddress *addr);

G_END_DECLS

#endif /* _ADDRESS_H */

