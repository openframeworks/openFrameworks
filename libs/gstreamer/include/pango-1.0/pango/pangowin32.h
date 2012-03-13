/* Pango
 * pangowin32.h:
 *
 * Copyright (C) 1999 Red Hat Software
 * Copyright (C) 2000 Tor Lillqvist
 * Copyright (C) 2001 Alexander Larsson
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __PANGOWIN32_H__
#define __PANGOWIN32_H__

#include <glib.h>
#include <pango/pango-font.h>
#include <pango/pango-layout.h>

G_BEGIN_DECLS

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501	/* To get ClearType-related macros */
#endif
#include <windows.h>
#undef STRICT

#define PANGO_RENDER_TYPE_WIN32 "PangoRenderWin32"

/* Calls for applications
 */
#ifndef PANGO_DISABLE_DEPRECATED
PangoContext * pango_win32_get_context        (void);
#endif

void           pango_win32_render             (HDC               hdc,
					       PangoFont        *font,
					       PangoGlyphString *glyphs,
					       gint              x,
					       gint              y);
void           pango_win32_render_layout_line (HDC               hdc,
					       PangoLayoutLine  *line,
					       int               x,
					       int               y);
void           pango_win32_render_layout      (HDC               hdc,
					       PangoLayout      *layout,
					       int               x,
					       int               y);

void           pango_win32_render_transformed (HDC         hdc,
					       const PangoMatrix *matrix,
					       PangoFont         *font,
					       PangoGlyphString  *glyphs,
					       int                x,
					       int                y);

#ifdef PANGO_ENABLE_ENGINE

/* For shape engines
 */

#ifndef PANGO_DISABLE_DEPRECATED
PangoGlyph     pango_win32_get_unknown_glyph  (PangoFont        *font,
					       gunichar          wc);
#endif /* PANGO_DISABLE_DEPRECATED */
gint	      pango_win32_font_get_glyph_index(PangoFont        *font,
					       gunichar          wc);

HDC            pango_win32_get_dc             (void);

gboolean       pango_win32_get_debug_flag     (void);

gboolean pango_win32_font_select_font        (PangoFont *font,
					      HDC        hdc);
void     pango_win32_font_done_font          (PangoFont *font);
double   pango_win32_font_get_metrics_factor (PangoFont *font);

#endif

/* API for libraries that want to use PangoWin32 mixed with classic
 * Win32 fonts.
 */
typedef struct _PangoWin32FontCache PangoWin32FontCache;

PangoWin32FontCache *pango_win32_font_cache_new          (void);
void                 pango_win32_font_cache_free         (PangoWin32FontCache *cache);

HFONT                pango_win32_font_cache_load         (PangoWin32FontCache *cache,
							  const LOGFONTA      *logfont);
HFONT                pango_win32_font_cache_loadw        (PangoWin32FontCache *cache,
							  const LOGFONTW      *logfont);
void                 pango_win32_font_cache_unload       (PangoWin32FontCache *cache,
							  HFONT                hfont);

PangoFontMap        *pango_win32_font_map_for_display    (void);
void                 pango_win32_shutdown_display        (void);
PangoWin32FontCache *pango_win32_font_map_get_font_cache (PangoFontMap       *font_map);

LOGFONTA            *pango_win32_font_logfont            (PangoFont          *font);
LOGFONTW            *pango_win32_font_logfontw           (PangoFont          *font);

PangoFontDescription *pango_win32_font_description_from_logfont (const LOGFONTA *lfp);

PangoFontDescription *pango_win32_font_description_from_logfontw (const LOGFONTW *lfp);

G_END_DECLS

#endif /* __PANGOWIN32_H__ */
