/* GIO - GLib Input, Output and Streaming Library
 *
 * Copyright (C) 2006-2007 Red Hat, Inc.
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
 * Author: Alexander Larsson <alexl@redhat.com>
 */

#if !defined (__GIO_GIO_H_INSIDE__) && !defined (GIO_COMPILATION)
#error "Only <gio/gio.h> can be included directly."
#endif

#ifndef __G_APP_INFO_H__
#define __G_APP_INFO_H__

#include <gio/giotypes.h>

G_BEGIN_DECLS

#define G_TYPE_APP_INFO            (g_app_info_get_type ())
#define G_APP_INFO(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), G_TYPE_APP_INFO, GAppInfo))
#define G_IS_APP_INFO(obj)	   (G_TYPE_CHECK_INSTANCE_TYPE ((obj), G_TYPE_APP_INFO))
#define G_APP_INFO_GET_IFACE(obj)  (G_TYPE_INSTANCE_GET_INTERFACE ((obj), G_TYPE_APP_INFO, GAppInfoIface))

#define G_TYPE_APP_LAUNCH_CONTEXT         (g_app_launch_context_get_type ())
#define G_APP_LAUNCH_CONTEXT(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), G_TYPE_APP_LAUNCH_CONTEXT, GAppLaunchContext))
#define G_APP_LAUNCH_CONTEXT_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), G_TYPE_APP_LAUNCH_CONTEXT, GAppLaunchContextClass))
#define G_IS_APP_LAUNCH_CONTEXT(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), G_TYPE_APP_LAUNCH_CONTEXT))
#define G_IS_APP_LAUNCH_CONTEXT_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), G_TYPE_APP_LAUNCH_CONTEXT))
#define G_APP_LAUNCH_CONTEXT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), G_TYPE_APP_LAUNCH_CONTEXT, GAppLaunchContextClass))

typedef struct _GAppLaunchContextClass   GAppLaunchContextClass;
typedef struct _GAppLaunchContextPrivate GAppLaunchContextPrivate;

/**
 * GAppInfo:
 *
 * Information about an installed application and methods to launch
 * it (with file arguments).
 */

/**
 * GAppInfoIface:
 * @g_iface: The parent interface.
 * @dup: Copies a #GAppInfo.
 * @equal: Checks two #GAppInfo<!-- -->s for equality.
 * @get_id: Gets a string identifier for a #GAppInfo.
 * @get_name: Gets the name of the application for a #GAppInfo.
 * @get_description: Gets a short description for the application described by the #GAppInfo.
 * @get_executable: Gets the executable name for the #GAppInfo.
 * @get_icon: Gets the #GIcon for the #GAppInfo.
 * @launch: Launches an application specified by the #GAppInfo.
 * @supports_uris: Indicates whether the application specified supports launching URIs.
 * @supports_files: Indicates whether the application specified accepts filename arguments.
 * @launch_uris: Launches an application with a list of URIs.
 * @should_show: Returns whether an application should be shown (e.g. when getting a list of installed applications).
 * <ulink url="http://standards.freedesktop.org/startup-notification-spec/startup-notification-latest.txt">
 * <citetitle>FreeDesktop.Org Startup Notification Specification</citetitle></ulink>.
 * @set_as_default_for_type: Sets an application as default for a given content type.
 * @set_as_default_for_extension: Sets an application as default for a given file extension.
 * @add_supports_type: Adds to the #GAppInfo information about supported file types.
 * @can_remove_supports_type: Checks for support for removing supported file types from a #GAppInfo.
 * @remove_supports_type: Removes a supported application type from a #GAppInfo.
 * @can_delete: Checks if a #GAppInfo can be deleted. Since 2.20
 * @do_delete: Deletes a #GAppInfo. Since 2.20
 * @get_commandline: Gets the commandline for the #GAppInfo. Since 2.20
 * @get_display_name: Gets the display name for the #GAppInfo. Since 2.24
 *
 * Application Information interface, for operating system portability.
 */
typedef struct _GAppInfoIface    GAppInfoIface;

struct _GAppInfoIface
{
  GTypeInterface g_iface;

  /* Virtual Table */

  GAppInfo *   (* dup)                          (GAppInfo           *appinfo);
  gboolean     (* equal)                        (GAppInfo           *appinfo1,
                                                 GAppInfo           *appinfo2);
  const char * (* get_id)                       (GAppInfo           *appinfo);
  const char * (* get_name)                     (GAppInfo           *appinfo);
  const char * (* get_description)              (GAppInfo           *appinfo);
  const char * (* get_executable)               (GAppInfo           *appinfo);
  GIcon *      (* get_icon)                     (GAppInfo           *appinfo);
  gboolean     (* launch)                       (GAppInfo           *appinfo,
                                                 GList              *filenames,
                                                 GAppLaunchContext  *launch_context,
                                                 GError            **error);
  gboolean     (* supports_uris)                (GAppInfo           *appinfo);
  gboolean     (* supports_files)               (GAppInfo           *appinfo);
  gboolean     (* launch_uris)                  (GAppInfo           *appinfo,
                                                 GList              *uris,
                                                 GAppLaunchContext  *launch_context,
                                                 GError            **error);
  gboolean     (* should_show)                  (GAppInfo           *appinfo);

