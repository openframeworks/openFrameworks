/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wtay@chello.be>
 *
 * gst_private.h: Private header for within libgst
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

#ifndef __GST_PRIVATE_H__
#define __GST_PRIVATE_H__

#ifdef HAVE_CONFIG_H
# ifndef GST_LICENSE   /* don't include config.h twice, it has no guards */
#  include "config.h"
# endif
#endif

/* This needs to be before glib.h, since it might be used in inline
 * functions */
extern const char             g_log_domain_gstreamer[];

#include <glib.h>

#include <stdlib.h>
#include <string.h>

/* Needed for GstRegistry * */
#include "gstregistry.h"
#include "gststructure.h"

/* we need this in pretty much all files */
#include "gstinfo.h"

/* for the flags in the GstPluginDep structure below */
#include "gstplugin.h"

G_BEGIN_DECLS

/* used by gstparse.c and grammar.y */
struct _GstParseContext {
  GList * missing_elements;
};

/* used by gstplugin.c and gstregistrybinary.c */
typedef struct {
  /* details registered via gst_plugin_add_dependency() */
  GstPluginDependencyFlags  flags;
  gchar **env_vars;
  gchar **paths;
  gchar **names;

  /* information saved from the last time the plugin was loaded (-1 = unset) */
  guint   env_hash;  /* hash of content of environment variables in env_vars */
  guint   stat_hash; /* hash of stat() on all relevant files and directories */
} GstPluginDep;

struct _GstPluginPrivate {
  GList *deps;    /* list of GstPluginDep structures */
  GstStructure *cache_data;
};

gboolean priv_gst_plugin_loading_have_whitelist (void);

guint32  priv_gst_plugin_loading_get_whitelist_hash (void);

gboolean priv_gst_plugin_desc_is_whitelisted (GstPluginDesc * desc,
                                              const gchar   * filename);

gboolean _priv_plugin_deps_env_vars_changed (GstPlugin * plugin);
gboolean _priv_plugin_deps_files_changed (GstPlugin * plugin);

gboolean _priv_gst_in_valgrind (void);

/* Initialize GStreamer private quark storage */
void _priv_gst_quarks_initialize (void);

/* Other init functions called from gst_init().
 * FIXME 0.11: rename to _priv_gst_foo_init() so they don't get exported
 * (can't do this now because these functions used to be in our public
 * headers, so at least the symbols need to continue to be available unless
 * we want enterprise edition packagers dancing on our heads) */
void  _gst_buffer_initialize (void);
void  _gst_buffer_list_initialize (void);
void  _gst_event_initialize (void);
void  _gst_format_initialize (void);
void  _gst_message_initialize (void);
void  _gst_plugin_initialize (void);
void  _gst_query_initialize (void);
void  _gst_tag_initialize (void);
void  _gst_value_initialize (void);

/* Private registry functions */
gboolean _priv_gst_registry_remove_cache_plugins (GstRegistry *registry);
void _priv_gst_registry_cleanup (void);
gboolean _gst_plugin_loader_client_run (void);

/* used in both gststructure.c and gstcaps.c; numbers are completely made up */
#define STRUCTURE_ESTIMATED_STRING_LEN(s) (16 + (s)->fields->len * 22)

gboolean  priv_gst_structure_append_to_gstring (const GstStructure * structure,
                                                GString            * s);
/* registry cache backends */
/* FIXME 0.11: use priv_ prefix */
gboolean 		gst_registry_binary_read_cache 	(GstRegistry * registry, const char *location);
gboolean 		gst_registry_binary_write_cache	(GstRegistry * registry, const char *location);


/* used in gstvalue.c and gststructure.c */
#define GST_ASCII_IS_STRING(c) (g_ascii_isalnum((c)) || ((c) == '_') || \
    ((c) == '-') || ((c) == '+') || ((c) == '/') || ((c) == ':') || \
    ((c) == '.'))

/* This is only meant for internal uses */
gint priv_gst_date_time_compare (gconstpointer dt1, gconstpointer dt2);

#ifndef GST_DISABLE_REGISTRY
/* Secret variable to initialise gst without registry cache */
extern gboolean _gst_disable_registry_cache;
#endif

/* provide inline gst_g_value_get_foo_unchecked(), used in gststructure.c */
#define DEFINE_INLINE_G_VALUE_GET_UNCHECKED(ret_type,name_type,v_field) \
static inline ret_type                                                  \
gst_g_value_get_##name_type##_unchecked (const GValue *value)           \
{                                                                       \
  return value->data[0].v_field;                                        \
}

DEFINE_INLINE_G_VALUE_GET_UNCHECKED(gboolean,boolean,v_int)
DEFINE_INLINE_G_VALUE_GET_UNCHECKED(gint,int,v_int)
DEFINE_INLINE_G_VALUE_GET_UNCHECKED(guint,uint,v_uint)
DEFINE_INLINE_G_VALUE_GET_UNCHECKED(gint64,int64,v_int64)
DEFINE_INLINE_G_VALUE_GET_UNCHECKED(guint64,uint64,v_uint64)
DEFINE_INLINE_G_VALUE_GET_UNCHECKED(gfloat,float,v_float)
DEFINE_INLINE_G_VALUE_GET_UNCHECKED(gdouble,double,v_double)
DEFINE_INLINE_G_VALUE_GET_UNCHECKED(const gchar *,string,v_pointer)


