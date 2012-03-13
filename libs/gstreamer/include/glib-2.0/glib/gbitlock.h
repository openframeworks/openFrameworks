/*
 * Copyright © 2008 Ryan Lortie
 * Copyright © 2010 Codethink Limited
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the licence, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author: Ryan Lortie <desrt@desrt.ca>
 */

#ifndef __G_BITLOCK_H__
#define __G_BITLOCK_H__

#include <glib/gtypes.h>

#if defined(G_DISABLE_SINGLE_INCLUDES) && !defined (__GLIB_H_INSIDE__) && !defined (GLIB_COMPILATION)
#error "Only <glib.h> can be included directly."
#endif

G_BEGIN_DECLS

void      g_bit_lock                      (volatile gint *address,
                                           gint           lock_bit);
gboolean  g_bit_trylock                   (volatile gint *address,
                                           gint           lock_bit);
void      g_bit_unlock                    (volatile gint *address,
                                           gint           lock_bit);

G_END_DECLS

#endif /* __G_BITLOCK_H_ */
