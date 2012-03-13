/*
 * Farsight2 - Private declarations
 *
 * Copyright 2008 Collabora Ltd.
 *  @author: Olivier Crete <olivier.crete@collabora.co.uk>
 * Copyright 2008 Nokia Corp.
 *
 * fs-conference-iface.h - Header file for gstreamer interface to be
 *                         implemented by farsight conference elements
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */

#ifndef __FS_PRIVATE_H__
#define __FS_PRIVATE_H__

#include <gst/gst.h>

G_BEGIN_DECLS

void fs_base_conference_init_debug (void);

GST_DEBUG_CATEGORY_EXTERN (fs_base_conference_debug);

G_END_DECLS

#endif /* __FS_PRIVATE_H__ */

