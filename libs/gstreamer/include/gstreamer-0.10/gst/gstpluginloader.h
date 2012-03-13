/* GStreamer
 * Copyright (C) 2008 Jan Schmidt <jan.schmidt@sun.com>
 *
 * gstpluginloader.h: Helper for out-of-process plugin loading. Private header.
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
#ifndef __GST_PLUGINLOADER_H__
#define __GST_PLUGINLOADER_H__

G_BEGIN_DECLS

typedef struct _GstPluginLoader GstPluginLoader;

typedef struct _GstPluginLoaderFuncs {
  GstPluginLoader * (*create)(GstRegistry *registry);
  gboolean (*destroy)(GstPluginLoader *loader);
  gboolean (*load)(GstPluginLoader *loader, const gchar *filename,
      off_t file_size, time_t file_mtime);
} GstPluginLoaderFuncs;

extern const GstPluginLoaderFuncs _priv_gst_plugin_loader_funcs;

G_END_DECLS

#endif /* __GST_PLUGINLOADER_H__ */
