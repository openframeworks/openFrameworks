/* GStreamer
 * Copyright (C) 2006 Josep Torra <josep@fluendo.com>
 * Copyright (C) 2006 Mathieu Garcia  <matthieu@fluendo.com>
 * Copyright (C) 2006 Stefan Kost <ensonic@sonicpulse.de>
 *
 * gstregistrybinary.h: Header for registry handling
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
#ifndef __GST_REGISTRYCHUNKS_H__
#define __GST_REGISTRYCHUNKS_H__

#include <gst/gstpad.h>
#include <gst/gstregistry.h>

/*
 * we reference strings directly from the plugins and in this case set CONST to
 * avoid freeing them. If g_free() should be used, the MALLOC flag is set,
 * otherwise g_slice_free1() will be used!
 */
enum {
  GST_REGISTRY_CHUNK_FLAG_NONE = 0,
  GST_REGISTRY_CHUNK_FLAG_CONST = 1,
  GST_REGISTRY_CHUNK_FLAG_MALLOC = 2,
};

/*
 * GstRegistryChunk:
 *
 * Header for binary blobs
 */
typedef struct _GstRegistryChunk
{
  gpointer data;
  guint size;
  guint flags;
  gboolean align;
} GstRegistryChunk;

typedef struct _GstRegistryChunkGlobalHeader
{
  guint32  filter_env_hash;
} GstRegistryChunkGlobalHeader;

/*
 * GstRegistryChunkPluginElement:
 *
 * @n_deps: Says how many dependency structures follows.
 *
 * @nfeatures: says how many binary plugin feature structures we will have
 * right after the structure itself.
 *
 * A structure containing (staticely) every information needed for a plugin
 */

typedef struct _GstRegistryChunkPluginElement
{
  gulong file_size;
  gulong file_mtime;

  guint n_deps;

  guint nfeatures;
} GstRegistryChunkPluginElement;

/* GstRegistryChunkDep:
 */
typedef struct _GstRegistryChunkDep
{
  guint flags;
  guint n_env_vars;
  guint n_paths;
  guint n_names;

  guint env_hash;
  guint stat_hash;
} GstRegistryChunkDep;

/*
 * GstRegistryChunkPluginFeature:
 * @rank: rank of the feature
 *
 * A structure containing the plugin features
 */
typedef struct _GstRegistryChunkPluginFeature
{
  gulong rank;
} GstRegistryChunkPluginFeature;

/*
 * GstRegistryChunkElementFactory:
 * @npadtemplates: stores the number of GstRegistryChunkPadTemplate structures
 * following the structure
 * @ninterfaces: stores the number of interface names following the structure
 * @nuriprotocols: stores the number of protocol strings following the structure
 *
 * A structure containing the element factory fields
 */
typedef struct _GstRegistryChunkElementFactory
{
  GstRegistryChunkPluginFeature plugin_feature;

  guint npadtemplates;
  guint ninterfaces;
  guint nuriprotocols;
} GstRegistryChunkElementFactory;

/*
 * GstRegistryChunkTypeFindFactory:
 * @nextensions: stores the number of typefind extensions
 *
 * A structure containing the element factory fields
 */
typedef struct _GstRegistryChunkTypeFindFactory
{
  GstRegistryChunkPluginFeature plugin_feature;

  guint nextensions;
} GstRegistryChunkTypeFindFactory;

/*
 * GstRegistryChunkPadTemplate:
 *
 * A structure containing the static pad templates of a plugin feature
 */
typedef struct _GstRegistryChunkPadTemplate
{
  guint direction;	               /* Either 0:"sink" or 1:"src" */
  GstPadPresence presence;
} GstRegistryChunkPadTemplate;

G_BEGIN_DECLS

gboolean
_priv_gst_registry_chunks_save_plugin (GList ** list, GstRegistry * registry,
    GstPlugin * plugin);

gboolean
_priv_gst_registry_chunks_load_plugin (GstRegistry * registry, gchar ** in,
    gchar *end, GstPlugin **out_plugin);

void
_priv_gst_registry_chunks_save_global_header (GList ** list,
    GstRegistry * registry, guint32 filter_env_hash);

gboolean
_priv_gst_registry_chunks_load_global_header (GstRegistry * registry,
    gchar ** in, gchar *end, guint32 * filter_env_hash);

void
_priv_gst_registry_chunk_free (GstRegistryChunk *chunk);

G_END_DECLS

#endif /* __GST_REGISTRYCHUNKS_H__ */
