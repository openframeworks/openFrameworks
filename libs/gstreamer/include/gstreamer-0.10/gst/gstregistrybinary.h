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

/* SUGGESTIONS AND TODO :
** ====================
** - Use a compressed registry, but would induce performance loss
** - Encrypt the registry, for security purpose, but would also reduce performances
*/

#ifndef __GST_REGISTRYBINARY_H__
#define __GST_REGISTRYBINARY_H__

#include <gst/gstpad.h>
#include <gst/gstregistry.h>

G_BEGIN_DECLS

/*
 * GST_MAGIC_BINARY_REGISTRY_STR:
 *
 * A tag, written at the beginning of the file
 */
#define GST_MAGIC_BINARY_REGISTRY_STR "\xc0\xde\xf0\x0d"
/*
 * GST_MAGIC_BINARY_REGISTRY_LEN:
 *
 * length of the header tag.
 */
#define GST_MAGIC_BINARY_REGISTRY_LEN (4)

/*
 * GST_MAGIC_BINARY_VERSION_STR:
 *
 * The current version of the binary registry format.
 * This _must_ be updated whenever the registry format changes,
 * we currently use the core version where this change happened.
 */
#define GST_MAGIC_BINARY_VERSION_STR ("0.10.30.1")

/*
 * GST_MAGIC_BINARY_VERSION_LEN:
 *
 * length of the version string.
 */
#define GST_MAGIC_BINARY_VERSION_LEN (64)

typedef struct _GstBinaryRegistryMagic
{
  gchar magic[GST_MAGIC_BINARY_REGISTRY_LEN];
  gchar version[GST_MAGIC_BINARY_VERSION_LEN];
} GstBinaryRegistryMagic;

G_END_DECLS

#endif /* !__GST_REGISTRYBINARY_H__ */

