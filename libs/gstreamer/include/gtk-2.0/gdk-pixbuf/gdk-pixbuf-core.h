/* GdkPixbuf library - GdkPixbuf data structure
 *
 * Copyright (C) 2003 The Free Software Foundation
 *
 * Authors: Mark Crichton <crichton@gimp.org>
 *          Miguel de Icaza <miguel@gnu.org>
 *          Federico Mena-Quintero <federico@gimp.org>
 *          Havoc Pennington <hp@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
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
 */

#if defined(GDK_PIXBUF_DISABLE_SINGLE_INCLUDES) && !defined (GDK_PIXBUF_H_INSIDE) && !defined (GDK_PIXBUF_COMPILATION)
#error "Only <gdk-pixbuf/gdk-pixbuf.h> can be included directly."
#endif

#ifndef GDK_PIXBUF_CORE_H
#define GDK_PIXBUF_CORE_H

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

/* Alpha compositing mode */
typedef enum
{
        GDK_PIXBUF_ALPHA_BILEVEL,
        GDK_PIXBUF_ALPHA_FULL
} GdkPixbufAlphaMode;

/* Color spaces; right now only RGB is supported.
 * Note that these values are encoded in inline pixbufs
 * as ints, so don't reorder them
 */
typedef enum {
	GDK_COLORSPACE_RGB
} GdkColorspace;

/* All of these are opaque structures */
typedef struct _GdkPixbuf GdkPixbuf;

#define GDK_TYPE_PIXBUF              (gdk_pixbuf_get_type ())
#define GDK_PIXBUF(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_PIXBUF, GdkPixbuf))
#define GDK_IS_PIXBUF(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_PIXBUF))


/* Handler that must free the pixel array */
typedef void (* GdkPixbufDestroyNotify) (guchar *pixels, gpointer data);

#define GDK_PIXBUF_ERROR gdk_pixbuf_error_quark ()

typedef enum {
        /* image data hosed */
        GDK_PIXBUF_ERROR_CORRUPT_IMAGE,
        /* no mem to load image */
        GDK_PIXBUF_ERROR_INSUFFICIENT_MEMORY,
        /* bad option passed to save routine */
        GDK_PIXBUF_ERROR_BAD_OPTION,
        /* unsupported image type (sort of an ENOSYS) */
        GDK_PIXBUF_ERROR_UNKNOWN_TYPE,
        /* unsupported operation (load, save) for image type */
        GDK_PIXBUF_ERROR_UNSUPPORTED_OPERATION,
        GDK_PIXBUF_ERROR_FAILED
} GdkPixbufError;

GQuark gdk_pixbuf_error_quark (void);



GType gdk_pixbuf_get_type (void) G_GNUC_CONST;

/* Reference counting */

#ifndef GDK_PIXBUF_DISABLE_DEPRECATED
GdkPixbuf *gdk_pixbuf_ref      (GdkPixbuf *pixbuf);
void       gdk_pixbuf_unref    (GdkPixbuf *pixbuf);
#endif

/* GdkPixbuf accessors */

GdkColorspace gdk_pixbuf_get_colorspace      (const GdkPixbuf *pixbuf);
int           gdk_pixbuf_get_n_channels      (const GdkPixbuf *pixbuf);
gboolean      gdk_pixbuf_get_has_alpha       (const GdkPixbuf *pixbuf);
int           gdk_pixbuf_get_bits_per_sample (const GdkPixbuf *pixbuf);
guchar       *gdk_pixbuf_get_pixels          (const GdkPixbuf *pixbuf);
int           gdk_pixbuf_get_width           (const GdkPixbuf *pixbuf);
int           gdk_pixbuf_get_height          (const GdkPixbuf *pixbuf);
int           gdk_pixbuf_get_rowstride       (const GdkPixbuf *pixbuf);



/* Create a blank pixbuf with an optimal rowstride and a new buffer */
GdkPixbuf *gdk_pixbuf_new (GdkColorspace colorspace, gboolean has_alpha, int bits_per_sample,
			   int width, int height);

/* Copy a pixbuf */

GdkPixbuf *gdk_pixbuf_copy (const GdkPixbuf *pixbuf);

/* Create a pixbuf which points to the pixels of another pixbuf */
GdkPixbuf *gdk_pixbuf_new_subpixbuf (GdkPixbuf *src_pixbuf,
                                     int        src_x,
                                     int        src_y,
                                     int        width,
                                     int        height);

/* Simple loading */

#ifdef G_OS_WIN32
/* DLL ABI stability hack. */
#define gdk_pixbuf_new_from_file gdk_pixbuf_new_from_file_utf8
#define gdk_pixbuf_new_from_file_at_size gdk_pixbuf_new_from_file_at_size_utf8
#define gdk_pixbuf_new_from_file_at_scale gdk_pixbuf_new_from_file_at_scale_utf8
#endif

GdkPixbuf *gdk_pixbuf_new_from_file (const char *filename,
                                     GError    **error);
