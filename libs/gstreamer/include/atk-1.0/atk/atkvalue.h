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

#ifndef __ATK_VALUE_H__
#define __ATK_VALUE_H__

#include <atk/atkobject.h>

G_BEGIN_DECLS

/*
 * The AtkValue interface should be supported by any object that 
 * supports a numerical value (e.g., a scroll bar). This interface 
 * provides the standard mechanism for an assistive technology to 
 * determine and set the numerical value as well as get the minimum 
 * and maximum values.
 */

#define ATK_TYPE_VALUE                    (atk_value_get_type ())
#define ATK_IS_VALUE(obj)                 G_TYPE_CHECK_INSTANCE_TYPE ((obj), ATK_TYPE_VALUE)
#define ATK_VALUE(obj)                    G_TYPE_CHECK_INSTANCE_CAST ((obj), ATK_TYPE_VALUE, AtkValue)
#define ATK_VALUE_GET_IFACE(obj)          (G_TYPE_INSTANCE_GET_INTERFACE ((obj), ATK_TYPE_VALUE, AtkValueIface))

#ifndef _TYPEDEF_ATK_VALUE_
#define _TYPEDEF_ATK_VALUE__
typedef struct _AtkValue AtkValue;
#endif
typedef struct _AtkValueIface AtkValueIface;

struct _AtkValueIface
{
  GTypeInterface parent;

  void     (* get_current_value) (AtkValue     *obj,
                                  GValue       *value);
  void     (* get_maximum_value) (AtkValue     *obj,
                                  GValue       *value);
  void     (* get_minimum_value) (AtkValue     *obj,
                                  GValue       *value);
  gboolean (* set_current_value) (AtkValue     *obj,
                                  const GValue *value);
  void     (* get_minimum_increment) (AtkValue   *obj,
				      GValue     *value);
  AtkFunction pad1;
};

GType            atk_value_get_type (void);

void      atk_value_get_current_value (AtkValue     *obj,
                                       GValue       *value);


void     atk_value_get_maximum_value  (AtkValue     *obj,
                                       GValue       *value);

void     atk_value_get_minimum_value  (AtkValue     *obj,
                                       GValue       *value);

gboolean atk_value_set_current_value  (AtkValue     *obj,
                                       const GValue *value);

void     atk_value_get_minimum_increment  (AtkValue     *obj,
					   GValue       *value);

/*
 * Additional GObject properties exported by GaccessibleValue:
 *    "accessible_value"
 *       (the accessible value has changed)
 */

G_END_DECLS

#endif /* __ATK_VALUE_H__ */
