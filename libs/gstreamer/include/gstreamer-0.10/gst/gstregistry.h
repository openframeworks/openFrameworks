/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wim.taymans@chello.be>
 *
 * gstregistry.h: Header for registry handling
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */


#ifndef __GST_REGISTRY_H__
#define __GST_REGISTRY_H__

#include <gst/gstconfig.h>
#include <gst/gstplugin.h>
#include <gst/gstpluginfeature.h>

G_BEGIN_DECLS

#define GST_TYPE_REGISTRY 		(gst_registry_get_type ())
#define GST_REGISTRY(obj) 		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_REGISTRY, GstRegistry))
#define GST_IS_REGISTRY(obj) 		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_REGISTRY))
#define GST_REGISTRY_CLASS(klass) 	(G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_REGISTRY, GstRegistryClass))
#define GST_IS_REGISTRY_CLASS(klass) 	(G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_REGISTRY))
#define GST_REGISTRY_GET_CLASS(obj) 	(G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_REGISTRY, GstRegistryClass))

typedef struct _GstRegistry GstRegistry;
typedef struct _GstRegistryClass GstRegistryClass;
typedef struct _GstRegistryPrivate GstRegistryPrivate;

/**
 * GstRegistry:
 *
 * Opaque #GstRegistry structure.
 */
struct _GstRegistry {
  GstObject 	 object;

  /*< private >*/
  GList *plugins;
  GList *features;

  GList 	*paths;

  /* FIXME move these elsewhere */
  int            cache_file;

  /* hash to speedup _lookup_feature_locked() */
  GHashTable *feature_hash;
  /* hash to speedup _lookup */
  GHashTable *basename_hash;

  GstRegistryPrivate *priv;

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING-3];
};

struct _GstRegistryClass {
  GstObjectClass	parent_class;

  /* signals */
  void 			(*plugin_added)		(GstRegistry *registry, GstPlugin *plugin);
  void 			(*feature_added)	(GstRegistry *registry, GstPluginFeature *feature);

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
};


/* normal GObject stuff */
GType			gst_registry_get_type		(void);

GstRegistry *           gst_registry_get_default        (void);

gboolean			gst_registry_scan_path		(GstRegistry *registry, const gchar *path);
void                    gst_registry_add_path           (GstRegistry * registry, const gchar * path);
GList*			gst_registry_get_path_list	(GstRegistry *registry);

gboolean		gst_registry_add_plugin		(GstRegistry *registry, GstPlugin *plugin);
void			gst_registry_remove_plugin	(GstRegistry *registry, GstPlugin *plugin);
gboolean                gst_registry_add_feature        (GstRegistry * registry, GstPluginFeature * feature);
void                    gst_registry_remove_feature     (GstRegistry * registry, GstPluginFeature * feature);

GList*                  gst_registry_get_plugin_list    (GstRegistry *registry);
GList*			gst_registry_plugin_filter	(GstRegistry *registry,
							 GstPluginFilter filter,
							 gboolean first,
							 gpointer user_data);
GList*			gst_registry_feature_filter	(GstRegistry *registry,
							 GstPluginFeatureFilter filter,
							 gboolean first,
							 gpointer user_data);
GList *                 gst_registry_get_feature_list   (GstRegistry *registry,
                                                         GType type);
GList *                 gst_registry_get_feature_list_by_plugin (GstRegistry *registry, const gchar *name);
guint32                 gst_registry_get_feature_list_cookie (GstRegistry *registry);

GstPlugin*		gst_registry_find_plugin	(GstRegistry *registry, const gchar *name);
GstPluginFeature*	gst_registry_find_feature	(GstRegistry *registry, const gchar *name, GType type);

GstPlugin * 		gst_registry_lookup 		(GstRegistry *registry, const char *filename);
GstPluginFeature * 	gst_registry_lookup_feature 	(GstRegistry *registry, const char *name);

/* These are only here because at some point they were in a public header
 * (even though they should have been private) and we can't really remove
 * them now (FIXME: 0.11). They don't do anything other than return FALSE. */
#ifndef GST_DISABLE_DEPRECATED
gboolean 		gst_registry_xml_read_cache 	(GstRegistry * registry, const char *location);
gboolean 		gst_registry_xml_write_cache 	(GstRegistry * registry, const char *location);
#endif

/* convinience defines for the default registry */

/**
 * gst_default_registry_add_plugin:
 * @plugin: the plugin to add
 *
 * Add the plugin to the default registry.
 * The plugin-added signal will be emitted.
 *
 * Returns: TRUE on success.
 */
#define gst_default_registry_add_plugin(plugin) \
  gst_registry_add_plugin (gst_registry_get_default(), plugin)

/**
 * gst_default_registry_add_path:
 * @path: the path to add to the registry
 *
 * Add the given path to the default registry. The syntax of the
 * path is specific to the registry. If the path has already been
 * added, do nothing.
 */
#define gst_default_registry_add_path(path) \
  gst_registry_add_path (gst_registry_get_default(), path)

/**
 * gst_default_registry_get_path_list:
 *
 * Get the list of paths for the default registry.
 *
 * Returns: A Glist of paths as strings. g_list_free after use.
 */
#define gst_default_registry_get_path_list() \
  gst_registry_get_path_list (gst_registry_get_default())
  
/**
 * gst_default_registry_get_plugin_list:
 *
 * Get a copy of all plugins registered in the default registry.
 *
 * Returns: a copy of the list. Free after use.
 */
#define gst_default_registry_get_plugin_list() \
  gst_registry_get_plugin_list (gst_registry_get_default())

/**
 * gst_default_registry_find_feature:
 * @name: the pluginfeature name to find
 * @type: the pluginfeature type to find
 *
 * Find the pluginfeature with the given name and type in the default registry.
 *
 * Returns: The pluginfeature with the given name and type or NULL
 * if the plugin was not found.
 */
#define gst_default_registry_find_feature(name,type) \
  gst_registry_find_feature (gst_registry_get_default(),name,type)

/**
 * gst_default_registry_find_plugin:
 * @name: the plugin name to find
 *
 * Find the plugin with the given name in the default registry.
 * The plugin will be reffed; caller is responsible for unreffing.
 *
 * Returns: The plugin with the given name or NULL if the plugin was not found.
 */
#define gst_default_registry_find_plugin(name) \
  gst_registry_find_plugin (gst_registry_get_default(),name)

/**
 * gst_default_registry_feature_filter:
 * @filter: the filter to use
 * @first: only return first match
 * @user_data: user data passed to the filter function
 *
 * Runs a filter against all features of the plugins in the default registry
 * and returns a GList with the results.
 * If the first flag is set, only the first match is
 * returned (as a list with a single object).
 *
 * Returns: a GList of plugin features, gst_plugin_feature_list_free after use.
 */
#define gst_default_registry_feature_filter(filter,first,user_data) \
  gst_registry_feature_filter (gst_registry_get_default(),filter,first,user_data)

/**
 * gst_default_registry_get_feature_list_cookie:
 *
 * Returns the default registrys feature list cookie. This changes
 * every time a feature is added or removed from the registry.
 *
 * Returns: the feature list cookie.
 *
 * Since: 0.10.26
 */
#define gst_default_registry_get_feature_list_cookie() \
  gst_registry_get_feature_list_cookie (gst_registry_get_default())

gboolean                gst_default_registry_check_feature_version (const gchar *feature_name,
                                                                    guint        min_major,
                                                                    guint        min_minor,
                                                                    guint        min_micro);

G_END_DECLS

#endif /* __GST_REGISTRY_H__ */

