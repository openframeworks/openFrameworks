/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set sw=4 sts=4 ts=4 expandtab: */
/* 
   rsvg-cairo.h: SAX-based renderer for SVG files using cairo
 
   Copyright (C) 2005 Red Hat, Inc.
  
   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.
  
   You should have received a copy of the GNU Library General Public
   License along with this program; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
  
   Author: Carl Worth <cworth@cworth.org>
*/

#ifndef RSVG_CAIRO_H
#define RSVG_CAIRO_H

#include <cairo.h>

G_BEGIN_DECLS 

gboolean    rsvg_handle_render_cairo     (RsvgHandle * handle, cairo_t * cr);
gboolean    rsvg_handle_render_cairo_sub (RsvgHandle * handle, cairo_t * cr, const char *id);

G_END_DECLS

#endif
