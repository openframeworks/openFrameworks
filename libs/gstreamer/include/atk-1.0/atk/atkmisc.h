/* ATK -  Accessibility Toolkit
 * Copyright 2007 Sun Microsystems Inc.
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

#ifndef __ATK_MISC_H__
#define __ATK_MISC_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define ATK_TYPE_MISC                   (atk_misc_get_type ())
#define ATK_IS_MISC(obj)                G_TYPE_CHECK_INSTANCE_TYPE ((obj), ATK_TYPE_MISC)
#define ATK_MISC(obj)                   G_TYPE_CHECK_INSTANCE_CAST ((obj), ATK_TYPE_MISC, AtkMisc)
#define ATK_MISC_CLASS(klass)                   (G_TYPE_CHECK_CLASS_CAST ((klass), ATK_TYPE_MISC, AtkMiscClass))
#define ATK_IS_MISC_CLASS(klass)                (G_TYPE_CHECK_CLASS_TYPE ((klass), ATK_TYPE_MISC))
#define ATK_MISC_GET_CLASS(obj)                 (G_TYPE_INSTANCE_GET_CLASS ((obj), ATK_TYPE_MISC, AtkMiscClass))


#ifndef _TYPEDEF_ATK_MISC_
#define _TYPEDEF_ATK_MISC_
typedef struct _AtkMisc      AtkMisc;
typedef struct _AtkMiscClass AtkMiscClass;
#endif

struct _AtkMisc
{
  GObject parent;
};

/*
 * Singleton instance - only the ATK implementation layer for
 * a given GUI toolkit/application instance should touch this
 * symbol directly.
 */
extern AtkMisc *atk_misc_instance;

struct _AtkMiscClass
{
   GObjectClass parent;
   void   (* threads_enter)                     (AtkMisc *misc);
   void   (* threads_leave)                     (AtkMisc *misc);
   gpointer vfuncs[32]; /* future bincompat */
};
GType atk_misc_get_type (void);

/*
 * Wrapper for thread lock, i.e. take the thread mutex for the GUI toolkit, 
 * if one exists.  
 * (This method is implemented by the toolkit ATK implementation layer;
 *  for instance, for GTK+, GAIL implements this via GDK_THREADS_ENTER).
 */
void     atk_misc_threads_enter  (AtkMisc *misc);

/*
 * Wrapper for thread lock, i.e. release the thread mutex for the GUI toolkit, 
 * if one exists.  
 * (This method is implemented by the toolkit ATK implementation layer;
 *  for instance, for GTK+, GAIL implements this via GDK_THREADS_LEAVE).
 */
void     atk_misc_threads_leave  (AtkMisc *misc);

/*
 * Obtain (singleton) instance of AtkMisc.
 */
const AtkMisc *atk_misc_get_instance (void);

G_END_DECLS

#endif /* __ATK_MISC_H__ */
