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

#ifndef __G_DBUS_PROXY_H__
#define __G_DBUS_PROXY_H__

#include <gio/giotypes.h>
#include <gio/gdbusintrospection.h>

G_BEGIN_DECLS

#define G_TYPE_DBUS_PROXY         (g_dbus_proxy_get_type ())
#define G_DBUS_PROXY(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), G_TYPE_DBUS_PROXY, GDBusProxy))
#define G_DBUS_PROXY_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), G_TYPE_DBUS_PROXY, GDBusProxyClass))
#define G_DBUS_PROXY_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), G_TYPE_DBUS_PROXY, GDBusProxyClass))
#define G_IS_DBUS_PROXY(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), G_TYPE_DBUS_PROXY))
#define G_IS_DBUS_PROXY_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), G_TYPE_DBUS_PROXY))

typedef struct _GDBusProxyClass   GDBusProxyClass;
typedef struct _GDBusProxyPrivate GDBusProxyPrivate;

/**
 * GDBusProxy:
 *
 * The #GDBusProxy structure contains only private data and
 * should only be accessed using the provided API.
 *
 * Since: 2.26
 */
struct _GDBusProxy
{
  /*< private >*/
  GObject parent_instance;
  GDBusProxyPrivate *priv;
};

/**
 * GDBusProxyClass:
 * @g_properties_changed: Signal class handler for the #GDBusProxy::g-properties-changed signal.
 * @g_signal: Signal class handler for the #GDBusProxy::g-signal signal.
 *
 * Class structure for #GDBusProxy.
 *
 * Since: 2.26
 */
struct _GDBusProxyClass
{
  /*< private >*/
  GObjectClass parent_class;

  /*< public >*/
  /* Signals */
  void (*g_properties_changed) (GDBusProxy          *proxy,
                                GVariant            *changed_properties,
                                const gchar* const  *invalidated_properties);
  void (*g_signal)             (GDBusProxy          *proxy,
                                const gchar         *sender_name,
                                const gchar         *signal_name,
                                GVariant            *parameters);

  /*< private >*/
  /* Padding for future expansion */
  gpointer padding[32];
};

GType            g_dbus_proxy_get_type                  (void) G_GNUC_CONST;
void             g_dbus_proxy_new                       (GDBusConnection     *connection,
                                                         GDBusProxyFlags      flags,
                                                         GDBusInterfaceInfo *info,
                                                         const gchar         *name,
                                                         const gchar         *object_path,
                                                         const gchar         *interface_name,
                                                         GCancellable        *cancellable,
                                                         GAsyncReadyCallback  callback,
                                                         gpointer             user_data);
GDBusProxy      *g_dbus_proxy_new_finish                (GAsyncResult        *res,
                                                         GError             **error);
GDBusProxy      *g_dbus_proxy_new_sync                  (GDBusConnection     *connection,
                                                         GDBusProxyFlags      flags,
                                                         GDBusInterfaceInfo *info,
                                                         const gchar         *name,
                                                         const gchar         *object_path,
                                                         const gchar         *interface_name,
                                                         GCancellable        *cancellable,
                                                         GError             **error);
void             g_dbus_proxy_new_for_bus               (GBusType             bus_type,
                                                         GDBusProxyFlags      flags,
                                                         GDBusInterfaceInfo *info,
                                                         const gchar         *name,
                                                         const gchar         *object_path,
                                                         const gchar         *interface_name,
                                                         GCancellable        *cancellable,
                                                         GAsyncReadyCallback  callback,
                                                         gpointer             user_data);
GDBusProxy      *g_dbus_proxy_new_for_bus_finish        (GAsyncResult        *res,
                                                         GError             **error);
GDBusProxy      *g_dbus_proxy_new_for_bus_sync          (GBusType             bus_type,
                                                         GDBusProxyFlags      flags,
                                                         GDBusInterfaceInfo *info,
                                                         const gchar         *name,
                                                         const gchar         *object_path,
                                                         const gchar         *interface_name,
                                                         GCancellable        *cancellable,
                                                         GError             **error);
GDBusConnection *g_dbus_proxy_get_connection            (GDBusProxy          *proxy);
GDBusProxyFlags  g_dbus_proxy_get_flags                 (GDBusProxy          *proxy);
const gchar     *g_dbus_proxy_get_name                  (GDBusProxy          *proxy);
gchar           *g_dbus_proxy_get_name_owner            (GDBusProxy          *proxy);
const gchar     *g_dbus_proxy_get_object_path           (GDBusProxy          *proxy);
const gchar     *g_dbus_proxy_get_interface_name        (GDBusProxy          *proxy);
gint             g_dbus_proxy_get_default_timeout       (GDBusProxy          *proxy);
void             g_dbus_proxy_set_default_timeout       (GDBusProxy          *proxy,
                                                         gint                 timeout_msec);
GDBusInterfaceInfo *g_dbus_proxy_get_interface_info     (GDBusProxy          *proxy);
void             g_dbus_proxy_set_interface_info        (GDBusProxy           *proxy,
                                                         GDBusInterfaceInfo   *info);
GVariant        *g_dbus_proxy_get_cached_property       (GDBusProxy          *proxy,
                                                         const gchar         *property_name);
void             g_dbus_proxy_set_cached_property       (GDBusProxy          *proxy,
                                                         const gchar         *property_name,
                                                         GVariant            *value);
gchar          **g_dbus_proxy_get_cached_property_names (GDBusProxy          *proxy);
void             g_dbus_proxy_call                      (GDBusProxy          *proxy,
                                                         const gchar         *method_name,
                                                         GVariant            *parameters,
                                                         GDBusCallFlags       flags,
                                                         gint                 timeout_msec,
                                                         GCancellable        *cancellable,
                                                         GAsyncReadyCallback  callback,
                                                         gpointer             user_data);
GVariant        *g_dbus_proxy_call_finish               (GDBusProxy          *proxy,
                                                         GAsyncResult        *res,
                                                         GError             **error);
GVariant        *g_dbus_proxy_call_sync                 (GDBusProxy          *proxy,
                                                         const gchar         *method_name,
                                                         GVariant            *parameters,
                                                         GDBusCallFlags       flags,
                                                         gint                 timeout_msec,
                                                         GCancellable        *cancellable,
                                                         GError             **error);

G_END_DECLS

#endif /* __G_DBUS_PROXY_H__ */
