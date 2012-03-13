/* ATK -  Accessibility Toolkit
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

#if defined(ATK_DISABLE_SINGLE_INCLUDES) && !defined (__ATK_H_INSIDE__) && !defined (ATK_COMPILATION)
#error "Only <atk/atk.h> can be included directly."
#endif

#ifndef __ATK_COMPONENT_H__
#define __ATK_COMPONENT_H__

#include <atk/atkobject.h>
#include <atk/atkutil.h>

G_BEGIN_DECLS

/*
 * The AtkComponent interface should be supported by any object that is 
 * rendered on the screen. The interface provides the standard mechanism 
 * for an assistive technology to determine and set the graphical
 * representation of an object.
 */

#define ATK_TYPE_COMPONENT                    (atk_component_get_type ())
#define ATK_IS_COMPONENT(obj)                 G_TYPE_CHECK_INSTANCE_TYPE ((obj), ATK_TYPE_COMPONENT)
#define ATK_COMPONENT(obj)                    G_TYPE_CHECK_INSTANCE_CAST ((obj), ATK_TYPE_COMPONENT, AtkComponent)
#define ATK_COMPONENT_GET_IFACE(obj)          (G_TYPE_INSTANCE_GET_INTERFACE ((obj), ATK_TYPE_COMPONENT, AtkComponentIface))

#ifndef _TYPEDEF_ATK_COMPONENT_
#define _TYPEDEF_ATK_COMPONENT_
typedef struct _AtkComponent AtkComponent;
#endif
typedef struct _AtkComponentIface  AtkComponentIface;

typedef void (*AtkFocusHandler) (AtkObject*, gboolean);

typedef struct _AtkRectangle       AtkRectangle;

struct _AtkRectangle
{
  gint x;
  gint y;
  gint width;
  gint height;
};

GType atk_rectangle_get_type (void);

#define ATK_TYPE_RECTANGLE (atk_rectangle_get_type ())
struct _AtkComponentIface
{
  GTypeInterface parent;

  guint          (* add_focus_handler)  (AtkComponent          *component,
                                         AtkFocusHandler        handler);

  gboolean       (* contains)           (AtkComponent          *component,
                                         gint                   x,
                                         gint                   y,
                                         AtkCoordType           coord_type);

  AtkObject*    (* ref_accessible_at_point)  (AtkComponent     *component,
                                         gint                   x,
                                         gint                   y,
                                         AtkCoordType           coord_type);
  void          (* get_extents)         (AtkComponent          *component,
                                         gint                  *x,
                                         gint                  *y,
                                         gint                  *width,
                                         gint                  *height,
                                         AtkCoordType          coord_type);
  void                     (* get_position)     (AtkComponent   *component,
                                                 gint           *x,
                                                 gint           *y,
                                                 AtkCoordType   coord_type);
  void                     (* get_size)                 (AtkComponent   *component,
                                                         gint           *width,
                                                         gint           *height);
  gboolean                 (* grab_focus)               (AtkComponent   *component);
  void                     (* remove_focus_handler)      (AtkComponent  *component,
                                                          guint         handler_id);
  gboolean                 (* set_extents)      (AtkComponent   *component,
                                                 gint           x,
                                                 gint           y,
                                                 gint           width,
                                                 gint           height,
                                                 AtkCoordType   coord_type);
  gboolean                 (* set_position)     (AtkComponent   *component,
                                                 gint           x,
                                                 gint           y,
                                                 AtkCoordType   coord_type);
  gboolean                 (* set_size)         (AtkComponent   *component,
                                                 gint           width,
                                                 gint           height);
  	
  AtkLayer                 (* get_layer)        (AtkComponent   *component);
  gint                     (* get_mdi_zorder)   (AtkComponent   *component);

  /*
   * signal handlers
   */
  void                     (* bounds_changed)   (AtkComponent   *component,
                                                 AtkRectangle   *bounds);
  gdouble                  (* get_alpha)        (AtkComponent   *component);
};

GType atk_component_get_type (void);

/* convenience functions */

guint                atk_component_add_focus_handler      (AtkComponent    *component,
                                                           AtkFocusHandler handler);
gboolean              atk_component_contains               (AtkComponent    *component,
                                                            gint            x,
                                                            gint            y,
                                                            AtkCoordType    coord_type);
AtkObject*            atk_component_ref_accessible_at_point(AtkComponent    *component,
                                                            gint            x,
                                                            gint            y,
                                                            AtkCoordType    coord_type);
void                  atk_component_get_extents            (AtkComponent    *component,
                                                            gint            *x,
                                                            gint            *y,
                                                            gint            *width,
                                                            gint            *height,
                                                            AtkCoordType    coord_type);
void                  atk_component_get_position           (AtkComponent    *component,
                                                            gint            *x,
                                                            gint            *y,
                                                            AtkCoordType    coord_type);
void                  atk_component_get_size               (AtkComponent    *component,
                                                            gint            *width,
                                                            gint            *height);
AtkLayer              atk_component_get_layer              (AtkComponent    *component);
gint                  atk_component_get_mdi_zorder         (AtkComponent    *component);
gboolean              atk_component_grab_focus             (AtkComponent    *component);
void                  atk_component_remove_focus_handler   (AtkComponent    *component,
                                                            guint           handler_id);
gboolean              atk_component_set_extents            (AtkComponent    *component,
                                                            gint            x,
                                                            gint            y,
                                                            gint            width,
                                                            gint            height,
                                                            AtkCoordType    coord_type);
gboolean              atk_component_set_position           (AtkComponent    *component,
                                                            gint            x,
                                                            gint            y,
                                                            AtkCoordType    coord_type);
gboolean              atk_component_set_size               (AtkComponent    *component,
                                                            gint            width,
                                                            gint            height);
gdouble               atk_component_get_alpha              (AtkComponent    *component);

G_END_DECLS

#endif /* __ATK_COMPONENT_H__ */
