/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set sw=4 sts=4 ts=4 expandtab: */
/* 
   rsvg.h: SAX-based renderer for SVG files into a GdkPixbuf.
 
   Copyright (C) 2000 Eazel, Inc.
  
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
  
   Author: Raph Levien <raph@artofcode.com>
*/

#ifndef RSVG_H
#define RSVG_H

#include <glib-object.h>
#include <gio/gio.h>

#include <gdk-pixbuf/gdk-pixbuf.h>

G_BEGIN_DECLS

#define RSVG_TYPE_HANDLE                  (rsvg_handle_get_type ())
#define RSVG_HANDLE(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), RSVG_TYPE_HANDLE, RsvgHandle))
#define RSVG_HANDLE_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), RSVG_TYPE_HANDLE, RsvgHandleClass))
#define RSVG_IS_HANDLE(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), RSVG_TYPE_HANDLE))
#define RSVG_IS_HANDLE_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), RSVG_TYPE_HANDLE))
#define RSVG_HANDLE_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), RSVG_TYPE_HANDLE, RsvgHandleClass))

GType rsvg_handle_get_type (void);

/**
 * An enumeration representing possible error domains
 */
typedef enum {
    RSVG_ERROR_FAILED
} RsvgError;

/**
 * 
 */
#define RSVG_ERROR (rsvg_error_quark ())
GQuark rsvg_error_quark (void) G_GNUC_CONST;

/**
 * The RsvgHandle is an object representing the parsed form of a SVG
 */
typedef struct _RsvgHandle RsvgHandle;
typedef struct RsvgHandlePrivate RsvgHandlePrivate;
typedef struct _RsvgHandleClass RsvgHandleClass;
typedef struct _RsvgDimensionData RsvgDimensionData;
typedef struct _RsvgPositionData RsvgPositionData;

struct _RsvgHandleClass {
    GObjectClass parent;

    gpointer _abi_padding[15];
};

struct _RsvgHandle {
    GObject parent;

    RsvgHandlePrivate *priv;

    gpointer _abi_padding[15];
};

/* RsvgDimensionData
 */
struct _RsvgDimensionData {
    /**
     * SVG's width, in pixels
     */
    int width;

    /**
     * SVG's height, in pixels
     */
    int height;

    /**
     * em
     */
    gdouble em;

    /**
     * ex
     */
    gdouble ex;
};

/**
 * Position of an SVG fragment.
 **/
struct _RsvgPositionData {
    int x;
    int y;
};

void rsvg_init (void);
void rsvg_term (void);

void rsvg_set_default_dpi	(double dpi);
void rsvg_set_default_dpi_x_y	(double dpi_x, double dpi_y);

void rsvg_handle_set_dpi	(RsvgHandle * handle, double dpi);
void rsvg_handle_set_dpi_x_y	(RsvgHandle * handle, double dpi_x, double dpi_y);

RsvgHandle  *rsvg_handle_new		(void);
gboolean     rsvg_handle_write		(RsvgHandle * handle, const guchar * buf, 
                                     gsize count, GError ** error);
gboolean     rsvg_handle_close		(RsvgHandle * handle, GError ** error);
GdkPixbuf   *rsvg_handle_get_pixbuf	(RsvgHandle * handle);
GdkPixbuf   *rsvg_handle_get_pixbuf_sub (RsvgHandle * handle, const char *id);

G_CONST_RETURN char	*rsvg_handle_get_base_uri (RsvgHandle * handle);
void                 rsvg_handle_set_base_uri (RsvgHandle * handle, const char *base_uri);

void rsvg_handle_get_dimensions (RsvgHandle * handle, RsvgDimensionData * dimension_data);

gboolean rsvg_handle_get_dimensions_sub (RsvgHandle * handle, RsvgDimensionData * dimension_data, const char *id);
gboolean rsvg_handle_get_position_sub (RsvgHandle * handle, RsvgPositionData * position_data, const char *id);

gboolean rsvg_handle_has_sub (RsvgHandle * handle, const char *id);

/* GIO APIs */

typedef enum {
    RSVG_HANDLE_FLAGS_NONE        = 0,
} RsvgHandleFlags;

void        rsvg_handle_set_base_gfile (RsvgHandle *handle,
                                        GFile      *base_file);

gboolean    rsvg_handle_read_stream_sync (RsvgHandle   *handle,
                                          GInputStream *stream,
                                          GCancellable *cancellable,
                                          GError      **error);

RsvgHandle *rsvg_handle_new_from_gfile_sync (GFile          *file,
                                             RsvgHandleFlags flags,
                                             GCancellable   *cancellable,
                                             GError        **error);

RsvgHandle *rsvg_handle_new_from_stream_sync (GInputStream   *input_stream,
                                              GFile          *base_file,
                                              RsvgHandleFlags flags,
                                              GCancellable   *cancellable,
                                              GError        **error);

/* Accessibility API */

G_CONST_RETURN char *rsvg_handle_get_title	(RsvgHandle * handle);
G_CONST_RETURN char *rsvg_handle_get_desc	(RsvgHandle * handle);
G_CONST_RETURN char *rsvg_handle_get_metadata	(RsvgHandle * handle);

RsvgHandle *rsvg_handle_new_from_data (const guint8 * data, gsize data_len, GError ** error);
RsvgHandle *rsvg_handle_new_from_file (const gchar * file_name, GError ** error);

#ifndef RSVG_DISABLE_DEPRECATED

void rsvg_handle_free (RsvgHandle * handle);

/**
 * RsvgSizeFunc ():
 * @width: Pointer to where to set/store the width
 * @height: Pointer to where to set/store the height
 * @user_data: User data pointer
 *
 * Function to let a user of the library specify the SVG's dimensions
 * @width: the ouput width the SVG should be
 * @height: the output height the SVG should be
 * @user_data: user data
 *
 * Deprecated: Set up a cairo matrix and use rsvg_handle_render_cairo() instead.
 */
typedef void (*RsvgSizeFunc) (gint * width, gint * height, gpointer user_data);
void rsvg_handle_set_size_callback (RsvgHandle * handle,
                                    RsvgSizeFunc size_func,
                                    gpointer user_data, GDestroyNotify user_data_destroy);

/* GdkPixbuf convenience API */

GdkPixbuf *rsvg_pixbuf_from_file            (const gchar * file_name, GError ** error);
GdkPixbuf *rsvg_pixbuf_from_file_at_zoom    (const gchar * file_name,
                                             double x_zoom, double y_zoom, GError ** error);
GdkPixbuf *rsvg_pixbuf_from_file_at_size    (const gchar * file_name, gint width, gint height,
                                             GError ** error);
GdkPixbuf *rsvg_pixbuf_from_file_at_max_size    (const gchar * file_name,
                                                 gint max_width, gint max_height, GError ** error);
GdkPixbuf *rsvg_pixbuf_from_file_at_zoom_with_max (const gchar * file_name,
                                                   double x_zoom,
                                                   double y_zoom,
                                                   gint max_width, gint max_height, GError ** error);

#endif                          /* RSVG_DISABLE_DEPRECATED */

G_END_DECLS

#endif                          /* RSVG_H */
