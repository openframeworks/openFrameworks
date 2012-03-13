/* 
   Internationalization of neon
   Copyright (C) 2005-2006, Joe Orton <joe@manyfish.co.uk>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
   MA 02111-1307, USA

*/

#ifndef NE_I18N_H
#define NE_I18N_H

#include "ne_defs.h"

NE_BEGIN_DECLS

/* Initialize translated error messages within neon.  If 'encoding' is
 * non-NULL, it specifies the character encoding for the generated
 * translated strings.  If it is NULL, the appropriate character
 * encoding for the locale will be used.
 *
 * This call is only strictly necessary if either:
 *
 * a) neon has been installed into a different prefix than the
 * gettext() implementation on which it depends for i18n purposes, or
 *
 * b) the caller requires that translated messages are in a particular
 * character encoding.
 *
 * If ne_i18n_init() is never called, the message catalogs will not be
 * found if case (a) applies (and so English error messages will be
 * used), and will use the default character encoding specified by the
 * process locale.  The library will otherwise operate correctly.
 *
 * Note that the encoding used is a process-global setting and so
 * results may be unexpected if other users of neon within the process
 * call ne_i18n_init() with a different encoding parameter.
 */
void ne_i18n_init(const char *encoding);

NE_END_DECLS

#endif /* NE_I18N_H */
