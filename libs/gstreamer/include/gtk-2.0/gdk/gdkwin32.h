/* GDK - The GIMP Drawing Kit
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
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

/*
 * Modified by the GTK+ Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/. 
 */

#ifndef __GDK_WIN32_H__
#define __GDK_WIN32_H__

#include <gdk/gdkprivate.h>

#ifndef STRICT
#define STRICT			/* We want strict type checks */
#endif
#include <windows.h>
#include <commctrl.h>

G_BEGIN_DECLS

#ifdef INSIDE_GDK_WIN32

#include "gdkprivate-win32.h"

#undef GDK_ROOT_PARENT /* internal access is direct */
#define GDK_ROOT_PARENT()             ((GdkWindow *) _gdk_parent_root)
#define GDK_WINDOW_HWND(win)          (GDK_DRAWABLE_IMPL_WIN32(((GdkWindowObject *)win)->impl)->handle)
#define GDK_PIXMAP_HBITMAP(pixmap)    (GDK_DRAWABLE_IMPL_WIN32(((GdkPixmapObject *)pixmap)->impl)->handle)
#define GDK_DRAWABLE_IMPL_WIN32_HANDLE(d) (((GdkDrawableImplWin32 *) d)->handle)
#define GDK_DRAWABLE_HANDLE(win)      (GDK_IS_WINDOW (win) ? GDK_WINDOW_HWND (win) : (GDK_IS_PIXMAP (win) ? GDK_PIXMAP_HBITMAP (win) : (GDK_IS_DRAWABLE_IMPL_WIN32 (win) ? GDK_DRAWABLE_IMPL_WIN32_HANDLE (win) : 0)))
#else
/* definition for exported 'internals' go here */
#define GDK_WINDOW_HWND(d) (gdk_win32_drawable_get_handle (d))

#endif

#define GDK_ROOT_WINDOW()             ((guint32) HWND_DESKTOP)
#define GDK_DISPLAY()                 NULL


/* These need to be here so gtkstatusicon.c can pick them up if needed. */
#ifndef WM_XBUTTONDOWN
#define WM_XBUTTONDOWN 0x020B
#endif
#ifndef WM_XBUTTONUP
#define WM_XBUTTONUP 0x020C
#endif
#ifndef GET_XBUTTON_WPARAM
#define GET_XBUTTON_WPARAM(w) (HIWORD(w))
#endif
#ifndef XBUTTON1
#define XBUTTON1 1
#endif
#ifndef XBUTTON2
#define XBUTTON2 2
#endif


/* Return true if the GdkWindow is a win32 implemented window */
gboolean      gdk_win32_window_is_win32 (GdkWindow *window);

/* Return the Gdk* for a particular HANDLE */
gpointer      gdk_win32_handle_table_lookup (GdkNativeWindow handle);

/* Translate from drawable to Windows handle */
HGDIOBJ       gdk_win32_drawable_get_handle (GdkDrawable *drawable);

/* Return a device context to draw in a drawable, given a GDK GC,
 * and a mask indicating which GC values might be used (for efficiency,
 * no need to muck around with text-related stuff if we aren't going
 * to output text, for instance).
 */
HDC           gdk_win32_hdc_get      (GdkDrawable    *drawable,
				      GdkGC          *gc,
				      GdkGCValuesMask usage);

/* Each HDC returned from gdk_win32_hdc_get must be released with
 * this function
 */
void          gdk_win32_hdc_release  (GdkDrawable    *drawable,
				      GdkGC          *gc,
				      GdkGCValuesMask usage);

void          gdk_win32_selection_add_targets (GdkWindow  *owner,
					       GdkAtom     selection,
					       gint	   n_targets,
					       GdkAtom    *targets);

/* For internal GTK use only */
GdkPixbuf    *gdk_win32_icon_to_pixbuf_libgtk_only (HICON hicon);
HICON         gdk_win32_pixbuf_to_hicon_libgtk_only (GdkPixbuf *pixbuf);
void          gdk_win32_set_modal_dialog_libgtk_only (HWND window);

GdkDrawable  *gdk_win32_begin_direct_draw_libgtk_only (GdkDrawable *drawable,
						       GdkGC *gc,
						       gpointer *priv_data,
						       gint *x_offset_out,
						       gint *y_offset_out);
void          gdk_win32_end_direct_draw_libgtk_only (gpointer priv_data);


G_END_DECLS

#endif /* __GDK_WIN32_H__ */
