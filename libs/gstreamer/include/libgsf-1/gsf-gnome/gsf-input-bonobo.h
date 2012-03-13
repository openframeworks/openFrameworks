/* vim: set sw=8: -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * gsf-input-bonobo.h:  bonobo based input
 *
 * Copyright (C) 2002-2003 Jon K Hellan (hellan@acm.org)
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

#ifndef GSF_INPUT_BONOBO_H
#define GSF_INPUT_BONOBO_H

#include <gsf/gsf-input.h>
#include <bonobo/Bonobo.h>

G_BEGIN_DECLS

#define GSF_INPUT_BONOBO_TYPE        (gsf_input_bonobo_get_type ())
#define GSF_INPUT_BONOBO(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), GSF_INPUT_BONOBO_TYPE, GsfInputBonobo))
#define GSF_IS_INPUT_BONOBO(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), GSF_INPUT_BONOBO_TYPE))

typedef struct _GsfInputBonobo GsfInputBonobo;

GType     gsf_input_bonobo_get_type (void);
GsfInput *gsf_input_bonobo_new      (Bonobo_Stream const stream, GError **err);

G_END_DECLS

#endif /* GSF_INPUT_BONOBO_H */
