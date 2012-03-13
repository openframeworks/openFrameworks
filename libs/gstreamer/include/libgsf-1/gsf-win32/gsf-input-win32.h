/* vim: set sw=8: -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * gsf-input-win32.h: 
 *
 * Copyright (C) 2003 Dom Lachowicz <cinamod@hotmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

#ifndef GSF_INPUT_WIN32_H
#define GSF_INPUT_WIN32_H

#include <gsf/gsf-input.h>
#include <windows.h>

G_BEGIN_DECLS

#define GSF_INPUT_ISTREAM_TYPE        (gsf_input_istream_get_type ())
#define GSF_INPUT_ISTREAM(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), GSF_INPUT_ISTREAM_TYPE, GsfInputIStream))
#define GSF_IS_INPUT_ISTREAM(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), GSF_INPUT_ISTREAM_TYPE))

typedef struct _GsfInputIStream GsfInputIStream;

GType     gsf_input_istream_get_type (void);
GsfInput *gsf_input_istream_new      (IStream * stream, GError **err);

G_END_DECLS

#endif /* GSF_INPUT_ISTREAM_H */
