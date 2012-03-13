/* Pango
 * pango-utils.c: Utilities for internal functions and modules
 *
 * Copyright (C) 2000 Red Hat Software
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

#ifndef __PANGO_UTILS_H__
#define __PANGO_UTILS_H__

#include <stdio.h>
#include <glib.h>
#include <pango/pango-font.h>

G_BEGIN_DECLS

char **  pango_split_file_list (const char *str);

char    *pango_trim_string     (const char *str);
gint     pango_read_line      (FILE        *stream,
			       GString     *str);
gboolean pango_skip_space     (const char **pos);
gboolean pango_scan_word      (const char **pos,
			       GString     *out);
gboolean pango_scan_string    (const char **pos,
			       GString     *out);
gboolean pango_scan_int       (const char **pos,
			       int         *out);

#ifdef PANGO_ENABLE_BACKEND
char *   pango_config_key_get (const char  *key);
void     pango_lookup_aliases (const char   *fontname,
			       char       ***families,
			       int          *n_families);
#endif /* PANGO_ENABLE_BACKEND */

gboolean pango_parse_enum     (GType       type,
			       const char *str,
			       int        *value,
			       gboolean    warn,
			       char      **possible_values);

/* Functions for parsing textual representations
 * of PangoFontDescription fields. They return TRUE if the input string
 * contains a valid value, which then has been assigned to the corresponding
 * field in the PangoFontDescription. If the warn parameter is TRUE,
 * a warning is printed (with g_warning) if the string does not
 * contain a valid value.
 */
gboolean pango_parse_style   (const char   *str,
			      PangoStyle   *style,
			      gboolean      warn);
gboolean pango_parse_variant (const char   *str,
			      PangoVariant *variant,
			      gboolean      warn);
gboolean pango_parse_weight  (const char   *str,
			      PangoWeight  *weight,
			      gboolean      warn);
gboolean pango_parse_stretch (const char   *str,
			      PangoStretch *stretch,
			      gboolean      warn);

#ifdef PANGO_ENABLE_BACKEND

/* On Unix, return the name of the "pango" subdirectory of SYSCONFDIR
 * (which is set at compile time). On Win32, return the Pango
 * installation directory (which is set at installation time, and
 * stored in the registry). The returned string should not be
 * g_free'd.
 */
G_CONST_RETURN char *   pango_get_sysconf_subdirectory (void) G_GNUC_PURE;

/* Ditto for LIBDIR/pango. On Win32, use the same Pango
 * installation directory. This returned string should not be
 * g_free'd either.
 */
G_CONST_RETURN char *   pango_get_lib_subdirectory (void) G_GNUC_PURE;

#endif /* PANGO_ENABLE_BACKEND */

/* Hint line position and thickness.
 */
void pango_quantize_line_geometry (int *thickness,
				   int *position);

/* A routine from fribidi that we either wrap or provide ourselves.
 */
guint8 * pango_log2vis_get_embedding_levels (const gchar    *text,
					     int             length,
					     PangoDirection *pbase_dir);

/* Unicode characters that are zero-width and should not be rendered
 * normally.
 */
gboolean pango_is_zero_width (gunichar ch) G_GNUC_CONST;

/* Pango version checking */

/* Encode a Pango version as an integer */
#define PANGO_VERSION_ENCODE(major, minor, micro) (     \
	  ((major) * 10000)                             \
	+ ((minor) *   100)                             \
	+ ((micro) *     1))

/* Encoded version of Pango at compile-time */
#define PANGO_VERSION PANGO_VERSION_ENCODE(     \
	PANGO_VERSION_MAJOR,                    \
	PANGO_VERSION_MINOR,                    \
	PANGO_VERSION_MICRO)

/* Check that compile-time Pango is as new as required */
#define PANGO_VERSION_CHECK(major,minor,micro)    \
	(PANGO_VERSION >= PANGO_VERSION_ENCODE(major,minor,micro))


/* Return encoded version of Pango at run-time */
int pango_version (void) G_GNUC_CONST;

/* Return run-time Pango version as an string */
G_CONST_RETURN char * pango_version_string (void) G_GNUC_CONST;

/* Check that run-time Pango is as new as required */
G_CONST_RETURN char * pango_version_check (int required_major,
					   int required_minor,
					   int required_micro) G_GNUC_CONST;

G_END_DECLS

#endif /* __PANGO_UTILS_H__ */