GdkPixbuf *gdk_pixbuf_new_from_file_at_size (const char *filename,
					     int         width, 
					     int         height,
					     GError    **error);
GdkPixbuf *gdk_pixbuf_new_from_file_at_scale (const char *filename,
					      int         width, 
					      int         height,
					      gboolean    preserve_aspect_ratio,
					      GError    **error);

GdkPixbuf *gdk_pixbuf_new_from_data (const guchar *data,
				     GdkColorspace colorspace,
				     gboolean has_alpha,
				     int bits_per_sample,
				     int width, int height,
				     int rowstride,
				     GdkPixbufDestroyNotify destroy_fn,
				     gpointer destroy_fn_data);

GdkPixbuf *gdk_pixbuf_new_from_xpm_data (const char **data);
GdkPixbuf* gdk_pixbuf_new_from_inline	(gint          data_length,
					 const guint8 *data,
					 gboolean      copy_pixels,
					 GError      **error);
       
/* Mutations */
void       gdk_pixbuf_fill              (GdkPixbuf    *pixbuf,
                                         guint32       pixel);

/* Saving */

#ifdef G_OS_WIN32
/* DLL ABI stability hack. */
#define gdk_pixbuf_save gdk_pixbuf_save_utf8
#define gdk_pixbuf_savev gdk_pixbuf_savev_utf8
#endif

gboolean gdk_pixbuf_save           (GdkPixbuf  *pixbuf, 
                                    const char *filename, 
                                    const char *type, 
                                    GError    **error,
                                    ...) G_GNUC_NULL_TERMINATED;

gboolean gdk_pixbuf_savev          (GdkPixbuf  *pixbuf, 
                                    const char *filename, 
                                    const char *type,
                                    char      **option_keys,
                                    char      **option_values,
                                    GError    **error);

/* Saving to a callback function */

typedef gboolean (*GdkPixbufSaveFunc)   (const gchar *buf,
					 gsize count,
					 GError **error,
					 gpointer data);

gboolean gdk_pixbuf_save_to_callback    (GdkPixbuf  *pixbuf,
					 GdkPixbufSaveFunc save_func,
					 gpointer user_data,
					 const char *type, 
					 GError    **error,
					 ...) G_GNUC_NULL_TERMINATED;

gboolean gdk_pixbuf_save_to_callbackv   (GdkPixbuf  *pixbuf, 
					 GdkPixbufSaveFunc save_func,
					 gpointer user_data,
					 const char *type,
					 char      **option_keys,
					 char      **option_values,
					 GError    **error);

/* Saving into a newly allocated char array */

gboolean gdk_pixbuf_save_to_buffer      (GdkPixbuf  *pixbuf,
					 gchar     **buffer,
					 gsize      *buffer_size,
					 const char *type, 
					 GError    **error,
					 ...) G_GNUC_NULL_TERMINATED;

gboolean gdk_pixbuf_save_to_bufferv     (GdkPixbuf  *pixbuf,
					 gchar     **buffer,
					 gsize      *buffer_size,
					 const char *type, 
					 char      **option_keys,
					 char      **option_values,
					 GError    **error);

GdkPixbuf *gdk_pixbuf_new_from_stream   (GInputStream   *stream,
					 GCancellable   *cancellable,
                                         GError        **error);

GdkPixbuf *gdk_pixbuf_new_from_stream_at_scale   (GInputStream   *stream,
                                                  gint            width,
                                                  gint            height,
                                                  gboolean        preserve_aspect_ratio,
						  GCancellable   *cancellable,
                                                  GError        **error);

gboolean   gdk_pixbuf_save_to_stream    (GdkPixbuf      *pixbuf,
                                         GOutputStream  *stream,
                                         const char     *type,
					 GCancellable   *cancellable,
                                         GError        **error,
                                         ...);

/* Adding an alpha channel */
GdkPixbuf *gdk_pixbuf_add_alpha (const GdkPixbuf *pixbuf, gboolean substitute_color,
				 guchar r, guchar g, guchar b);

/* Copy an area of a pixbuf onto another one */
void gdk_pixbuf_copy_area (const GdkPixbuf *src_pixbuf,
			   int src_x, int src_y,
			   int width, int height,
			   GdkPixbuf *dest_pixbuf,
			   int dest_x, int dest_y);

/* Brighten/darken and optionally make it pixelated-looking */
void gdk_pixbuf_saturate_and_pixelate (const GdkPixbuf *src,
                                       GdkPixbuf       *dest,
                                       gfloat           saturation,
                                       gboolean         pixelate);

/* Transform an image to agree with its embedded orientation option / tag */
GdkPixbuf *gdk_pixbuf_apply_embedded_orientation (GdkPixbuf *src);

G_CONST_RETURN gchar * gdk_pixbuf_get_option (GdkPixbuf   *pixbuf,
                                              const gchar *key);


G_END_DECLS


#endif /* GDK_PIXBUF_CORE_H */
