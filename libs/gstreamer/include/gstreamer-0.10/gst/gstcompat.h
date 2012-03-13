/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2004 Wim Taymans <wim@fluendo.com>
 *
 * gstcompat.h: backwards compatibility stuff
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
/**
 * SECTION:gstcompat
 * @short_description: Deprecated API entries
 *
 * Please do not use these in new code.
 * These symbols are only available by defining GST_DISABLE_DEPRECATED.
 * This can be done in CFLAGS for compiling old code.
 */

/* API compatibility stuff */
#ifndef __GSTCOMPAT_H__
#define __GSTCOMPAT_H__

G_BEGIN_DECLS

#ifndef GST_DISABLE_DEPRECATED

#endif /* not GST_DISABLE_DEPRECATED */

G_END_DECLS

#endif /* __GSTCOMPAT_H__ */
