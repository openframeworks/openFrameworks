/* GAIL - The GNOME Accessibility Implementation Library
 * Copyright 2001 Sun Microsystems Inc.
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

#ifndef __GAIL_TEXT_UTIL_H__
#define __GAIL_TEXT_UTIL_H__

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GAIL_TYPE_TEXT_UTIL                  (gail_text_util_get_type ())
#define GAIL_TEXT_UTIL(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GAIL_TYPE_TEXT_UTIL, GailTextUtil))
#define GAIL_TEXT_UTIL_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GAIL_TYPE_TEXT_UTIL, GailTextUtilClass))
#define GAIL_IS_TEXT_UTIL(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GAIL_TYPE_TEXT_UTIL))
#define GAIL_IS_TEXT_UTIL_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GAIL_TYPE_TEXT_UTIL))
#define GAIL_TEXT_UTIL_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GAIL_TYPE_TEXT_UTIL, GailTextUtilClass))

/**
 *GailOffsetType:
 *@GAIL_BEFORE_OFFSET: Text before offset is required.
 *@GAIL_AT_OFFSET: Text at offset is required,
 *@GAIL_AFTER_OFFSET: Text after offset is required.
 *
 * Specifies which of the functions atk_text_get_text_before_offset(),
 * atk_text_get_text_at_offset(), atk_text_get_text_after_offset() the
 * function gail_text_util_get_text() is being called for.
 **/
typedef enum
{
  GAIL_BEFORE_OFFSET,
  GAIL_AT_OFFSET,
  GAIL_AFTER_OFFSET
}GailOffsetType;

typedef struct _GailTextUtil		GailTextUtil;
typedef struct _GailTextUtilClass	GailTextUtilClass;

struct _GailTextUtil
{
  GObject parent;

  GtkTextBuffer *buffer;
};

struct _GailTextUtilClass
{
  GObjectClass parent_class;
};

GType         gail_text_util_get_type      (void);
GailTextUtil* gail_text_util_new           (void);

void          gail_text_util_text_setup    (GailTextUtil    *textutil,
                                            const gchar     *text);
void          gail_text_util_buffer_setup  (GailTextUtil    *textutil,
                                            GtkTextBuffer   *buffer);
gchar*        gail_text_util_get_text      (GailTextUtil    *textutil,
                                             gpointer        layout,
                                            GailOffsetType  function,
                                            AtkTextBoundary boundary_type,
                                            gint            offset,
                                            gint            *start_offset,
                                            gint            *end_offset);
gchar*        gail_text_util_get_substring (GailTextUtil    *textutil,
                                            gint            start_pos,
                                            gint            end_pos);

G_END_DECLS

#endif /*__GAIL_TEXT_UTIL_H__ */
