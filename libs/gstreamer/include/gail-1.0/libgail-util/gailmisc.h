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

#ifndef __GAIL_MISC_H__
#define __GAIL_MISC_H__

#include <glib-object.h>
#include <gtk/gtk.h>
#include <pango/pango.h>

G_BEGIN_DECLS

AtkAttributeSet* gail_misc_add_attribute          (AtkAttributeSet   *attrib_set,
                                                   AtkTextAttribute   attr,
                                                   gchar             *value);
AtkAttributeSet* gail_misc_layout_get_run_attributes
                                                  (AtkAttributeSet   *attrib_set,
                                                   PangoLayout       *layout,
                                                   gchar             *text,
                                                   gint              offset,
                                                   gint              *start_offset,
                                                   gint              *end_offset);

AtkAttributeSet* gail_misc_get_default_attributes (AtkAttributeSet   *attrib_set,
                                                   PangoLayout       *layout,
                                                   GtkWidget         *widget);

void             gail_misc_get_extents_from_pango_rectangle
                                                  (GtkWidget         *widget,
                                                   PangoRectangle    *char_rect,
                                                   gint              x_layout,
                                                   gint              y_layout,
                                                   gint              *x,
                    		                   gint              *y,
                                                   gint              *width,
                                                   gint              *height,
                                                   AtkCoordType      coords);

gint             gail_misc_get_index_at_point_in_layout
                                                  (GtkWidget         *widget,
                                                   PangoLayout       *layout, 
                                                   gint              x_layout,
                                                   gint              y_layout,
                                                   gint              x,
                                                   gint              y,
                                                   AtkCoordType      coords);

void		 gail_misc_get_origins            (GtkWidget         *widget,
                                                   gint              *x_window,
					           gint              *y_window,
					           gint              *x_toplevel,
					           gint              *y_toplevel);

AtkAttributeSet* gail_misc_add_to_attr_set        (AtkAttributeSet   *attrib_set,
			                           GtkTextAttributes *attrs,
			                           AtkTextAttribute  attr);

AtkAttributeSet* gail_misc_buffer_get_run_attributes
                                                  (GtkTextBuffer     *buffer,
                                                   gint              offset,
                                                   gint              *start_offset,
                                                   gint              *end_offset);

G_END_DECLS

#endif /*__GAIL_MISC_H__ */
