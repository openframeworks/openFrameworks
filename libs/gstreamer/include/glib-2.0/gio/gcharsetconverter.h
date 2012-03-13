/* GIO - GLib Input, Output and Streaming Library
 *
 * Copyright (C) 2009 Red Hat, Inc.
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

#ifndef __G_CHARSET_CONVERTER_H__
#define __G_CHARSET_CONVERTER_H__

#include <gio/gconverter.h>

G_BEGIN_DECLS

#define G_TYPE_CHARSET_CONVERTER         (g_charset_converter_get_type ())
#define G_CHARSET_CONVERTER(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), G_TYPE_CHARSET_CONVERTER, GCharsetConverter))
#define G_CHARSET_CONVERTER_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), G_TYPE_CHARSET_CONVERTER, GCharsetConverterClass))
#define G_IS_CHARSET_CONVERTER(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), G_TYPE_CHARSET_CONVERTER))
#define G_IS_CHARSET_CONVERTER_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), G_TYPE_CHARSET_CONVERTER))
#define G_CHARSET_CONVERTER_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), G_TYPE_CHARSET_CONVERTER, GCharsetConverterClass))

typedef struct _GCharsetConverterClass   GCharsetConverterClass;

struct _GCharsetConverterClass
{
  GObjectClass parent_class;
};

GType              g_charset_converter_get_type      (void) G_GNUC_CONST;

GCharsetConverter *g_charset_converter_new            (const gchar  *to_charset,
						       const gchar  *from_charset,
						       GError **error);
void               g_charset_converter_set_use_fallback (GCharsetConverter *converter,
							 gboolean use_fallback);
gboolean           g_charset_converter_get_use_fallback (GCharsetConverter *converter);
guint              g_charset_converter_get_num_fallbacks (GCharsetConverter *converter);

G_END_DECLS

#endif /* __G_CHARSET_CONVERTER_H__ */
