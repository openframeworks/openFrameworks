/* GDBus - GLib D-Bus Library
 *
 * Copyright (C) 2008-2010 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author: David Zeuthen <davidz@redhat.com>
 */

#if !defined (__GIO_GIO_H_INSIDE__) && !defined (GIO_COMPILATION)
#error "Only <gio/gio.h> can be included directly."
#endif

#ifndef __G_DBUS_SERVER_H__
#define __G_DBUS_SERVER_H__

#include <gio/giotypes.h>

G_BEGIN_DECLS

#define G_TYPE_DBUS_SERVER         (g_dbus_server_get_type ())
#define G_DBUS_SERVER(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), G_TYPE_DBUS_SERVER, GDBusServer))
#define G_IS_DBUS_SERVER(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), G_TYPE_DBUS_SERVER))

GType             g_dbus_server_get_type           (void) G_GNUC_CONST;
GDBusServer      *g_dbus_server_new_sync           (const gchar       *address,
                                                    GDBusServerFlags   flags,
                                                    const gchar       *guid,
                                                    GDBusAuthObserver *observer,
                                                    GCancellable      *cancellable,
                                                    GError           **error);
const gchar      *g_dbus_server_get_client_address (GDBusServer       *server);
const gchar      *g_dbus_server_get_guid           (GDBusServer       *server);
GDBusServerFlags  g_dbus_server_get_flags          (GDBusServer       *server);
void              g_dbus_server_start              (GDBusServer       *server);
void              g_dbus_server_stop               (GDBusServer       *server);
gboolean          g_dbus_server_is_active          (GDBusServer       *server);

G_END_DECLS

#endif /* __G_DBUS_SERVER_H__ */
