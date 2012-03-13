/*
 * Copyright (C) 2008 Tor Lillqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; see the file COPYING.LIB.txt.  If
 * not, write to the Free Software Foundation, Inc., 51 Franklin
 * Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef _LIBINTL_H
#define _LIBINTL_H      1

#include <locale.h>

#ifndef LC_MESSAGES
# define LC_MESSAGES 1729       /* Use same value as in GNU gettext */
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern char *gettext (const char *msgid);

extern char *dgettext (const char *domainname,
		       const char *msgid);

extern char *dcgettext (const char *domainname,
			const char *msgid,
			int         category);

extern char *ngettext (const char       *msgid1,
		       const char       *msgid2,
		       unsigned long int n);

extern char *dngettext (const char       *domainname,
			const char       *msgid1,
			const char       *msgid2,
			unsigned long int n);

extern char *dcngettext (const char       *domainname,
			 const char       *msgid1,
			 const char       *msgid2,
			 unsigned long int n,
			 int               category);

extern char *textdomain (const char *domainname);

extern char *bindtextdomain (const char *domainname,
                             const char *dirname);

extern char *bind_textdomain_codeset (const char *domainname,
				      const char *codeset);


#ifdef __cplusplus
}
#endif

#endif /* _LIBINTL_H */
