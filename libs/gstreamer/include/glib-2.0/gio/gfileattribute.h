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

#ifndef __G_FILE_ATTRIBUTE_H__
#define __G_FILE_ATTRIBUTE_H__

#include <gio/giotypes.h>

G_BEGIN_DECLS

/**
 * GFileAttributeInfo:
 * @name: the name of the attribute.
 * @type: the #GFileAttributeType type of the attribute.
 * @flags: a set of #GFileAttributeInfoFlags.
 *
 * Information about a specific attribute.
 **/
struct _GFileAttributeInfo
{
  char                    *name;
  GFileAttributeType       type;
  GFileAttributeInfoFlags  flags;
};

/**
 * GFileAttributeInfoList:
 * @infos: an array of #GFileAttributeInfo<!-- -->s.
 * @n_infos: the number of values in the array.
 *
 * Acts as a lightweight registry for possible valid file attributes.
 * The registry stores Key-Value pair formats as #GFileAttributeInfo<!-- -->s.
 **/
struct _GFileAttributeInfoList
{
  GFileAttributeInfo *infos;
  int                 n_infos;
};

GType                     g_file_attribute_info_list_get_type (void);
GFileAttributeInfoList *  g_file_attribute_info_list_new    (void);
GFileAttributeInfoList *  g_file_attribute_info_list_ref    (GFileAttributeInfoList *list);
void                      g_file_attribute_info_list_unref  (GFileAttributeInfoList *list);
GFileAttributeInfoList *  g_file_attribute_info_list_dup    (GFileAttributeInfoList *list);
const GFileAttributeInfo *g_file_attribute_info_list_lookup (GFileAttributeInfoList *list,
							     const char             *name);
void                      g_file_attribute_info_list_add    (GFileAttributeInfoList *list,
							     const char             *name,
							     GFileAttributeType      type,
							     GFileAttributeInfoFlags flags);

G_END_DECLS

#endif /* __G_FILE_INFO_H__ */
