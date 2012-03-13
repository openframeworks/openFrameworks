/*
 * interfaces.h - Source for interface discovery code
 *
 * Farsight Helper functions
 * Copyright (C) 2006 Youness Alaoui <kakaroto@kakaroto.homelinux.net>
 * Copyright (C) 2008-2009 Collabora, Nokia
 *  Contact: Youness Alaoui
 * Copyright (C) 2008-2009 Nokia Corporation. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */

#ifndef __INTERFACES_H__
#define __INTERFACES_H__

/**
 * SECTION:interfaces
 * @short_description: Utility functions to discover local network interfaces
 * @include: interfaces.h
 * @stability: Stable
 *
 * These utility functions allow the discovery of local network interfaces
 * in a portable manner, they also allow finding the local ip addresses or
 * the address allocated to a network interface.
 */

#include <glib.h>

G_BEGIN_DECLS


/**
 * nice_interfaces_get_ip_for_interface:
 * @interface_name: name of local interface
 *
 * Retreives the IPv4 address of an interface by its name
 *
 * Returns: a newly-allocated string with the IP address
 */
gchar * nice_interfaces_get_ip_for_interface (gchar *interface_name);


/**
 * nice_interfaces_get_local_ips:
 * @include_loopback: Include any loopback devices
 *
 * Get a list of local ipv4 interface addresses
 *
 * Returns: a newly-allocated #GList of strings. The caller must free it.
 */

GList * nice_interfaces_get_local_ips (gboolean include_loopback);


/**
 * nice_interfaces_get_local_interfaces:
 *
 * Get the list of local interfaces
 *
 * Returns: a newly-allocated #GList of strings. The caller must free it.
 */
GList * nice_interfaces_get_local_interfaces (void);

G_END_DECLS

#endif
