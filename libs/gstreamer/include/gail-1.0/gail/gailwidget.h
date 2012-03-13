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

#ifndef __GAIL_WIDGET_H__
#define __GAIL_WIDGET_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GAIL_TYPE_WIDGET                     (gail_widget_get_type ())
#define GAIL_WIDGET(obj)                     (G_TYPE_CHECK_INSTANCE_CAST ((obj), GAIL_TYPE_WIDGET, GailWidget))
#define GAIL_WIDGET_CLASS(klass)             (G_TYPE_CHECK_CLASS_CAST ((klass), GAIL_TYPE_WIDGET, GailWidgetClass))
#define GAIL_IS_WIDGET(obj)                  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GAIL_TYPE_WIDGET))
#define GAIL_IS_WIDGET_CLASS(klass)          (G_TYPE_CHECK_CLASS_TYPE ((klass), GAIL_TYPE_WIDGET))
#define GAIL_WIDGET_GET_CLASS(obj)           (G_TYPE_INSTANCE_GET_CLASS ((obj), GAIL_TYPE_WIDGET, GailWidgetClass))

typedef struct _GailWidget                   GailWidget;
typedef struct _GailWidgetClass              GailWidgetClass;

struct _GailWidget
{
  GtkAccessible parent;
};

GType gail_widget_get_type (void);

struct _GailWidgetClass
{
  GtkAccessibleClass parent_class;

  /*
   * Signal handler for notify signal on GTK widget
   */
  void (*notify_gtk)                   (GObject             *object,
                                        GParamSpec          *pspec);
  /*
   * Signal handler for focus_in_event and focus_out_event signal on GTK widget
   */
  gboolean (*focus_gtk)                (GtkWidget           *widget,
                                        GdkEventFocus       *event);

};

AtkObject*     gail_widget_new         (GtkWidget       *widget);

G_END_DECLS

#endif /* __GAIL_WIDGET_H__ */