/*** debugging categories *****************************************************/

#ifndef GST_REMOVE_GST_DEBUG

GST_EXPORT GstDebugCategory *GST_CAT_GST_INIT;
GST_EXPORT GstDebugCategory *GST_CAT_AUTOPLUG; /* FIXME 0.11: remove? */
GST_EXPORT GstDebugCategory *GST_CAT_AUTOPLUG_ATTEMPT; /* FIXME 0.11: remove? */
GST_EXPORT GstDebugCategory *GST_CAT_PARENTAGE;
GST_EXPORT GstDebugCategory *GST_CAT_STATES;
GST_EXPORT GstDebugCategory *GST_CAT_SCHEDULING;
GST_EXPORT GstDebugCategory *GST_CAT_BUFFER;
GST_EXPORT GstDebugCategory *GST_CAT_BUFFER_LIST;
GST_EXPORT GstDebugCategory *GST_CAT_BUS;
GST_EXPORT GstDebugCategory *GST_CAT_CAPS;
GST_EXPORT GstDebugCategory *GST_CAT_CLOCK;
GST_EXPORT GstDebugCategory *GST_CAT_ELEMENT_PADS;
GST_EXPORT GstDebugCategory *GST_CAT_PADS;
GST_EXPORT GstDebugCategory *GST_CAT_PERFORMANCE;
GST_EXPORT GstDebugCategory *GST_CAT_PIPELINE;
GST_EXPORT GstDebugCategory *GST_CAT_PLUGIN_LOADING;
GST_EXPORT GstDebugCategory *GST_CAT_PLUGIN_INFO;
GST_EXPORT GstDebugCategory *GST_CAT_PROPERTIES;
GST_EXPORT GstDebugCategory *GST_CAT_XML;
GST_EXPORT GstDebugCategory *GST_CAT_NEGOTIATION;
GST_EXPORT GstDebugCategory *GST_CAT_REFCOUNTING;
GST_EXPORT GstDebugCategory *GST_CAT_ERROR_SYSTEM;
GST_EXPORT GstDebugCategory *GST_CAT_EVENT;
GST_EXPORT GstDebugCategory *GST_CAT_MESSAGE;
GST_EXPORT GstDebugCategory *GST_CAT_PARAMS;
GST_EXPORT GstDebugCategory *GST_CAT_CALL_TRACE;
GST_EXPORT GstDebugCategory *GST_CAT_SIGNAL;
GST_EXPORT GstDebugCategory *GST_CAT_PROBE;
GST_EXPORT GstDebugCategory *GST_CAT_REGISTRY;
GST_EXPORT GstDebugCategory *GST_CAT_QOS;
GST_EXPORT GstDebugCategory *GST_CAT_TYPES; /* FIXME 0.11: remove? */

/* Categories that should be completely private to
 * libgstreamer should be done like this: */
#define GST_CAT_POLL _priv_GST_CAT_POLL
extern GstDebugCategory *_priv_GST_CAT_POLL;

#else

#define GST_CAT_GST_INIT         NULL
#define GST_CAT_AUTOPLUG         NULL
#define GST_CAT_AUTOPLUG_ATTEMPT NULL
#define GST_CAT_PARENTAGE        NULL
#define GST_CAT_STATES           NULL
#define GST_CAT_SCHEDULING       NULL
#define GST_CAT_DATAFLOW         NULL
#define GST_CAT_BUFFER           NULL
#define GST_CAT_BUFFER_LIST      NULL
#define GST_CAT_BUS              NULL
#define GST_CAT_CAPS             NULL
#define GST_CAT_CLOCK            NULL
#define GST_CAT_ELEMENT_PADS     NULL
#define GST_CAT_PADS             NULL
#define GST_CAT_PERFORMANCE      NULL
#define GST_CAT_PIPELINE         NULL
#define GST_CAT_PLUGIN_LOADING   NULL
#define GST_CAT_PLUGIN_INFO      NULL
#define GST_CAT_PROPERTIES       NULL
#define GST_CAT_XML              NULL
#define GST_CAT_NEGOTIATION      NULL
#define GST_CAT_REFCOUNTING      NULL
#define GST_CAT_ERROR_SYSTEM     NULL
#define GST_CAT_EVENT            NULL
#define GST_CAT_MESSAGE          NULL
#define GST_CAT_PARAMS           NULL
#define GST_CAT_CALL_TRACE       NULL
#define GST_CAT_SIGNAL           NULL
#define GST_CAT_PROBE            NULL
#define GST_CAT_REGISTRY         NULL
#define GST_CAT_QOS              NULL
#define GST_CAT_TYPES            NULL
#define GST_CAT_POLL             NULL

#endif

G_END_DECLS
#endif /* __GST_PRIVATE_H__ */
