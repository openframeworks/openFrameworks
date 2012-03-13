/*
 * fs-interfaces.h - Source for interface discovery code
 *
 * Farsight Helper functions
 * Copyright (C) 2006 Youness Alaoui <kakaroto@kakaroto.homelinux.net>
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

#ifndef __FS_INTERFACES_H__
#define __FS_INTERFACES_H__

#include <glib.h>

G_BEGIN_DECLS

gchar * fs_interfaces_get_ip_for_interface (gchar *interface_name);
GList * fs_interfaces_get_local_ips (gboolean include_loopback);
GList * fs_interfaces_get_local_interfaces (void);

G_END_DECLS

#endif
