/* Pango
 * pango-break.h:
 *
 * Copyright (C) 1999 Red Hat Software
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

#ifndef __PANGO_BREAK_H__
#define __PANGO_BREAK_H__

#include <glib.h>

G_BEGIN_DECLS

#include <pango/pango-item.h>

/* Logical attributes of a character.
 */
struct _PangoLogAttr
{
  guint is_line_break : 1;      /* Can break line in front of character */

  guint is_mandatory_break : 1; /* Must break line in front of character */

  guint is_char_break : 1;      /* Can break here when doing char wrap */

  guint is_white : 1;           /* Whitespace character */

  /* Cursor can appear in front of character (i.e. this is a grapheme
   * boundary, or the first character in the text).
   */
  guint is_cursor_position : 1;

  /* Note that in degenerate cases, you could have both start/end set on
   * some text, most likely for sentences (e.g. no space after a period, so
   * the next sentence starts right away).
   */

  guint is_word_start : 1;      /* first character in a word */
  guint is_word_end   : 1;      /* is first non-word char after a word */

  /* There are two ways to divide sentences. The first assigns all
   * intersentence whitespace/control/format chars to some sentence,
   * so all chars are in some sentence; is_sentence_boundary denotes
   * the boundaries there. The second way doesn't assign
   * between-sentence spaces, etc. to any sentence, so
   * is_sentence_start/is_sentence_end mark the boundaries of those
   * sentences.
   */
  guint is_sentence_boundary : 1;
  guint is_sentence_start : 1;  /* first character in a sentence */
  guint is_sentence_end : 1;    /* first non-sentence char after a sentence */

  /* If set, backspace deletes one character rather than
   * the entire grapheme cluster.
   */
  guint backspace_deletes_character : 1;

  /* Only few space variants (U+0020 and U+00A0) have variable
   * width during justification.
   */
  guint is_expandable_space : 1;

  /* Word boundary as defined by UAX#29 */
  guint is_word_boundary : 1;	/* is NOT in the middle of a word */
};

/* Determine information about cluster/word/line breaks in a string
 * of Unicode text.
 */
void pango_break (const gchar   *text,
		  int            length,
		  PangoAnalysis *analysis,
		  PangoLogAttr  *attrs,
		  int            attrs_len);

void pango_find_paragraph_boundary (const gchar *text,
				    gint         length,
				    gint        *paragraph_delimiter_index,
				    gint        *next_paragraph_start);

void pango_get_log_attrs (const char    *text,
			  int            length,
			  int            level,
			  PangoLanguage *language,
			  PangoLogAttr  *log_attrs,
			  int            attrs_len);

#ifdef PANGO_ENABLE_ENGINE

/* This is the default break algorithm, used if no language
 * engine overrides it. Normally you should use pango_break()
 * instead; this function is mostly useful for chaining up
 * from a language engine override.
 */
void pango_default_break (const gchar   *text,
			  int            length,
			  PangoAnalysis *analysis,
			  PangoLogAttr  *attrs,
			  int            attrs_len);

#endif /* PANGO_ENABLE_ENGINE */

G_END_DECLS

#endif /* __PANGO_BREAK_H__ */