  /* For changing associations */
  gboolean     (* set_as_default_for_type)      (GAppInfo           *appinfo,
                                                 const char         *content_type,
                                                 GError            **error);
  gboolean     (* set_as_default_for_extension) (GAppInfo           *appinfo,
                                                 const char         *extension,
                                                 GError            **error);
  gboolean     (* add_supports_type)            (GAppInfo           *appinfo,
                                                 const char         *content_type,
                                                 GError            **error);
  gboolean     (* can_remove_supports_type)     (GAppInfo           *appinfo);
  gboolean     (* remove_supports_type)         (GAppInfo           *appinfo,
                                                 const char         *content_type,
                                                 GError            **error);
  gboolean     (* can_delete)                   (GAppInfo           *appinfo);
  gboolean     (* do_delete)                    (GAppInfo           *appinfo);
  const char * (* get_commandline)              (GAppInfo           *appinfo);
  const char * (* get_display_name)             (GAppInfo           *appinfo);
};

GType       g_app_info_get_type                     (void) G_GNUC_CONST;
GAppInfo *  g_app_info_create_from_commandline      (const char           *commandline,
						     const char           *application_name,
						     GAppInfoCreateFlags   flags,
						     GError              **error);
GAppInfo *  g_app_info_dup                          (GAppInfo             *appinfo);
gboolean    g_app_info_equal                        (GAppInfo             *appinfo1,
						     GAppInfo             *appinfo2);
const char *g_app_info_get_id                       (GAppInfo             *appinfo);
const char *g_app_info_get_name                     (GAppInfo             *appinfo);
const char *g_app_info_get_display_name             (GAppInfo             *appinfo);
const char *g_app_info_get_description              (GAppInfo             *appinfo);
const char *g_app_info_get_executable               (GAppInfo             *appinfo);
const char *g_app_info_get_commandline              (GAppInfo             *appinfo);
GIcon *     g_app_info_get_icon                     (GAppInfo             *appinfo);
gboolean    g_app_info_launch                       (GAppInfo             *appinfo,
						     GList                *files,
						     GAppLaunchContext    *launch_context,
						     GError              **error);
gboolean    g_app_info_supports_uris                (GAppInfo             *appinfo);
gboolean    g_app_info_supports_files               (GAppInfo             *appinfo);
gboolean    g_app_info_launch_uris                  (GAppInfo             *appinfo,
						     GList                *uris,
						     GAppLaunchContext    *launch_context,
						     GError              **error);
gboolean    g_app_info_should_show                  (GAppInfo             *appinfo);

gboolean    g_app_info_set_as_default_for_type      (GAppInfo             *appinfo,
						     const char           *content_type,
						     GError              **error);
gboolean    g_app_info_set_as_default_for_extension (GAppInfo             *appinfo,
						     const char           *extension,
						     GError              **error);
gboolean    g_app_info_add_supports_type            (GAppInfo             *appinfo,
						     const char           *content_type,
						     GError              **error);
gboolean    g_app_info_can_remove_supports_type     (GAppInfo             *appinfo);
gboolean    g_app_info_remove_supports_type         (GAppInfo             *appinfo,
						     const char           *content_type,
						     GError              **error);
gboolean    g_app_info_can_delete                   (GAppInfo   *appinfo);
gboolean    g_app_info_delete                       (GAppInfo   *appinfo);

GList *   g_app_info_get_all                     (void);
GList *   g_app_info_get_all_for_type            (const char  *content_type);
void      g_app_info_reset_type_associations     (const char  *content_type);
GAppInfo *g_app_info_get_default_for_type        (const char  *content_type,
						  gboolean     must_support_uris);
GAppInfo *g_app_info_get_default_for_uri_scheme  (const char  *uri_scheme);

gboolean  g_app_info_launch_default_for_uri      (const char              *uri,
					          GAppLaunchContext       *launch_context,
					          GError                 **error);

/**
 * GAppLaunchContext:
 *
 * Integrating the launch with the launching application. This is used to
 * handle for instance startup notification and launching the new application
 * on the same screen as the launching window.
 */
struct _GAppLaunchContext
{
  GObject parent_instance;

  /*< private >*/
  GAppLaunchContextPrivate *priv;
};

struct _GAppLaunchContextClass
{
  GObjectClass parent_class;

  char * (* get_display)           (GAppLaunchContext *context,
                                    GAppInfo          *info,
                                    GList             *files);
  char * (* get_startup_notify_id) (GAppLaunchContext *context,
                                    GAppInfo          *info,
                                    GList             *files);
  void   (* launch_failed)         (GAppLaunchContext *context,
                                    const char        *startup_notify_id);

  /* Padding for future expansion */
  void (*_g_reserved1) (void);
  void (*_g_reserved2) (void);
  void (*_g_reserved3) (void);
  void (*_g_reserved4) (void);
  void (*_g_reserved5) (void);
};

GType              g_app_launch_context_get_type              (void) G_GNUC_CONST;
GAppLaunchContext *g_app_launch_context_new                   (void);
char *             g_app_launch_context_get_display           (GAppLaunchContext *context,
							       GAppInfo          *info,
							       GList             *files);
char *             g_app_launch_context_get_startup_notify_id (GAppLaunchContext *context,
							       GAppInfo          *info,
							       GList             *files);
void               g_app_launch_context_launch_failed         (GAppLaunchContext *context,
							       const char *       startup_notify_id);

G_END_DECLS

#endif /* __G_APP_INFO_H__ */
